package com.nvidia.devtech;

import android.content.Context;
import android.content.res.AssetManager;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class NvAPKFileHelper {
    private static NvAPKFileHelper instance = new NvAPKFileHelper();
    private static final boolean logAssetFiles = false;
    String[] apkFiles;
    private Context context = null;
    int apkCount = 0;
    int myApkCount = 0;
    boolean hasAPKFiles = false;

    private int findInAPKFiles(String str) {
        if (this.myApkCount == 0) {
            return -1;
        }
        String str2 = str + ".mp3";
        int i = 0;
        while (true) {
            String[] strArr = this.apkFiles;
            if (i >= strArr.length) {
                return -1;
            }
            if (str.compareToIgnoreCase(strArr[i]) == 0 || str2.compareToIgnoreCase(this.apkFiles[i]) == 0) {
                break;
            }
            i++;
        }
        str.compareTo(this.apkFiles[i]);
        return i;
    }

    public static NvAPKFileHelper getInstance() {
        return instance;
    }

    void AddAssetFile(String str) {
        String[] strArr = this.apkFiles;
        int i = this.myApkCount;
        this.myApkCount = i + 1;
        strArr[i] = str;
    }

    void GetAssetList() {
        try {
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(this.context.getAssets().open("assetfile.txt")));
            int i = Integer.parseInt(bufferedReader.readLine());
            this.myApkCount = 0;
            if (i <= 0) {
                return;
            }
            this.apkFiles = new String[i];
            while (true) {
                String line = bufferedReader.readLine();
                if (line == null) {
                    return;
                }
                String[] strArr = this.apkFiles;
                int i2 = this.myApkCount;
                this.myApkCount = i2 + 1;
                strArr[i2] = line;
            }
        } catch (Exception unused) {
            AssetManager assets = this.context.getAssets();
            getDirectoryListing(assets, "", 0);
            getDirectoryListing(assets, "", this.apkCount);
        }
    }

    public void closeFileAndroid(NvAPKFile nvAPKFile) {
        try {
            nvAPKFile.is.close();
        } catch (IOException unused) {
        }
        nvAPKFile.data = new byte[0];
        nvAPKFile.is = null;
    }

    int getDirectoryListing(AssetManager assetManager, String str, int i) {
        try {
            if (this.apkFiles == null && i > 0) {
                this.apkFiles = new String[i];
            }
            String[] list = assetManager.list(str);
            if (list.length == 0) {
                if (i > 0) {
                    AddAssetFile(str);
                } else {
                    this.apkCount++;
                }
            }
            for (int i2 = 0; i2 < list.length; i2++) {
                if (list[i2].indexOf(46) == -1) {
                    getDirectoryListing(assetManager, str.length() > 0 ? str + "/" + list[i2] : list[i2], i);
                } else if (i > 0) {
                    AddAssetFile(str.length() > 0 ? str + "/" + list[i2] : list[i2]);
                } else {
                    this.apkCount++;
                }
            }
        } catch (Exception e) {
            System.out.println("ERROR: getDirectoryListing " + e.getMessage());
        }
        return 0;
    }

    public NvAPKFile openFileAndroid(String str) {
        if (!this.hasAPKFiles) {
            this.apkCount = 0;
            this.apkFiles = null;
            GetAssetList();
            this.hasAPKFiles = true;
        }
        int iFindInAPKFiles = findInAPKFiles(str);
        if (iFindInAPKFiles == -1) {
            return null;
        }
        NvAPKFile nvAPKFile = new NvAPKFile();
        nvAPKFile.is = null;
        nvAPKFile.length = 0;
        nvAPKFile.position = 0;
        nvAPKFile.bufferSize = 0;
        try {
            nvAPKFile.is = this.context.getAssets().open(this.apkFiles[iFindInAPKFiles]);
            nvAPKFile.length = nvAPKFile.is.available();
            nvAPKFile.is.mark(268435456);
            nvAPKFile.bufferSize = 1024;
            nvAPKFile.data = new byte[nvAPKFile.bufferSize];
            return nvAPKFile;
        } catch (Exception unused) {
            return null;
        }
    }

    public void readFileAndroid(NvAPKFile nvAPKFile, int i) {
        if (i > nvAPKFile.bufferSize) {
            nvAPKFile.data = new byte[i];
            nvAPKFile.bufferSize = i;
        }
        try {
            nvAPKFile.is.read(nvAPKFile.data, 0, i);
            nvAPKFile.position += i;
        } catch (IOException unused) {
        }
    }

    public long seekFileAndroid(NvAPKFile nvAPKFile, int i) {
        long j = 0;
        try {
            nvAPKFile.is.reset();
            long jSkip = 0;
            for (int i2 = 128; i > 0 && i2 > 0; i2--) {
                try {
                    jSkip = nvAPKFile.is.skip(i);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                j += jSkip;
                i = (int) (((long) i) - jSkip);
            }
        } catch (IOException unused) {
        }
        nvAPKFile.position = (int) j;
        return j;
    }

    public void setContext(Context context) {
        this.context = context;
    }
}
