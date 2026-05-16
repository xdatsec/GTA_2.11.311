package com.kurdish.roleplay.launcher;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.DecelerateInterpolator;
import android.view.inputmethod.InputMethodManager;

import androidx.appcompat.app.AppCompatActivity;

import com.joom.paranoid.Obfuscate;
import com.kurdish.roleplay.R;
import com.kurdish.roleplay.launcher.util.ConfigValidator;

import java.io.File;

@Obfuscate
public class MainActivity extends AppCompatActivity {

    private View btnHome;
    private View btnSettings;
    private View btnWeb;
    private View activeCircle;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        btnHome = findViewById(R.id.btnHome);
        btnSettings = findViewById(R.id.btnSettings);
        btnWeb = findViewById(R.id.btnWeb);
        activeCircle = findViewById(R.id.activeCircle);

        showHomeFragment();

        ConfigValidator.validateConfigFiles(this);

        View[] buttons = {btnHome, btnSettings, btnWeb};

        View.OnClickListener navClick = v -> {
            if (activeCircle.getVisibility() != View.VISIBLE)
                activeCircle.setVisibility(View.VISIBLE);

            float targetX = v.getX() + (v.getWidth() / 2f) - (activeCircle.getWidth() / 2f);

            activeCircle.animate()
                    .translationX(targetX)
                    .setDuration(300)
                    .setInterpolator(new DecelerateInterpolator())
                    .start();
        };

        for (View b : buttons) {
            b.setOnClickListener(navClick);
        }

        btnHome.post(() -> {
            activeCircle.setVisibility(View.VISIBLE);
            float targetX = btnHome.getX() + (btnHome.getWidth() / 2f) - (activeCircle.getWidth() / 2f);
            activeCircle.setTranslationX(targetX);
        });

        btnHome.setOnClickListener(v -> {
            navClick.onClick(v);
            openHome();
        });

        btnSettings.setOnClickListener(v -> {
            navClick.onClick(v);
            openSettings();
        });

        btnWeb.setOnClickListener(v -> {
            navClick.onClick(v);
            openWeb();
        });

        setupSimpleAnimations();
    }

    private void showHomeFragment() {
        getSupportFragmentManager().beginTransaction()
                .replace(R.id.fragmentContainer, new HomeFragment())
                .commit();
    }

    private void openHome() {
        getSupportFragmentManager().beginTransaction()
                .setCustomAnimations(R.anim.fade_in, R.anim.fade_out)
                .replace(R.id.fragmentContainer, new HomeFragment())
                .commit();
    }

    private void openSettings() {
        getSupportFragmentManager().beginTransaction()
                .setCustomAnimations(R.anim.fade_in, R.anim.fade_out)
                .replace(R.id.fragmentContainer, new SettingsActivity())
                .commit();
    }

    private void openWeb() {
        getSupportFragmentManager().beginTransaction()
                .setCustomAnimations(R.anim.fade_in, R.anim.fade_out)
                .replace(R.id.fragmentContainer, new WebFragment())
                .commit();
    }

    public static void hideKeyboard(Activity activity) {
        InputMethodManager inputManager = (InputMethodManager) activity
                .getSystemService(Context.INPUT_METHOD_SERVICE);

        View currentFocusedView = activity.getCurrentFocus();
        if (currentFocusedView != null) {
            inputManager.hideSoftInputFromWindow(currentFocusedView.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
        }
    }

    private void setupSimpleAnimations() {
        final Animation scaleDown = AnimationUtils.loadAnimation(this, R.anim.btn_scale_modern);
        final Animation scaleUp = AnimationUtils.loadAnimation(this, R.anim.btn_release_modern);

        View.OnTouchListener touchListener = (v, event) -> {
            switch (event.getAction()) {
                case android.view.MotionEvent.ACTION_DOWN:
                    v.startAnimation(scaleDown);
                    break;
                case android.view.MotionEvent.ACTION_UP:
                    v.startAnimation(scaleUp);
                    break;
            }
            return false;
        };

        btnHome.setOnTouchListener(touchListener);
        btnSettings.setOnTouchListener(touchListener);
        btnWeb.setOnTouchListener(touchListener);
    }
    @Override
    public void onBackPressed() {
    }

}
