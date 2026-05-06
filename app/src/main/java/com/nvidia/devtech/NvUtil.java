package com.nvidia.devtech;

import android.app.Activity;
import java.util.HashMap;

/* JADX INFO: loaded from: classes.dex */
public class NvUtil {
	private static NvUtil instance = new NvUtil();
	private Activity activity = null;
	private HashMap<String, String> appLocalValues = new HashMap<>();

	private NvUtil() {
	}

	public static NvUtil getInstance() {
		return instance;
	}

	public String getAppLocalValue(String str) {
		return this.appLocalValues.get(str);
	}

	public String getParameter(String str) {
		return this.activity.getIntent().getStringExtra(str);
	}

	public boolean hasAppLocalValue(String str) {
		return this.appLocalValues.containsKey(str);
	}

	public void setActivity(Activity activity) {
		this.activity = activity;
	}

	public void setAppLocalValue(String str, String str2) {
		this.appLocalValues.put(str, str2);
	}
}
