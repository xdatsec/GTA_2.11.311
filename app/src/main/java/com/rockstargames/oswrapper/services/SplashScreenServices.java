package com.rockstargames.oswrapper.services;

import com.rockstargames.oswrapper.GameView;
import kotlin.jvm.internal.Intrinsics;

public final class SplashScreenServices implements ISplashScreenServices {
    private final GameView view;

    public SplashScreenServices(GameView view) {
        Intrinsics.checkNotNullParameter(view, "view");
        this.view = view;
    }

    public final GameView getView() {
        return this.view;
    }

    @Override // com.rockstargames.oswrapper.services.ISplashScreenServices
    public void hideSplashScreen() {
        this.view.getSplashScreenHandler().hide();
    }

    @Override // com.rockstargames.oswrapper.services.ISplashScreenServices
    public boolean isSplashScreenVisible() {
        return this.view.getSplashScreenHandler().getVisible();
    }

    @Override // com.rockstargames.oswrapper.services.ISplashScreenServices
    public void setSplashImage(String filename) {
        Intrinsics.checkNotNullParameter(filename, "filename");
        this.view.getSplashScreenHandler().setImage(filename);
    }

    @Override // com.rockstargames.oswrapper.services.ISplashScreenServices
    public void setSplashText(String text) {
        Intrinsics.checkNotNullParameter(text, "text");
        this.view.getSplashScreenHandler().setText(text);
    }

    @Override // com.rockstargames.oswrapper.services.ISplashScreenServices
    public void showSplashScreen() {
        this.view.getSplashScreenHandler().show();
    }
}
