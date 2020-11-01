package com.crazyflystudio.floodfillnativelib;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public class ColorViewDrawable extends Drawable {

    int drawColor = Color.RED;
    private Rect rect = new Rect();
    Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

    @Override
    public void draw(@NonNull Canvas canvas) {
        copyBounds(rect);

        paint.setAntiAlias(true);
        paint.setStyle(Paint.Style.FILL);
        paint.setDither(true);
        paint.setColor(Color.BLACK);
        canvas.drawCircle((rect.right - rect.left)/2f,(rect.bottom - rect.top)/2f,(rect.right - rect.left)/2f,paint);
        paint.setColor(drawColor);
        canvas.drawCircle((rect.right - rect.left)/2f,(rect.bottom - rect.top)/2f,(rect.right - rect.left)/2f * 0.90f,paint);
    }

    @Override
    public void setAlpha(int alpha) {

    }

    @Override
    public void setColorFilter(@Nullable ColorFilter colorFilter) {

    }

    @Override
    public int getOpacity() {
        return PixelFormat.TRANSLUCENT;
    }

    public void setDrawColor(int drawColor) {
        this.drawColor = drawColor;
    }
}
