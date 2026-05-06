package com.rockstargames.oswrapper.services;

import com.rockstargames.oswrapper.GameActivityBase;
import kotlin.jvm.internal.Intrinsics;

public class LanguageServices implements ILanguageServices {
    private static String TAG = "LanguageServices";
    private GameActivityBase activity;

    public LanguageServices(GameActivityBase activity) {
        Intrinsics.checkNotNullParameter(activity, "activity");
        this.activity = activity;
    }

    /* JADX WARN: Removed duplicated region for block: B:56:0x0092  */
    @Override // com.rockstargames.oswrapper.services.ILanguageServices
    /*
        Code decompiled incorrectly, please refer to instructions dump.
        To view partially-correct code enable 'Show inconsistent code' option in preferences
    */
    public int getDeviceLocale() {
        try {
            // ئەم کۆدە زمانی مۆبایلەکە وەردەگرێت و دەیگۆڕێت بۆ ئەو ژمارەیەی یارییەکە تێی دەگات
            String language = java.util.Locale.getDefault().getLanguage();

            if (language.equals("en")) return 0; // ئینگلیزی
            if (language.equals("fr")) return 1; // فەڕەنسی
            if (language.equals("de")) return 2; // ئەڵمانی
            if (language.equals("it")) return 3; // ئیتاڵی
            if (language.equals("es")) return 4; // ئیسپانی
            if (language.equals("ru")) return 5; // ڕووسی
            if (language.equals("ja")) return 6; // ژاپۆنی

            return 0; // ئەگەر زمانەکەت نەدۆزیەوە، با وەک بنەڕەت ئینگلیزی بێت
        } catch (Exception e) {
            return 0;
        }
    }
}
