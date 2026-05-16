package com.rockstargames.oswrapper;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.content.res.AppCompatResources;

import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.android.play.core.assetpacks.AssetPackLocation;
import com.google.android.play.core.assetpacks.AssetPackManager;
import com.google.android.play.core.assetpacks.AssetPackManagerFactory;
import com.google.android.play.core.assetpacks.AssetPackState;
import com.google.android.play.core.assetpacks.AssetPackStateUpdateListener;
import com.google.android.play.core.assetpacks.model.AssetPackStatus;
import com.google.android.play.core.assetpacks.model.AssetPackStorageMethod;
import com.rockstargames.gtasa.MainActivity;
import com.kurdish.roleplay.R;
import com.rockstargames.oswrapper.DownloaderItem;
import com.rockstargames.oswrapper.DownloaderPackConfig;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import kotlin.Unit;
import kotlin.jvm.functions.Function1;
import kotlin.jvm.functions.Function2;
import kotlin.jvm.internal.DefaultConstructorMarker;
import kotlin.jvm.internal.Intrinsics;
import kotlin.text.Typography;

/* JADX INFO: compiled from: DownloaderActivityBase.kt */
/* JADX INFO: loaded from: classes.dex */


public abstract class DownloaderActivityBase extends AppCompatActivity implements AssetPackStateUpdateListener {
    private static final String LOG_TAG = "DownloaderActivityBase";
    private AssetPackManager assetPackManager;
    public Handler handler;
    private ViewGroup layoutDownloader;
    private ViewGroup layoutWifi;
    public static final Companion INSTANCE = new Companion(null);

    static final List<Integer> PACK_VIEWS = Arrays.asList(
            R.id.packItem0,
            R.id.packItem1,
            R.id.packItem2,
            R.id.packItem3,
            R.id.packItem4,
            R.id.packItem5,
            R.id.packItem6,
            R.id.packItem7
    );


    private final AtomicBoolean complete = new AtomicBoolean();
    private final AtomicInteger attempt = new AtomicInteger();
    final Map<String, DownloaderItem> packs = new LinkedHashMap();
    private final Runnable tickTask = new Runnable() {
        @Override
        public void run() {
            // Call the outer class method
            if (com.rockstargames.oswrapper.DownloaderActivityBase.this.onTick()) {
                // Access the handler from the outer class
                Handler handler = com.rockstargames.oswrapper.DownloaderActivityBase.this.handler;

                if (handler != null) {
                    // Schedule the next tick
                    handler.postDelayed(this, 1000L);
                } else {
                    throw new IllegalStateException("Handler not initialized");
                }
            }
        }
    };
    public static final class Companion {
        private Companion() {
        }

        public /* synthetic */ Companion(DefaultConstructorMarker defaultConstructorMarker) {
            this();
        }

        public final List<Integer> getPACK_VIEWS$oswrapper_release() {
            return com.rockstargames.oswrapper.DownloaderActivityBase.PACK_VIEWS;
        }
    }


    private final String describeStatus(int status) {
        switch (status) {
            case 0:
                return "UNKNOWN";
            case 1:
                return "PENDING";
            case 2:
                return "DOWNLOADING";
            case 3:
                return "TRANSFERRING";
            case 4:
                return "COMPLETED";
            case 5:
                return "FAILED";
            case 6:
                return "CANCELED";
            case 7:
                return "WAITING_FOR_WIFI";
            case 8:
                return "NOT_INSTALLED";
            default:
                return "<unknown " + status + Typography.greater;
        }
    }

    private final void downloadAssetPacks(final List<String> packs, final int attempt) {

    }

    static /* synthetic */ void downloadAssetPacks$default(com.rockstargames.oswrapper.DownloaderActivityBase downloaderActivityBase, List list, int i, int i2, Object obj) {
        if (obj != null) {
            throw new UnsupportedOperationException("Super calls with default arguments not supported in this target, function: downloadAssetPacks");
        }
        if ((i2 & 2) != 0) {
            i = 0;
        }
        downloaderActivityBase.downloadAssetPacks(list, i);
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static final void downloadAssetPacks$lambda$3(Function1 tmp0, Object obj) {
        Intrinsics.checkNotNullParameter(tmp0, "$tmp0");
        tmp0.invoke(obj);
    }

    /* JADX INFO: Access modifiers changed from: private */
    public final void downloadAssetPacks$lambda$5(final int i, final com.rockstargames.oswrapper.DownloaderActivityBase this$0, final List packs, Exception it) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        Intrinsics.checkNotNullParameter(packs, "$packs");
        Intrinsics.checkNotNullParameter(it, "it");
        Log.e(LOG_TAG, "[!!] downloadAssetPacks: failed (attempt " + i + ')', it);
        if (this$0.attempt.compareAndSet(i, i + 1)) {
            Handler handler = this$0.handler;
            if (handler == null) {
                Intrinsics.throwUninitializedPropertyAccessException("handler");
                handler = null;
            }
            handler.postDelayed(new Runnable() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase$$ExternalSyntheticLambda5
                @Override // java.lang.Runnable
                public final void run() {
                    com.rockstargames.oswrapper.DownloaderActivityBase.downloadAssetPacks$lambda$5$lambda$4(com.rockstargames.oswrapper.DownloaderActivityBase.this, packs, i);
                }
            }, 1000L);
        }
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static final void downloadAssetPacks$lambda$5$lambda$4(com.rockstargames.oswrapper.DownloaderActivityBase this$0, List packs, int i) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        Intrinsics.checkNotNullParameter(packs, "$packs");
        Log.i(LOG_TAG, "downloadAssetPack: retrying download");
        this$0.downloadAssetPacks(packs, i + 1);
    }

    /* JADX INFO: Access modifiers changed from: private */
    public final boolean hasAssetPack(String name) {
        AssetPackLocation packLocation;
        try {
            AssetPackManager assetPackManager = this.assetPackManager;
            if (assetPackManager == null) {
                Intrinsics.throwUninitializedPropertyAccessException("assetPackManager");
                assetPackManager = null;
            }
            packLocation = assetPackManager.getPackLocation(name);
        } catch (Exception e) {
            Log.e(LOG_TAG, "getPackLocation failed: " + name, e);
            packLocation = null;
        }
        return (packLocation != null ? packLocation.assetsPath() : null) != null;
    }

    private final void initializeddl() {
        final ArrayList arrayList = new ArrayList();

        for (Integer viewId : PACK_VIEWS) {
            findViewById(viewId).setVisibility(View.GONE);
        }
        getConfig().forEachRuntimePack(new Function2<Integer, DownloaderPackConfig, Unit>() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase.initializeDownload.2


            @Override // kotlin.jvm.functions.Function2
            public /* bridge */ /* synthetic */ Unit invoke(Integer num, DownloaderPackConfig downloaderPackConfig) {
                invoke(num.intValue(), downloaderPackConfig);
                return Unit.INSTANCE;
            }

            public final void invoke(int i, DownloaderPackConfig pack) {
                Intrinsics.checkNotNullParameter(pack, "pack");
                View viewFindViewById = com.rockstargames.oswrapper.DownloaderActivityBase.this.findViewById(INSTANCE.getPACK_VIEWS$oswrapper_release().get(i).intValue());
                com.rockstargames.oswrapper.DownloaderActivityBase downloaderActivityBase = com.rockstargames.oswrapper.DownloaderActivityBase.this;
                Intrinsics.checkNotNull(viewFindViewById);
                String string = com.rockstargames.oswrapper.DownloaderActivityBase.this.getString(pack.getLabelId());
                Intrinsics.checkNotNullExpressionValue(string, "getString(...)");
                DownloaderItem downloaderItem = new DownloaderItem(downloaderActivityBase, viewFindViewById, string);
                if (com.rockstargames.oswrapper.DownloaderActivityBase.this.hasAssetPack(pack.getId())) {
                    Log.i(com.rockstargames.oswrapper.DownloaderActivityBase.LOG_TAG, "[!!] Pack already downloaded: " + pack.getId());
                    downloaderItem.update(4, -1, 0L, 0L);
                } else {
                    Log.i(com.rockstargames.oswrapper.DownloaderActivityBase.LOG_TAG, "[!!] Requesting download: " + pack.getId());
                    arrayList.add(pack.getId());
                }
                viewFindViewById.setVisibility(View.VISIBLE);
                com.rockstargames.oswrapper.DownloaderActivityBase.this.packs.put(pack.getId(), downloaderItem);
            }
        });
        if (arrayList.size() != 0) {
            downloadAssetPacks$default(this, arrayList, 0, 2, null);
        } else {
            Log.i(LOG_TAG, "[!!] All packs present");
            onDownloadComplete();
        }
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static final void onCreate$lambda$0(com.rockstargames.oswrapper.DownloaderActivityBase this$0, View view) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        Log.i(LOG_TAG, "[!!] Intent(Settings.ACTION_WIFI_SETTINGS)");
        this$0.startActivity(new Intent("android.settings.WIFI_SETTINGS"));
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static final void onCreate$lambda$2(final com.rockstargames.oswrapper.DownloaderActivityBase this$0, com.rockstargames.oswrapper.DownloaderActivityBase activity, View view) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        Intrinsics.checkNotNullParameter(activity, "$activity");
        Log.i(LOG_TAG, "[!!] showCellularDataConfirmation");
        AssetPackManager assetPackManager = this$0.assetPackManager;
        if (assetPackManager == null) {
            Intrinsics.throwUninitializedPropertyAccessException("assetPackManager");
            assetPackManager = null;
        }
        Task<Integer> taskShowCellularDataConfirmation = assetPackManager.showCellularDataConfirmation(activity);
        final Function1<Integer, Unit> function1 = new Function1<Integer, Unit>() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase$onCreate$2$1
            @Override // kotlin.jvm.functions.Function1
            public /* bridge */ /* synthetic */ Unit invoke(Integer num) {
                invoke2(num);
                return Unit.INSTANCE;
            }

            /* JADX INFO: renamed from: invoke, reason: avoid collision after fix types in other method */
            public final void invoke2(Integer num) {
                Log.i("DownloaderActivityBase", "[!!] showCellularDataConfirmation: result: " + num);
                this$0.setShowWifiButtons(num == null || num.intValue() != -1);
            }
        };
        taskShowCellularDataConfirmation.addOnSuccessListener(new OnSuccessListener() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase$$ExternalSyntheticLambda3
            @Override // com.google.android.gms.tasks.OnSuccessListener
            public final void onSuccess(Object obj) {
                com.rockstargames.oswrapper.DownloaderActivityBase.onCreate$lambda$2$lambda$1(function1, obj);
            }
        });
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static final void onCreate$lambda$2$lambda$1(Function1 tmp0, Object obj) {
        Intrinsics.checkNotNullParameter(tmp0, "$tmp0");
        tmp0.invoke(obj);
    }

    private final void onDownloadComplete() {
        if (!this.complete.compareAndSet(false, true)) {
            Log.i(LOG_TAG, "onDownloadComplete: duplicated call");
            return;
        }
        Log.i(LOG_TAG, "onDownloadComplete: launching the game");
        final ArrayList arrayList = new ArrayList();
        final ArrayList arrayList2 = new ArrayList();
        AssetPackManager assetPackManager2 = this.assetPackManager;
        getConfig().forEachPack(new Function2<Integer, DownloaderPackConfig, Unit>() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase.onDownloadComplete.1


            @Override // kotlin.jvm.functions.Function2
            public /* bridge */ /* synthetic */ Unit invoke(Integer num, DownloaderPackConfig downloaderPackConfig) {
                invoke(num.intValue(), downloaderPackConfig);
                return Unit.INSTANCE;
            }

            public final void invoke(int i, DownloaderPackConfig pack) {
                Intrinsics.checkNotNullParameter(pack, "pack");
                arrayList.add(pack.getId());
                AssetPackManager assetPackManager = assetPackManager2;
                String strPath = null;
                if (assetPackManager == null) {
                    Intrinsics.throwUninitializedPropertyAccessException("assetPackManager");
                    assetPackManager = null;
                }
                AssetPackLocation packLocation = assetPackManager.getPackLocation(pack.getId());
                if (packLocation == null) {
                    throw new RuntimeException("onDownloadComplete: getPackLocation(" + pack.getId() + ") returned null");
                }
                Log.i(com.rockstargames.oswrapper.DownloaderActivityBase.LOG_TAG, "Pack " + pack.getId() + "\nstorageMethod = " + packLocation.packStorageMethod() + "\npath = " + packLocation.path() + "\nassetsPath = " + packLocation.assetsPath());
                List<String> list = arrayList2;
                if (packLocation.packStorageMethod() == AssetPackStorageMethod.STORAGE_FILES && (strPath = packLocation.path()) == null) {
                    throw new RuntimeException("onDownloadComplete: getPackLocation(" + pack.getId() + ").path returned null for STORAGE_FILES pack");
                }
                list.add(strPath);
            }
        });
        Intent intent = new Intent().setClass(this, getConfig().getActivity());
        Intrinsics.checkNotNullExpressionValue(intent, "setClass(...)");
        intent.putExtra("packNames", (String[]) arrayList.toArray(new String[0]));
        intent.putExtra("packPaths", (String[]) arrayList2.toArray(new String[0]));
        startActivity(intent);
        finish();
    }

    private final void onDownloadFailed() {
        Log.i(LOG_TAG, "onDownloadFailed: showing the error");
        this.complete.set(true);
        new AlertDialog.Builder(this).setTitle(R.string.downloaderError).setMessage(R.string.downloaderNoData).setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase$$ExternalSyntheticLambda4
            @Override // android.content.DialogInterface.OnClickListener
            public final void onClick(DialogInterface dialogInterface, int i) {
                com.rockstargames.oswrapper.DownloaderActivityBase.onDownloadFailed$lambda$9(com.rockstargames.oswrapper.DownloaderActivityBase.this, dialogInterface, i);
            }
        }).setIcon(android.R.drawable.ic_dialog_alert).show();
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static final void onDownloadFailed$lambda$9(com.rockstargames.oswrapper.DownloaderActivityBase this$0, DialogInterface dialogInterface, int i) {
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        dialogInterface.dismiss();
        this$0.finish();
    }

    /* JADX INFO: Access modifiers changed from: private */
    public static final void onStateUpdate$lambda$7(AssetPackState state, com.rockstargames.oswrapper.DownloaderActivityBase this$0) {
        Intrinsics.checkNotNullParameter(state, "$state");
        Intrinsics.checkNotNullParameter(this$0, "this$0");
        StringBuilder sb = new StringBuilder("[!!] onStateUpdate: ");
        String strName = state.name();
        Intrinsics.checkNotNullExpressionValue(strName, "name()");
        Log.i(LOG_TAG, sb.append(strName).append(" = ").append(this$0.describeStatus(state.status())).append(" (").append(state.transferProgressPercentage()).append(')').toString());
        if (state.status() == AssetPackStatus.WAITING_FOR_WIFI) {
            this$0.setShowWifiButtons(true);
        }
        Map<String, DownloaderItem> map = this$0.packs;
        String strName2 = state.name();
        Intrinsics.checkNotNullExpressionValue(strName2, "name()");
        DownloaderItem downloaderItem = map.get(strName2);
        Intrinsics.checkNotNull(downloaderItem);
        downloaderItem.update(state.status(), (int) ((state.bytesDownloaded() / state.totalBytesToDownload()) * 100), state.bytesDownloaded(), state.totalBytesToDownload());
    }

    /* JADX INFO: Access modifiers changed from: private */
    public final boolean onTick() {
        // If download is already complete, nothing to do
        if (this.complete.get()) {
            return false;
        }

        boolean allDone = true; // Assume all done until proven otherwise

        if (!this.packs.isEmpty()) {
            for (DownloaderItem item : this.packs.values()) {
                if (!item.getDone()) {
                    allDone = false; // Found a pack that's not done
                    break;
                }
            }
        }

        Log.i(LOG_TAG, "[!!] onTick: done = " + allDone);

        if (allDone) {
            onDownloadComplete();
        }

        return !allDone; // Continue ticking only if not all done
    }

    /* JADX INFO: Access modifiers changed from: private */
    public final void setShowWifiButtons(boolean show) {
        Log.i(LOG_TAG, "[!!] setShowWifiButtons: " + show);
        ViewGroup viewGroup = this.layoutWifi;
        if (viewGroup == null) {
            Intrinsics.throwUninitializedPropertyAccessException("layoutWifi");
            viewGroup = null;
        }
        viewGroup.setVisibility(show ? View.VISIBLE : View.GONE);
    }

    protected abstract DownloaderConfig getConfig();


    @Override // androidx.fragment.app.FragmentActivity, androidx.activity.ComponentActivity, androidx.core.app.ComponentActivity, android.app.Activity
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.downloader);
        AssetPackManager assetPackManagerFactory = AssetPackManagerFactory.getInstance(getApplicationContext());
        Intrinsics.checkNotNullExpressionValue(assetPackManagerFactory, "getInstance(...)");
        this.assetPackManager = assetPackManagerFactory;
        this.handler = new Handler(Looper.getMainLooper());
        View viewFindViewById = findViewById(R.id.layoutDownloader);
        Intrinsics.checkNotNullExpressionValue(viewFindViewById, "findViewById(...)");
        ViewGroup viewGroup = (ViewGroup) viewFindViewById;
        this.layoutDownloader = viewGroup;
        Handler handler = null;
        if (viewGroup == null) {
            Intrinsics.throwUninitializedPropertyAccessException("layoutDownloader");
            viewGroup = null;
        }
        viewGroup.setBackground(AppCompatResources.getDrawable(this, getConfig().getBackground()));
        Button button = (Button) findViewById(R.id.btnResumeDownload);
        Button button2 = (Button) findViewById(R.id.btnWifiSettings);
        View viewFindViewById2 = findViewById(R.id.layoutWifi);
        Intrinsics.checkNotNullExpressionValue(viewFindViewById2, "findViewById(...)");
        this.layoutWifi = (ViewGroup) viewFindViewById2;
        button2.setOnClickListener(new View.OnClickListener() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase$$ExternalSyntheticLambda0
            @Override // android.view.View.OnClickListener
            public final void onClick(View view) {
                com.rockstargames.oswrapper.DownloaderActivityBase.onCreate$lambda$0(com.rockstargames.oswrapper.DownloaderActivityBase.this, view);
            }
        });
        button.setOnClickListener(new View.OnClickListener() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase$$ExternalSyntheticLambda1
            @Override // android.view.View.OnClickListener
            public final void onClick(View view) {
                com.rockstargames.oswrapper.DownloaderActivityBase.onCreate$lambda$2(com.rockstargames.oswrapper.DownloaderActivityBase.this, com.rockstargames.oswrapper.DownloaderActivityBase.this, view);
            }
        });
        AssetPackManager assetPackManager = this.assetPackManager;
        if (assetPackManager == null) {
            Intrinsics.throwUninitializedPropertyAccessException("assetPackManager");
            assetPackManager = null;
        }
        assetPackManager.registerListener(this);
        setShowWifiButtons(false);
        initializeddl();
        Handler handler2 = this.handler;
        if (handler2 == null) {
            Intrinsics.throwUninitializedPropertyAccessException("handler");
        } else {
            handler = handler2;
        }

        handler.postDelayed(this.tickTask, 1000L);
    }

    @Override // androidx.appcompat.app.AppCompatActivity, androidx.fragment.app.FragmentActivity, android.app.Activity
    protected void onDestroy() {
        Log.i(LOG_TAG, "[!!] Cleaning up");
        Handler handler = this.handler;
        AssetPackManager assetPackManager = null;
        if (handler == null) {
            Intrinsics.throwUninitializedPropertyAccessException("handler");
            handler = null;
        }
        handler.removeCallbacks(this.tickTask);
        AssetPackManager assetPackManager2 = this.assetPackManager;
        if (assetPackManager2 == null) {
            Intrinsics.throwUninitializedPropertyAccessException("assetPackManager");
        } else {
            assetPackManager = assetPackManager2;
        }
        assetPackManager.unregisterListener(this);
        super.onDestroy();
    }

    @Override // com.google.android.play.core.listener.StateUpdatedListener
    public void onStateUpdate(final AssetPackState state) {
        Intrinsics.checkNotNullParameter(state, "state");
        if (this.complete.get()) {
            return;
        }
        Handler handler = this.handler;
        if (handler == null) {
            Intrinsics.throwUninitializedPropertyAccessException("handler");
            handler = null;
        }
        handler.post(new Runnable() { // from class: com.rockstargames.oswrapper.DownloaderActivityBase$$ExternalSyntheticLambda2
            @Override // java.lang.Runnable
            public final void run() {
                com.rockstargames.oswrapper.DownloaderActivityBase.onStateUpdate$lambda$7(state, com.rockstargames.oswrapper.DownloaderActivityBase.this);
            }
        });
    }

}