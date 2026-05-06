package com.rockstargames.oswrapper;

import com.rockstargames.oswrapper.services.ILanguageServices;
import com.rockstargames.oswrapper.services.IMiscServices;
import com.rockstargames.oswrapper.services.IMovieServices;
import com.rockstargames.oswrapper.services.IPlaylistServices;
import com.rockstargames.oswrapper.services.IRockstarServices;
import com.rockstargames.oswrapper.services.ISplashScreenServices;
import com.rockstargames.oswrapper.services.LanguageServices;
import com.rockstargames.oswrapper.services.MiscServices;
import com.rockstargames.oswrapper.services.MovieServices;
import com.rockstargames.oswrapper.services.PlaylistServices;
import com.rockstargames.oswrapper.services.RockstarServices;
import com.rockstargames.oswrapper.services.SplashScreenServices;

public final class GamePlatformServices implements
        ILanguageServices,
        IMovieServices,
        IPlaylistServices,
        IRockstarServices,
        ISplashScreenServices,
        IMiscServices {

    private final LanguageServices    languageServices;
    private final MovieServices       movieServices;
    private final PlaylistServices    playlistServices;
    private final RockstarServices    rockstarServices;
    private final SplashScreenServices splashScreenServices;
    private final MiscServices        miscServices;

    public final GameActivityBase activity;
    public final GameView         view;

    public GamePlatformServices(GameActivityBase activity, GameView view) {
        if (activity == null) throw new NullPointerException("activity is null");
        if (view == null)     throw new NullPointerException("view is null");

        this.activity = activity;
        this.view     = view;

        this.languageServices     = new LanguageServices(activity);
        this.movieServices        = new MovieServices(view);
        this.playlistServices     = new PlaylistServices(view);
        this.rockstarServices     = new RockstarServices(activity);
        this.splashScreenServices = new SplashScreenServices(view);
        this.miscServices         = new MiscServices(activity);
    }

    // -----------------------------------------------------------------------
    // ILanguageServices
    // -----------------------------------------------------------------------

    @Override
    public int getDeviceLocale() {
        return languageServices.getDeviceLocale();
    }

    // -----------------------------------------------------------------------
    // IMovieServices
    // -----------------------------------------------------------------------

    @Override
    public boolean isMoviePlaying() {
        return movieServices.isMoviePlaying();
    }

    @Override
    public void playMovie(String filename, boolean skippable) {
        if (filename == null) throw new NullPointerException("filename is null");
        movieServices.playMovie(filename, skippable);
    }

    @Override
    public void stopMovie() {
        movieServices.stopMovie();
    }

    @Override
    public void setMovieText(String text) {
        if (text == null) throw new NullPointerException("text is null");
        movieServices.setMovieText(text);
    }

    @Override
    public void setMovieTextScale(int scale) {
        movieServices.setMovieTextScale(scale);
    }

    // -----------------------------------------------------------------------
    // IPlaylistServices
    // -----------------------------------------------------------------------

    @Override
    public int playlistCount() {
        return playlistServices.playlistCount();
    }

    @Override
    public boolean playlistIsPlaying() {
        return playlistServices.playlistIsPlaying();
    }

    @Override
    public void playlistOpen(String name) {
        if (name == null) throw new NullPointerException("name is null");
        playlistServices.playlistOpen(name);
    }

    @Override
    public void playlistPause() {
        playlistServices.playlistPause();
    }

    @Override
    public void playlistPlay() {
        playlistServices.playlistPlay();
    }

    @Override
    public void playlistSetVolume(float volume) {
        playlistServices.playlistSetVolume(volume);
    }

    @Override
    public void playlistStop() {
        playlistServices.playlistStop();
    }

    // -----------------------------------------------------------------------
    // IRockstarServices
    // -----------------------------------------------------------------------

    @Override
    public void httpCancel(int id) {
        rockstarServices.httpCancel(id);
    }

    @Override
    public void httpGet(int id, String url, String[] headerNames, String[] headerValues) {
        if (url == null)          throw new NullPointerException("url is null");
        if (headerNames == null)  throw new NullPointerException("headerNames is null");
        if (headerValues == null) throw new NullPointerException("headerValues is null");
        rockstarServices.httpGet(id, url, headerNames, headerValues);
    }

    @Override
    public void httpHead(int id, String url) {
        if (url == null) throw new NullPointerException("url is null");
        rockstarServices.httpHead(id, url);
    }

    @Override
    public void httpPost(int id, String url, String[] headerNames, String[] headerValues, byte[] data) {
        if (url == null)          throw new NullPointerException("url is null");
        if (headerNames == null)  throw new NullPointerException("headerNames is null");
        if (headerValues == null) throw new NullPointerException("headerValues is null");
        if (data == null)         throw new NullPointerException("data is null");
        rockstarServices.httpPost(id, url, headerNames, headerValues, data);
    }

    @Override
    public void rockstarAccountDeletion() {
        rockstarServices.rockstarAccountDeletion();
    }

    @Override
    public void rockstarFetchId() {
        rockstarServices.rockstarFetchId();
    }

    @Override
    public boolean rockstarInTrial() {
        return rockstarServices.rockstarInTrial();
    }

    @Override
    public void rockstarRequestReview() {
        rockstarServices.rockstarRequestReview();
    }

    @Override
    public void rockstarSetLocalePriority(String localeTag) {
        rockstarServices.rockstarSetLocalePriority(localeTag);
    }

    @Override
    public void rockstarShowCloudDisabled() {
        rockstarServices.rockstarShowCloudDisabled();
    }

    @Override
    public void rockstarShowGate(int id) {
        rockstarServices.rockstarShowGate(id);
    }

    @Override
    public void rockstarShowInitial() {
        rockstarServices.rockstarShowInitial();
    }

    @Override
    public void rockstarSignIn() {
        rockstarServices.rockstarSignIn();
    }

    @Override
    public void rockstarSignOut() {
        rockstarServices.rockstarSignOut();
    }

    // -----------------------------------------------------------------------
    // ISplashScreenServices
    // -----------------------------------------------------------------------

    @Override
    public void hideSplashScreen() {
        splashScreenServices.hideSplashScreen();
    }

    @Override
    public boolean isSplashScreenVisible() {
        return splashScreenServices.isSplashScreenVisible();
    }

    @Override
    public void setSplashImage(String filename) {
        if (filename == null) throw new NullPointerException("filename is null");
        splashScreenServices.setSplashImage(filename);
    }

    @Override
    public void setSplashText(String text) {
        if (text == null) throw new NullPointerException("text is null");
        splashScreenServices.setSplashText(text);
    }

    @Override
    public void showSplashScreen() {
        splashScreenServices.showSplashScreen();
    }

    // -----------------------------------------------------------------------
    // IMiscServices
    // -----------------------------------------------------------------------

    @Override
    public String getAppVersion() {
        return miscServices.getAppVersion();
    }

    @Override
    public void openLink(String url) {
        if (url == null) throw new NullPointerException("url is null");
        miscServices.openLink(url);
    }

    @Override
    public void quit() {
        miscServices.quit();
    }
}