package com.rockstargames.oswrapper;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;
import java.io.*;

public class AssetUtils {
    private static final String TAG = "AssetUtils";

    public static void extractAssets(Context context) {
        // لیستی ئەو فۆڵدەرانەی کە دەمانەوێت لە assetsـەوە کۆپییان بکەین
        String[] foldersToCopy = {"anim", "audio", "data", "models", "texdb","text", "textures", "rockstar"};
        File targetDir = context.getExternalFilesDir(null);

        for (String folder : foldersToCopy) {
            copyAssetFolder(context.getAssets(), folder, targetDir.getAbsolutePath() + "/" + folder);
        }
    }

    private static void copyAssetFolder(AssetManager assetManager, String assetPath, String outputPath) {
        try {
            String[] files = assetManager.list(assetPath);
            File outDir = new File(outputPath);

            if (files.length == 0) {
                // ئەگەر فایل بوو، کۆپی بکە
                copyFile(assetManager, assetPath, outputPath);
            } else {
                // ئەگەر فۆڵدەر بوو، دروستی بکە و بڕۆ ناوەووەی
                if (!outDir.exists()) outDir.mkdirs();
                for (String file : files) {
                    copyAssetFolder(assetManager, assetPath + "/" + file, outputPath + "/" + file);
                }
            }
        } catch (IOException e) {
            Log.e(TAG, "Error copying folder: " + assetPath, e);
        }
    }

    private static void copyFile(AssetManager assetManager, String assetPath, String outputPath) {
        try (InputStream in = assetManager.open(assetPath);
             OutputStream out = new FileOutputStream(outputPath)) {
            byte[] buffer = new byte[1024 * 8];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
        } catch (IOException e) {
            Log.e(TAG, "Error copying file: " + assetPath, e);
        }
    }
}