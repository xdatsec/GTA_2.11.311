package com.rockstargames.oswrapper.services;

public interface IRockstarServices {
    void httpCancel(int id);

    void httpGet(int id, String url, String[] headerNames, String[] headerValues);

    void httpHead(int id, String url);

    void httpPost(int id, String url, String[] headerNames, String[] headerValues, byte[] data);

    void rockstarAccountDeletion();

    void rockstarFetchId();

    boolean rockstarInTrial();

    void rockstarRequestReview();

    void rockstarSetLocalePriority(String localeTag);

    void rockstarShowCloudDisabled();

    void rockstarShowGate(int id);

    void rockstarShowInitial();

    void rockstarSignIn();

    void rockstarSignOut();
}
