package com.kurdish.roleplay.launcher.util;

import com.google.firebase.remoteconfig.FirebaseRemoteConfig;
import com.google.firebase.remoteconfig.FirebaseRemoteConfigSettings;
import android.util.Log;

public class RemoteConfigManager {

    private static FirebaseRemoteConfig remoteConfig;
    private static boolean isInitialized = false;

    // Initialize only once
    private static void init() {
        if (isInitialized) return;

        remoteConfig = FirebaseRemoteConfig.getInstance();

        FirebaseRemoteConfigSettings settings =
                new FirebaseRemoteConfigSettings.Builder()
                        .setMinimumFetchIntervalInSeconds(0)
                        .build();

        remoteConfig.setConfigSettingsAsync(settings);

        // Defaults optional
        // remoteConfig.setDefaultsAsync(R.xml.remote_config_defaults);

        remoteConfig.fetchAndActivate()
                .addOnCompleteListener(task -> {
                    Log.d("RemoteConfig", "Fetched & activated");
                });

        isInitialized = true;
    }

    // Get parameter string
    public static String getString(String key) {
        init();
        String val = remoteConfig.getString(key);
        Log.e("RC_DEBUG", key + " = " + val);
        return val;
    }
}
