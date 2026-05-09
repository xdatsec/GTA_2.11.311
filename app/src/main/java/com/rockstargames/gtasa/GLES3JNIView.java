package com.rockstargames.gtasa;
import android.opengl.GLSurfaceView;
import android.content.Context;
import android.graphics.PixelFormat;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

public class GLES3JNIView extends GLSurfaceView implements GLSurfaceView.Renderer {
	
	public static byte fontData[];
    public GLES3JNIView(Context context) {
        super(context);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
		getHolder().setFormat(-3);
        setEGLContextClientVersion(3);
        setRenderer(this);
    }
    
	public void onDrawFrame(GL10 gl) {
		step();
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		resize(width, height);
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		init();
	}
	
    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        imgui_Shutdown();
    }
	
	public static native void init();
	public static native void resize(int width, int height);
	public static native void step();
	public static native void imgui_Shutdown();
	public static native void MotionEventClick(boolean down,float PosX,float PosY);
	public static native String getWindowRect();
}
