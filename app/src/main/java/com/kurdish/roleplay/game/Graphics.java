package com.kurdish.roleplay.game;

public class Graphics {

    public static native void nativeSetBloomEnabled(boolean enabled);
    public static native void nativeSetBloomIntensity(float intensity);

    public static void setBloomEnabled(boolean enabled) {
        nativeSetBloomEnabled(enabled);
    }

    public static void setBloomIntensity(float intensity) {
        nativeSetBloomIntensity(intensity);
    }

    public enum Preset {
        LOW(0),
        MEDIUM(1),
        HIGH(2),
        ULTRA(3);

        private final int value;
        Preset(int value) {
            this.value = value;
        }
        public int getValue() {
            return value;
        }

        public static Preset fromInt(int v) {
            switch (v) {
                case 0: return LOW;
                case 1: return MEDIUM;
                case 2: return HIGH;
                case 3: return ULTRA;
                default: return MEDIUM;
            }
        }
    }

    // =============================
    // HDR SETTINGS
    // =============================

    public static native void nativeSetHDREnabled(boolean enabled);

    public static void setHDREnabled(boolean enabled) {
        nativeSetHDREnabled(enabled);
    }

    // =============================
    // COLOR CORRECTION
    // =============================

    public static native void nativeSetColorCorrection(float saturation, float contrast, float brightness);

    public static void setColorCorrection(float saturation, float contrast, float brightness) {
        nativeSetColorCorrection(saturation, contrast, brightness);
    }

    // =============================
    // PRESETS
    // =============================

    public static native void nativeSetPreset(int preset);
    public static native int nativeGetPreset();

    public static void setPreset(Preset preset) {
        nativeSetPreset(preset.getValue());
    }

    public static Preset getPreset() {
        return Preset.fromInt(nativeGetPreset());
    }


    // =============================
    // LIGHT SYSTEM
    // =============================

    public static native int nativeAddLight(float x, float y, float z,
                                            int r, int g, int b,
                                            float intensity, float radius);

    public static native void nativeRemoveLight(int id);

    public static int addLight(float x, float y, float z,
                               int r, int g, int b,
                               float intensity, float radius) {

        return nativeAddLight(x, y, z, r, g, b, intensity, radius);
    }

    public static void removeLight(int id) {
        nativeRemoveLight(id);
    }
}
