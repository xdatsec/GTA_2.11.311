package com.rockstargames.oswrapper.services;

import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.util.Log;
import com.rockstargames.oswrapper.GameActivityBase;

public final class MiscServices implements IMiscServices {
    private static final String TAG = "MiscServices";
    private final GameActivityBase activity;

    public MiscServices(GameActivityBase activity) {
        if (activity == null) {
            throw new IllegalArgumentException("activity cannot be null");
        }
        this.activity = activity;
    }

    public GameActivityBase getActivity() {
        return this.activity;
    }

    @Override
    public String getAppVersion() {
        String versionName;
        try {
            if (Build.VERSION.SDK_INT >= 33) {
                versionName = this.activity.getPackageManager()
                        .getPackageInfo(this.activity.getPackageName(), PackageManager.PackageInfoFlags.of(0))
                        .versionName;
            } else {
                versionName = this.activity.getPackageManager()
                        .getPackageInfo(this.activity.getPackageName(), 0)
                        .versionName;
            }
        } catch (Exception e) {
            versionName = "unknown";
        }

        Log.i(TAG, "[!!] getAppVersion: " + versionName);
        return versionName != null ? versionName : "unknown";
    }

    @Override
    public void openLink(final String url) {
        if (url == null) return;

        Log.i(TAG, "[!!] openLink: " + url);
        try {
            final Uri uri = Uri.parse(url);
            this.activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Intent intent = new Intent(Intent.ACTION_VIEW, uri);
                    activity.startActivity(intent);
                }
            });
        } catch (Exception e) {
            Log.e(TAG, "[!!] openLink: invalid URL", e);
        }
    }

    @Override
    public void quit() {
        Log.i(TAG, "[!!] quit");
        // داخستنی هەموو چالاکییەکانی یارییەکە و چوونە دەرەوە
        this.activity.finishAffinity();
    }
}