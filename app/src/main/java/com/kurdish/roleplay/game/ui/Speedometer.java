package com.kurdish.roleplay.game.ui;

import android.annotation.SuppressLint;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.Rect;
import android.os.Build;
import android.os.Handler;
import android.util.Log;
import android.app.Activity;
import android.widget.*;
import android.view.View;
import android.view.ViewTreeObserver;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.kurdish.roleplay.R;
import com.kurdish.roleplay.launcher.util.SeekArc;
import com.kurdish.roleplay.launcher.util.Util;


public class Speedometer {
    private Activity activity;
    private ImageView mBelt;
    private TextView mCarHP;
    private SeekArc mCarHPLine;
    private ImageView mEngine;
    private TextView mFuel;
    private SeekArc mFuelLine;
    private ConstraintLayout mInputLayout;
    private ImageView mLight;
    private ImageView mKey;
    private TextView mSpeed;
    private SeekArc mSpeedLine;
    private ImageView imageView8;

    public int povorright, povorleft;
    public ImageView datpovorleft, datpovorright;
    public ImageView povorotnikleft, povorotnikright;
    public int probeg;
    public TextView probeg_text;

    private boolean isLeftBlinking = false, isRightBlinking = false;
    private final Handler handler = new Handler();
    private Runnable leftBlinker, rightBlinker;

    private boolean mKeyboardVisible = false;

    public native void sendClick(int clickId);

    public static final int BUTTON_ENGINE = 0;
    public static final int BUTTON_LIGHT = 1;
    public static final int BUTTON_TURN_LEFT = 2;
    public static final int BUTTON_TURN_RIGHT = 3;
    public static final int BUTTON_TURN_ALL = 4;

    private ImageView blinkerIcon;
    private ImageView turnLeftIcon;
    private ImageView turnRightIcon;


    private volatile boolean turnlight_all_state = false;
    private volatile boolean turnlight_left_state = false;
    private volatile boolean turnlight_right_state = false;

    private Thread threadTurnAll;
    private Thread threadTurnLeft;
    private Thread threadTurnRight;




    // Статическая ссылка для доступа из нативного кода
    private static Speedometer instance;

    public Speedometer(Activity activity){
        this.activity = activity;
        instance = this;
        initializeViews();
        initListeners();
        setupKeyboardListener();

        if (mInputLayout != null) {
            Util.HideLayout(mInputLayout, false);
        }

        Log.d("Speedometer", "Speedometer initialized");
    }

    // Статический метод для обновления из нативного кода
    public static void UpdateFromNative(int speed, int fuel, int hp, int mileage,
                                        int engine, int light, int belt, int lock) {
        Log.d("Speedometer", "UpdateFromNative called: " + speed + " km/h");
        if (instance != null) {
            instance.UpdateSpeedInfo(speed, fuel, hp, mileage, engine, light, belt, lock);
        } else {
            Log.w("Speedometer", "Instance is null - cannot update");
        }
    }

    // JNI метод для прямого вызова
    public static void NativeUpdateSpeedometer(int speed, int fuel, int hp, int mileage,
                                               int engine, int light, int belt, int lock) {
        UpdateFromNative(speed, fuel, hp, mileage, engine, light, belt, lock);
    }

    private void initListeners() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if (blinkerIcon != null) {
                    blinkerIcon.setOnClickListener(v ->
                            sendClick(BUTTON_TURN_ALL)
                    );
                }

                if (turnLeftIcon != null) {
                    turnLeftIcon.setOnClickListener(v -> {
                        sendClick(BUTTON_TURN_LEFT);
                    });
                }

                if (turnRightIcon != null) {
                    turnRightIcon.setOnClickListener(v -> {
                        sendClick(BUTTON_TURN_RIGHT);
                    });
                }

                if (mEngine != null) {
                    mEngine.setOnClickListener(v -> {
                        sendClick(BUTTON_ENGINE);
                    });
                }

                if (mLight != null) {
                    mLight.setOnClickListener(v -> {
                        sendClick(BUTTON_LIGHT);
                    });
                }
            }
        });
    }



    private void initializeViews() {
        try {
            mInputLayout = activity.findViewById(R.id.speedometer);
            mSpeed = activity.findViewById(R.id.speed_text);
            mFuel = activity.findViewById(R.id.speed_fuel_text);
            mCarHP = activity.findViewById(R.id.speed_car_hp_text);
            mSpeedLine = activity.findViewById(R.id.speed_line);
            mFuelLine = activity.findViewById(R.id.speed_fuel_line);
            mCarHPLine = activity.findViewById(R.id.speed_car_hp_line);
            mEngine = activity.findViewById(R.id.ic_engine);
            mLight = activity.findViewById(R.id.ic_lights);
            mBelt = activity.findViewById(R.id.ic_remen);
            mKey = activity.findViewById(R.id.ic_key);
            probeg_text = activity.findViewById(R.id.probeg_text);
            imageView8 = activity.findViewById(R.id.imageView8);

            povorotnikleft = activity.findViewById(R.id.povorotnikleft);
            povorotnikright = activity.findViewById(R.id.povorotnikright);
            datpovorleft = activity.findViewById(R.id.datpovorleft);
            datpovorright = activity.findViewById(R.id.datpovorright);

            blinkerIcon = activity.findViewById(R.id.blinkerIcon);
            turnLeftIcon = activity.findViewById(R.id.turnLeftIcon);
            turnRightIcon = activity.findViewById(R.id.turnRightIcon);


            // Логируем какие элементы найдены
            Log.d("Speedometer", "Layout initialized - povorotnikleft: " + (povorotnikleft != null) +
                    ", povorotnikright: " + (povorotnikright != null) +
                    ", imageView8: " + (imageView8 != null));

            SetProbeg(probeg);
        } catch (Exception e) {
            Log.e("Speedometer", "Error initializing views: " + e.getMessage());
        }
    }

    private final Runnable runnable_turnlight_all = new Runnable() {
        @Override
        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                activity.runOnUiThread(() -> {
                    if (turnlight_all_state) {
                        blinkerIcon.setImageTintList(null);
                        turnLeftIcon.setImageResource(R.drawable.speedometr_turn_off);
                        turnRightIcon.setImageResource(R.drawable.speedometr_turn_off);
                        turnlight_all_state = false;

                        //Samp.soundPool.play(turnlight_tick_sound_1, 0.2f, 0.1f, 1, 0, 1.0f);
                    } else {
                        blinkerIcon.setImageTintList(
                                ColorStateList.valueOf(Color.parseColor("#f44336"))
                        );
                        turnLeftIcon.setImageResource(R.drawable.speedometr_turn_on);
                        turnRightIcon.setImageResource(R.drawable.speedometr_turn_on);
                        //Samp.soundPool.play(turnlight_tick_sound_2, 0.2f, 0.1f, 1, 0, 1.0f);
                        turnlight_all_state = true;
                    }
                });

                try {
                    Thread.sleep(400);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
    };

    private final Runnable runnable_turnlight_left = new Runnable() {
        @Override
        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                activity.runOnUiThread(() -> {
                    if (turnlight_left_state) {
                        turnLeftIcon.setImageResource(R.drawable.speedometr_turn_off);
                        //Samp.soundPool.play(turnlight_tick_sound_1, 0.2f, 0.1f, 1, 0, 1.0f);
                        turnlight_left_state = false;
                    } else {
                        turnLeftIcon.setImageResource(R.drawable.speedometr_turn_on);
                        //Samp.soundPool.play(turnlight_tick_sound_2, 0.2f, 0.1f, 1, 0, 1.0f);
                        turnlight_left_state = true;
                    }
                });

                try {
                    Thread.sleep(400);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
    };

    private final Runnable runnable_turnlight_right = new Runnable() {
        @Override
        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                activity.runOnUiThread(() -> {
                    if (turnlight_right_state) {
                        turnRightIcon.setImageResource(R.drawable.speedometr_turn_off);
                        //Samp.soundPool.play(turnlight_tick_sound_1, 0.1f, 0.2f, 1, 0, 1.0f);
                        turnlight_right_state = false;
                    } else {
                        turnRightIcon.setImageResource(R.drawable.speedometr_turn_on);
                        //Samp.soundPool.play(turnlight_tick_sound_2, 0.1f, 0.2f, 1, 0, 1.0f);
                        turnlight_right_state = true;
                    }
                });

                try {
                    Thread.sleep(400);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
    };
    private void startTurnAll() {
        stopAllTurnSignals();
        threadTurnAll = new Thread(runnable_turnlight_all);
        threadTurnAll.start();
    }

    private void startTurnLeft() {
        stopAllTurnSignals();
        threadTurnLeft = new Thread(runnable_turnlight_left);
        threadTurnLeft.start();
    }

    private void startTurnRight() {
        stopAllTurnSignals();
        threadTurnRight = new Thread(runnable_turnlight_right);
        threadTurnRight.start();
    }

    private void stopAllTurnSignals() {
        if (threadTurnAll != null) threadTurnAll.interrupt();
        if (threadTurnLeft != null) threadTurnLeft.interrupt();
        if (threadTurnRight != null) threadTurnRight.interrupt();

        turnlight_all_state = false;
        turnlight_left_state = false;
        turnlight_right_state = false;
    }


    private void setupKeyboardListener() {
        try {
            final View activityRootView = activity.getWindow().getDecorView().findViewById(android.R.id.content);

            activityRootView.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
                private boolean wasKeyboardOpened = false;
                private final int defaultKeyboardHeightDP = 100;
                private final int EstimatedKeyboardDP = defaultKeyboardHeightDP + (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP ? 48 : 0);
                private final Rect rect = new Rect();

                @Override
                public void onGlobalLayout() {
                    activityRootView.getWindowVisibleDisplayFrame(rect);

                    int screenHeight = activityRootView.getRootView().getHeight();
                    int keypadHeight = screenHeight - rect.bottom;

                    float scale = activity.getResources().getDisplayMetrics().density;
                    int keypadHeightDP = (int) (keypadHeight / scale);

                    boolean isKeyboardNowVisible = keypadHeightDP >= EstimatedKeyboardDP;

                    if (isKeyboardNowVisible == wasKeyboardOpened) {
                        return;
                    }

                    wasKeyboardOpened = isKeyboardNowVisible;
                    mKeyboardVisible = isKeyboardNowVisible;

                    if (isKeyboardNowVisible) {
                        Log.d("Speedometer", "Keyboard opened - hiding speedometer");
                        HideSpeed();
                    } else {
                        Log.d("Speedometer", "Keyboard closed - showing speedometer");
                    }
                }
            });
        } catch (Exception e) {
            Log.e("Speedometer", "Error setting up keyboard listener: " + e.getMessage());
        }
    }

    private boolean isKeyboardVisible() {
        return mKeyboardVisible;
    }

    private void startLeftBlinker() {
        if (datpovorleft == null) {
            Log.w("Speedometer", "datpovorleft is null - cannot start left blinker");
            return;
        }

        stopBlinking();

        isLeftBlinking = true;
        leftBlinker = new Runnable() {
            private boolean isOn = false;

            @Override
            public void run() {
                if (datpovorleft != null) {
                    datpovorleft.setImageResource(isOn ? R.drawable.datpovorne : R.drawable.datpovorda);
                    isOn = !isOn;
                    handler.postDelayed(this, 500);
                }
            }
        };
        handler.post(leftBlinker);
    }

    private void startRightBlinker() {
        if (datpovorright == null) {
            Log.w("Speedometer", "datpovorright is null - cannot start right blinker");
            return;
        }

        stopBlinking();

        isRightBlinking = true;
        rightBlinker = new Runnable() {
            private boolean isOn = false;

            @Override
            public void run() {
                if (datpovorright != null) {
                    datpovorright.setImageResource(isOn ? R.drawable.datpovorne : R.drawable.datpovorda);
                    isOn = !isOn;
                    handler.postDelayed(this, 500);
                }
            }
        };
        handler.post(rightBlinker);
    }

    private void stopBlinking() {
        if (isLeftBlinking && leftBlinker != null) {
            handler.removeCallbacks(leftBlinker);
            if (datpovorleft != null) {
                datpovorleft.setImageResource(R.drawable.datpovorne);
            }
        }
        if (isRightBlinking && rightBlinker != null) {
            handler.removeCallbacks(rightBlinker);
            if (datpovorright != null) {
                datpovorright.setImageResource(R.drawable.datpovorne);
            }
        }
        isLeftBlinking = false;
        isRightBlinking = false;
    }

    public void SetProbeg(float probeg) {
        if (probeg_text != null) {
            probeg_text.setText(String.valueOf(probeg));
        }
    }

    @SuppressLint("SetTextI18n")
    public void UpdateSpeedInfo(int speed, int fuel, int hp, int mileage, int engine, int light, int belt, int lock) {
        Log.d("Speedometer", "UpdateSpeedInfo: speed=" + speed + ", fuel=" + fuel + ", hp=" + hp);

        if (mInputLayout != null && mInputLayout.getVisibility() != View.VISIBLE && !isKeyboardVisible()) {
            Log.d("Speedometer", "Auto-showing speedometer on data update");
            ShowSpeed();
        }

        float rot = (float) (speed * 0.938 - 122.03418);
        if (rot > 121.8f) {
            rot = 121.8f;
        }
        if (fuel > 100) {
            fuel = 100;
        }

        // Обновляем все элементы интерфейса с проверками на null
        if (mFuel != null) mFuel.setText(String.valueOf(fuel));
        if (mCarHP != null) mCarHP.setText(String.format("%d%%", hp));
        if (mSpeed != null) mSpeed.setText(String.valueOf(speed));
        if (mSpeedLine != null) mSpeedLine.setProgress(speed);
        if (mCarHPLine != null) mCarHPLine.setProgress(hp);
        if (mFuelLine != null) mFuelLine.setProgress(fuel);
        if (mEngine != null) mEngine.setBackgroundResource(engine == 1 ? R.drawable.engineactive : R.drawable.engine);
        if (mLight != null) mLight.setBackgroundResource(light == 1 ? R.drawable.lightsactive : R.drawable.lights);
        if (mBelt != null) mBelt.setBackgroundResource(belt == 1 ? R.drawable.remenactive : R.drawable.remen);
        if (mKey != null) mKey.setBackgroundResource(lock == 1 ? R.drawable.keyactive : R.drawable.key);

        SetProbeg(mileage);
    }

    public void ShowSpeed() {
        Log.d("Speedometer", "ShowSpeed called");
        if (mInputLayout != null && !isKeyboardVisible()) {
            Util.ShowLayout(mInputLayout, false);
            Log.d("Speedometer", "Speedometer should be visible now");
        } else {
            Log.d("Speedometer", "Speedometer not shown - keyboard is visible or layout is null");
        }
    }

    public void HideSpeed() {
        Log.d("Speedometer", "HideSpeed called");
        if (mInputLayout != null) {
            Util.HideLayout(mInputLayout, false);
        }
    }

    public ConstraintLayout getInputLayout() {
        return this.mInputLayout;
    }

    public boolean isVisible() {
        return mInputLayout != null && mInputLayout.getVisibility() == View.VISIBLE;
    }

    // Метод для очистки статической ссылки
    public static void cleanup() {
        instance = null;
    }
}
