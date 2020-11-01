package com.crazyflystudio.floodfillnativelib;

import android.graphics.Bitmap;

public class NativeLib {

    static {
        System.loadLibrary("native-lib");
    }

    /**
     * Flood fill the area with specified target color over source color
     * source color for replacement will be taken from pixel by provided coordinates
     *
     * @param bitmap  bitmaps to color
     * @param x  x pixel coordinate
     * @param y  y pixel coordinate
     * @param targetColor target color
     * @param tolerance source color tolerance
     */
    public static native void floodFill1(Bitmap bitmap, int x, int y, int targetColor, int tolerance);

    /**
     * Flood fill the area with specified target color over source color
     *
     * @param bitmap  bitmaps to color
     * @param x  x pixel coordinate
     * @param y  y pixel coordinate
     * @param sourceColor source color to replace
     * @param targetColor target color
     * @param tolerance source color tolerance
     */
    public static native void floodFill2(Bitmap bitmap, int x, int y, int sourceColor,int targetColor, int tolerance);
}
