package com.rockstargames.oswrapper.services;

public interface ISplashScreenServices {
    void hideSplashScreen();

    boolean isSplashScreenVisible();

    void setSplashImage(String filename);

    void setSplashText(String text);

    void showSplashScreen();
}
