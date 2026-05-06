package com.rockstargames.oswrapper.services;

import android.util.Log;
import com.rockstargames.oswrapper.GameView;

import kotlin.jvm.internal.Intrinsics;

public final class PlaylistServices implements IPlaylistServices {
    private static final String TAG = "PlaylistServices";
    private final GameView view;

    public PlaylistServices(GameView view) {
        Intrinsics.checkNotNullParameter(view, "view");
        this.view = view;
    }

    @Override // com.rockstargames.oswrapper.services.IPlaylistServices
    public int playlistCount() {
        return this.view.getPlaylistHandler().getCount();
    }

    @Override // com.rockstargames.oswrapper.services.IPlaylistServices
    public boolean playlistIsPlaying() {
        return this.view.getPlaylistHandler().isPlaying();
    }

    @Override // com.rockstargames.oswrapper.services.IPlaylistServices
    public void playlistOpen(String name) {
        Intrinsics.checkNotNullParameter(name, "name");
        Log.i(TAG, "[!!] playlistOpen: " + name);
        this.view.getPlaylistHandler().open(name);
    }

    @Override // com.rockstargames.oswrapper.services.IPlaylistServices
    public void playlistPause() {
        Log.i(TAG, "[!!] playlistPause");
        this.view.getPlaylistHandler().pause();
    }

    @Override // com.rockstargames.oswrapper.services.IPlaylistServices
    public void playlistPlay() {
        Log.i(TAG, "[!!] playlistPlay");
        this.view.getPlaylistHandler().play();
    }

    @Override // com.rockstargames.oswrapper.services.IPlaylistServices
    public void playlistSetVolume(float volume) {
        Log.i(TAG, "[!!] playlistSetVolume: " + volume);
        this.view.getPlaylistHandler().setVolume(volume);
    }

    @Override // com.rockstargames.oswrapper.services.IPlaylistServices
    public void playlistStop() {
        Log.i(TAG, "[!!] playlistStop");
        this.view.getPlaylistHandler().stop();
    }
}
