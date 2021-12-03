package at.tributsch.msp_mobile_model_viewer;

import android.content.res.AssetManager;

public class ModelViewerJniBridge {
    static {
        System.loadLibrary("msp_mobile_model_viewer");
    }

    public static native void Init(AssetManager assetManager);
    public static native void Resize(int width, int height);
    public static native void Step();
    public static native void SetScale(float scale);
}
