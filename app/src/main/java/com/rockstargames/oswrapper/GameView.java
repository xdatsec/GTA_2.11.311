package com.rockstargames.oswrapper;

import android.content.Context;
import android.content.res.Configuration;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import com.rockstargames.oswrapper.view.GameViewHandler;
import com.rockstargames.oswrapper.view.InputHandler;
import com.rockstargames.oswrapper.view.MovieHandler;
import com.rockstargames.oswrapper.view.PlaylistHandler;
import com.rockstargames.oswrapper.view.SensorHandler;
import com.rockstargames.oswrapper.view.SplashScreenHandler;
import java.util.Iterator;
import java.util.List;
import kotlin.jvm.internal.Intrinsics;


public   class GameView extends SurfaceView implements SurfaceHolder.Callback2 {
    private static  String TAG = "GameView";
    private GameActivityBase activity;
    private  List<GameViewHandler> handlers;
    private boolean hasFocus;
    private  InputHandler inputHandler;
    private  MovieHandler movieHandler;
    private  PlaylistHandler playlistHandler;
    private  SensorHandler sensorHandler;
    private  SplashScreenHandler splashScreenHandler;

    /* JADX WARN: 'super' call moved to the top of the method (can break code semantics) */
    public GameView(Context context, AttributeSet attribs) {
        super(context, attribs);

        getHolder().addCallback(this);

        // سڕینەوەی Intrinsics چونکە لە جاڤادا پێویست نین
        this.hasFocus = true;

        // دروستکردنی هاندلەرەکان بە شێوەیەکی ڕێک
        this.inputHandler = InputHandler.INSTANCE;
        this.movieHandler = new MovieHandler();
        this.playlistHandler = new PlaylistHandler();
        this.sensorHandler = new SensorHandler(context, this);
        this.splashScreenHandler = new SplashScreenHandler();

        // چاککردنی لیستەکە بە بەکارهێنانی Arrays.asList بۆ ئەوەی کێشەی تایپی نەمێنێت
        // ئەمە ڕێگری دەکات لە ئیرۆری Found: List<Object>, Required: List<GameViewHandler>
        this.handlers = java.util.Arrays.asList(
                this.inputHandler,
                this.movieHandler,
                this.playlistHandler,
                this.sensorHandler,
                this.splashScreenHandler
        );
    }

    private   void setHasFocus(boolean z) {
        Log.i(TAG, "[!!] hasFocus = " + z);
        this.hasFocus = z;
    }

    public   boolean getHasFocus() {
        return this.hasFocus;
    }

    public   InputHandler getInputHandler() {
        return this.inputHandler;
    }

    public   MovieHandler getMovieHandler() {
        return this.movieHandler;
    }

    public   PlaylistHandler getPlaylistHandler() {
        return this.playlistHandler;
    }

    public   SensorHandler getSensorHandler() {
        return this.sensorHandler;
    }

    public   SplashScreenHandler getSplashScreenHandler() {
        return this.splashScreenHandler;
    }

    @Override // android.view.View
    protected void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        Log.i(TAG, "[!!] onConfigurationChanged: " + newConfig);
        this.sensorHandler.updateRotation();
    }

    @Override // android.view.View
    public boolean onGenericMotionEvent(MotionEvent event) {
        return this.inputHandler.onGenericMotionEvent(event) || super.onGenericMotionEvent(event);
    }

    @Override // android.view.View, android.view.KeyEvent.Callback
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return this.inputHandler.onKeyDown(keyCode, event) || super.onKeyDown(keyCode, event);
    }

    @Override // android.view.View, android.view.KeyEvent.Callback
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        return this.inputHandler.onKeyUp(keyCode, event) || super.onKeyUp(keyCode, event);
    }

    public void onPause() {
        Log.i(TAG, "[!!] onPause");
        GameThread.INSTANCE.onPause();
        Iterator<GameViewHandler> it = this.handlers.iterator();
        while (it.hasNext()) {
            it.next().onPause();
        }
    }

    public void onResume() {
        Log.i(TAG, "[!!] onResume");
        GameThread.INSTANCE.onResume();
        // چاککردنی لووپەکە لێرەدا
        for (GameViewHandler handler : this.handlers) {
            handler.onResume();
        }
    }

    @Override // android.view.View
    public boolean onTouchEvent(MotionEvent event) {
        Intrinsics.checkNotNullParameter(event, "event");
        return this.inputHandler.onTouchEvent(event) || super.onTouchEvent(event);
    }

    @Override // android.view.View
    public void onWindowFocusChanged(boolean hasWindowFocus) {
        super.onWindowFocusChanged(hasWindowFocus);
        Log.i(TAG, "[!!] onWindowFocusChanged: " + hasWindowFocus);
        setHasFocus(hasWindowFocus);
        if (this.hasFocus) {
            onResume();
        } else {
            onPause();
        }
    }

    public void setup(GameActivityBase activity) {
        Intrinsics.checkNotNullParameter(activity, "activity");
        getHolder().setFormat(3);
        getHolder().addCallback(this);
        setKeepScreenOn(true);
        setFocusableInTouchMode(true);
        requestFocus();
        this.activity = activity;
        Iterator<GameViewHandler> it = this.handlers.iterator();
        while (it.hasNext()) {
            ((GameViewHandler) it.next()).setup(activity);
        }

    }
    @Override // android.view.SurfaceHolder.Callback
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Intrinsics.checkNotNullParameter(holder, "holder");
        Surface surface = holder.getSurface();
        Log.i(TAG, "[!!] surfaceChanged: " + width + 'x' + height + ": " + surface);
        GameThread gameThread = GameThread.INSTANCE;
        Intrinsics.checkNotNull(surface);
        gameThread.onSurfaceChanged(surface, width, height);
    }

    @Override // android.view.SurfaceHolder.Callback
    public void surfaceCreated(SurfaceHolder holder) {
        Intrinsics.checkNotNullParameter(holder, "holder");
        Log.i(TAG, "[!!] surfaceCreated: " + holder.getSurface());


        GameThread.INSTANCE.onSurfaceCreated();
    }

    @Override // android.view.SurfaceHolder.Callback
    public void surfaceDestroyed(SurfaceHolder holder) {
        Intrinsics.checkNotNullParameter(holder, "holder");
        Log.i(TAG, "[!!] surfaceDestroyed");
        GameThread.INSTANCE.onSurfaceDestroyed();
    }

    @Override // android.view.SurfaceHolder.Callback2
    public void surfaceRedrawNeeded(SurfaceHolder holder) {
        Intrinsics.checkNotNullParameter(holder, "holder");
    }

    @Override // android.view.SurfaceHolder.Callback2
    public void surfaceRedrawNeededAsync(SurfaceHolder holder, Runnable drawingFinished) {
        Intrinsics.checkNotNullParameter(holder, "holder");
        Intrinsics.checkNotNullParameter(drawingFinished, "drawingFinished");
        GameThread.INSTANCE.onSurfaceRedrawNeededAsync(drawingFinished);
    }
}
