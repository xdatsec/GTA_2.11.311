package com.rockstargames.oswrapper.ui.util;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class ConfigValidator {


    static File documentsDir = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS);
    static File gameDir = new File(documentsDir, "SampMobile");

    public static void validateConfigFiles(Context context) {

        File file = new File(gameDir.getAbsolutePath(), "SAMP/settings.ini");
        if (!file.exists() || file.length() == 0) {
            file.getParentFile().mkdirs();
            boolean copied = copyAsset(context.getAssets(), "settings.ini", file.toString());
            if (!copied) {
                try {
                    FileOutputStream fos = new FileOutputStream(file);
                    StringBuilder sb = new StringBuilder();
                    sb.append("[client]\n");
                    sb.append("name=AndroidUser\n");
                    sb.append("password=\n");
                    sb.append("version=0.3.7\n\n");
                    sb.append("[gui]\n");
                    sb.append("androidkeyboard=false\n");
                    sb.append("VoiceChatEnable=true\n");
                    sb.append("speedo=false\n");
                    sb.append("fps=0\n");
                    sb.append("ChatPosX=110.0\n");
                    sb.append("ChatPosY=20.0\n");
                    sb.append("ChatSizeX=500.0\n");
                    sb.append("ChatSizeY=120.0\n");
                    sb.append("ChatMaxMessages=8\n");
                    sb.append("FPSLimit=120\n");
                    fos.write(sb.toString().getBytes());
                    fos.flush();
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        /*File file2 = new File(externalFilesDir, "gta_sa.set");
        if (!file2.exists()) {
            file2.getParentFile().mkdirs();
            copyAsset(context.getAssets(), "gta_sa.set", file2.toString());
        }*/
    }

    static boolean copyAsset(AssetManager assetManager, String str, String str2) {
        try {
            InputStream open = assetManager.open(str);
            new File(str2).createNewFile();
            FileOutputStream fileOutputStream = new FileOutputStream(str2);
            copyFile(open, fileOutputStream);
            open.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    static void copyFile(InputStream inputStream, OutputStream outputStream) throws IOException {
        byte[] bArr = new byte[1024];
        while (true) {
            int read = inputStream.read(bArr);
            if (read != -1) {
                outputStream.write(bArr, 0, read);
            } else {
                return;
            }
        }
    }
}
