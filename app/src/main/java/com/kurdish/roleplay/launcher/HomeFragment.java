package com.kurdish.roleplay.launcher;

import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import com.kurdish.roleplay.R;
import com.kurdish.roleplay.game.SAMP;
import com.kurdish.roleplay.launcher.util.RemoteConfigManager;

public class HomeFragment extends Fragment {

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater,
                             @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {

        View view = inflater.inflate(R.layout.fragment_home, container, false);

        View btnPlay = view.findViewById(R.id.btnPlay);

        View discordBtn = view.findViewById(R.id.discordBtn);
        View webBtn = view.findViewById(R.id.webBtn);
        View youtubeBtn = view.findViewById(R.id.youtubeBtn);

        discordBtn.setOnClickListener(v -> {
            String link = RemoteConfigManager.getString("discord");
            openUrl(link);
        });

        webBtn.setOnClickListener(v -> {
            String link = RemoteConfigManager.getString("website");
            openUrl(link);
        });

        youtubeBtn.setOnClickListener(v -> {
            String link = RemoteConfigManager.getString("youtube");
            openUrl(link);
        });

        btnPlay.setOnClickListener(v -> {
            android.util.Log.d("HomeFragment", "Play button clicked");

            Intent intent = new Intent(requireActivity(), SAMP.class);
            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
            startActivity(intent);

            requireActivity().overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);
        });

        return view;
    }
    private void openUrl(String url) {
        Intent intent = new Intent(Intent.ACTION_VIEW, android.net.Uri.parse(url));
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(intent);
    }
}
