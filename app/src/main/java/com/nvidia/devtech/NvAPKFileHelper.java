package com.nvidia.devtech;

import android.content.Context;
import android.content.res.AssetManager;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;

public class NvAPKFileHelper {
    private static final NvAPKFileHelper instance = new NvAPKFileHelper();

    private Context context = null;
    int myApkCount = 0;
    boolean hasAPKFiles = false;

    // O(1) lookup: lowercase asset path -> original asset path
    private HashMap<String, String> apkFileMap = new HashMap<>();

    private NvAPKFileHelper() {}

    public static NvAPKFileHelper getInstance() {
        return instance;
    }

    // Returns the real asset path for the given name, or null if not found.
    // Tries exact match first, then appends ".mp3" (original behaviour).
    private String findInAPKFiles(String str) {
        String lower = str.toLowerCase();
        String path = apkFileMap.get(lower);
        if (path != null) return path;
        return apkFileMap.get(lower + ".mp3");
    }

    void AddAssetFile(String str) {
        apkFileMap.put(str.toLowerCase(), str);
        myApkCount++;
    }

    void GetAssetList() {
        // Prefer the pre-built index file — avoids the expensive recursive scan.
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(
                    this.context.getAssets().open("assetfile.txt")));
            int count = Integer.parseInt(br.readLine().trim());
            if (count > 0) {
                apkFileMap = new HashMap<>(count * 2);
                String line;
                while ((line = br.readLine()) != null) {
                    line = line.trim();
                    if (!line.isEmpty()) {
                        apkFileMap.put(line.toLowerCase(), line);
                        myApkCount++;
                    }
                }
            }
            br.close();
            return;
        } catch (Exception ignored) {
            // Fall through to directory scan
        }

        // Single-pass recursive scan (was two-pass in the original).
        apkFileMap = new HashMap<>();
        getDirectoryListing(this.context.getAssets(), "");
    }

    public void closeFileAndroid(NvAPKFile nvAPKFile) {
        try {
            nvAPKFile.is.close();
        } catch (IOException ignored) {
        }
        nvAPKFile.data = new byte[0];
        nvAPKFile.is = null;
    }

    // Single-pass: builds the map directly without a counting pre-pass.
    void getDirectoryListing(AssetManager assetManager, String path) {
        try {
            String[] list = assetManager.list(path);
            if (list == null) return;

            if (list.length == 0) {
                // It's a file, not a directory
                if (!path.isEmpty()) {
                    AddAssetFile(path);
                }
                return;
            }

            for (String entry : list) {
                String child = path.isEmpty() ? entry : path + "/" + entry;
                if (entry.indexOf('.') == -1) {
                    // No extension → treat as directory
                    getDirectoryListing(assetManager, child);
                } else {
                    AddAssetFile(child);
                }
            }
        } catch (Exception e) {
            System.out.println("ERROR: getDirectoryListing " + e.getMessage());
        }
    }

    public NvAPKFile openFileAndroid(String str) {
        if (!this.hasAPKFiles) {
            GetAssetList();
            this.hasAPKFiles = true;
        }

        String filePath = findInAPKFiles(str);
        if (filePath == null) {
            return null;
        }

        NvAPKFile nvAPKFile = new NvAPKFile();
        nvAPKFile.is = null;
        nvAPKFile.length = 0;
        nvAPKFile.position = 0;
        nvAPKFile.bufferSize = 0;
        try {
            nvAPKFile.is = this.context.getAssets().open(filePath);
            nvAPKFile.length = nvAPKFile.is.available();
            nvAPKFile.is.mark(268435456);
            nvAPKFile.bufferSize = 4096;
            nvAPKFile.data = new byte[nvAPKFile.bufferSize];
            return nvAPKFile;
        } catch (Exception ignored) {
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
        } catch (IOException ignored) {
        }
    }

    // Fixed: was a 128-iteration retry loop that could leave the stream at the
    // wrong position; now loops until all bytes are skipped or EOF is reached.
    public long seekFileAndroid(NvAPKFile nvAPKFile, int targetPos) {
        try {
            nvAPKFile.is.reset();
            long remaining = targetPos;
            while (remaining > 0) {
                long skipped = nvAPKFile.is.skip(remaining);
                if (skipped <= 0) break;
                remaining -= skipped;
            }
            nvAPKFile.position = (int) (targetPos - remaining);
            return nvAPKFile.position;
        } catch (IOException ignored) {
            return nvAPKFile.position;
        }
    }

    public void setContext(Context context) {
        this.context = context;
    }
}
