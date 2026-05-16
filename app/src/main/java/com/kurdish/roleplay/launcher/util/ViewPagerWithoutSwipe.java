package com.kurdish.roleplay.launcher.util;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;

import androidx.viewpager.widget.ViewPager;

public final class ViewPagerWithoutSwipe extends ViewPager {
    @Override
    public boolean onInterceptTouchEvent(MotionEvent motionEvent) {
        return false;
    }

    public ViewPagerWithoutSwipe(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);
    }
}
