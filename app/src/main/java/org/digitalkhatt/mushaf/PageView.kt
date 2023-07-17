package org.digitalkhatt.mushaf

import android.content.Context
import android.graphics.Bitmap
import android.graphics.Canvas
import android.util.AttributeSet
import android.view.View
import java.lang.Math.ceil

class PageView : View {

    private lateinit var fSkiaBitmap: Bitmap;
    private var mPageIndex : Int = -1;
    private var w = 0
    private var h = 0

    fun getPageIndex(): Int {
        return mPageIndex
    }

    fun setPageIndex(pageIndex: Int) {
        mPageIndex = pageIndex
        invalidate()
        requestLayout()
    }

    constructor(context: Context, attrs: AttributeSet, defStyle: Int) : super(context, attrs, defStyle) {
        // ...
    }

    constructor(context: Context, attrs: AttributeSet) : this(context, attrs, 0) {}


    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        fSkiaBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888)
        this.w = w
        this.h = h
        //drawPage(fSkiaBitmap, mPageIndex, w - 15, 100, 115, 60)
    }

    external fun drawPage(
        image: Bitmap?,pageIndex: Int, xStart: Int,yStart : Int, interLine: Int, charHeight: Int, justify : Boolean
    )

    override fun onDraw(canvas: Canvas) {
        drawPage(fSkiaBitmap, mPageIndex, w - 15, 100, 120, 60,MushafApp.instance.justify)
        canvas.drawBitmap(fSkiaBitmap, 0f, 0f, null)
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {


        val widthMode = MeasureSpec.getMode(widthMeasureSpec)
        val widthSize = MeasureSpec.getSize(widthMeasureSpec)
        val heightMode = MeasureSpec.getMode(heightMeasureSpec)
        val heightSize = MeasureSpec.getMode(heightMeasureSpec)


        val width: Int
        val height: Int

        //Measure Width
        width = if (widthMode == MeasureSpec.EXACTLY) {
            //Must be this size
            widthSize
        } else if (widthMode == MeasureSpec.AT_MOST) {
            //Can't be bigger than...
            widthSize
        } else {
            //Be whatever you want
            widthSize
        }

        val desiredHeight : Int = kotlin.math.ceil(width * 1.618033988749).toInt();

        //Measure Height
        height = if (heightMode == MeasureSpec.EXACTLY) {
            //Must be this size
            heightSize
        } else if (heightMode == MeasureSpec.AT_MOST) {
            //Can't be bigger than...
            Math.min(desiredHeight, heightSize)
        } else {
            //Be whatever you want
            desiredHeight
        }

        //MUST CALL THIS
        setMeasuredDimension(width, height)
    }
}