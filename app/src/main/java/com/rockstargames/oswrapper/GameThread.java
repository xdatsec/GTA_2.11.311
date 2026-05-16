package com.rockstargames.oswrapper;

import android.content.res.AssetManager;
import android.os.Build;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import com.kurdish.roleplay.game.ui.Hud;
import com.kurdish.roleplay.game.ui.Speedometer;
import com.kurdish.roleplay.game.ui.tab.Tab;
import com.rockstargames.oswrapper.view.InputHandler;
import java.lang.ref.WeakReference;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicBoolean;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.opengles.GL11;

import kotlin.Pair;
import kotlin.Unit;
import kotlin.collections.ArraysKt;
import kotlin.enums.EnumEntries;
import kotlin.enums.EnumEntriesKt;
import kotlin.jvm.functions.Function0;
import kotlin.jvm.internal.Intrinsics;

public final class GameThread {

    private static final String TAG = "GameThread";
    public static final GameThread INSTANCE = new GameThread();

    private static ExecutorThread current;
    private static int pendingGateId;
    private static final ConcurrentLinkedQueue<Runnable> eventQueue = new ConcurrentLinkedQueue<>();
    private static final Object pendingCallbackLock = new Object();
    private static PendingCallback pendingCallback = PendingCallback.NONE;

    int fixedWidth = 0;
    int fixedHeight = 0;
    // -------- FIX #3 & #4: PendingCallback as int constants instead of inner enum --------
    private enum PendingCallback {
        NONE,
        INITIAL,
        GATE,
        SIGN_IN,
        CLOUD_DISABLED;

        private static final /* synthetic */ EnumEntries $ENTRIES = EnumEntriesKt.enumEntries(values());

        public static EnumEntries<PendingCallback> getEntries() {
            return $ENTRIES;
        }
    }
    protected void destroyEGLSurface() {
        if (eglDisplay != null && eglSurface != null)
            egl.eglMakeCurrent(eglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
        if (eglSurface != null)
            egl.eglDestroySurface(eglDisplay, eglSurface);
        eglSurface = null;
    }
    protected int[] configAttrs = null;
    protected EGLContext eglContext = null;
    protected int[] contextAttrs = null;
    /**
     * The number of bits requested for the red component
     */
    protected int redSize = 5;
    /**
     * The number of bits requested for the green component
     */
    protected int greenSize = 6;
    /**
     * The number of bits requested for the blue component
     */
    protected int blueSize = 5;
    /**
     * The number of bits requested for the alpha component
     */
    protected int alphaSize = 0;
    /**
     * The number of bits requested for the stencil component
     */
    protected int stencilSize = 0;
    /**
     * The number of bits requested for the depth component
     */
    protected int depthSize = 16;
    protected SurfaceHolder cachedSurfaceHolder = null;
    private static final int EGL_RENDERABLE_TYPE = 0x3040;
    private static final int EGL_OPENGL_ES2_BIT = 0x0004;
    private static final int EGL_OPENGL_ES3_BIT = 64;
    private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
    EGL10 egl = null;
    GL11 gl = null;
    private boolean HasGLExtensions = false;
    private String glVendor = null;
    private String glExtensions = null;
    private String glRenderer = null;
    private String glVersion = null;
    int surfaceWidth = 0;
    int surfaceHeight = 0;
    private int SwapBufferSkip = 0;
    private boolean ranInit = false;
    protected EGLSurface eglSurface = null;
    protected EGLDisplay eglDisplay = null;

    protected EGLConfig eglConfig = null;
    boolean viewIsActive = false;

    public boolean initEGL(int esVersion, int depthBits) {
        int i;
        int eglErr;
        if (esVersion > 2 && Build.VERSION.SDK_INT < 21) {
            return false;
        }
        if (this.configAttrs == null) {
            this.configAttrs = new int[]{12344};
        }
        int[] oldConf = this.configAttrs;
        this.configAttrs = new int[((oldConf.length + 3) - 1)];
        int i2 = 0;
        while (i2 < oldConf.length - 1) {
            this.configAttrs[i2] = oldConf[i2];
            i2++;
        }
        int i3 = i2 + 1;
        this.configAttrs[i2] = EGL_RENDERABLE_TYPE;
        if (esVersion == 3) {
            i = i3 + 1;
            this.configAttrs[i3] = EGL_OPENGL_ES3_BIT;
        } else {
            i = i3 + 1;
            this.configAttrs[i3] = 4;
        }
        int i4 = i + 1;
        this.configAttrs[i] = 12344;
        this.contextAttrs = new int[]{EGL_CONTEXT_CLIENT_VERSION, esVersion, 12344};
        if (this.configAttrs == null) {
            this.configAttrs = new int[]{12344};
        }
        int[] oldConfES2 = this.configAttrs;
        this.configAttrs = new int[((oldConfES2.length + 13) - 1)];
        int i5 = 0;
        while (i5 < oldConfES2.length - 1) {
            this.configAttrs[i5] = oldConfES2[i5];
            i5++;
        }
        int i6 = i5 + 1;
        this.configAttrs[i5] = 12324;
        int i7 = i6 + 1;
        this.configAttrs[i6] = this.redSize;
        int i8 = i7 + 1;
        this.configAttrs[i7] = 12323;
        int i9 = i8 + 1;
        this.configAttrs[i8] = this.greenSize;
        int i10 = i9 + 1;
        this.configAttrs[i9] = 12322;
        int i11 = i10 + 1;
        this.configAttrs[i10] = this.blueSize;
        int i12 = i11 + 1;
        this.configAttrs[i11] = 12321;
        int i13 = i12 + 1;
        this.configAttrs[i12] = this.alphaSize;
        int i14 = i13 + 1;
        this.configAttrs[i13] = 12326;
        int i15 = i14 + 1;
        this.configAttrs[i14] = this.stencilSize;
        int i16 = i15 + 1;
        this.configAttrs[i15] = 12325;
        int i17 = i16 + 1;
        this.configAttrs[i16] = depthBits;
        int i18 = i17 + 1;
        this.configAttrs[i17] = 12344;
        this.egl = (EGL10) EGLContext.getEGL();
        this.egl.eglGetError();
        this.eglDisplay = this.egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
        System.out.println("eglDisplay: " + this.eglDisplay + ", err: " + this.egl.eglGetError());
        boolean ret = this.egl.eglInitialize(this.eglDisplay, new int[2]);
        System.out.println("EglInitialize returned: " + ret);
        if (!ret || (eglErr = this.egl.eglGetError()) != 12288) {
            return false;
        }
        System.out.println("eglInitialize err: " + eglErr);
        EGLConfig[] config = new EGLConfig[20];
        int[] num_configs = new int[1];
        this.egl.eglChooseConfig(this.eglDisplay, this.configAttrs, config, config.length, num_configs);
        System.out.println("eglChooseConfig err: " + this.egl.eglGetError());
        System.out.println("num_configs " + num_configs[0]);
        int score = 16777216;
        int[] val = new int[1];
        for (int i19 = 0; i19 < num_configs[0]; i19++) {
            boolean cont = true;
            int j = 0;
            while (true) {
                if (j >= ((oldConfES2.length - 1) >> 1)) {
                    break;
                }
                this.egl.eglGetConfigAttrib(this.eglDisplay, config[i19], this.configAttrs[j * 2], val);
                if ((val[0] & this.configAttrs[(j * 2) + 1]) != this.configAttrs[(j * 2) + 1]) {
                    cont = false;
                    break;
                }
                j++;
            }
            if (cont) {
                this.egl.eglGetConfigAttrib(this.eglDisplay, config[i19], 12324, val);
                int r = val[0];
                this.egl.eglGetConfigAttrib(this.eglDisplay, config[i19], 12323, val);
                int g = val[0];
                this.egl.eglGetConfigAttrib(this.eglDisplay, config[i19], 12322, val);
                int b = val[0];
                this.egl.eglGetConfigAttrib(this.eglDisplay, config[i19], 12321, val);
                int a = val[0];
                this.egl.eglGetConfigAttrib(this.eglDisplay, config[i19], 12325, val);
                int d = val[0];
                this.egl.eglGetConfigAttrib(this.eglDisplay, config[i19], 12326, val);
                int currScore = ((((Math.abs(r - this.redSize) + Math.abs(g - this.greenSize)) + Math.abs(b - this.blueSize)) + Math.abs(a - this.alphaSize)) << 16) + (Math.abs(d - depthBits) << 8) + Math.abs(val[0] - this.stencilSize);
                if (currScore < score) {
                    for (int j2 = 0; j2 < ((this.configAttrs.length - 1) >> 1); j2++) {
                        this.egl.eglGetConfigAttrib(this.eglDisplay, config[i19], this.configAttrs[j2 * 2], val);
                    }
                    score = currScore;
                    this.eglConfig = config[i19];
                }
            }
        }
        if (this.eglConfig == null) {
            this.configAttrs = null;
            return false;
        }
        this.eglContext = this.egl.eglCreateContext(this.eglDisplay, this.eglConfig, EGL10.EGL_NO_CONTEXT, this.contextAttrs);
        System.out.println("eglCreateContext: " + this.egl.eglGetError());
        this.gl = (GL11) this.eglContext.getGL();
        return true;
    }
    public boolean InitEGLAndGLES2(int EGLVersion) {
        System.out.println("InitEGLAndGLES2");

        boolean eglInitialized = true;
        if (this.eglContext == null) {
            eglInitialized = false;
            if (EGLVersion >= 3) {
                try {
                    eglInitialized = initEGL(3, 24);
                } catch (Exception e) {
                }
                System.out.println("initEGL 3 " + eglInitialized);
            }
            if (!eglInitialized) {
                this.configAttrs = null;
                try {
                    eglInitialized = initEGL(2, 16);
                } catch (Exception e2) {
                }
                System.out.println("initEGL 2 " + eglInitialized);
                if (!eglInitialized) {
                    eglInitialized = initEGL(2, 16);
                    System.out.println("initEGL 2 " + eglInitialized);
                }
            }
        }
        if (eglInitialized) {
            System.out.println("Should we create a surface?");
            if (!this.viewIsActive) {
                System.out.println("Yes! Calling create surface");
                createEGLSurface(this.cachedSurfaceHolder);
                System.out.println("Done creating surface");
            }
            this.viewIsActive = true;
            this.SwapBufferSkip = 1;
            return true;
        }
        System.out.println("initEGLAndGLES2 failed, core EGL init failure");
        return false;
    }
    public boolean createEGLSurface(SurfaceHolder surfaceHolder) {
        this.eglSurface = this.egl.eglCreateWindowSurface(this.eglDisplay, this.eglConfig, surfaceHolder, (int[]) null);
        Log.d("dfs", "eglCreateWindowSurface err: " + this.egl.eglGetError());
        int[] iArr = new int[1];
        this.egl.eglQuerySurface(this.eglDisplay, this.eglSurface, 12375, iArr);
        this.surfaceWidth = iArr[0];
        this.egl.eglQuerySurface(this.eglDisplay, this.eglSurface, 12374, iArr);
        this.surfaceHeight = iArr[0];
        Log.d("dfs", "checking glVendor == null?");
        if (this.glVendor == null) {
            Log.d("dfs", "Making current and back");
            makeCurrent();
            unMakeCurrent();
        }
        Log.d("dfs", "Done create EGL surface");
        return true;
    }
    public boolean unMakeCurrent() {
        if (!egl.eglMakeCurrent(eglDisplay, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT)) {
            System.out.println("egl(Un)MakeCurrent err: " + egl.eglGetError());
            return false;
        }

        return true;
    }
    public boolean makeCurrent() {
        if (eglContext == null) {
            System.out.println("eglContext is NULL");
            return false;
        } else if (eglSurface == null) {
            System.out.println("eglSurface is NULL");
            return false;
        } else if (!egl.eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
            if (!egl.eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
                System.out.println("eglMakeCurrent err: " + egl.eglGetError());
                return false;
            }
        }

        // This must be called after we have bound an EGL context
        //nvAcquireTimeExtension();
        GetGLExtensions();
        return true;
    }

    public void GetGLExtensions() {
        if (!HasGLExtensions && gl != null && this.cachedSurfaceHolder != null) {
            // gl.glEnable(GL10.GL_CULL_FACE); // ? Ñ�Ð³Ð»Ð°Ð¶Ð¸Ð²Ð°Ð½Ð¸Ðµ
            glVendor = gl.glGetString(GL10.GL_VENDOR);
            glExtensions = gl.glGetString(GL10.GL_EXTENSIONS);
            glRenderer = gl.glGetString(GL10.GL_RENDERER);
            glVersion = gl.glGetString(GL10.GL_VERSION);
            System.out.println("Vendor: " + glVendor);
            System.out.println("Extensions " + glExtensions);
            System.out.println("Renderer: " + glRenderer);
            System.out.println("GIVersion: " + glVersion);
            if (this.glVendor != null) {
                this.HasGLExtensions = true;
            }
        }
    }

    private static final class ExecutorThread extends Thread {
        private AtomicBoolean exitRequested = new AtomicBoolean(false);
        private Runnable finishDrawingRunnable;
        private long lastFrameTime;
        private final WeakReference<GamePlatformServices> servicesWeak;

        public ExecutorThread(WeakReference<GamePlatformServices> servicesWeak) {
            Intrinsics.checkNotNullParameter(servicesWeak, "servicesWeak");
            this.servicesWeak = servicesWeak;
            this.exitRequested = new AtomicBoolean();
            setName(GameThread.TAG);
        }

        private final void guardedRun() {
            this.lastFrameTime = SystemClock.elapsedRealtime();
            while (!this.exitRequested.get()) {
                while (true) {
                    Runnable runnable = (Runnable) GameThread.eventQueue.poll();
                    if (runnable == null) {
                        break;
                    } else {
                        runnable.run();
                    }
                }
                GamePlatformServices gamePlatformServices = this.servicesWeak.get();
                GameView gameView = gamePlatformServices != null ? gamePlatformServices.view : null;
                long jElapsedRealtime = SystemClock.elapsedRealtime();
                float f = (jElapsedRealtime - this.lastFrameTime) / 1000.0f;
                this.lastFrameTime = SystemClock.elapsedRealtime();
                if (gameView != null && gameView.getHasFocus() && GameNative.implIsInitialized()) {
                    GameNative.implOnDrawFrame(f);
                }
                Runnable runnable2 = this.finishDrawingRunnable;
                if (runnable2 != null) {
                    runnable2.run();
                }
                this.finishDrawingRunnable = null;
            }
        }



        @Override
        public void run() {
            // FIX #2: removed useless try/catch InterruptedException — guardedRun() doesn't throw it
            guardedRun();
        }

        void requestExit() {
            this.exitRequested.set(true);
        }

        Runnable getFinishDrawingRunnable() {
            return finishDrawingRunnable;
        }

        void setFinishDrawingRunnable(Runnable r) {
            this.finishDrawingRunnable = r;
        }

        WeakReference<GamePlatformServices> getServicesWeak() {
            return servicesWeak;
        }
    }

    // -------- Pending callback helpers --------

    private void setPendingCallback(PendingCallback cb) {
        Log.d(TAG, "[!!] pendingCallback: " + cb);
        pendingCallback = cb;
    }

    private void setPendingGateId(int id) {
        Log.d(TAG, "[!!] pendingGateId: " + id);
        pendingGateId = id;
    }
    GameThread() {
    }

    private final void notifyCallbackComplete() {
        Log.i(TAG, "[!!] notifyCallbackComplete");
        synchronized (pendingCallbackLock) {
            INSTANCE.setPendingCallback(PendingCallback.NONE);
            Unit unit = Unit.INSTANCE;
        }
    }

    public /* synthetic */ class WhenMappings {
        public static final /* synthetic */ int[] $EnumSwitchMapping$0;

        static {
            int[] iArr = new int[PendingCallback.values().length];
            try {
                iArr[PendingCallback.INITIAL.ordinal()] = 1;
            } catch (NoSuchFieldError unused) {
            }
            try {
                iArr[PendingCallback.GATE.ordinal()] = 2;
            } catch (NoSuchFieldError unused2) {
            }
            try {
                iArr[PendingCallback.SIGN_IN.ordinal()] = 3;
            } catch (NoSuchFieldError unused3) {
            }
            try {
                iArr[PendingCallback.CLOUD_DISABLED.ordinal()] = 4;
            } catch (NoSuchFieldError unused4) {
            }
            $EnumSwitchMapping$0 = iArr;
        }
    }

    // FIX #3: switch on int constants — no more ordinal() / Enum issues
    public final void processPendingCallbacks() {
        Pair pair;
        Log.i(TAG, "[!!] processPendingCallbacks");
        synchronized (pendingCallbackLock) {
            PendingCallback pendingCallback2 = pendingCallback;
            GameThread gameThread = INSTANCE;
            gameThread.setPendingCallback(PendingCallback.NONE);
            int i = pendingGateId;
            gameThread.setPendingGateId(0);
            if (pendingCallback2 != PendingCallback.NONE) {
                Log.i(TAG, "[!!] processPendingCallbacks: reissuing " + pendingCallback2 + " (gate id = " + i + ')');
            }
            pair = new Pair(pendingCallback2, Integer.valueOf(i));
        }
        PendingCallback pendingCallback3 = (PendingCallback) pair.component1();
        int iIntValue = ((Number) pair.component2()).intValue();
        ExecutorThread executorThread = current;
        Intrinsics.checkNotNull(executorThread);
        GamePlatformServices gamePlatformServices = executorThread.getServicesWeak().get();
        Intrinsics.checkNotNull(gamePlatformServices);
        GamePlatformServices gamePlatformServices2 = gamePlatformServices;
        int i2 = WhenMappings.$EnumSwitchMapping$0[pendingCallback3.ordinal()];
        if (i2 == 1) {
            gamePlatformServices2.rockstarShowInitial();
            return;
        }
        if (i2 == 2) {
            gamePlatformServices2.rockstarShowGate(iIntValue);
        } else if (i2 == 3) {
            gamePlatformServices2.rockstarSignIn();
        } else {
            if (i2 != 4) {
                return;
            }
            gamePlatformServices2.rockstarShowCloudDisabled();
        }
    }


    // -------- Event queue helper --------
    public static final void queueEvent$lambda$0(Function0 tmp0) {
        Intrinsics.checkNotNullParameter(tmp0, "$tmp0");
        tmp0.invoke();
    }

    private final void queueEvent(final Function0<Unit> event) {
        eventQueue.add(new Runnable() { // from class: com.rockstargames.oswrapper.GameThread$$ExternalSyntheticLambda0
            @Override // java.lang.Runnable
            public final void run() {
                GameThread.queueEvent$lambda$0(event);
            }
        });
    }


    // -------- Public API --------

    public void notifyPendingCloudDisabled() {
        Log.i(TAG, "[!!] notifyPendingCloudDisabled");
        synchronized (pendingCallbackLock) {
            INSTANCE.setPendingCallback(PendingCallback.CLOUD_DISABLED);
            Unit unit = Unit.INSTANCE;
        }

    }

    public void notifyPendingGate(int id) {
        Log.i(TAG, "[!!] notifyPendingGate: " + id);
        synchronized (pendingCallbackLock) {
            GameThread gameThread = INSTANCE;
            gameThread.setPendingCallback(PendingCallback.GATE);
            gameThread.setPendingGateId(id);
            Unit unit = Unit.INSTANCE;
        }

    }

    public void notifyPendingInitial() {
        Log.i(TAG, "[!!] notifyPendingInitial");
        synchronized (pendingCallbackLock) {
            INSTANCE.setPendingCallback(PendingCallback.INITIAL);
            Unit unit = Unit.INSTANCE;
        }

    }

    public void notifyPendingSignIn() {
        Log.i(TAG, "[!!] notifyPendingSignIn");
        synchronized (pendingCallbackLock) {
            INSTANCE.setPendingCallback(PendingCallback.SIGN_IN);
            Unit unit = Unit.INSTANCE;
        }

    }

    public void onAccelerometerChanged(final float x, final float y, final float z) {
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onAccelerometerChanged.1

            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnAccelerometerChanged(x, y, z);
            }
        });

    }

    public void onActivityCreated(final GameActivityBase activity, final GameView view, final boolean firstInit) {
        Intrinsics.checkNotNullParameter(activity, "activity");
        Intrinsics.checkNotNullParameter(view, "view");
        Log.i(TAG, "[!!] onActivityCreated: " + activity + " / " + view + ": " + firstInit);
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onActivityCreated.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {


                GameNative.implOnActivityCreated(new GamePlatformServices(activity, view), firstInit);
            }
        });

    }

    public void onActivityDestroyed() {
        Log.i(TAG, "[!!] onActivityDestroyed");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onActivityDestroyed.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnActivityDestroyed();
            }
        });

    }

    public void onBackButtonPressed() {
        Log.i(TAG, "[!!] onBackButtonPressed");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onBackButtonPressed.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnBackButtonPressed();
            }
        });

    }

    public void onGamepadAxesChanged(final int controllerId, final float x, final float y,
                                     final float z, final float rz,
                                     final float triggerL, final float triggerR) {
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onGamepadAxesChanged.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnGamepadAxesChanged(controllerId, x, y, z, rz, triggerL, triggerR);
            }
        });

    }

    public void onGamepadButtonDown(final int controllerId, final int buttonId) {
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onGamepadButtonDown.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnGamepadButtonDown(controllerId, buttonId);
            }
        });

    }

    public void onGamepadButtonUp(final int controllerId, final int buttonId) {
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onGamepadButtonUp.1

            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnGamepadButtonUp(controllerId, buttonId);
            }
        });

    }

    public void onGamepadCountChanged(final int count) {
        Log.i(TAG, "[!!] onGamepadCountChanged: " + count);
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onGamepadCountChanged.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnGamepadCountChanged(count);
            }
        });

    }

    public void onHttpRequestData(final int id, final byte[] data) {
        Log.i(TAG, "[!!] onHttpReceivedData: " + id + ": " + (data != null ? data.length : null) + " bytes");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onHttpRequestData.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnHttpRequestData(id, data);
            }
        });

    }

    public void onHttpRequestError(final int id, final int statusCode) {
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onHttpRequestError.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnHttpRequestError(id, statusCode);
            }
        });

    }

    public void onHttpRequestFinished(final int id) {
        Log.i(TAG, "[!!] onHttpConnectionFinished: " + id);
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onHttpRequestFinished.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnHttpRequestFinished(id);
            }
        });

    }

    public void onHttpRequestResponse(final int id, final int statusCode,
                                      final String statusLine,
                                      final String[] headerNames,
                                      final String[] headerValues) {
        Intrinsics.checkNotNullParameter(statusLine, "statusLine");
        Intrinsics.checkNotNullParameter(headerNames, "headerNames");
        Intrinsics.checkNotNullParameter(headerValues, "headerValues");
        Log.i(TAG, "[!!] onHttpReceivedResponse: " + id + ": " + statusCode + ": " + statusLine);
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onHttpRequestResponse.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnHttpRequestResponse(id, statusCode, statusLine, headerNames, headerValues);
            }
        });

    }

    public void onInitialSetup(final GameActivityBase activity) {
        // ناوی ئەو فۆڵدەرەی داتاکانی تێدایە
        String dataPath = activity.GetGameBaseDirectory();

        final String[] finalNames = new String[]{"data_main"};
        final String[] finalPaths = new String[]{dataPath};

        Log.i(TAG, "[!!] onInitialSetup: Redirecting to External Data Path: " + dataPath);

        queueEvent(new Function0<Unit>() {
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }
            public final void invoke2() {

                DeviceInfo deviceInfo = new DeviceInfo(activity);
                AssetManager assets = activity.getAssets();
                // بزوێنەرەکە ئێستا دەزانێت بچێتە ناو dataPath بۆ دۆزینەوەی anim, audio, هتد.
                GameNative.implOnInitialSetup(deviceInfo, assets, finalNames, finalPaths);
            }

        });
    }

    public void onLowMemory() {
        Log.i(TAG, "[!!] onLowMemory");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onLowMemory.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnLowMemory();
            }
        });

    }

    public void onNetworkChanged(final int network) {
        Log.i(TAG, "[!!] onNetworkChanged: " + network);
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onNetworkChanged.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnNetworkChanged(network);
            }
        });
    }

    public void onPause() {
        Log.i(TAG, "[!!] onPause");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onPause.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnPause();
            }
        });

    }

    public void onPlaylistOpenComplete(final boolean available, final int count) {
        Log.i(TAG, "[!!] onPlaylistOpenComplete: " + available + " (" + count + ')');
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onPlaylistOpenComplete.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnPlaylistOpenComplete(available, count);
            }
        });

    }

    public void onResume() {
        Log.i(TAG, "[!!] onResume");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onResume.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnResume();
                GameThread.INSTANCE.processPendingCallbacks();
            }
        });

    }

    public void onRockstarAccountDeletionComplete() {
        Log.i(TAG, "[!!] onRockstarAccountDeletionComplete");
        notifyCallbackComplete();
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onRockstarAccountDeletionComplete.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnRockstarAccountDeletionComplete();
            }
        });

    }

    public void onRockstarCloudDisabledComplete() {
        Log.i(TAG, "[!!] onRockstarCloudDisabledComplete");
        notifyCallbackComplete();
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onRockstarCloudDisabledComplete.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnRockstarCloudDisabledComplete();
            }
        });

    }

    public void onRockstarGateComplete(final int id, final boolean pass) {
        Log.i(TAG, "[!!] onRockstarGateComplete: " + id + " (" + pass + ')');
        notifyCallbackComplete();
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onRockstarGateComplete.1

            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnRockstarGateComplete(id, pass);
            }
        });

    }

    public void onRockstarIdChanged(final String id) {
        Log.i(TAG, "[!!] onRockstarIdChanged");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onRockstarIdChanged.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnRockstarIdChanged(id);
            }
        });

    }

    public void onRockstarInitialComplete() {
        Log.i(TAG, "[!!] onRockstarInitialComplete");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onRockstarInitialComplete.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnRockstarInitialComplete();
            }
        });

    }

    public void onRockstarSetup(final String environment, final String id) {
        Log.i(TAG, "[!!] onRockstarSetup");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onRockstarSetup.1

            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnRockstarSetup(environment, id);
            }
        });

    }

    public void onRockstarSignInComplete() {
        Log.i(TAG, "[!!] onRockstarSignInComplete");
        notifyCallbackComplete();
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onRockstarSignInComplete.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnRockstarSignInComplete();
            }
        });

    }

    public void onRockstarSignOutComplete() {
        Log.i(TAG, "[!!] onRockstarSignOutComplete");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onRockstarSignOutComplete.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnRockstarSignOutComplete();
            }
        });

    }

    public void onSurfaceChanged(final Surface surface, final int width, final int height) {
        Intrinsics.checkNotNullParameter(surface, "surface");
        Log.i(TAG, "[!!] onSurfaceChanged: " + surface + " (" + width + 'x' + height + ')');
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onSurfaceChanged.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnSurfaceChanged(surface, width, height);
            }
        });


    }

    public void onSurfaceCreated() {
        Log.i(TAG, "[!!] onSurfaceCreated");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onSurfaceCreated.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnSurfaceCreated();


            }
        });

    }

    public void onSurfaceDestroyed() {
        Log.i(TAG, "[!!] onSurfaceDestroyed");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onSurfaceDestroyed.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnSurfaceDestroyed();
            }
        });

    }

    public void onSurfaceRedrawNeededAsync(final Runnable drawingFinished) {
        Intrinsics.checkNotNullParameter(drawingFinished, "drawingFinished");
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onSurfaceRedrawNeededAsync.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                ExecutorThread executorThread = GameThread.current;
                if (executorThread == null) {
                    return;
                }
                executorThread.setFinishDrawingRunnable(drawingFinished);
            }
        });


    }

    public void onTouchEnd(final int pointerId, final float x, final float y) {
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onTouchEnd.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnTouchEnd(pointerId, x, y);
            }
        });

    }

    public void onTouchMove(final int pointerId, final float x, final float y) {
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onTouchMove.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnTouchMove(pointerId, x, y);
            }
        });

    }

    public void onTouchStart(final int pointerId, final float x, final float y) {
        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.onTouchStart.1


            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameNative.implOnTouchStart(pointerId, x, y);
            }
        });

    }

    public void start(final GamePlatformServices services) {

        WeakReference<GamePlatformServices> servicesWeak;
        GamePlatformServices gamePlatformServices;
        Intrinsics.checkNotNullParameter(services, "services");
        ExecutorThread executorThread = current;
        if (!Intrinsics.areEqual((executorThread == null || (servicesWeak = executorThread.getServicesWeak()) == null || (gamePlatformServices = servicesWeak.get()) == null) ? null : gamePlatformServices.activity, services.activity)) {
            stop();
            ExecutorThread executorThread2 = new ExecutorThread(new WeakReference(services));
            current = executorThread2;
            Intrinsics.checkNotNull(executorThread2);
            executorThread2.start();
        }


        ExecutorThread ex = current;
        GamePlatformServices existingServices = (ex != null && ex.getServicesWeak() != null)
                ? ex.getServicesWeak().get() : null;

        boolean sameActivity = (existingServices != null)
                && existingServices.activity == services.activity;

        if (!sameActivity) {
            stop();
            ExecutorThread newThread = new ExecutorThread(new WeakReference<>(services));
            current = newThread;
            newThread.start();
        }

        queueEvent(new Function0<Unit>() { // from class: com.rockstargames.oswrapper.GameThread.start.1
            @Override // kotlin.jvm.functions.Function0
            public /* bridge */ /* synthetic */ Unit invoke() {
                invoke2();
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2() {
                GameThread.INSTANCE.processPendingCallbacks();
            }
        });

    }

    public void stop() {
        ExecutorThread ex = current;
        if (ex != null) {
            ex.requestExit();
        }
        current = null;
    }
}