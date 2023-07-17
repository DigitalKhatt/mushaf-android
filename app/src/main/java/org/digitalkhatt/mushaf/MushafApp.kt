package org.digitalkhatt.mushaf

import android.app.Application
import android.content.Context
import android.content.res.AssetManager

class MushafApp : Application() {

    private lateinit var mgr: AssetManager;

    private external fun nativeInit(mgr : AssetManager)

    private external fun set_tajweed(tajweed : Boolean)

    var justify : Boolean = true

    var tajweed = true
        set(value) {
            field = value
            set_tajweed(value)
        }

    companion object {
        var ctx: Context? = null

        lateinit var instance: MushafApp
            private set

        init {

            System.loadLibrary("skia")
            System.loadLibrary("mushaf")
        }
    }

    override fun onCreate() {
        super.onCreate()
        ctx = applicationContext
        mgr = getResources().getAssets()
        nativeInit(mgr)
        instance = this
    }

}