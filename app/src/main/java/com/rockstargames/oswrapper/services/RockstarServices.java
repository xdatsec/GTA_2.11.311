package com.rockstargames.oswrapper.services;

import com.rockstargames.oswrapper.GameActivityBase;
import com.rockstargames.oswrapper.GameThread;

/**
 * ئەم کلاسە بە تەواوی پەکخراوە.
 * هیچ پەیوەندییەک بە ئینتەرنێت یان سێرڤەرەکانی ڕۆکستارەوە ئەنجام نادات.
 */
public final class RockstarServices implements IRockstarServices {

    public RockstarServices(GameActivityBase activity) {
        // هیچ ناکات
    }

    // --- بەشی HTTP (هەمووی بەتاڵ کراوەتەوە) ---

    @Override public void httpCancel(int id) {}
    @Override public void httpGet(int id, String url, String[] names, String[] values) {}
    @Override public void httpHead(int id, String url) {}
    @Override public void httpPost(int id, String url, String[] names, String[] values, byte[] data) {}

    // --- بەشی خزمەتگوزارییەکان (تەنها وەڵامی سەرکەوتنی وەهمی دەنێرێتەوە بۆ ئەوەی یارییەکە نەوەستێت) ---

    @Override public void rockstarFetchId() {
        // ناردنی ناسنامەیەکی بەتاڵ بۆ ئەوەی یارییەکە بەردەوام بێت
        GameThread.INSTANCE.onRockstarIdChanged("OFFLINE_USER");
    }

    @Override public boolean rockstarInTrial() {
        return false;
    }

    @Override public void rockstarRequestReview() {}
    @Override public void rockstarSetLocalePriority(String localeTag) {}

    @Override public void rockstarShowCloudDisabled() {
        GameThread.INSTANCE.onRockstarCloudDisabledComplete();
    }

    @Override public void rockstarShowGate(int id) {
        // ڕاستەوخۆ دەڵێین سەرکەوتوو بوو (true) تاوەکو یارییەکە یەکسەر دەست پێ بکات
        GameThread.INSTANCE.onRockstarGateComplete(id, true);
    }

    @Override public void rockstarShowInitial() {
        GameThread.INSTANCE.onRockstarInitialComplete();
    }

    @Override public void rockstarSignIn() {
        GameThread.INSTANCE.onRockstarSignInComplete();
    }

    @Override public void rockstarSignOut() {
        GameThread.INSTANCE.onRockstarSignOutComplete();
    }



    @Override public void rockstarAccountDeletion() {
        GameThread.INSTANCE.onRockstarAccountDeletionComplete();
    }
}