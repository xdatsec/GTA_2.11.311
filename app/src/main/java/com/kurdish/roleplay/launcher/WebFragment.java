package com.kurdish.roleplay.launcher;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import androidx.fragment.app.Fragment;

import com.kurdish.roleplay.R;

public class WebFragment extends Fragment {

    private WebView webView;

    // 🔥 ئەمە جێگای لینکەکەتە
    private String webUrl = "http://46.247.108.8/krp/rules.html";
    // یان ئەو index.php کە HTMLەکەت تێدایە

    public WebFragment() {}

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_web, container, false);

        webView = view.findViewById(R.id.webView);
        setupWebView();

        return view;
    }

    private void setupWebView() {

        WebSettings settings = webView.getSettings();

        // HTML Tailwind + Video Background پێویستی بە ئەمانە هەیە
        settings.setJavaScriptEnabled(true);
        settings.setDomStorageEnabled(true);
        settings.setMediaPlaybackRequiresUserGesture(false);

        // 🔥 گونجاو بوونی وێبسایتت (Full Width + Responsive)
        settings.setLoadWithOverviewMode(true);
        settings.setUseWideViewPort(true);
        webView.setFitsSystemWindows(false);

        // 🔥 Scale = 80% دەبێت ئەوەی داوات کرد
        webView.setInitialScale(100);

        // 🔥 زووم بسەرەوە بۆ Launcher
        settings.setSupportZoom(false);
        settings.setBuiltInZoomControls(false);
        settings.setDisplayZoomControls(false);

        // سرعت لۆد کردنی Tailwind / Video
        settings.setCacheMode(WebSettings.LOAD_NO_CACHE);

        // بەرەوپێشکردنی Compatibility (بەتایبەتی video autoplay)
        webView.setWebChromeClient(new WebChromeClient());
        webView.setWebViewClient(new WebViewClient());

        // بۆ ئەوە هیچ کێشەی cache نەباتەوە
        String finalUrl = webUrl + "?v=" + System.currentTimeMillis();

        webView.loadUrl(finalUrl);
    }

    @Override
    public void onDestroyView() {
        if (webView != null) webView.destroy();
        super.onDestroyView();
    }
}
