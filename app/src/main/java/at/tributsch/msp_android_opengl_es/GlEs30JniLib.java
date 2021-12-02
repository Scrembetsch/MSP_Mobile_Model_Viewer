package at.tributsch.msp_android_opengl_es;

public class GlEs30JniLib {

    static {
        System.loadLibrary("gl_es_3x_jni");
    }

    public static native void init();
    public static native void resize(int width, int height);
    public static native void step();
}