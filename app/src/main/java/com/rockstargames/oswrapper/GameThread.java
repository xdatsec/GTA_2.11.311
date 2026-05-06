package com.rockstargames.oswrapper;

import android.content.res.AssetManager;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;
import com.rockstargames.oswrapper.view.InputHandler;
import java.lang.ref.WeakReference;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public final class GameThread {

    private static final String TAG = "GameThread";
    public static final GameThread INSTANCE = new GameThread();

    private static ExecutorThread current;
    private static int pendingGateId;
    private static final ConcurrentLinkedQueue<Runnable> eventQueue = new ConcurrentLinkedQueue<>();
    private static final Object pendingCallbackLock = new Object();
    private static int pendingCallback = PendingCallback.NONE;

    // -------- FIX #3 & #4: PendingCallback as int constants instead of inner enum --------
    static final class PendingCallback {
        static final int NONE          = 0;
        static final int INITIAL       = 1;
        static final int GATE          = 2;
        static final int SIGN_IN       = 3;
        static final int CLOUD_DISABLED = 4;

        private PendingCallback() {}
    }

    private GameThread() {}

    // -------- FIX #1: r3 -> SystemClock.elapsedRealtime() --------
    private static final class ExecutorThread extends Thread {
        private final AtomicBoolean exitRequested = new AtomicBoolean(false);
        private Runnable finishDrawingRunnable;
        private long lastFrameTime;
        private final WeakReference<GamePlatformServices> servicesWeak;

        ExecutorThread(WeakReference<GamePlatformServices> servicesWeak) {
            this.servicesWeak = servicesWeak;
            setName(TAG);
        }

        private void guardedRun() {
            this.lastFrameTime = SystemClock.elapsedRealtime();
            int lastControllerCount = -1;

            while (!this.exitRequested.get()) {
                // Drain event queue
                Runnable r;
                while ((r = eventQueue.poll()) != null) {
                    r.run();
                }

                GamePlatformServices services = this.servicesWeak.get();
                GameView gameView = (services != null) ? services.view : null;

                // FIX #1: was `r3` (bad decompile) — should be elapsedRealtime()
                long now = SystemClock.elapsedRealtime();
                float deltaSeconds = (now - this.lastFrameTime) / 1000.0f;
                this.lastFrameTime = now;

                boolean hasFocus = (gameView != null && gameView.getHasFocus());

                if (hasFocus && GameNative.implIsInitialized()) {
                    GameNative.implOnDrawFrame(deltaSeconds);

                    int controllerCount = InputHandler.INSTANCE.getControllerCount();
                    if (lastControllerCount != controllerCount) {
                        GameNative.implOnGamepadCountChanged(controllerCount);
                        lastControllerCount = controllerCount;
                    }
                }

                if (this.finishDrawingRunnable != null) {
                    this.finishDrawingRunnable.run();
                    this.finishDrawingRunnable = null;
                }
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

    private void setPendingCallback(int cb) {
        Log.d(TAG, "[!!] pendingCallback: " + cb);
        pendingCallback = cb;
    }

    private void setPendingGateId(int id) {
        Log.d(TAG, "[!!] pendingGateId: " + id);
        pendingGateId = id;
    }

    private void notifyCallbackComplete() {
        Log.i(TAG, "[!!] notifyCallbackComplete");
        synchronized (pendingCallbackLock) {
            setPendingCallback(PendingCallback.NONE);
        }
    }

    // FIX #3: switch on int constants — no more ordinal() / Enum issues
    private void processPendingCallbacks() {
        Log.i(TAG, "[!!] processPendingCallbacks");

        int cb;
        int gateId;

        synchronized (pendingCallbackLock) {
            cb = pendingCallback;
            gateId = pendingGateId;
            setPendingCallback(PendingCallback.NONE);
            setPendingGateId(0);

            if (cb != PendingCallback.NONE) {
                Log.i(TAG, "[!!] processPendingCallbacks: reissuing " + cb + " (gate id = " + gateId + ')');
            }
        }

        ExecutorThread thread = current;
        if (thread == null) return;

        GamePlatformServices services = thread.getServicesWeak().get();
        if (services == null) return;

        switch (cb) {
            case PendingCallback.INITIAL:
                services.rockstarShowInitial();
                break;
            case PendingCallback.GATE:
                services.rockstarShowGate(gateId);
                break;
            case PendingCallback.SIGN_IN:
                services.rockstarSignIn();
                break;
            case PendingCallback.CLOUD_DISABLED:
                services.rockstarShowCloudDisabled();
                break;
            default:
                break;
        }
    }

    // -------- Event queue helper --------

    private void queueEvent(final Runnable event) {
        eventQueue.add(event);
    }

    // -------- Public API --------

    public void notifyPendingCloudDisabled() {
        Log.i(TAG, "[!!] notifyPendingCloudDisabled");
        synchronized (pendingCallbackLock) {
            setPendingCallback(PendingCallback.CLOUD_DISABLED);
        }
    }

    public void notifyPendingGate(int id) {
        Log.i(TAG, "[!!] notifyPendingGate: " + id);
        synchronized (pendingCallbackLock) {
            setPendingCallback(PendingCallback.GATE);
            setPendingGateId(id);
        }
    }

    public void notifyPendingInitial() {
        Log.i(TAG, "[!!] notifyPendingInitial");
        synchronized (pendingCallbackLock) {
            setPendingCallback(PendingCallback.INITIAL);
        }
    }

    public void notifyPendingSignIn() {
        Log.i(TAG, "[!!] notifyPendingSignIn");
        synchronized (pendingCallbackLock) {
            setPendingCallback(PendingCallback.SIGN_IN);
        }
    }

    public void onAccelerometerChanged(final float x, final float y, final float z) {
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnAccelerometerChanged(x, y, z); }
        });
    }

    public void onActivityCreated(final GameActivityBase activity, final GameView view, final boolean firstInit) {
        Log.i(TAG, "[!!] onActivityCreated: " + activity + " / " + view + ": " + firstInit);
        queueEvent(new Runnable() {
            @Override public void run() {
                GameNative.implOnActivityCreated(new GamePlatformServices(activity, view), firstInit);
            }
        });
    }

    public void onActivityDestroyed() {
        Log.i(TAG, "[!!] onActivityDestroyed");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnActivityDestroyed(); }
        });
    }

    public void onBackButtonPressed() {
        Log.i(TAG, "[!!] onBackButtonPressed");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnBackButtonPressed(); }
        });
    }

    public void onGamepadAxesChanged(final int controllerId, final float x, final float y,
                                     final float z, final float rz,
                                     final float triggerL, final float triggerR) {
        queueEvent(new Runnable() {
            @Override public void run() {
                GameNative.implOnGamepadAxesChanged(controllerId, x, y, z, rz, triggerL, triggerR);
            }
        });
    }

    public void onGamepadButtonDown(final int controllerId, final int buttonId) {
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnGamepadButtonDown(controllerId, buttonId); }
        });
    }

    public void onGamepadButtonUp(final int controllerId, final int buttonId) {
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnGamepadButtonUp(controllerId, buttonId); }
        });
    }

    public void onGamepadCountChanged(final int count) {
        Log.i(TAG, "[!!] onGamepadCountChanged: " + count);
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnGamepadCountChanged(count); }
        });
    }

    public void onHttpRequestData(final int id, final byte[] data) {
        Log.i(TAG, "[!!] onHttpReceivedData: " + id + ": " + (data != null ? data.length : null) + " bytes");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnHttpRequestData(id, data); }
        });
    }

    public void onHttpRequestError(final int id, final int statusCode) {
        Log.i(TAG, "[!!] onHttpError: " + id + ": " + statusCode);
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnHttpRequestError(id, statusCode); }
        });
    }

    public void onHttpRequestFinished(final int id) {
        Log.i(TAG, "[!!] onHttpConnectionFinished: " + id);
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnHttpRequestFinished(id); }
        });
    }

    public void onHttpRequestResponse(final int id, final int statusCode,
                                      final String statusLine,
                                      final String[] headerNames,
                                      final String[] headerValues) {
        Log.i(TAG, "[!!] onHttpReceivedResponse: " + id + ": " + statusCode + ": " + statusLine);
        queueEvent(new Runnable() {
            @Override public void run() {
                GameNative.implOnHttpRequestResponse(id, statusCode, statusLine, headerNames, headerValues);
            }
        });
    }

    public void onInitialSetup(final GameActivityBase activity) {
        // ناوی ئەو فۆڵدەرەی داتاکانی تێدایە
        String dataPath = activity.getExternalFilesDir(null).getAbsolutePath();

        final String[] finalNames = new String[]{"data_main"};
        final String[] finalPaths = new String[]{dataPath};

        Log.i(TAG, "[!!] onInitialSetup: Redirecting to External Data Path: " + dataPath);

        queueEvent(new Runnable() {
            @Override public void run() {
                DeviceInfo deviceInfo = new DeviceInfo(activity);
                AssetManager assets = activity.getAssets();
                // بزوێنەرەکە ئێستا دەزانێت بچێتە ناو dataPath بۆ دۆزینەوەی anim, audio, هتد.
                GameNative.implOnInitialSetup(deviceInfo, assets, finalNames, finalPaths);
            }
        });
    }

    public void onLowMemory() {
        Log.i(TAG, "[!!] onLowMemory");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnLowMemory(); }
        });
    }

    public void onNetworkChanged(final int network) {
        Log.i(TAG, "[!!] onNetworkChanged: " + network);
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnNetworkChanged(network); }
        });
    }

    public void onPause() {
        Log.i(TAG, "[!!] onPause");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnPause(); }
        });
    }

    public void onPlaylistOpenComplete(final boolean available, final int count) {
        Log.i(TAG, "[!!] onPlaylistOpenComplete: " + available + " (" + count + ')');
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnPlaylistOpenComplete(available, count); }
        });
    }

    public void onResume() {
        Log.i(TAG, "[!!] onResume");
        queueEvent(new Runnable() {
            @Override public void run() {
                GameNative.implOnResume();
                GameThread.INSTANCE.processPendingCallbacks();
            }
        });
    }

    public void onRockstarAccountDeletionComplete() {
        Log.i(TAG, "[!!] onRockstarAccountDeletionComplete");
        notifyCallbackComplete();
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnRockstarAccountDeletionComplete(); }
        });
    }

    public void onRockstarCloudDisabledComplete() {
        Log.i(TAG, "[!!] onRockstarCloudDisabledComplete");
        notifyCallbackComplete();
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnRockstarCloudDisabledComplete(); }
        });
    }

    public void onRockstarGateComplete(final int id, final boolean pass) {
        Log.i(TAG, "[!!] onRockstarGateComplete: " + id + " (" + pass + ')');
        notifyCallbackComplete();
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnRockstarGateComplete(id, pass); }
        });
    }

    public void onRockstarIdChanged(final String id) {
        Log.i(TAG, "[!!] onRockstarIdChanged");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnRockstarIdChanged(id); }
        });
    }

    public void onRockstarInitialComplete() {
        Log.i(TAG, "[!!] onRockstarInitialComplete");
        notifyCallbackComplete();
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnRockstarInitialComplete(); }
        });
    }

    public void onRockstarSetup(final String environment, final String id) {
        Log.i(TAG, "[!!] onRockstarSetup");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnRockstarSetup(environment, id); }
        });
    }

    public void onRockstarSignInComplete() {
        Log.i(TAG, "[!!] onRockstarSignInComplete");
        notifyCallbackComplete();
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnRockstarSignInComplete(); }
        });
    }

    public void onRockstarSignOutComplete() {
        Log.i(TAG, "[!!] onRockstarSignOutComplete");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnRockstarSignOutComplete(); }
        });
    }

    public void onSurfaceChanged(final Surface surface, final int width, final int height) {
        Log.i(TAG, "[!!] onSurfaceChanged: " + surface + " (" + width + 'x' + height + ')');
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnSurfaceChanged(surface, width, height); }
        });
    }

    public void onSurfaceCreated() {
        Log.i(TAG, "[!!] onSurfaceCreated");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnSurfaceCreated(); }
        });
    }

    public void onSurfaceDestroyed() {
        Log.i(TAG, "[!!] onSurfaceDestroyed");
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnSurfaceDestroyed(); }
        });
    }

    public void onSurfaceRedrawNeededAsync(final Runnable drawingFinished) {
        queueEvent(new Runnable() {
            @Override public void run() {
                ExecutorThread t = GameThread.current;
                if (t != null) {
                    t.setFinishDrawingRunnable(drawingFinished);
                }
            }
        });
    }

    public void onTouchEnd(final int pointerId, final float x, final float y) {
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnTouchEnd(pointerId, x, y); }
        });
    }

    public void onTouchMove(final int pointerId, final float x, final float y) {
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnTouchMove(pointerId, x, y); }
        });
    }

    public void onTouchStart(final int pointerId, final float x, final float y) {
        queueEvent(new Runnable() {
            @Override public void run() { GameNative.implOnTouchStart(pointerId, x, y); }
        });
    }

    public void start(final GamePlatformServices services) {
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

        queueEvent(new Runnable() {
            @Override public void run() {
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