package com.rockstargames.oswrapper.ui.tab;

public class PlayerData {
    int mID;
    int mLevel;
    String mName;
    int mPing;
    String mcolor;
    public PlayerData(int id, String name, int level, int ping, String color) {
        this.mID = id;
        this.mName = name;
        this.mLevel = level;
        this.mPing = ping;
        this.mcolor = color;
    }

    public int getId() {
        return this.mID;
    }

    public String getName() {
        return this.mName;
    }

    public int getPing() {
        return this.mPing;
    }
    public String getColor() {
        return this.mcolor;
    }

    public int getLevel() {
        return this.mLevel;
    }

    public void setId(int id) {
        this.mID = id;
    }

    public void setName(String name) {
        this.mName = name;
    }

    public void setPing(int ping) {
        this.mPing = ping;
    }

    public void setLevel(int level) {
        this.mLevel = level;
    }
}
