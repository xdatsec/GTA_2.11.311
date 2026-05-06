package com.rockstargames.oswrapper.view;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;
import com.rockstargames.oswrapper.GameActivityBase;
import com.rockstargames.oswrapper.GameThread;
import com.rockstargames.oswrapper.GameView;

public final class SensorHandler implements SensorEventListener, GameViewHandler {

    private static final String TAG = "GameView.SensorHandler";

    private final GameView        view;
    private final SensorManager   sensorManager;
    private final WindowManager   windowManager;
    private final Sensor          accelerometer;

    private int     displayRotation = 0;
    private boolean sensorsActive   = false;

    // ─── Constructor ─────────────────────────────────────────────────────────────

    public SensorHandler(Context context, GameView view) {
        if (context == null) throw new NullPointerException("context is null");
        if (view == null)    throw new NullPointerException("view is null");

        this.view          = view;
        this.sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        this.windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        this.accelerometer = (sensorManager != null)
                ? sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)
                : null;
    }

    // ─── Sensor Control ──────────────────────────────────────────────────────────

    public final void enableSensors() {
        Log.i(TAG, "[!!] enableSensors: " + sensorsActive);
        if (sensorsActive) return;

        if (accelerometer != null) {
            sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_GAME);
        }
        updateRotation();
    }

    public final void disableSensors() {
        Log.i(TAG, "[!!] disableSensors: " + sensorsActive);
        if (sensorsActive && accelerometer != null) {
            sensorManager.unregisterListener(this);
        }
    }

    public final void updateRotation() {
        int rotation;
        if (Build.VERSION.SDK_INT >= 30) {
            Display display = view.getDisplay();
            rotation = (display != null) ? display.getRotation() : 0;
        } else {
            rotation = windowManager.getDefaultDisplay().getRotation();
        }
        setDisplayRotation(rotation);
    }

    // ─── SensorEventListener ─────────────────────────────────────────────────────

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // not needed
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        // Guard: only handle accelerometer events
        if (event == null || event.sensor == null
                || event.sensor.getType() != Sensor.TYPE_ACCELEROMETER) {
            return;
        }

        float rawX = event.values[0];
        float rawY = event.values[1];
        float rawZ = event.values[2];

        float x, y, z;

        /*
         * Remap axes based on current display rotation:
         *   ROTATION_0 / ROTATION_180  → portrait:  swap x/y, negate x
         *   ROTATION_90 / ROTATION_270 → landscape: keep x/y as-is
         *
         * Original bytecode logic reconstructed from the decompiled smali.
         */
        switch (displayRotation) {
            case 0:   // portrait
            case 2:   // reverse portrait (180°)
                x =  rawY;
                y = -rawX;
                z =  rawZ;
                break;
            case 1:   // landscape (90°)
            case 3:   // reverse landscape (270°)
            default:
                x = rawX;
                y = rawY;
                z = rawZ;
                break;
        }

        // Mirror y when x is negative (preserves quadrant sign from original logic)
        if (x < 0) y = -y;

        GameThread.INSTANCE.onAccelerometerChanged(x, y, z);
    }

    // ─── GameViewHandler ─────────────────────────────────────────────────────────

    @Override
    public void setup(GameActivityBase activity) {
        if (activity == null) throw new NullPointerException("activity is null");
        enableSensors();
    }

    @Override
    public void onPause() {
        disableSensors();
    }

    @Override
    public void onResume() {
        enableSensors();
    }

    // ─── Getters / Setters ───────────────────────────────────────────────────────

    public final boolean getSensorsActive() {
        return sensorsActive;
    }

    public final void setSensorsActive(boolean value) {
        Log.i(TAG, "[!!] sensorsActive = " + value);
        sensorsActive = value;
    }

    public final GameView getView() {
        return view;
    }

    private void setDisplayRotation(int value) {
        Log.i(TAG, "[!!] displayRotation = " + value);
        displayRotation = value;
    }
}