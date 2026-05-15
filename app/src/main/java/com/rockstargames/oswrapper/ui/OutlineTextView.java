package com.rockstargames.oswrapper.ui;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffColorFilter;
import android.util.AttributeSet;
import androidx.appcompat.widget.AppCompatTextView;

public class OutlineTextView extends AppCompatTextView {

    // This forces the "8-direction" passes to be solid Black
    private final PorterDuffColorFilter strokeColorFilter =
            new PorterDuffColorFilter(Color.BLACK, PorterDuff.Mode.SRC_IN);

    public OutlineTextView(Context context) {
        super(context);
    }

    public OutlineTextView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        int originalColor = getCurrentTextColor();

        // 1. SET THE STROKE COLOR FILTER
        // This ensures every offset pass is BLACK, even if the text has {hex} colors
        getPaint().setColorFilter(strokeColorFilter);
        getPaint().setColorFilter(strokeColorFilter);
        getPaint().setColorFilter(strokeColorFilter);
        // 2. DRAW THE 8-DIRECTION OFFSET (The "Strong" Outline)
        int offset = 1; // Increase this to 2 or 3 for a THICKER, STRONGER border

        canvas.save(); canvas.translate(-offset, 0); super.onDraw(canvas); canvas.restore();
        canvas.save(); canvas.translate(offset, 0); super.onDraw(canvas); canvas.restore();
        canvas.save(); canvas.translate(0, -offset); super.onDraw(canvas); canvas.restore();
        canvas.save(); canvas.translate(0, offset); super.onDraw(canvas); canvas.restore();

        // Diagonal offsets make the corners "full"
        canvas.save(); canvas.translate(-offset, -offset); super.onDraw(canvas); canvas.restore();
        canvas.save(); canvas.translate(offset, -offset); super.onDraw(canvas); canvas.restore();
        canvas.save(); canvas.translate(-offset, offset); super.onDraw(canvas); canvas.restore();
        canvas.save(); canvas.translate(offset, offset); super.onDraw(canvas); canvas.restore();

        // 3. DRAW ORIGINAL TEXT (The Fill)
        // Remove the filter so the span colors ({B9C9BF}, etc.) work again
        getPaint().setColorFilter(strokeColorFilter);
        setTextColor(originalColor);
        super.onDraw(canvas);
    }
}