package com.rockstargames.oswrapper;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.kurdish.roleplay.R;

import java.util.ArrayList;
import java.util.List;

import kotlin.Metadata;
import kotlin.jvm.internal.Intrinsics;

/* JADX INFO: compiled from: DownloaderActivityBase.kt */
/* JADX INFO: loaded from: classes.dex */
final class DownloaderItem {
    private static final String TAG = "DownloaderItem";
    private final ProgressBar barProgress;
    private final Context context;
    private boolean done;
    private final TextView txtName;
    private final TextView txtPercent;
    private final TextView txtStatus;

    public DownloaderItem(Context context, View view, String packName) {
        Intrinsics.checkNotNullParameter(context, "context");
        Intrinsics.checkNotNullParameter(view, "view");
        Intrinsics.checkNotNullParameter(packName, "packName");
        this.context = context;
        View viewFindViewById = view.findViewById(R.id.barProgress);
        Intrinsics.checkNotNullExpressionValue(viewFindViewById, "findViewById(...)");
        this.barProgress = (ProgressBar) viewFindViewById;
        View viewFindViewById2 = view.findViewById(R.id.txtPackName);
        Intrinsics.checkNotNullExpressionValue(viewFindViewById2, "findViewById(...)");
        TextView textView = (TextView) viewFindViewById2;
        this.txtName = textView;
        View viewFindViewById3 = view.findViewById(R.id.txtProgressStatus);
        Intrinsics.checkNotNullExpressionValue(viewFindViewById3, "findViewById(...)");
        this.txtStatus = (TextView) viewFindViewById3;
        View viewFindViewById4 = view.findViewById(R.id.txtProgressPercent);
        Intrinsics.checkNotNullExpressionValue(viewFindViewById4, "findViewById(...)");
        this.txtPercent = (TextView) viewFindViewById4;
        textView.setText(packName);
        update(0, -1, 0L, 0L);
    }

    public final Context getContext() {
        return this.context;
    }

    public final boolean getDone() {
        return this.done;
    }

    public final void setDone(boolean z) {
        this.done = z;
    }

    public final void update(int status, int progress, long bytesDownloaded, long bytesTotal) {
        String string;
        if (this.done) {
            return;
        }
        Log.d(TAG, "update: " + status + " (" + progress + "%) " + bytesDownloaded + " / " + bytesTotal);
        if (progress < 0 || status != 2) {
            this.barProgress.setIndeterminate(true);
            this.barProgress.setProgress(0);
            this.txtPercent.setText("");
        } else {
            this.barProgress.setIndeterminate(false);
            this.barProgress.setProgress(progress);
            this.txtPercent.setText(this.context.getString(R.string.downloaderStatusPercentage, Integer.valueOf(progress)));
        }
        TextView textView = this.txtStatus;
        switch (status) {
            case 0:
                string = this.context.getString(R.string.downloaderStateUnknown);
                break;
            case 1:
                string = this.context.getString(R.string.downloaderStateIdle);
                break;
            case 2:
                long j = 1024;
                string = this.context.getString(R.string.downloaderStatusSize, Long.valueOf((bytesDownloaded / j) / j), Long.valueOf((bytesTotal / j) / j));
                break;
            case 3:
                string = this.context.getString(R.string.downloaderStateTransferring);
                break;
            case 4:
                string = this.context.getString(R.string.downloaderStateCompleted);
                break;
            case 5:
                string = this.context.getString(R.string.downloaderStateFailed);
                break;
            case 6:
                string = this.context.getString(R.string.downloaderStateCancelled);
                break;
            case 7:
                string = this.context.getString(R.string.downloaderStateAwaitingWifi);
                break;
            case 8:
                string = this.context.getString(R.string.downloaderStateIdle);
                break;
            default:
                string = this.context.getString(R.string.downloaderStateUnknown);
                break;
        }
        textView.setText(string);
        if (status == 4) {
            this.barProgress.setVisibility(View.INVISIBLE);
            this.done = true;
        }
    }
}
