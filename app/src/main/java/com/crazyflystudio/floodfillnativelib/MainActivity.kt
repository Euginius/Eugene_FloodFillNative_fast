package com.crazyflystudio.floodfillnativelib

import android.annotation.SuppressLint
import android.content.Context
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Point
import android.graphics.drawable.BitmapDrawable
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.MotionEvent
import android.view.View
import android.view.WindowManager
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import com.crazyflystudio.floodfillnativelib.databinding.ActivityMainBinding
import com.github.dhaval2404.colorpicker.ColorPickerDialog
import com.github.dhaval2404.colorpicker.model.ColorShape

class MainActivity : AppCompatActivity() {

    private lateinit var dataBinding: ActivityMainBinding
    private lateinit var colorDrawable: ColorViewDrawable
    private var selectedColor: Int = Color.RED
    private lateinit var bpm: Bitmap


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val view = LayoutInflater.from(this).inflate(R.layout.activity_main, null)
        setContentView(view)
        dataBinding = ActivityMainBinding.bind(view)

        // Initiate color picker butt
        initColorPickerBtn()
        // Initiate draw surface
        initColoringSurface()
    }

    /** Initiate color picker button*/
    fun initColorPickerBtn() {
        colorDrawable = ColorViewDrawable()
        colorDrawable.setDrawColor(Color.RED)
        dataBinding.colorPicker.setImageDrawable(colorDrawable)

        // Select color for fload and fil
        dataBinding.colorPicker.setOnClickListener {
            openColorPicker()
        }
    }

    /** Initiate color picker button*/
    @SuppressLint("ClickableViewAccessibility")
    fun initColoringSurface() {

        val dispSize = getDisplaySize(this)
        bpm = Bitmap.createBitmap(dispSize.x, dispSize.y, Bitmap.Config.ARGB_8888)
        ViewCompat.setBackground(dataBinding.imageView, BitmapDrawable(resources, bpm))

        // Color the area wit selected color
        dataBinding.imageView.setOnTouchListener { v, event ->
            when (event.action and MotionEvent.ACTION_MASK) {
                MotionEvent.ACTION_DOWN -> {
                    val time = System.currentTimeMillis()
                    NativeLib.floodFill1(bpm, event.x.toInt(), event.y.toInt(), selectedColor, 0)
                    Log.d("TTTTTTT"," Time = " + (System.currentTimeMillis() - time))
                    dataBinding.imageView.invalidate()
                }

                else -> {

                }
            }

            true
        }
    }


    /** Open color picker dialog */
    fun openColorPicker() {
        ColorPickerDialog
            .Builder(this)                    // Pass Activity Instance
            .setColorShape(ColorShape.SQAURE)   // Default ColorShape.CIRCLE
            .setDefaultColor(Color.RED)            // Pass Default Color
            .setColorListener { color, colorHex ->
                colorDrawable.drawColor = color
                dataBinding.colorPicker.invalidate()
                selectedColor = color
            }
            .show()
    }


    /**
     * Get display size
     */
    fun getDisplaySize(context: Context): Point {
        val point = Point()
        val wm = context.getSystemService(WINDOW_SERVICE) as WindowManager
        val display = wm.defaultDisplay
        val size = Point()
        display.getSize(size)
        point.x = size.x
        point.y = size.y
        return point
    }
}