package com.kurdish.roleplay.launcher;

import android.Manifest;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.app.ActivityCompat;

import com.google.firebase.crashlytics.FirebaseCrashlytics;
import com.joom.paranoid.Obfuscate;
import com.kurdish.roleplay.R;
import com.kurdish.roleplay.launcher.util.Config;
import com.kurdish.roleplay.launcher.util.Util;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

@Obfuscate
public class SplashActivity extends AppCompatActivity {

    public int mGpuType;

    private AlertDialog permissionDialog;
    private boolean launcherStarted = false;

    SharedPreferences prefs = null;
    AlertDialog.Builder builder;

    public IncomingHandler mInHandler;
    public Messenger mMessenger;
    public Messenger mService;

    private boolean mIsBind;

    private static final int REQ_STORAGE = 2000;

    // 🔵 FIX #1 — دوبارە زیادکرا
    private final ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            mService = new Messenger(iBinder);
            checkUpdate();
        }

        public void onServiceDisconnected(ComponentName componentName) {
            mService = null;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        initializeAnimations();   // 🔵 FIX #2 — فانکشنی زیادکرا

        Config.currentContext = this;
        prefs = getSharedPreferences("com.kurdish.roleplay", MODE_PRIVATE);
        builder = new AlertDialog.Builder(this);

        requestStoragePermission();
    }

    // ------------------------------------------------------
    // PERMISSION CHECK
    // ------------------------------------------------------

    private boolean hasStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            return Environment.isExternalStorageManager();
        } else {
            return ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) ==
                    PackageManager.PERMISSION_GRANTED &&
                    ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) ==
                            PackageManager.PERMISSION_GRANTED;
        }
    }

    private void requestStoragePermission() {
        if (hasStoragePermission()) {
            continueLauncher();
            return;
        }
        showModernPermissionDialog();
    }

    private void showModernPermissionDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this, R.style.CustomDialog);
        View view = getLayoutInflater().inflate(R.layout.dialog_storage, null);
        builder.setView(view);
        builder.setCancelable(false);

        permissionDialog = builder.create();
        if (permissionDialog.getWindow() != null) {
            permissionDialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);
        }

        Button btnAllow = view.findViewById(R.id.btnAllow);

        btnAllow.setOnClickListener(v -> {

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                try {
                    Intent intent = new Intent(android.provider.Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                    intent.setData(android.net.Uri.parse("package:" + getPackageName()));
                    startActivity(intent);
                } catch (Exception e) {
                    Intent intent = new Intent(android.provider.Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                    startActivity(intent);
                }
            } else {
                ActivityCompat.requestPermissions(
                        this,
                        new String[]{Manifest.permission.READ_EXTERNAL_STORAGE,
                                Manifest.permission.WRITE_EXTERNAL_STORAGE},
                        REQ_STORAGE
                );
            }
        });

        permissionDialog.show();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == REQ_STORAGE) {
            if (grantResults.length > 0 &&
                    grantResults[0] == PackageManager.PERMISSION_GRANTED &&
                    grantResults[1] == PackageManager.PERMISSION_GRANTED) {

                if (permissionDialog != null) permissionDialog.dismiss();
                continueLauncher();
            }
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (!launcherStarted && hasStoragePermission()) {
            if (permissionDialog != null) permissionDialog.dismiss();
            continueLauncher();
        }
    }

    // ------------------------------------------------------
    // START LAUNCHER
    // ------------------------------------------------------

    private void continueLauncher() {

        if (launcherStarted) return;
        launcherStarted = true;

        if (!Util.isNetworkConnected(Config.currentContext)) {

            builder.setMessage("There is no internet connection.\n" +
                            "Please exit, connect to the internet and reenter the launcher")
                    .setCancelable(false)
                    .setPositiveButton("Ok", (dialog, id) -> {
                        FirebaseCrashlytics.getInstance().setCrashlyticsCollectionEnabled(false);
                        finishAndRemoveTask();
                        System.exit(0);
                        dialog.dismiss();
                    });
            AlertDialog alert = builder.create();
            alert.setTitle("Info");
            alert.show();
            return;
        }

        mInHandler = new IncomingHandler();
        mMessenger = new Messenger(mInHandler);

        GLSurfaceView.Renderer mGlRenderer = new GLSurfaceView.Renderer() {

            @Override
            public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

                String glExt = gl10.glGetString(GL10.GL_EXTENSIONS);

                if (glExt.contains("GL_IMG_texture_compression_pvrtc")) {
                    mGpuType = 3;
                } else if (glExt.contains("GL_EXT_texture_compression_dxt1") ||
                        glExt.contains("GL_EXT_texture_compression_s3tc") ||
                        glExt.contains("GL_AMD_compressed_ATC_texture")) {
                    mGpuType = 1;
                } else {
                    mGpuType = 2;
                }

                runOnUiThread(() -> {
                    new Handler(Looper.getMainLooper()).postDelayed(() -> {
                        Intent intent = new Intent(SplashActivity.this, UpdateService.class);
                        mIsBind = bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
                    }, 4000);
                });
            }

            @Override public void onSurfaceChanged(GL10 gl10, int i, int i1) {}
            @Override public void onDrawFrame(GL10 gl10) {}
        };

        ConstraintLayout gpuLayout = findViewById(R.id.gpu);
        GLSurfaceView mGlSurfaceView = new GLSurfaceView(this);
        mGlSurfaceView.setRenderer(mGlRenderer);
        gpuLayout.addView(mGlSurfaceView);
    }

    // ------------------------------------------------------
    // GPU Animations
    // ------------------------------------------------------

    // 🔵 FIX #3 — initializeAnimations() back
    private void initializeAnimations() {
        ImageView appLogo = findViewById(R.id.appLogo);
        Animation fadeIn = AnimationUtils.loadAnimation(this, R.anim.fade_in);
        appLogo.startAnimation(fadeIn);

        Animation slideUp = AnimationUtils.loadAnimation(this, R.anim.slide_up);
        findViewById(R.id.appNameLayout).startAnimation(slideUp);

        ImageView loadingIcon = findViewById(R.id.loadingIcon);
        Animation rotateAnimation = AnimationUtils.loadAnimation(this, R.anim.rotate_animation);
        loadingIcon.startAnimation(rotateAnimation);

        Animation fadeInDelay = AnimationUtils.loadAnimation(this, R.anim.fade_in_delay);
        findViewById(R.id.taglineText).startAnimation(fadeInDelay);
    }

    // ------------------------------------------------------
    // Update Handler
    // ------------------------------------------------------

    public class IncomingHandler extends Handler {
        public IncomingHandler() {}

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            if (msg.what == 4) {
                UpdateActivity.UpdateStatus valueOf =
                        UpdateActivity.UpdateStatus.valueOf(msg.getData().getString("status", ""));

                if (valueOf == UpdateActivity.UpdateStatus.Undefined) {
                    Message obtain = Message.obtain(null, 5);
                    obtain.replyTo = mMessenger;
                    try {
                        mService.send(obtain);
                    } catch (RemoteException ignored) {}
                } else if (valueOf == UpdateActivity.UpdateStatus.CheckUpdate) {
                    Message obtain2 = Message.obtain(null, 4);
                    obtain2.replyTo = mMessenger;
                    try {
                        mService.send(obtain2);
                    } catch (RemoteException ignored) {}
                }
            }
            else if (msg.what == 5) {

                UpdateActivity.GameStatus status =
                        UpdateActivity.GameStatus.valueOf(msg.getData().getString("status", ""));

                if (status == UpdateActivity.GameStatus.UpdateRequired) {
                    startUpdate(UpdateActivity.UpdateMode.GameDataUpdate);
                }
                else if (status == UpdateActivity.GameStatus.GameUpdateRequired) {
                    startUpdate(UpdateActivity.UpdateMode.GameUpdate);
                }
                else {
                    openMain();
                }
            }
        }
    }

    private void startUpdate(UpdateActivity.UpdateMode mode) {
        Intent intent = new Intent(SplashActivity.this, UpdateActivity.class);
        intent.putExtra("mode", mode.name());
        startActivity(intent);
        overridePendingTransition(R.anim.fade_in, R.anim.fade_out);
        finish();
    }

    private void openMain() {
        startActivity(new Intent(SplashActivity.this, MainActivity.class));
        overridePendingTransition(R.anim.fade_in, R.anim.fade_out);
        finish();
    }

    public void checkUpdate() {
        Message obtain = Message.obtain(null, 0);
        obtain.getData().putInt("gputype", mGpuType);
        obtain.replyTo = mMessenger;
        try {
            mService.send(obtain);
        } catch (RemoteException ignored) {}
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mIsBind) {
            unbindService(mConnection);
            mIsBind = false;
        }
    }

    @Override public void onBackPressed() {}
}
