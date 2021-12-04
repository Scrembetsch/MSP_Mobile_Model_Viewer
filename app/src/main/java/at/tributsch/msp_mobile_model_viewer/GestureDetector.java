package at.tributsch.msp_mobile_model_viewer;

import android.content.Context;
import android.util.Log;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.VelocityTracker;
import android.view.View;

public class GestureDetector implements View.OnTouchListener, ScaleGestureDetector.OnScaleGestureListener {
    private View mView;
    private ScaleGestureDetector mGestureScale;
    private float mLastX;
    private float mLastY;
    private boolean mInScale = false;

    public GestureDetector (Context c){
        mGestureScale = new ScaleGestureDetector(c, this);
    }

    @Override
    public boolean onTouch(View view, MotionEvent event) {
        this.mView = view;
        mGestureScale.onTouchEvent(event);

        int index = event.getActionIndex();
        int action = event.getActionMasked();
        int pointerId = event.getPointerId(index);

        switch(action) {
            case MotionEvent.ACTION_DOWN:
                mLastX = event.getX();
                mLastY = event.getY();
                break;

            case MotionEvent.ACTION_MOVE:
            {
                float xMove = mLastX - event.getX();
                float yMove = mLastY - event.getY();
                mLastX = event.getX();
                mLastY = event.getY();
                if(!mInScale)
                {
                    ModelViewerJniBridge.Move(xMove, yMove);
                }
            }
                break;

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_CANCEL:
                break;
        }

        return true;
    }

    @Override
    public boolean onScale(ScaleGestureDetector detector) {
        float scale = ((float)((int)(detector.getScaleFactor() * 100))) / 100; // Change precision to help with jitter when user just rests their fingers //
        ModelViewerJniBridge.SetScale(scale);
        return true;
    }

    @Override
    public boolean onScaleBegin(ScaleGestureDetector detector) {
        mInScale = true;
        return true;
    }

    @Override
    public void onScaleEnd(ScaleGestureDetector detector) {
        mInScale = false;
    }
}
