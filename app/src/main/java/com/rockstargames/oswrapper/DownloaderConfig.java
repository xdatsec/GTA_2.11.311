package com.rockstargames.oswrapper;

import java.util.ArrayList;
import java.util.List;

import kotlin.Unit;
import kotlin.collections.CollectionsKt;
import kotlin.jvm.functions.Function2;
import kotlin.jvm.internal.Intrinsics;

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
    public final void forEachPack(Function2<? super Integer, ? super DownloaderPackConfig, Unit> action) {
        Intrinsics.checkNotNullParameter(action, "action");

        int index = 0;
        // Use the specific type instead of Object to avoid casting issues
        for (DownloaderPackConfig pack : this.packs) {
            // Standard index overflow check (Kotlin style)
            if (index < 0) {
                CollectionsKt.throwIndexOverflow();
            }

            // Invoke the Kotlin Function2
            action.invoke(index, pack);

            index++;
        }
    }


    public final void forEachRuntimePack(Function2<? super Integer, ? super DownloaderPackConfig, Unit> action) {
        Intrinsics.checkNotNullParameter(action, "action");

        // 1. Filter the packs list for RUNTIME types
        List<DownloaderPackConfig> list = this.packs;
        ArrayList<DownloaderPackConfig> runtimePacks = new ArrayList<>();

        for (DownloaderPackConfig pack : list) {
            if (pack.getType() == DownloaderPackType.RUNTIME) {
                runtimePacks.add(pack);
            }
        }

        // 2. Iterate with a proper index counter
        int index = 0;
        for (DownloaderPackConfig pack : runtimePacks) {
            // Kotlin's forEachIndexed logic:
            action.invoke(index, pack);
            index++;
        }
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