package com.rockstargames.oswrapper.view;

import android.graphics.PointF;
import android.util.Log;
import android.util.SparseArray;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

import com.rockstargames.oswrapper.GameActivityBase;
import com.rockstargames.oswrapper.GameThread;
import com.rockstargames.oswrapper.UtilsKt;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import kotlin.collections.CollectionsKt;
import kotlin.jvm.internal.Intrinsics;

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

    public final int getControllerCount() {
        int[] deviceIds = InputDevice.getDeviceIds();
        Intrinsics.checkNotNullExpressionValue(deviceIds, "getDeviceIds(...)");
        ArrayList arrayList = new ArrayList(deviceIds.length);
        for (int i : deviceIds) {
            arrayList.add(InputDevice.getDevice(i));
        }
        List listFilterNotNull = CollectionsKt.filterNotNull(arrayList);
        if ((listFilterNotNull instanceof Collection) && listFilterNotNull.isEmpty()) {
            return 0;
        }
        Iterator it = listFilterNotNull.iterator();
        int i2 = 0;
        while (it.hasNext()) {
            if (((((InputDevice) it.next()).getSources() & 16778257) != 0) && (i2 = i2 + 1) < 0) {
                CollectionsKt.throwCountOverflow();
            }
        }
        return i2;
    }


    // -----------------------------------------------------------------------
    // Key mapping
    // -----------------------------------------------------------------------

    private final int toGamepadButton(int keyCode) {
        if (keyCode == 4) {
            return 14;
        }
        if (keyCode == 82) {
            return 15;
        }
        if (keyCode == 110) {
            return 4;
        }
        if (keyCode == 96) {
            return 0;
        }
        if (keyCode == 97) {
            return 1;
        }
        if (keyCode == 99) {
            return 2;
        }
        if (keyCode == 100) {
            return 3;
        }
        if (keyCode == 102) {
            return 6;
        }
        if (keyCode == 103) {
            return 7;
        }
        switch (keyCode) {
            case 106:
                return 12;
            case 107:
                return 13;
            case 108:
                return 4;
            default:
                return -1;
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

    public final boolean onGenericMotionEvent(MotionEvent event) {
        if (event == null || (event.getSource() & 16) == 0) {
            return false;
        }
        float axisValue = event.getAxisValue(0);
        float axisValue2 = event.getAxisValue(1);
        float axisValue3 = event.getAxisValue(11);
        float axisValue4 = event.getAxisValue(14);
        float axisValue5 = event.getAxisValue(15);
        float axisValue6 = event.getAxisValue(16);
        if (axisValue5 > 0.2f) {
            GameThread.INSTANCE.onGamepadButtonDown(0, 11);
            this.dpadRightPressed = true;
        } else if (axisValue5 < -0.2f) {
            GameThread.INSTANCE.onGamepadButtonDown(0, 10);
            this.dpadLeftPressed = true;
        } else {
            if (this.dpadLeftPressed) {
                GameThread.INSTANCE.onGamepadButtonUp(0, 10);
            }
            if (this.dpadRightPressed) {
                GameThread.INSTANCE.onGamepadButtonUp(0, 11);
            }
            this.dpadLeftPressed = false;
            this.dpadRightPressed = false;
        }
        if (axisValue6 < -0.2f) {
            GameThread.INSTANCE.onGamepadButtonDown(0, 8);
            this.dpadUpPressed = true;
        } else if (axisValue6 > 0.2f) {
            GameThread.INSTANCE.onGamepadButtonDown(0, 9);
            this.dpadDownPressed = true;
        } else {
            if (this.dpadUpPressed) {
                GameThread.INSTANCE.onGamepadButtonUp(0, 8);
            }
            if (this.dpadDownPressed) {
                GameThread.INSTANCE.onGamepadButtonUp(0, 9);
            }
            this.dpadUpPressed = false;
            this.dpadDownPressed = false;
        }
        float fMax3 = UtilsKt.max3(event.getAxisValue(18), event.getAxisValue(19), event.getAxisValue(22));
        GameThread.INSTANCE.onGamepadAxesChanged(0, axisValue, axisValue2, axisValue3, axisValue4, Math.max(event.getAxisValue(17), event.getAxisValue(23)), fMax3);
        return true;
    }


    // -----------------------------------------------------------------------
    // Touch events
    // -----------------------------------------------------------------------

    public final boolean onTouchEvent(MotionEvent event) {
        Intrinsics.checkNotNullParameter(event, "event");
        int actionIndex = event.getActionIndex();
        int pointerId = event.getPointerId(actionIndex);
        int i = 0;
        switch (event.getActionMasked()) {
            case 0:
            case 5:
                float x = event.getX(actionIndex);
                float y = event.getY(actionIndex);
                PointF pointF = new PointF();
                pointF.x = x;
                pointF.y = y;
                this.touches.put(pointerId, pointF);
                GameThread.INSTANCE.onTouchStart(pointerId, x, y);
                return true;
            case 1:
            case 4:
            case 6:
                PointF pointF2 = this.touches.get(pointerId);
                if (pointF2 != null) {
                    GameThread.INSTANCE.onTouchEnd(pointerId, pointF2.x, pointF2.y);
                }
                this.touches.remove(pointerId);
                return true;
            case 2:
                while (i < event.getPointerCount()) {
                    float x2 = event.getX(i);
                    float y2 = event.getY(i);
                    PointF pointF3 = new PointF();
                    pointF3.x = x2;
                    pointF3.y = y2;
                    int pointerId2 = event.getPointerId(i);
                    this.touches.put(pointerId2, pointF3);
                    GameThread.INSTANCE.onTouchMove(pointerId2, x2, y2);
                    i++;
                }
                return true;
            case 3:
                while (i < this.touches.size()) {
                    int iKeyAt = this.touches.keyAt(i);
                    PointF pointF4 = this.touches.get(iKeyAt);
                    if (pointF4 != null) {
                        GameThread.INSTANCE.onTouchEnd(iKeyAt, pointF4.x, pointF4.y);
                    }
                    i++;
                }
                this.touches.clear();
                return true;
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