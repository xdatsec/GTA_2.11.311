package com.rockstargames.oswrapper.view;

import android.content.res.AssetFileDescriptor;
import android.graphics.Canvas;
import android.graphics.PorterDuff;
import android.media.MediaPlayer;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import androidx.core.view.ViewCompat;


import com.rockstargames.gtasa.R;
import com.rockstargames.oswrapper.GameActivityBase;
import java.util.concurrent.Callable;
import java.util.concurrent.FutureTask;

public final class MovieHandler implements GameViewHandler {

    private static final String TAG = "GameView.MovieHandler";

    private GameActivityBase activity;
    private int moviePausedAt   = 0;
    private MediaPlayer moviePlayer;
    private boolean moviePlaying        = false;
    private boolean movieSurfaceReady   = false;
    private boolean movieTextSurfaceReady = false;
    private SurfaceView viewMovie;
    private SurfaceView viewMovieText;

    private String  movieCurrentText      = "";
    private int     movieCurrentTextScale = 32;
    private boolean movieSkippable        = true;
    private String  movieFilename         = "";

    // ─── Setup ───────────────────────────────────────────────────────────────────

    @Override
    public void setup(GameActivityBase activity) {
        if (activity == null) throw new NullPointerException("activity is null");
        this.activity = activity;

        viewMovie     = (SurfaceView) activity.findViewById(R.id.viewMovie);
        viewMovieText = (SurfaceView) activity.findViewById(R.id.viewMovieText);

        if (viewMovie == null)     throw new IllegalStateException("viewMovie not found in layout");
        if (viewMovieText == null) throw new IllegalStateException("viewMovieText not found in layout");

        // Video surface
        viewMovie.getHolder().setFormat(-3);
        viewMovie.setZOrderOnTop(true);
        viewMovie.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                Log.i(TAG, "[!!] [vid] surfaceCreated: " + holder);
            }
            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                Log.i(TAG, "[!!] [vid] surfaceChanged: " + holder);
                setMovieSurfaceReady(true);
                movieTryStartPlayback();
            }
            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.i(TAG, "[!!] [vid] surfaceDestroyed: " + holder);
                setMovieSurfaceReady(false);
                setMovieCurrentText("");
            }
        });

        // Text surface
        viewMovieText.getHolder().setFormat(-3);
        viewMovieText.setZOrderOnTop(true);
        viewMovieText.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                Log.i(TAG, "[!!] [txt] surfaceCreated: " + holder);
            }
            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                Log.i(TAG, "[!!] [txt] surfaceChanged: " + holder);
                if (!movieTextSurfaceReady) {
                    Canvas canvas = holder.lockCanvas();
                    canvas.drawColor(0, PorterDuff.Mode.CLEAR);
                    holder.unlockCanvasAndPost(canvas);
                }
                setMovieTextSurfaceReady(true);
                movieTryStartPlayback();
            }
            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.i(TAG, "[!!] [txt] surfaceDestroyed: " + holder);
                setMovieTextSurfaceReady(false);
            }
        });
    }

    // ─── Playback ────────────────────────────────────────────────────────────────

    public final void playMovie(final String filename, final boolean skippable)
            throws Exception {
        if (filename == null) throw new NullPointerException("filename is null");
        Log.i(TAG, "[!!] playMovie: " + filename);

        stopMovie();

        FutureTask<Void> task = new FutureTask<>((Callable<Void>) () -> {
            setMoviePlaying(true);
            movieSkippable = skippable;
            movieFilename  = filename;
            viewMovie.setVisibility(SurfaceView.VISIBLE);
            viewMovieText.setVisibility(SurfaceView.VISIBLE);
            return null;
        });

        getActivity().runOnUiThread(task);
        task.get();
    }

    public final void stopMovie() throws Exception {
        Log.i(TAG, "[!!] stopMovie");

        FutureTask<Void> task = new FutureTask<>((Callable<Void>) () -> {
            setMoviePlaying(false);
            setMoviePausedAt(0);
            viewMovie.setVisibility(SurfaceView.GONE);
            viewMovieText.setVisibility(SurfaceView.GONE);
            if (moviePlayer != null) {
                moviePlayer.release();
                moviePlayer = null;
            }
            return null;
        });

        getActivity().runOnUiThread(task);
        task.get();
    }

    private void movieTryStartPlayback() {
        Log.i(TAG, "[!!] movieTryStartPlayback: vid=" + movieSurfaceReady
                + " txt=" + movieTextSurfaceReady + " player=" + moviePlayer);

        if (!movieSurfaceReady || !movieTextSurfaceReady) return;

        getActivity().runOnUiThread(() -> {
            Log.i(TAG, "[!!] movieTryStartPlayback: starting: " + movieFilename + " @ " + moviePausedAt);
            try {
                AssetFileDescriptor afd = getActivity().getAssets().openFd(movieFilename);

                if (moviePlayer != null) {
                    moviePlayer.release();
                    moviePlayer = null;
                }

                moviePlayer = new MediaPlayer();

                moviePlayer.setOnPreparedListener(mp -> {
                    Log.i(TAG, "[!!] moviePlayer: onPrepared: " + moviePausedAt);
                    mp.seekTo(moviePausedAt);
                });

                moviePlayer.setOnSeekCompleteListener(mp -> {
                    Log.i(TAG, "[!!] moviePlayer: onSeekComplete");
                    mp.start();
                });

                moviePlayer.setOnErrorListener((mp, what, extra) -> {
                    Log.e(TAG, "[!!] moviePlayer: onError: " + what + " / " + extra);
                    try { stopMovie(); } catch (Exception e) { Log.w(TAG, "stopMovie failed", e); }
                    return false;
                });

                moviePlayer.setOnCompletionListener(mp -> {
                    Log.i(TAG, "[!!] moviePlayer: onCompletion");
                    try { stopMovie(); } catch (Exception e) { Log.w(TAG, "stopMovie failed", e); }
                });

                moviePlayer.setDataSource(
                        afd.getFileDescriptor(),
                        afd.getStartOffset(),
                        afd.getLength());
                moviePlayer.setDisplay(viewMovie.getHolder());
                moviePlayer.prepareAsync();

            } catch (Exception e) {
                Log.e(TAG, "[!!] Failed to open movie asset: " + movieFilename, e);
                try { stopMovie(); } catch (Exception ex) { Log.w(TAG, "stopMovie failed", ex); }
            }
        });
    }

    // ─── Text / Subtitle ─────────────────────────────────────────────────────────

    public final void setMovieText(final String text) {
        if (text == null) throw new NullPointerException("text is null");
        Log.i(TAG, "[!!] setMovieText: " + text);
        getActivity().runOnUiThread(() -> {
            setMovieCurrentText(text);
            drawMovieText();
        });
    }

    public final void setMovieTextScale(final int scale) {
        getActivity().runOnUiThread(() -> {
            setMovieCurrentTextScale(scale);
            drawMovieText();
        });
    }

    private void drawMovieText() {
        getActivity().runOnUiThread(() -> {
            if (!movieTextSurfaceReady) return;

            Canvas canvas = viewMovieText.getHolder().lockCanvas();
            if (canvas == null) return;

            canvas.drawColor(0, PorterDuff.Mode.CLEAR);

            if (!movieCurrentText.isEmpty()) {
                int width  = canvas.getWidth();
                int height = canvas.getHeight();
                int textWidth = width - 100;

                // White (foreground) paint
                TextPaint paintFg = new TextPaint();
                paintFg.setTextSize(movieCurrentTextScale);
                paintFg.setColor(0xFFFFFFFF);

                // Shadow (background) paint
                TextPaint paintBg = new TextPaint();
                paintBg.setTextSize(movieCurrentTextScale);
                paintBg.setColor(ViewCompat.MEASURED_STATE_MASK);

                StaticLayout layoutFg = StaticLayout.Builder
                        .obtain(movieCurrentText, 0, movieCurrentText.length(), paintFg, textWidth)
                        .setAlignment(Layout.Alignment.ALIGN_CENTER)
                        .setLineSpacing(1.0f, 1.0f)
                        .setIncludePad(true)
                        .build();

                StaticLayout layoutBg = StaticLayout.Builder
                        .obtain(movieCurrentText, 0, movieCurrentText.length(), paintBg, textWidth)
                        .setAlignment(Layout.Alignment.ALIGN_CENTER)
                        .setLineSpacing(1.0f, 1.0f)
                        .setIncludePad(true)
                        .build();

                // Draw shadow first, then foreground offset by 2px
                canvas.save();
                canvas.translate(52.0f, (height - layoutBg.getHeight()) - 5);
                layoutBg.draw(canvas);
                canvas.translate(-2.0f, -2.0f);
                layoutFg.draw(canvas);
                canvas.restore();
            }

            viewMovieText.getHolder().unlockCanvasAndPost(canvas);
        });
    }

    // ─── GameViewHandler ─────────────────────────────────────────────────────────

    @Override
    public void onPause() {
        if (isMoviePlaying()) {
            try { stopMovie(); } catch (Exception e) { Log.w(TAG, "stopMovie failed on pause", e); }
        }
    }

    @Override
    public void onResume() {
        // nothing needed
    }

    // ─── Getters / Setters ───────────────────────────────────────────────────────

    public final boolean isMoviePlaying() {
        return moviePlaying;
    }

    private void setMoviePlaying(boolean value) {
        Log.i(TAG, "[!!] moviePlaying = " + value);
        moviePlaying = value;
        if (!value) setMoviePausedAt(0);
    }

    private void setMoviePausedAt(int value) {
        Log.i(TAG, "[!!] moviePausedAt = " + value);
        moviePausedAt = value;
    }

    private void setMovieCurrentText(String value) {
        Log.i(TAG, "[!!] movieCurrentText = " + value);
        movieCurrentText = value;
    }

    private void setMovieCurrentTextScale(int value) {
        Log.i(TAG, "[!!] movieCurrentTextScale = " + value);
        movieCurrentTextScale = value;
    }

    private void setMovieSurfaceReady(boolean value) {
        Log.i(TAG, "[!!] movieSurfaceReady = " + value);
        movieSurfaceReady = value;
    }

    private void setMovieTextSurfaceReady(boolean value) {
        Log.i(TAG, "[!!] movieTextSurfaceReady = " + value);
        movieTextSurfaceReady = value;
    }

    // ─── Private Util ────────────────────────────────────────────────────────────

    private GameActivityBase getActivity() {
        if (activity == null) throw new IllegalStateException("activity not set — call setup() first");
        return activity;
    }
}