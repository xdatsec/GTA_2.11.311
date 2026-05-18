package com.rockstargames.oswrapper;

import static android.opengl.EGL14.EGL_CONTEXT_CLIENT_VERSION;
import static android.opengl.EGL15.EGL_OPENGL_ES3_BIT;
import static javax.microedition.khronos.egl.EGL10.EGL_RENDERABLE_TYPE;

import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AppCompatActivity;

import com.kurdish.roleplay.game.ui.Hud;
import com.kurdish.roleplay.game.ui.Speedometer;
import com.kurdish.roleplay.game.ui.tab.Tab;
import com.nvidia.devtech.NvAPKFile;
import com.nvidia.devtech.NvAPKFileHelper;
import com.nvidia.devtech.NvUtil;
import com.rockstargames.gtasa.MainActivity;
import com.kurdish.roleplay.R;


import java.io.File;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.opengles.GL11;
import android.view.SurfaceHolder.Callback;
import android.view.WindowManager;

import kotlin.Unit;
import kotlin.jvm.functions.Function1;
import kotlin.jvm.internal.Intrinsics;

public abstract class GameActivityBase extends AppCompatActivity {
    public Handler handler = null;
    protected boolean supportPauseResume = true;
    private boolean ranInit = false;
    public static final String TAG = "GameActivityBase";
    protected boolean ResumeEventDone = false;
    protected ConnectivityManager connectivityManager;
    protected WifiManager wifiManager;
    public GamePlatformServices services;
    public GameView view;
    private int lastNetworkType;
    private ActivityResultLauncher<String> requestPermissionLauncher;
    private PermissionCallback requestPermissionCallback;
    private static GameActivityBase instance;

    // Simple callback interface replacing Kotlin Function1<Boolean, Unit>
    public interface PermissionCallback {
        void onResult(boolean granted);
    }


    protected ConnectivityManager getConnectivityManager() {
        if (connectivityManager == null)
            throw new IllegalStateException("connectivityManager not initialized");
        return connectivityManager;
    }

    protected WifiManager getWifiManager() {
        if (wifiManager == null)
            throw new IllegalStateException("wifiManager not initialized");
        return wifiManager;
    }

    public GamePlatformServices getServices() {
        if (services == null)
            throw new IllegalStateException("services not initialized");
        return services;
    }

    public GameView getView() {
        if (view == null)
            throw new IllegalStateException("view not initialized");
        return view;
    }

    public void setServices(GamePlatformServices s) {
        if (s == null) throw new NullPointerException("services is null");
        this.services = s;
    }

    public void setView(GameView v) {
        if (v == null) throw new NullPointerException("view is null");
        this.view = v;
    }

    protected void setConnectivityManager(ConnectivityManager cm) {
        if (cm == null) throw new NullPointerException("connectivityManager is null");
        this.connectivityManager = cm;
    }

    protected void setWifiManager(WifiManager wm) {
        if (wm == null) throw new NullPointerException("wifiManager is null");
        this.wifiManager = wm;
    }

    protected int getLastNetworkType() { return lastNetworkType; }

    // -----------------------------------------------------------------------
    // Network helpers
    // -----------------------------------------------------------------------

    protected boolean isNetworkAvailable() {
        NetworkInfo info = getConnectivityManager().getActiveNetworkInfo();
        return info != null && info.isAvailable() && info.isConnected();
    }

    protected boolean isWifiAvailable() {
        return getWifiManager().isWifiEnabled();
    }

    protected int getCurrentNetworkType() {
        if (!isNetworkAvailable()) return 0;
        return isWifiAvailable() ? 2 : 1;
    }

    private void checkNetworkType() {
        int current = getCurrentNetworkType();
        if (current != lastNetworkType) {
            lastNetworkType = current;
            GameThread.INSTANCE.onNetworkChanged(lastNetworkType);
        }
    }

    // -----------------------------------------------------------------------
    // UI helpers
    // -----------------------------------------------------------------------

    private void hideSystemUI() {
        getWindow().getDecorView().setSystemUiVisibility(5894);
    }

    // -----------------------------------------------------------------------
    // Overridable hooks
    // -----------------------------------------------------------------------

    protected void onActivitySetup() {}
    protected void onActivityReady(boolean firstInit) {}

    // -----------------------------------------------------------------------
    // Inject event
    // -----------------------------------------------------------------------

    // -----------------------------------------------------------------------
    // Lifecycle
    // -----------------------------------------------------------------------

    private String baseDirectory;
    private String baseDirectoryRoot;

    public String GetGameBaseDirectory() {
        // Standard path: /storage/emulated/0/Documents/SampMobile/
        File documentsDir = new File("/storage/emulated/0/Android/media/");
        File gameDir = new File(documentsDir, "com.kurdish.roleplay");

        try {
            if (!gameDir.exists()) {
                boolean created = gameDir.mkdirs();

            }
            return gameDir.getAbsolutePath() + "/";
        } catch (Exception e) {

            return "";
        }
    }
    public static GameActivityBase getInstance() {
        return instance;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //SavesRestoring.DoSmth(this);
        //UnityPIayerNativeActivity.Init(this);
        //injectEvent();
        instance = this;

        Log.i(TAG, "[!!] onCreate");
        super.onCreate(savedInstanceState);

        //AssetUtils.extractAssets(this);
        handler = new Handler();
        // Resolve storage paths
        File extDir = getExternalFilesDir(null);
        if (extDir == null) throw new IllegalStateException("getExternalFilesDir returned null");
        String storagePath = extDir.getAbsolutePath();

        // دۆزینەوەی storageBase بۆ ئەوەی ئیرۆر نەدات
        int androidIdx = storagePath.indexOf("/Android");
        if (androidIdx < 0) throw new IllegalStateException("Cannot find /Android in path: " + storagePath);
        String storageBase = storagePath.substring(0, androidIdx);

        Log.i(TAG, "[!!] onCreate: STORAGE_ROOT = " + storagePath
                + " STORAGE_ROOT_BASE = " + storageBase);
        this.baseDirectory = GetGameBaseDirectory();

        // 3. ناردنی زانیارییەکان بۆ کتێبخانەی گرافیک و ناوەکی
        NvUtil.getInstance().setActivity(this);
        NvUtil.getInstance().setAppLocalValue("STORAGE_ROOT", storagePath);
        NvUtil.getInstance().setAppLocalValue("STORAGE_ROOT_BASE", storageBase);
        NvAPKFileHelper.getInstance().setContext(this);
        NvAPKFile file = new NvAPKFile();
        file.is = null;
        try {
            Intent intent = getIntent();

            GameThread.INSTANCE.onInitialSetup(this);
        } catch (UnsatisfiedLinkError e) {
        }


        onActivitySetup();

        boolean initialized = GameNative.implIsInitialized();
        Log.i(TAG, "[!!] onCreate: initialized = " + initialized);

        // 4. خزمەتگوزارییەکانی سیستەم
        setWifiManager((WifiManager) getSystemService(WifiManager.class));
        setConnectivityManager((ConnectivityManager) getSystemService(ConnectivityManager.class));

        // Network callback
        getConnectivityManager().registerDefaultNetworkCallback(new ConnectivityManager.NetworkCallback() {
            @Override
            public void onAvailable(Network network) {
                Log.i(TAG, "[!!] ConnectivityManager: onAvailable: " + network);
                checkNetworkType();
            }

            @Override
            public void onLost(Network network) {
                Log.i(TAG, "[!!] ConnectivityManager: onLost: " + network);
                checkNetworkType();
            }

            @Override
            public void onUnavailable() {
                Log.i(TAG, "[!!] ConnectivityManager: onUnavailable");
                checkNetworkType();
            }
        });

        // Permission launcher
        requestPermissionLauncher = registerForActivityResult(
                new ActivityResultContracts.RequestPermission(),
                granted -> {
                    Log.i(TAG, "[!!] requestPermission: ActivityResultLauncher: result = " + granted);
                    if (requestPermissionCallback != null) {
                        requestPermissionCallback.onResult(granted);
                    }
                    requestPermissionCallback = null;
                });

        /*
        // Rockstar SDK setup (only once)
        if (!initialized) {
            Log.i(TAG, "[!!] SDK setup");
            final GameThread gt = GameThread.INSTANCE;
            Rockstar.setup(
                    this,
                    () -> gt.onRockstarStateChanged(),
                    ticket -> gt.onRockstarTicketChanged(ticket)
            );
        } */

        hideSystemUI();
        setContentView(R.layout.game);

        // 1. Get the correct class directly (Matches XML)
        view = findViewById(R.id.viewGame);

// 2. Performance settings
        getWindow().setSustainedPerformanceMode(true);

        SurfaceHolder holder = view.getHolder();
        holder.setKeepScreenOn(true); // Keep only the necessary calls

        holder.addCallback(new SurfaceHolder.Callback2() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                Log.i("GL", "Surface Created");

                // Update the thread's reference immediately
                GameThread.INSTANCE.cachedSurfaceHolder = holder;
                GameThread.INSTANCE.viewIsActive = true;

                // Dynamic Resolution Scaling for Speed
                if (GameThread.INSTANCE.fixedWidth > 0) {
                    holder.setFixedSize(GameThread.INSTANCE.fixedWidth, GameThread.INSTANCE.fixedHeight);
                }
                GameThread.INSTANCE.InitEGLAndGLES2(3);
                // Trigger the native engine initialization
                // Note: Ensure ResumeEventDone logic is handled safely
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                GameThread.INSTANCE.surfaceWidth = width;
                GameThread.INSTANCE.surfaceHeight = height;
                // Optionally notify native engine of resolution change
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.i("GL", "Surface Destroyed");
                GameThread.INSTANCE.viewIsActive = false;
                GameThread.INSTANCE.destroyEGLSurface();

                // CRITICAL: Clear the reference so we don't use a dead surface later
                GameThread.INSTANCE.cachedSurfaceHolder = null;
            }

            @Override
            public void surfaceRedrawNeeded(SurfaceHolder holder) {
                // Callback2 requirement - helpful for rotation smoothness
            }
        });
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setServices(new GamePlatformServices(this, getView()));

        GameThread.INSTANCE.start(getServices());

        /*
        GameThread.INSTANCE.onRockstarSetup(
                Rockstar.socialClubEnvironment(),
                Rockstar.rockstarUserID()); */

        getView().setup(this);
        GameThread.INSTANCE.onActivityCreated(this, getView(), !initialized);
        onActivityReady(!initialized);

       // try {
       //     GameNative.initializeSAMP();
       // } catch (UnsatisfiedLinkError e5) {
       //     Log.e(TAG, e5.getMessage());
       // }
        //GameThread.INSTANCE.onRockstarGateComplete(1, true);
       // GameThread.INSTANCE.onRockstarGateComplete(2, true);
        //GameThread.INSTANCE.onRockstarGateComplete(3, true);
    }


    public boolean getSupportPauseResume() {
        return supportPauseResume;
    }
    @Override
    protected void onDestroy() {
        Log.i(TAG, "[!!] onDestroy");
        super.onDestroy();
        GameThread.INSTANCE.onActivityDestroyed();
        GameThread.INSTANCE.stop();
    }

    @Override
    public void onAttachedToWindow() {
        Log.i(TAG, "[!!] onAttachedToWindow");
        super.onAttachedToWindow();
        GameThread.INSTANCE.start(getServices());
    }

    @Override
    public void onDetachedFromWindow() {
        Log.i(TAG, "[!!] onDetachedFromWindow");
        super.onDetachedFromWindow();
        GameThread.INSTANCE.stop();
    }

    @Override
    public void onLowMemory() {
        Log.i(TAG, "[!!] onLowMemory");
        super.onLowMemory();
        GameThread.INSTANCE.onLowMemory();
    }

    @Override
    protected void onPause() {
        Log.i(TAG, "[!!] onPause");
        super.onPause();
        getView().onPause();
    }

    @Override
    protected void onResume() {
        Log.i(TAG, "[!!] onResume");
        super.onResume();
        hideSystemUI();
        getView().onResume();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        Log.i(TAG, "[!!] onWindowFocusChanged: " + hasFocus);
        super.onWindowFocusChanged(hasFocus);
        hideSystemUI();
    }

    // -----------------------------------------------------------------------
    // Permission request
    // -----------------------------------------------------------------------

    public void requestPermission(String permission, PermissionCallback callback) {
        if (permission == null) throw new NullPointerException("permission is null");
        if (callback   == null) throw new NullPointerException("callback is null");
        Log.i(TAG, "[!!] requestPermission: " + permission);
        this.requestPermissionCallback = callback;
        if (requestPermissionLauncher == null)
            throw new IllegalStateException("requestPermissionLauncher not initialized");
        requestPermissionLauncher.launch(permission);
    }
}