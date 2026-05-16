package com.rockstargames.oswrapper;

import android.content.res.AssetManager;
import android.view.Surface;

import com.bytedance.shadowhook.ShadowHook;


public class GameNative {
    public static GameNative INSTANCE = new GameNative();

    static {
        ShadowHook.init(new ShadowHook.ConfigBuilder()
                .setMode(ShadowHook.Mode.UNIQUE)
                .build());
        System.loadLibrary("Game");
        System.loadLibrary("samp");
    }

    private GameNative() {
    }

    public static native void initializeSAMP();

    public static native boolean implIsInitialized();


    public static   native void implOnAccelerometerChanged(float x, float y, float z);

     
    public static   native void implOnActivityCreated(GamePlatformServices services, boolean firstInit);

     
    public static   native void implOnActivityDestroyed();

     
    public static   native void implOnBackButtonPressed();

     
    public static   native void implOnDrawFrame(float dt);

     
    public static   native void implOnGamepadAxesChanged(int controllerId, float x, float y, float z, float rz, float triggerL, float triggerR);

     
    public static   native void implOnGamepadButtonDown(int controllerId, int buttonId);

     
    public static   native void implOnGamepadButtonUp(int controllerId, int buttonId);

     
    public static   native void implOnGamepadCountChanged(int count);

     
    public static   native void implOnHttpRequestData(int id, byte[] data);

     
    public static   native void implOnHttpRequestError(int id, int statusCode);

     
    public static   native void implOnHttpRequestFinished(int id);

     
    public static   native void implOnHttpRequestResponse(int id, int statusCode, String statusLine, String[] headerNames, String[] headerValues);

     
    public static   native void implOnInitialSetup(DeviceInfo deviceInfo, AssetManager assets, String[] assetPackNames, String[] assetPackPaths);

     
    public static   native void implOnLowMemory();

     
    public static   native void implOnNetworkChanged(int network);

     
    public static   native void implOnPause();

     
    public static   native void implOnPlaylistOpenComplete(boolean available, int count);

     
    public static   native void implOnResume();

     
    public static   native void implOnRockstarAccountDeletionComplete();

     
    public static   native void implOnRockstarCloudDisabledComplete();

     
    public static   native void implOnRockstarGateComplete(int id, boolean pass);

     
    public static   native void implOnRockstarIdChanged(String id);

     
    public static   native void implOnRockstarInitialComplete();

     
    public static   native void implOnRockstarSetup(String environment, String rockstarId);

     
    public static   native void implOnRockstarSignInComplete();

     
    public static   native void implOnRockstarSignOutComplete();

     
    public static   native void implOnRockstarStateChanged(boolean pauseGameplay);

     
    public static   native void implOnRockstarTicketChanged(String ticket);

     
    public static   native void implOnSurfaceChanged(Surface surface, int width, int height);

     
    public static   native void implOnSurfaceCreated();

     
    public static   native void implOnSurfaceDestroyed();

     
    public static   native void implOnTouchEnd(int pointerId, float x, float y);

     
    public static   native void implOnTouchMove(int pointerId, float x, float y);

     
    public static   native void implOnTouchStart(int pointerId, float x, float y);
}
