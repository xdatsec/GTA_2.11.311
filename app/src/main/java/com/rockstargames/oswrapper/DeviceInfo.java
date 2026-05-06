package com.rockstargames.oswrapper;

import android.app.UiModeManager;
import android.content.Context;
import android.os.Build;
import android.os.Vibrator;
import android.util.Log;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class DeviceInfo {

    public static final String TAG = "DeviceInfo";

    public final int cpuFrequency;
    public final String hardware;
    public final boolean hasTouchScreen;
    public final boolean hasVibrator;
    public final boolean isPhone;
    public final boolean isTvDevice;
    public final String manufacturer;
    public final String model;
    public final int osVersion;
    public final String product;

    public DeviceInfo(Context context) {
        if (context == null) throw new NullPointerException("context is null");

        UiModeManager uiModeManager = (UiModeManager) context.getSystemService(Context.UI_MODE_SERVICE);
        Vibrator vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);

        // CPU Frequency
        int cpuFreq = 0;
        try {
            File cpuFile = new File("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
            BufferedReader reader = new BufferedReader(new FileReader(cpuFile));
            String line = reader.readLine();
            reader.close();
            if (line != null) {
                cpuFreq = Integer.parseInt(line.trim());
            }
        } catch (Exception e) {
            Log.w(TAG, "Failed to read cpufreq", e);
        }
        this.cpuFrequency = cpuFreq;

        // TV Device check
        this.isTvDevice = (uiModeManager != null) && (uiModeManager.getCurrentModeType() == UiModeManager.MODE_NIGHT_NO + 3);

        // Vibrator check
        this.hasVibrator = (vibrator != null) && vibrator.hasVibrator();

        // OS Version
        this.osVersion = Build.VERSION.SDK_INT;

        // Device info
        this.model = Build.MODEL != null ? Build.MODEL : "";
        this.product = Build.PRODUCT != null ? Build.PRODUCT : "";
        this.manufacturer = Build.MANUFACTURER != null ? Build.MANUFACTURER : "";
        this.hardware = Build.HARDWARE != null ? Build.HARDWARE : "";

        // Screen type
        int screenLayout = context.getResources().getConfiguration().screenLayout & 0x0F;
        this.isPhone = screenLayout < 3;

        // Touch screen
        this.hasTouchScreen = context.getResources().getConfiguration().touchscreen != 1;
    }
}