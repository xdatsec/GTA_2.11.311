package com.rockstargames.oswrapper;

public final class DownloaderPackConfig {

    private final String id;
    private final int labelId;
    private final DownloaderPackType type;

    public DownloaderPackConfig(String id, int labelId, DownloaderPackType type) {
        if (id   == null) throw new NullPointerException("id is null");
        if (type == null) throw new NullPointerException("type is null");
        this.id      = id;
        this.labelId = labelId;
        this.type    = type;
    }

    public String getId()            { return id;      }
    public int    getLabelId()       { return labelId; }
    public DownloaderPackType getType() { return type; }

    @Override
    public boolean equals(Object other) {
        if (this == other) return true;
        if (!(other instanceof DownloaderPackConfig)) return false;
        DownloaderPackConfig o = (DownloaderPackConfig) other;
        return id.equals(o.id) && labelId == o.labelId && type == o.type;
    }

    @Override
    public int hashCode() {
        int result = id.hashCode();
        result = 31 * result + Integer.hashCode(labelId);
        result = 31 * result + type.hashCode();
        return result;
    }

    @Override
    public String toString() {
        return "DownloaderPackConfig(id=" + id
                + ", labelId=" + labelId
                + ", type=" + type + ')';
    }
}