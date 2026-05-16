package com.kurdish.roleplay.game.ui;

import android.app.Activity;
import android.os.Handler;
import android.os.Looper;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.ImageView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.kurdish.roleplay.R;

public class LoadingScreen {

    private Activity activity;
    private ConstraintLayout mainLayout;
    private ProgressBar progressBar;
    private TextView progressPercent;
    private TextView loadingText;
    private View dot1, dot2, dot3;

    private Handler handler;
    private Runnable progressRunnable;
    private int progress = 0;
    private static final int TOTAL_DURATION = 4000;
    private static final int UPDATE_INTERVAL = 40;


    public LoadingScreen(Activity activity) {
        this.activity = activity;

        mainLayout = (ConstraintLayout) activity.getLayoutInflater()
                .inflate(R.layout.loadingscreen, null);
        activity.addContentView(mainLayout,
                new ConstraintLayout.LayoutParams(-1, -1));

        initializeViews();
        setupAnimations();
        startProgressAnimation();
    }

    private void initializeViews() {
        progressBar = mainLayout.findViewById(R.id.progressBarr);
        progressPercent = mainLayout.findViewById(R.id.progressPercent);
        loadingText = mainLayout.findViewById(R.id.loadingText);
        dot1 = mainLayout.findViewById(R.id.dot1);
        dot2 = mainLayout.findViewById(R.id.dot2);
        dot3 = mainLayout.findViewById(R.id.dot3);

        handler = new Handler(Looper.getMainLooper());
    }

    private void setupAnimations() {
        Animation dotAnimation = AnimationUtils.loadAnimation(activity, R.anim.loading_dots);
        dot1.startAnimation(dotAnimation);

        Animation dotAnimation2 = AnimationUtils.loadAnimation(activity, R.anim.loading_dots);
        dotAnimation2.setStartOffset(200);
        dot2.startAnimation(dotAnimation2);

        Animation dotAnimation3 = AnimationUtils.loadAnimation(activity, R.anim.loading_dots);
        dotAnimation3.setStartOffset(400);
        dot3.startAnimation(dotAnimation3);

        ImageView loadingIcon = mainLayout.findViewById(R.id.loadingIcon);
        Animation rotateAnimation =
                AnimationUtils.loadAnimation(activity, R.anim.rotate_animation);
        loadingIcon.startAnimation(rotateAnimation);
    }

    private void startProgressAnimation() {
        progressRunnable = new Runnable() {
            @Override
            public void run() {
                if (progress < 100) {
                    progress++;
                    updateProgress(progress);
                    updateLoadingText(progress);
                    handler.postDelayed(this, UPDATE_INTERVAL);
                } else {
                    onLoadingComplete();
                }
            }
        };

        handler.postDelayed(progressRunnable, 500);
    }

    private void updateProgress(int progress) {
        progressBar.setProgress(progress);
        progressPercent.setText(progress + "%");
    }

    private void updateLoadingText(int progress) {
        String[] loadingMessages = {
                "Loading resources...",
                "Preparing game...",
                "Initializing systems...",
                "Almost ready...",
                "Starting Game..."
        };

        int messageIndex = progress / 20;
        if (messageIndex < loadingMessages.length) {
            loadingText.setText(loadingMessages[messageIndex]);
        }
    }

    private void onLoadingComplete() {
        Animation fadeOut = AnimationUtils.loadAnimation(activity, R.anim.fade_out);
        mainLayout.startAnimation(fadeOut);

        fadeOut.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) { }

            @Override
            public void onAnimationEnd(Animation animation) {
                hide();
            }

            @Override
            public void onAnimationRepeat(Animation animation) { }
        });
    }

    public void hide() {
        if (handler != null && progressRunnable != null) {
            handler.removeCallbacks(progressRunnable);
        }

        if (dot1 != null) dot1.clearAnimation();
        if (dot2 != null) dot2.clearAnimation();
        if (dot3 != null) dot3.clearAnimation();

        mainLayout.setVisibility(View.GONE);
    }

    public void show() {
        mainLayout.setVisibility(View.VISIBLE);
        progress = 0;
        progressBar.setProgress(0);
        startProgressAnimation();
    }

    public void destroy() {
        hide();
        handler = null;
        progressRunnable = null;
    }
}
