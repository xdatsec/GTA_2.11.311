package com.rockstargames.oswrapper.services;

import com.rockstargames.oswrapper.GameView;

public final class MovieServices implements IMovieServices {
    private final GameView view;

    public MovieServices(GameView view) {
        this.view = view;
    }

    @Override
    public boolean isMoviePlaying() {
        // هەمیشە دەڵێین ڤیدیۆ لێنادات بۆ ئەوەی یارییەکە نەوەستێت و چاوەڕێ نەکات
        return false;
    }

    @Override
    public void playMovie(String filename, boolean skippable) {
        // هیچ مەکە - ڤیدیۆ لێ مەدە
    }

    @Override
    public void setMovieText(String text) {
        // هیچ مەکە
    }

    @Override
    public void setMovieTextScale(int scale) {
        // هیچ مەکە
    }

    @Override
    public void stopMovie() {
        // هیچ مەکە
    }
}