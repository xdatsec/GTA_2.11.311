package com.rockstargames.oswrapper.view;

import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.kurdish.roleplay.R;
import com.rockstargames.oswrapper.GameActivityBase;
import java.io.InputStream;

import kotlin.jvm.internal.Intrinsics;

public final class SplashScreenHandler implements GameViewHandler {

    private static final String TAG = "GameView.SplashScreenHandler";

    private GameActivityBase activity;
    private ImageView image;
    private TextView  text;
    private View      view;

    // ─── Setup ───────────────────────────────────────────────────────────────────

    @Override
    public void setup(GameActivityBase activity) {
        if (activity == null) throw new NullPointerException("activity is null");
        this.activity = activity;

        view  = activity.findViewById(R.id.viewSplash);
        if (view == null) throw new IllegalStateException("viewSplash not found in layout");

        image = view.findViewById(R.id.splashImage);
        if (image == null) throw new IllegalStateException("splashImage not found in layout");

        text  = view.findViewById(R.id.splashText);
        if (text == null) throw new IllegalStateException("splashText not found in layout");
    }

    // ─── Visibility ──────────────────────────────────────────────────────────────
    public static final void show$lambda$0(SplashScreenHandler this$0) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        View view = this$0.view;
        if (view == null) {
            Intrinsics.throwUninitializedPropertyAccessException("view");
            view = null;
        }
        view.setVisibility(View.VISIBLE);
    }

    public final void show() {
        Log.i(TAG, "[!!] show");
        GameActivityBase gameActivityBase = this.activity;
        if (gameActivityBase == null) {
            Intrinsics.throwUninitializedPropertyAccessException("activity");
            gameActivityBase = null;
        }
        gameActivityBase.runOnUiThread(new Runnable() { // from class: com.rockstargames.oswrapper.view.SplashScreenHandler$$ExternalSyntheticLambda3
            @Override // java.lang.Runnable
            public final void run() {
                SplashScreenHandler.show$lambda$0(SplashScreenHandler.this);
            }
        });
    }

    public static final void hide$lambda$1(SplashScreenHandler this$0) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        ImageView imageView = this$0.image;
        View view = null;
        if (imageView == null) {
            Intrinsics.throwUninitializedPropertyAccessException("image");
            imageView = null;
        }
        imageView.setImageDrawable(null);
        TextView textView = this$0.text;
        if (textView == null) {
            Intrinsics.throwUninitializedPropertyAccessException("text");
            textView = null;
        }
        textView.setText("");
        View view2 = this$0.view;
        if (view2 == null) {
            Intrinsics.throwUninitializedPropertyAccessException("view");
        } else {
            view = view2;
        }
        view.setVisibility(View.GONE);
    }

    public final void hide() {
        Log.i(TAG, "[!!] hide");
        GameActivityBase gameActivityBase = this.activity;
        if (gameActivityBase == null) {
            Intrinsics.throwUninitializedPropertyAccessException("activity");
            gameActivityBase = null;
        }
        gameActivityBase.runOnUiThread(new Runnable() { // from class: com.rockstargames.oswrapper.view.SplashScreenHandler$$ExternalSyntheticLambda1
            @Override // java.lang.Runnable
            public final void run() {
                SplashScreenHandler.hide$lambda$1(SplashScreenHandler.this);
            }
        });
    }


    public final boolean getVisible() {
        return view != null && view.getVisibility() != View.GONE;
    }

    // ─── Content ─────────────────────────────────────────────────────────────────

    public final void setImage(final String filename) {
        if (filename == null) throw new NullPointerException("filename is null");
        Log.i(TAG, "[!!] setImage: " + filename);

        getActivity().runOnUiThread(() -> {
            try (InputStream stream = getActivity().getAssets().open(filename)) {
                image.setImageDrawable(Drawable.createFromStream(stream, null));
                view.setVisibility(View.VISIBLE);
            } catch (Exception e) {
                Log.w(TAG, "[!!] setImage: failed to load: " + filename, e);
            }
        });
    }

    public final void setText(String value) {
        if (value == null) throw new NullPointerException("value is null");
        final String trimmed = value.trim();
        Log.i(TAG, "[!!] setText: " + trimmed);

        getActivity().runOnUiThread(() -> {
            view.setVisibility(View.VISIBLE);

            float scaledSize   = 24.0f * (view.getMeasuredHeight() / 1024.0f);
            float adjustedSize = scaledSize / view.getResources().getDisplayMetrics().density;

            Log.d(TAG, "[!!] setText: SC size = " + scaledSize + " adjusted = " + adjustedSize);

            text.setText(trimmed);
            text.setTextSize(adjustedSize);
        });
    }

    // ─── GameViewHandler ─────────────────────────────────────────────────────────

    @Override
    public void onPause()  { /* nothing needed */ }

    @Override
    public void onResume() { /* nothing needed */ }

    // ─── Private Util ────────────────────────────────────────────────────────────

    private GameActivityBase getActivity() {
        if (activity == null) throw new IllegalStateException("activity not set — call setup() first");
        return activity;
    }
}