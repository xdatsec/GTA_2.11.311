package com.rockstargames.oswrapper;

import java.util.ArrayList;
import java.util.List;

public final class DownloaderConfig {

    private static final int MAX_PACKS = 8; // matches PACK_VIEWS size

    private final int background;
    private final Class<? extends GameActivityBase> activity;
    private final List<DownloaderPackConfig> packs;

    public DownloaderConfig(int background,
                            Class<? extends GameActivityBase> activity,
                            List<DownloaderPackConfig> packs) throws Exception {
        if (activity == null) throw new NullPointerException("activity is null");
        if (packs    == null) throw new NullPointerException("packs is null");

        if (packs.size() > MAX_PACKS) {
            throw new Exception(
                    "Exceeded maximum number of configurable data packs (got "
                            + packs.size() + ", expected at most " + MAX_PACKS + ')');
        }

        this.background = background;
        this.activity   = activity;
        this.packs      = packs;
    }

    // -----------------------------------------------------------------------
    // Getters
    // -----------------------------------------------------------------------

    public int getBackground() {
        return background;
    }

    public Class<? extends GameActivityBase> getActivity() {
        return activity;
    }

    /** Returns ALL packs (INSTALL + RUNTIME). */
    public List<DownloaderPackConfig> getAllPacks() {
        return packs;
    }

    /** Returns only RUNTIME packs (used by the downloader UI). */
    public List<DownloaderPackConfig> getRuntimePacks() {
        List<DownloaderPackConfig> result = new ArrayList<>();
        for (DownloaderPackConfig pack : packs) {
            if (pack.getType() == DownloaderPackType.RUNTIME) {
                result.add(pack);
            }
        }
        return result;
    }

    // -----------------------------------------------------------------------
    // equals / hashCode / toString
    // -----------------------------------------------------------------------

    @Override
    public boolean equals(Object other) {
        if (this == other) return true;
        if (!(other instanceof DownloaderConfig)) return false;
        DownloaderConfig o = (DownloaderConfig) other;
        return background == o.background
                && activity.equals(o.activity)
                && packs.equals(o.packs);
    }

    @Override
    public int hashCode() {
        int result = Integer.hashCode(background);
        result = 31 * result + activity.hashCode();
        result = 31 * result + packs.hashCode();
        return result;
    }

    @Override
    public String toString() {
        return "DownloaderConfig(background=" + background
                + ", activity=" + activity
                + ", packs=" + packs + ')';
    }
}