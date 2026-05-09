package com.rockstargames.oswrapper.view;

import com.rockstargames.oswrapper.GameActivityBase;

public interface GameViewHandler  {
    void onPause();

    void onResume();

    void setup(GameActivityBase activity);
}
