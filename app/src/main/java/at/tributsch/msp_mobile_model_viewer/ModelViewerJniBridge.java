package at.tributsch.msp_mobile_model_viewer;

public class ModelViewerJniBridge {
    static {
        System.loadLibrary("msp_mobile_model_viewer");
    }

    public static native void Init();
    public static native void Resize(int width, int height);
    public static native void Step();
}
