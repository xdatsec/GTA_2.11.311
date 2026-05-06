package com.rockstargames.oswrapper.view;

import android.content.ContentUris;
import android.database.Cursor;
import android.media.AudioAttributes;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.provider.MediaStore;
import android.util.Log;
import com.rockstargames.oswrapper.GameActivityBase;
import com.rockstargames.oswrapper.GameThread;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;

public final class PlaylistHandler implements GameViewHandler,
        MediaPlayer.OnCompletionListener,
        MediaPlayer.OnPreparedListener {

    private static final String TAG = "GameView.PlaylistHandler";
    private static final String SOURCE_ID = "SourceId";

    private GameActivityBase activity;
    private int currentAt;
    private int currentIndex;
    private int currentLength;
    private boolean isPaused;
    private boolean wasPlaying;
    private MediaPlayer player;
    private float volume = 1.0f;

    private final Object lock = new Object();
    private final List<Uri> playlist = new ArrayList<>();
    private final AtomicBoolean isAvailable = new AtomicBoolean(false);

    // ─── Helpers ────────────────────────────────────────────────────────────────

    private void add(long id) {
        Uri uri = ContentUris.withAppendedId(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, id);
        Log.i(TAG, "[!!] add: " + uri);
        playlist.add(uri);
    }

    // ─── Open ────────────────────────────────────────────────────────────────────

    public final void open(final String name) {
        if (name == null) throw new NullPointerException("name is null");
        Log.i(TAG, "[!!] open: " + name);

        String permission = Build.VERSION.SDK_INT >= 33
                ? "android.permission.READ_MEDIA_AUDIO"
                : "android.permission.READ_EXTERNAL_STORAGE";

        Log.i(TAG, "[!!] open: requesting permission " + permission);

        getActivity().requestPermission(permission, granted -> {
            Log.i(TAG, "[!!] open: permission = " + granted);
            doOpen(name, granted);
        });
    }

    private void doOpen(final String name, final boolean granted) {
        Log.i(TAG, "[!!] doOpen: " + name + " (permission = " + granted + ")");
        getActivity().runOnUiThread(() -> {
            synchronized (lock) {
                currentLength = 0;
                currentIndex  = 0;
                currentAt     = 0;
                isPaused      = false;

                if (player != null) {
                    player.release();
                    player = null;
                }
                playlist.clear();

                if (granted) {
                    doOpenGoogleMusic(name);
                    doOpenStorageFiles(name);
                }

                Log.i(TAG, "[!!] open: final playlist count: " + playlist.size());
                isAvailable.set(!playlist.isEmpty());
            }
            GameThread.INSTANCE.onPlaylistOpenComplete(isAvailable.get(), getCount());
        });
    }

    private void doOpenGoogleMusic(String name) {
        try {
            Log.i(TAG, "[!!] doOpenGoogleMusic: " + name);

            Cursor playlistCursor = getActivity().getContentResolver().query(
                    Uri.parse("content://com.google.android.music.MusicContent/playlists"),
                    new String[]{"_id"},
                    "playlist_name = ?",
                    new String[]{name},
                    null);

            if (playlistCursor == null) return;

            Long playlistId = null;
            try {
                if (playlistCursor.getCount() == 1) {
                    playlistCursor.moveToFirst();
                    playlistId = playlistCursor.getLong(playlistCursor.getColumnIndexOrThrow("_id"));
                }
            } finally {
                playlistCursor.close();
            }

            if (playlistId == null) return;

            Log.i(TAG, "[!!] doOpenGoogleMusic: playlist ID: " + playlistId);

            Cursor memberCursor = getActivity().getContentResolver().query(
                    Uri.parse("content://com.google.android.music.MusicContent/playlists/" + playlistId + "/members"),
                    new String[]{SOURCE_ID},
                    null, null, null);

            if (memberCursor == null) return;

            try {
                memberCursor.moveToFirst();
                Log.i(TAG, "[!!] doOpenGoogleMusic: playlist count: " + memberCursor.getCount());
                while (!memberCursor.isAfterLast()) {
                    add(memberCursor.getLong(memberCursor.getColumnIndexOrThrow(SOURCE_ID)));
                    memberCursor.moveToNext();
                }
            } finally {
                memberCursor.close();
            }

        } catch (Exception e) {
            Log.w(TAG, "[!!] doOpenGoogleMusic: failed", e);
        }
    }

    private void doOpenStorageFiles(String name) {
        try {
            Log.i(TAG, "[!!] doOpenStorageFiles: " + name);

            Cursor cursor = getActivity().getContentResolver().query(
                    MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
                    new String[]{"_id", "_data"},
                    null, null, null);

            if (cursor == null) return;

            try {
                cursor.moveToFirst();
                Log.i(TAG, "[!!] doOpenStorageFiles: files count: " + cursor.getCount());
                String prefix = "/storage/emulated/0/music/" + name + "/";
                while (!cursor.isAfterLast()) {
                    String path = cursor.getString(cursor.getColumnIndexOrThrow("_data"));
                    long id     = cursor.getLong(cursor.getColumnIndexOrThrow("_id"));
                    if (path != null && path.toLowerCase().startsWith(prefix.toLowerCase())) {
                        add(id);
                    }
                    cursor.moveToNext();
                }
            } finally {
                cursor.close();
            }

        } catch (Exception e) {
            Log.w(TAG, "[!!] doOpenStorageFiles: failed", e);
        }
    }

    // ─── Playback ────────────────────────────────────────────────────────────────

    public final void play() {
        Log.i(TAG, "[!!] play");
        if (!isAvailable.get()) return;
        getActivity().runOnUiThread(() -> {
            synchronized (lock) {
                if (currentIndex >= playlist.size()) {
                    Log.w(TAG, "[!!] play: cannot play: playlist empty");
                    isAvailable.set(false);
                    return;
                }
                try {
                    isPaused = false;
                    if (player == null) {
                        player = new MediaPlayer();
                        player.setAudioAttributes(
                                new AudioAttributes.Builder()
                                        .setLegacyStreamType(3)
                                        .build());
                    }
                    Uri uri = playlist.get(currentIndex);
                    Log.i(TAG, "[!!] play: " + uri);
                    player.reset();
                    player.setOnCompletionListener(this);
                    player.setOnPreparedListener(this);
                    player.setDataSource(getActivity(), uri);
                    player.setVolume(volume, volume);
                    player.prepareAsync();

                } catch (IOException e) {
                    Log.w(TAG, "[!!] play: I/O error", e);
                    playlist.remove(currentIndex);
                    if (currentIndex >= playlist.size()) currentIndex = 0;
                    if (player != null) player.reset();
                    play();

                } catch (Exception e) {
                    Log.w(TAG, "[!!] play: failed to start", e);
                }
            }
        });
    }

    public final void pause() {
        Log.i(TAG, "[!!] pause");
        if (!isAvailable.get()) return;
        getActivity().runOnUiThread(() -> {
            try {
                synchronized (lock) {
                    if (player != null) player.stop();
                    currentAt = (player != null) ? player.getCurrentPosition() : 0;
                }
                isPaused = true;
            } catch (Exception e) {
                Log.w(TAG, "[!!] pause: error", e);
            }
        });
    }

    public final void stop() {
        Log.i(TAG, "[!!] stop");
        if (!isAvailable.get()) return;
        getActivity().runOnUiThread(() -> {
            try {
                synchronized (lock) {
                    if (player != null) player.stop();
                }
                currentAt = 0;
            } catch (Exception e) {
                Log.w(TAG, "[!!] stop: error", e);
            }
        });
    }

    public final void setVolume(final float volume) {
        Log.i(TAG, "[!!] setVolume: " + volume);
        getActivity().runOnUiThread(() -> {
            this.volume = volume;
            if (player != null) player.setVolume(volume, volume);
        });
    }

    // ─── MediaPlayer Listeners ───────────────────────────────────────────────────

    @Override
    public void onPrepared(MediaPlayer mp) {
        Log.i(TAG, "[!!] onPrepared");
        if (mp != null) {
            mp.seekTo(currentAt);
            mp.start();
        }
        currentAt     = 0;
        currentLength = (mp != null) ? mp.getDuration() : -1;
    }

    @Override
    public void onCompletion(MediaPlayer mp) {
        Log.i(TAG, "[!!] onCompletion");
        if (currentAt >= currentLength || currentLength == -1) {
            currentIndex++;
            if (currentIndex >= playlist.size()) currentIndex = 0;
        }
        if (mp != null) mp.reset();
        if (!isPaused) play();
    }

    // ─── GameViewHandler ─────────────────────────────────────────────────────────

    @Override
    public void setup(GameActivityBase activity) {
        if (activity == null) throw new NullPointerException("activity is null");
        this.activity = activity;
    }

    @Override
    public void onPause() {
        wasPlaying = isPlaying();
        pause();
    }

    @Override
    public void onResume() {
        if (wasPlaying) {
            wasPlaying = false;
            play();
        }
    }

    // ─── Getters ─────────────────────────────────────────────────────────────────

    public final int getCount() {
        synchronized (lock) {
            return playlist.size();
        }
    }

    public final boolean isPlaying() {
        synchronized (lock) {
            return (player != null) && player.isPlaying();
        }
    }

    // ─── Private Util ────────────────────────────────────────────────────────────

    private GameActivityBase getActivity() {
        if (activity == null) throw new IllegalStateException("activity not set — call setup() first");
        return activity;
    }
}