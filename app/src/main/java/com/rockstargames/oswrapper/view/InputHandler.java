package com.rockstargames.oswrapper.view;

import android.graphics.PointF;
import android.util.Log;
import android.util.SparseArray;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

import com.rockstargames.oswrapper.GameActivityBase;
import com.rockstargames.oswrapper.GameThread;

public class InputHandler implements GameViewHandler {

    public static  InputHandler INSTANCE = new InputHandler();

    private static   String TAG = "GameView.InputHandler";

    // Xbox 360 button mappings
    private static   int OSX360_A         = 0;
    private static   int OSX360_B         = 1;
    private static   int OSX360_X         = 2;
    private static   int OSX360_Y         = 3;
    private static   int OSX360_START     = 4;
    private static   int OSX360_BACK      = 5;
    private static   int OSX360_L1        = 6;
    private static   int OSX360_R1        = 7;
    private static   int OSX360_DPADUP    = 8;
    private static   int OSX360_DPADDOWN  = 9;
    private static   int OSX360_DPADLEFT  = 10;
    private static   int OSX360_DPADRIGHT = 11;
    private static   int OSX360_L3        = 12;
    private static   int OSX360_R3        = 13;

    // OSX Platform buttons
    private static   int OSXP_BACK    = 14;
    private static   int OSXP_GP_MENU = 15;
    private static   int OSXP_MENU    = 12;
    private static   int OSXP_SEARCH  = 13;

    private boolean dpadUpPressed;
    private boolean dpadDownPressed;
    private boolean dpadLeftPressed;
    private boolean dpadRightPressed;

    private   SparseArray<PointF> touches = new SparseArray<>();

    private InputHandler() {}

    // -----------------------------------------------------------------------
    // Controller count (replaces Kotlin Companion)
    // -----------------------------------------------------------------------

    public int getControllerCount() {
        int[] deviceIds = InputDevice.getDeviceIds();
        int count = 0;
        for (int id : deviceIds) {
            InputDevice device = InputDevice.getDevice(id);
            if (device != null && (device.getSources() & 0x1000011) != 0) {
                count++;
            }
        }
        return count;
    }

    // -----------------------------------------------------------------------
    // Key mapping
    // -----------------------------------------------------------------------

    private int toGamepadButton(int keyCode) {
        switch (keyCode) {
            case 4:   return OSXP_BACK;     // Android back button
            case 82:  return OSXP_GP_MENU;  // Menu key
            case 110: return OSX360_START;  // Button START
            case 96:  return OSX360_A;
            case 97:  return OSX360_B;
            case 99:  return OSX360_X;
            case 100: return OSX360_Y;
            case 102: return OSX360_L1;
            case 103: return OSX360_R1;
            default:  return -1;
        }
    }

    // -----------------------------------------------------------------------
    // Back button
    // -----------------------------------------------------------------------

    private void onBackPressed() {
        Log.i(TAG, "[!!] onBackPressed");
        GameThread.INSTANCE.onBackButtonPressed();
    }

    // -----------------------------------------------------------------------
    // Key events
    // -----------------------------------------------------------------------

    public boolean onKeyDown(int keyCode, KeyEvent event) {
        boolean isGamepad = ((event != null ? event.getSource() : 0) & 16) != 0;
        if (keyCode == 4 && !isGamepad) {
            return true; // consume, handle on up
        }
        int button = toGamepadButton(keyCode);
        if (button == -1) return false;
        GameThread.INSTANCE.onGamepadButtonDown(0, button);
        return true;
    }

    public boolean onKeyUp(int keyCode, KeyEvent event) {
        boolean isGamepad = ((event != null ? event.getSource() : 0) & 16) != 0;
        if (keyCode == 4 && !isGamepad) {
            onBackPressed();
            return true;
        }
        int button = toGamepadButton(keyCode);
        if (button == -1) return false;
        GameThread.INSTANCE.onGamepadButtonUp(0, button);
        return true;
    }

    // -----------------------------------------------------------------------
    // Gamepad axes / motion
    // -----------------------------------------------------------------------

    public boolean onGenericMotionEvent(MotionEvent event) {
        if (event == null || (event.getSource() & 16) == 0) return false;

        float axisX    = event.getAxisValue(MotionEvent.AXIS_X);
        float axisY    = event.getAxisValue(MotionEvent.AXIS_Y);
        float axisZ    = event.getAxisValue(MotionEvent.AXIS_Z);
        float axisRZ   = event.getAxisValue(MotionEvent.AXIS_RZ);
        float axisHatX = event.getAxisValue(MotionEvent.AXIS_HAT_X);
        float axisHatY = event.getAxisValue(MotionEvent.AXIS_HAT_Y);

        // D-pad horizontal
        if (axisHatX > 0.2f) {
            GameThread.INSTANCE.onGamepadButtonDown(0, OSX360_DPADRIGHT);
            dpadRightPressed = true;
        } else if (axisHatX < -0.2f) {
            GameThread.INSTANCE.onGamepadButtonDown(0, OSX360_DPADLEFT);
            dpadLeftPressed = true;
        } else {
            if (dpadLeftPressed)  GameThread.INSTANCE.onGamepadButtonUp(0, OSX360_DPADLEFT);
            if (dpadRightPressed) GameThread.INSTANCE.onGamepadButtonUp(0, OSX360_DPADRIGHT);
            dpadLeftPressed  = false;
            dpadRightPressed = false;
        }

        // D-pad vertical
        if (axisHatY < -0.2f) {
            GameThread.INSTANCE.onGamepadButtonDown(0, OSX360_DPADUP);
            dpadUpPressed = true;
        } else if (axisHatY > 0.2f) {
            GameThread.INSTANCE.onGamepadButtonDown(0, OSX360_DPADDOWN);
            dpadDownPressed = true;
        } else {
            if (dpadUpPressed)   GameThread.INSTANCE.onGamepadButtonUp(0, OSX360_DPADUP);
            if (dpadDownPressed) GameThread.INSTANCE.onGamepadButtonUp(0, OSX360_DPADDOWN);
            dpadUpPressed   = false;
            dpadDownPressed = false;
        }

        // Triggers — take max of possible axis mappings
        float triggerL = Math.max(event.getAxisValue(MotionEvent.AXIS_LTRIGGER),
                event.getAxisValue(MotionEvent.AXIS_BRAKE));
        float triggerR = max3(event.getAxisValue(MotionEvent.AXIS_RTRIGGER),
                event.getAxisValue(MotionEvent.AXIS_GAS),
                event.getAxisValue(MotionEvent.AXIS_THROTTLE));

        GameThread.INSTANCE.onGamepadAxesChanged(0, axisX, axisY, axisZ, axisRZ, triggerL, triggerR);
        return true;
    }

    // -----------------------------------------------------------------------
    // Touch events
    // -----------------------------------------------------------------------

    public boolean onTouchEvent(MotionEvent event) {
        if (event == null) return false;

        int actionIndex = event.getActionIndex();
        int pointerId   = event.getPointerId(actionIndex);

        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN: {
                float x = event.getX(actionIndex);
                float y = event.getY(actionIndex);
                PointF pt = new PointF(x, y);
                touches.put(pointerId, pt);
                GameThread.INSTANCE.onTouchStart(pointerId, x, y);
                return true;
            }
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_OUTSIDE: {
                PointF pt = touches.get(pointerId);
                if (pt != null) {
                    GameThread.INSTANCE.onTouchEnd(pointerId, pt.x, pt.y);
                }
                touches.remove(pointerId);
                return true;
            }
            case MotionEvent.ACTION_MOVE: {
                for (int i = 0; i < event.getPointerCount(); i++) {
                    float x = event.getX(i);
                    float y = event.getY(i);
                    int pid = event.getPointerId(i);
                    touches.put(pid, new PointF(x, y));
                    GameThread.INSTANCE.onTouchMove(pid, x, y);
                }
                return true;
            }
            case MotionEvent.ACTION_CANCEL: {
                for (int i = 0; i < touches.size(); i++) {
                    int key = touches.keyAt(i);
                    PointF pt = touches.get(key);
                    if (pt != null) {
                        GameThread.INSTANCE.onTouchEnd(key, pt.x, pt.y);
                    }
                }
                touches.clear();
                return true;
            }
            default:
                return false;
        }
    }

    // -----------------------------------------------------------------------
    // GameViewHandler
    // -----------------------------------------------------------------------

    @Override
    public void setup(GameActivityBase activity) {
        // nothing needed
    }

    @Override
    public void onPause() {}

    @Override
    public void onResume() {}

    // -----------------------------------------------------------------------
    // Helpers
    // -----------------------------------------------------------------------

    private static float max3(float a, float b, float c) {
        return Math.max(a, Math.max(b, c));
    }
}