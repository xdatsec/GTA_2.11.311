package com.rockstargames.oswrapper;

import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.util.Log;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AppCompatActivity;

import com.nvidia.devtech.NvAPKFileHelper;
import com.nvidia.devtech.NvUtil;
import com.rockstargames.gtasa.R;


import java.io.File;

public abstract class GameActivityBase extends AppCompatActivity {

    public static final String TAG = "GameActivityBase";

    protected ConnectivityManager connectivityManager;
    protected WifiManager wifiManager;
    public GamePlatformServices services;
    public GameView view;

    private int lastNetworkType;
    private ActivityResultLauncher<String> requestPermissionLauncher;
    private PermissionCallback requestPermissionCallback;

    // Simple callback interface replacing Kotlin Function1<Boolean, Unit>
    public interface PermissionCallback {
        void onResult(boolean granted);
    }

    // -----------------------------------------------------------------------
    // Getters / Setters
    // -----------------------------------------------------------------------

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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //SavesRestoring.DoSmth(this);
        //UnityPIayerNativeActivity.Init(this);
        //injectEvent();

        Log.i(TAG, "[!!] onCreate");
        super.onCreate(savedInstanceState);

        //AssetUtils.extractAssets(this);

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

        // 3. ناردنی زانیارییەکان بۆ کتێبخانەی گرافیک و ناوەکی
        NvUtil.getInstance().setActivity(this);
        NvUtil.getInstance().setAppLocalValue("STORAGE_ROOT", storagePath);
        NvUtil.getInstance().setAppLocalValue("STORAGE_ROOT_BASE", storageBase);
        NvAPKFileHelper.getInstance().setContext(this);

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

        setView((GameView) findViewById(R.id.viewGame));
        setServices(new GamePlatformServices(this, getView()));

        GameThread.INSTANCE.start(getServices());
        GameThread.INSTANCE.onInitialSetup(this);
        /*
        GameThread.INSTANCE.onRockstarSetup(
                Rockstar.socialClubEnvironment(),
                Rockstar.rockstarUserID()); */

        getView().setup(this);
        GameThread.INSTANCE.onActivityCreated(this, getView(), !initialized);
        onActivityReady(!initialized);
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