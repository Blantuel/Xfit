package com.willygames.certainwarrior;

import android.app.NativeActivity;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

public class XfitActivity extends NativeActivity {
    public native void ApplyTextEditBox(String string);
    public native void EnterTextEditBox(String string);

    static {
        System.loadLibrary("CertainWarrior");
    };

    EditText editText;
    String text;
    String clipText;
    InputMethodManager imm;
    XfitActivity THIS = this;

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

        editText = new EditText(this);

        editText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                imm.hideSoftInputFromWindow(editText.getWindowToken(), 0);
                ApplyTextEditBox(editText.getText().toString());
                return false;
            }
        });
        editText.addTextChangedListener(new TextWatcher() {
                @Override
                public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                }

                @Override
                public void onTextChanged(CharSequence s, int start, int before, int count) {

                }

                @Override
                public void afterTextChanged(Editable s) {
                    EnterTextEditBox(s.toString());
                }
        }
        );
        addContentView(editText, new ViewGroup.LayoutParams(200,200));

        editText.setFocusable(true);
        editText.setFocusableInTouchMode(true);
    }
    public void SetClipBoardData(String data) {
        clipText = data;
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                ClipboardManager clipboardManager = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);

                ClipData clipData = ClipData.newPlainText("Xfit", clipText);
                clipboardManager.setPrimaryClip(clipData);
            }
        });
    }

    public String GetClipBoardData() throws InterruptedException {
        synchronized(this) {
            Handler handler = new Handler(Looper.getMainLooper());
            Runnable runnable = new Runnable() {
                @Override
                public void run() {
                    synchronized (THIS) {
                        ClipboardManager clipboardManager = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
                        if (clipboardManager.hasPrimaryClip() && clipboardManager.getPrimaryClipDescription().hasMimeType(ClipDescription.MIMETYPE_TEXT_PLAIN)) {
                            ClipData.Item item = clipboardManager.getPrimaryClip().getItemAt(0);
                            clipText = item.getText().toString();
                        } else {
                            clipText = "";
                        }
                        THIS.notifyAll();
                    }
                }
            };
            handler.post(runnable);
            wait();
        }
        return clipText;
    }

    public void ShowKeyboard(String string) {
        text = string;

        editText.post(new Runnable() {
            @Override
            public void run() {
                editText.requestFocus();
                imm.showSoftInput(editText, 0);
                //editText.setEnabled(true);
                editText.setText(text);
                editText.setSelection(editText.length());
            }
        });
    }
    public void HideKeyboard() {
        imm.hideSoftInputFromWindow(editText.getWindowToken(), 0);
    }

    public void OpenBrowser(String url) {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
        startActivity(browserIntent);
    }

    @Override
    protected void onResume() {
        //Hide toolbar
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT < 19) {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_LOW_PROFILE);
        } else if (SDK_INT >= 19) {
            setImmersiveSticky();
        }
        super.onResume();
    }

    @Override
    public void onBackPressed() {

    }

    public int GetIP() {
        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService (WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo ();
        int address = wifiInfo.getIpAddress();
        if(address != 0)return address;
        try {
            List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface intf : interfaces) {
                List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
                for (InetAddress addr : addrs) {
                    if (!addr.isLoopbackAddress()) {
                        byte[] sAddr = addr.getAddress();

                        long ret=0;
                        for(int i=0;i<sAddr.length;i++)
                        {

                            ret |= ((sAddr[i]& 0xff)<< (8*i));

                        }

                        return (int)ret;
                    }
                }
            }
        } catch (Exception ignored) { } // for now eat exceptions
        return 0;
    }
}
