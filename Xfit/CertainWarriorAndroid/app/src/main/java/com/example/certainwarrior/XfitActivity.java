package com.example.certainwarrior;

import android.app.NativeActivity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

public class XfitActivity extends NativeActivity {
    public native void ApplyTextEditBox(String string);

    static {
        System.loadLibrary("CertainWarrior");
    };

    EditText editText;
    LinearLayout layout;
    String text;
    HandlerThread handlerThread;
    Handler handler;
    InputMethodManager imm;


    void setImmersiveSticky() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT >= 19) {
            setImmersiveSticky();

            View decorView = getWindow().getDecorView();
            decorView.setOnSystemUiVisibilityChangeListener
                    (new View.OnSystemUiVisibilityChangeListener() {
                        @Override
                        public void onSystemUiVisibilityChange(int visibility) {
                            setImmersiveSticky();
                        }
                    });
        }
        super.onCreate(savedInstanceState);

        imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);

        handlerThread = new HandlerThread("android_handler");
        handlerThread.start();
        handler = new Handler(handlerThread.getLooper());

        editText = new EditText(this);

        editText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                imm.hideSoftInputFromWindow(editText.getWindowToken(), 0);
                ApplyTextEditBox(editText.getText().toString());
                //editText.setEnabled(false);
                return false;
            }
        });

        addContentView(editText, new ViewGroup.LayoutParams(200,200));

        editText.setFocusable(true);
        editText.setFocusableInTouchMode(true);
    }
    public void ShowKeyboard(String string) {
        text = string;

        imm.showSoftInput(editText, 0);
        handler.post(new Runnable() {
            @Override
            public void run() {
                //editText.setEnabled(true);
                editText.setText(text);
                editText.setSelection(editText.length());
            }
        });
    }

    @Override
    protected void onResume() {
        //Hide toolbar
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT >= 11 && SDK_INT < 14) {
            getWindow().getDecorView().setSystemUiVisibility(View.STATUS_BAR_HIDDEN);
        } else if (SDK_INT >= 14 && SDK_INT < 19) {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_LOW_PROFILE);
        } else if (SDK_INT >= 19) {
            setImmersiveSticky();
        }
        super.onResume();
    }

    @Override
    public void onBackPressed() {

    }
}
