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


import com.kurdish.roleplay.R;
import com.rockstargames.oswrapper.GameActivityBase;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

import kotlin.jvm.internal.Intrinsics;

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
    public static final void movieTryStartPlayback$lambda$9$lambda$6(MediaPlayer mediaPlayer) {
        Log.i(TAG, "[!!] moviePlayer: onSeekCompleteListener");
        mediaPlayer.start();
    }
    public static final void movieTryStartPlayback$lambda$9$lambda$5(MovieHandler this$0, MediaPlayer mediaPlayer) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        Log.i(TAG, "[!!] moviePlayer: onPreparedListener: " + this$0.moviePausedAt);
        mediaPlayer.seekTo(this$0.moviePausedAt);
    }
    public static final boolean movieTryStartPlayback$lambda$9$lambda$7(MovieHandler this$0, MediaPlayer mediaPlayer, int i, int i2) throws Exception {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        Log.e(TAG, "[!!] moviePlayer: onErrorListener: " + i + " / " + i2);
        this$0.stopMovie();
        return false;
    }
    public static final void movieTryStartPlayback$lambda$9$lambda$8(MovieHandler this$0, MediaPlayer mediaPlayer) throws Exception {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        Log.i(TAG, "[!!] moviePlayer: onCompletionListener");
        this$0.stopMovie();
    }

    public void movieTryStartPlayback(MovieHandler this) {
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

                moviePlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() { // from class: com.rockstargames.oswrapper.view.MovieHandler$$ExternalSyntheticLambda3
                    @Override // android.media.MediaPlayer.OnPreparedListener
                    public final void onPrepared(MediaPlayer mediaPlayer3) {
                        MovieHandler.movieTryStartPlayback$lambda$9$lambda$5(MovieHandler.this, mediaPlayer3);
                    }
                });


                moviePlayer.setOnSeekCompleteListener(new MediaPlayer.OnSeekCompleteListener() { // from class: com.rockstargames.oswrapper.view.MovieHandler$$ExternalSyntheticLambda4
                    @Override // android.media.MediaPlayer.OnSeekCompleteListener
                    public final void onSeekComplete(MediaPlayer mediaPlayer3) {
                        MovieHandler.movieTryStartPlayback$lambda$9$lambda$6(mediaPlayer3);
                    }
                });


                moviePlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() { // from class: com.rockstargames.oswrapper.view.MovieHandler$$ExternalSyntheticLambda5
                    @Override // android.media.MediaPlayer.OnErrorListener
                    public final boolean onError(MediaPlayer mediaPlayer3, int i, int i2) {
                        try {
                            return MovieHandler.movieTryStartPlayback$lambda$9$lambda$7(MovieHandler.this, mediaPlayer3, i, i2);
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }
                    }
                });
                moviePlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() { // from class: com.rockstargames.oswrapper.view.MovieHandler$$ExternalSyntheticLambda6
                    @Override // android.media.MediaPlayer.OnCompletionListener
                    public final void onCompletion(MediaPlayer mediaPlayer3) {
                        try {
                            MovieHandler.movieTryStartPlayback$lambda$9$lambda$8(MovieHandler.this, mediaPlayer3);
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }
                    }
                });
                moviePlayer.setDataSource(
                        afd.getFileDescriptor(),
                        afd.getStartOffset(),
                        afd.getLength());

                SurfaceView surfaceView = null;
                SurfaceView surfaceView2 = this.viewMovie;
                if (surfaceView2 == null) {
                    Intrinsics.throwUninitializedPropertyAccessException("viewMovie");
                } else {
                    surfaceView = surfaceView2;
                }
                moviePlayer.setDisplay(surfaceView.getHolder());
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

    private final void drawMovieText() {
        GameActivityBase gameActivityBase = this.activity;
        if (gameActivityBase == null) {
            Intrinsics.throwUninitializedPropertyAccessException("activity");
            gameActivityBase = null;
        }
        gameActivityBase.runOnUiThread(new Runnable() { // from class: com.rockstargames.oswrapper.view.MovieHandler$$ExternalSyntheticLambda0
            @Override // java.lang.Runnable
            public final void run() {
                MovieHandler.drawMovieText$lambda$4(MovieHandler.this);
            }
        });
    }
    public static final void drawMovieText$lambda$4(MovieHandler this$0) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        if (this$0.movieTextSurfaceReady) {
            SurfaceView surfaceView = this$0.viewMovieText;
            SurfaceView surfaceView2 = null;
            if (surfaceView == null) {
                Intrinsics.throwUninitializedPropertyAccessException("viewMovieText");
                surfaceView = null;
            }
            Canvas canvasLockCanvas = surfaceView.getHolder().lockCanvas();
            canvasLockCanvas.drawColor(0, PorterDuff.Mode.CLEAR);
            if (this$0.movieCurrentText.length() > 0) {
                int width = canvasLockCanvas.getWidth();
                int height = canvasLockCanvas.getHeight();
                TextPaint textPaint = new TextPaint();
                textPaint.setTextSize(this$0.movieCurrentTextScale);
                textPaint.setColor(-1);
                TextPaint textPaint2 = new TextPaint();
                textPaint2.setTextSize(textPaint.getTextSize());
                textPaint2.setColor(ViewCompat.MEASURED_STATE_MASK);
                String str = this$0.movieCurrentText;
                int i = width - 100;
                StaticLayout staticLayoutBuild = StaticLayout.Builder.obtain(str, 0, str.length(), textPaint, i).setAlignment(Layout.Alignment.ALIGN_CENTER).setLineSpacing(1.0f, 1.0f).setIncludePad(true).build();
                Intrinsics.checkNotNullExpressionValue(staticLayoutBuild, "build(...)");
                String str2 = this$0.movieCurrentText;
                StaticLayout staticLayoutBuild2 = StaticLayout.Builder.obtain(str2, 0, str2.length(), textPaint2, i).setAlignment(Layout.Alignment.ALIGN_CENTER).setLineSpacing(1.0f, 1.0f).setIncludePad(true).build();
                Intrinsics.checkNotNullExpressionValue(staticLayoutBuild2, "build(...)");
                canvasLockCanvas.translate(52.0f, (height - staticLayoutBuild2.getHeight()) - 5);
                staticLayoutBuild2.draw(canvasLockCanvas);
                canvasLockCanvas.translate(-2.0f, -2.0f);
                staticLayoutBuild.draw(canvasLockCanvas);
            }
            SurfaceView surfaceView3 = this$0.viewMovieText;
            if (surfaceView3 == null) {
                Intrinsics.throwUninitializedPropertyAccessException("viewMovieText");
            } else {
                surfaceView2 = surfaceView3;
            }
            surfaceView2.getHolder().unlockCanvasAndPost(canvasLockCanvas);
        }
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