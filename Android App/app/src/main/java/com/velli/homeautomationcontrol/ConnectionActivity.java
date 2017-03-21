/*
 *
 *  * MIT License
 *  *
 *  * Copyright (c) [2017] [velli20]
 *  *
 *  * Permission is hereby granted, free of charge, to any person obtaining a copy
 *  * of this software and associated documentation files (the "Software"), to deal
 *  * in the Software without restriction, including without limitation the rights
 *  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  * copies of the Software, and to permit persons to whom the Software is
 *  * furnished to do so, subject to the following conditions:
 *  *
 *  * The above copyright notice and this permission notice shall be included in all
 *  * copies or substantial portions of the Software.
 *  *
 *  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  * SOFTWARE.
 *
 */

package com.velli.homeautomationcontrol;

import android.bluetooth.BluetoothAdapter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;

import com.afollestad.materialdialogs.DialogAction;
import com.afollestad.materialdialogs.MaterialDialog;

import java.lang.ref.WeakReference;
import java.util.ArrayList;


import com.velli.homeautomationcontrol.interfaces.OnBtServiceStateChangedListener;


public class ConnectionActivity extends AppCompatActivity {
    private String mBtDeviceName = "";
    private String mBtDeviceAddress = "";

    private boolean mEndNodeRequiresLogin = true;

    private MaterialDialog mDialog;
    private ArrayList<OnBtServiceStateChangedListener> mBtStateChangedCallbacks = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        BluetoothService.getInstance().setHandler(new BluetoothServiceHandler(this));
    }



    public void connectToDevice(String deviceName, String deviceAdress) {
        mBtDeviceName = deviceName;
        mBtDeviceAddress = deviceAdress;

        hideDialog();
        BluetoothService.getInstance().connect(BluetoothAdapter.getDefaultAdapter().getRemoteDevice(deviceAdress));
        mDialog = new MaterialDialog.Builder(this)
                .content(getString(R.string.action_connecting_to_device) + " " + deviceName)
                .cancelable(false)
                .progress(true, 0)
                .show();
    }

    private void showConnectionFailed() {
        hideDialog();

        mDialog = new MaterialDialog.Builder(this)
                .content(getString(R.string.action_connecting_to_device_failed))
                .positiveText(R.string.action_retry)
                .negativeText(R.string.action_cancel)
                .onPositive(new MaterialDialog.SingleButtonCallback() {
                    @Override
                    public void onClick(@NonNull MaterialDialog materialDialog, @NonNull DialogAction dialogAction) {
                        materialDialog.dismiss();
                        connectToDevice(mBtDeviceName, mBtDeviceAddress);
                    }
                })
                .show();
    }

    public void showConnectedToDevice() {
        hideDialog();
        Snackbar.make(findViewById(android.R.id.content),
                getText(R.string.action_connected_to_device) + " " + mBtDeviceAddress,
                Snackbar.LENGTH_LONG).show();
    }

    private void hideDialog() {
        if(mDialog != null && mDialog.isShowing()) {
            mDialog.dismiss();
        }
        mDialog = null;
    }

    private static class BluetoothServiceHandler extends Handler {
        private WeakReference<ConnectionActivity> mActivity;

        public BluetoothServiceHandler(ConnectionActivity activity) {
            mActivity = new WeakReference<>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            if(mActivity == null || mActivity.get() == null) {return;}

            switch (msg.what) {
                case Constants.MESSAGE_STATE_CHANGE:
                    mActivity.get().notifyBtStateCallbacks(msg.arg1);
                    switch(msg.arg1){
                        case Constants.STATE_NONE:
                            break;
                        case Constants.STATE_CONNECTING:
                            break;
                        case Constants.STATE_CONNECTED:
                            mActivity.get().showConnectedToDevice();
                            break;
                        case Constants.STATE_CONNECTION_FAILED:
                            mActivity.get().showConnectionFailed();
                            break;
                    }
                    break;
                case Constants.MESSAGE_READ:

                    break;
            }
        }
    }

    public void notifyBtStateCallbacks(int newState) {
        for(OnBtServiceStateChangedListener c : mBtStateChangedCallbacks) {
            c.onBluetoothStateChanged(newState);
        }
    }
}
