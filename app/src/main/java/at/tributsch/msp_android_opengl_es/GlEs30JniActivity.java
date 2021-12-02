package at.tributsch.msp_android_opengl_es;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

public class GlEs30JniActivity extends Activity {

    GlEs30JniView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new GlEs30JniView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN);
    }
}