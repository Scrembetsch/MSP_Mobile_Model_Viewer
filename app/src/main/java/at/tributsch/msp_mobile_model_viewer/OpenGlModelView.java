package at.tributsch.msp_mobile_model_viewer;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class OpenGlModelView extends GLSurfaceView {

    public OpenGlModelView(Context context) {
        super(context);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
        Renderer.sAssetManager = context.getAssets();
        setRenderer(new Renderer());
        setOnTouchListener(new GestureDetector(context));
    }

    private static class Renderer implements GLSurfaceView.Renderer
    {
        public static AssetManager sAssetManager;

        public void onDrawFrame(GL10 gl) {
            ModelViewerJniBridge.Step();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            ModelViewerJniBridge.Resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            ModelViewerJniBridge.Init(sAssetManager);
        }
    }
}
