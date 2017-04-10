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
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import org.xmlpull.v1.XmlPullParserException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.util.LinkedHashMap;

import com.velli.homeautomationcontrol.collections.Room;

public class BluetoothService {


    private BluetoothAdapter mBtAdapter;
    private ConnectThread mConnectThread;
    private ConnectedThread mConnectedThread;

    private Handler mHandler;
    private int mState;
    private static BluetoothService sInstance;

    public static BluetoothService getInstance(){
        if(sInstance == null) sInstance = getSync();
        return sInstance;
    }

    private static synchronized BluetoothService getSync() {
        if(sInstance == null) sInstance = new BluetoothService();
        return sInstance;
    }

    public BluetoothService() {
        mBtAdapter = BluetoothAdapter.getDefaultAdapter();
        mState = Constants.STATE_NONE;
    }

    private synchronized void setState(int state) {
        mState = state;
        // Give the new state to the Handler so the UI Activity can update
        if(mHandler != null) {
            mHandler.obtainMessage(Constants.MESSAGE_STATE_CHANGE, state, -1).sendToTarget();
        }
    }
    public synchronized int getState() {
        return mState;
    }

    public synchronized void connect(BluetoothDevice device) {

        // Start the thread to connect with the given device
        // Cancel any thread attempting to make a connection
        if (mState == Constants.STATE_CONNECTING) {
            if (mConnectThread != null) {mConnectThread.cancel(); mConnectThread = null;}
        }
        mConnectThread = new ConnectThread(device);
        mConnectThread.start();
        setState(Constants.STATE_CONNECTING);

    }

    public void setHandler(Handler handler) {
        mHandler = handler;
    }

    public void disconnect() {
        setState(Constants.STATE_NONE);
        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }
    }

    public synchronized void connected(BluetoothSocket socket, BluetoothDevice device) {
        // Cancel the thread that completed the connection
        if (mConnectThread != null) {mConnectThread.cancel(); mConnectThread = null; }
        // Cancel the ConnectedThread to make sure that it's not running currently connection
        if (mConnectedThread != null) {mConnectedThread.cancel(); mConnectedThread = null;}
        // Send the name of the connected device back to the UI Activity
        sendMessageToUiThread(Constants.DEVICE_NAME, device.getName());
        // Start the ConnectedThread to manage connection and perform transmission
        mConnectedThread = new ConnectedThread(socket);
        mConnectedThread.start();
        // Set state of connection to STATE_CONNECTED
        setState(Constants.STATE_CONNECTED);
    }

    private void sendMessageToUiThread(String key, String message) {
        if(mHandler == null) {
            return;
        }
        Message msg = mHandler.obtainMessage(Constants.MESSAGE_DEVICE_NAME);
        Bundle bundle = new Bundle();
        bundle.putString(key, message);
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    private void sendObjectToUiThread(Object object) {
        if(mHandler == null) {
            return;
        }
        Message msg = mHandler.obtainMessage(Constants.MESSAGE_READ);
        msg.obj = object;
        mHandler.sendMessage(msg);
    }

    public void write(byte[] buffer) {
        if(mConnectedThread != null && getState() == Constants.STATE_CONNECTED) {
            mConnectedThread.write(buffer);
        }
    }

    private class ConnectThread extends Thread {
        private final BluetoothSocket mSocket;
        private final BluetoothDevice mDevice;

        public ConnectThread(BluetoothDevice device) {
            mDevice = device;

            BluetoothSocket tmp = null;
            try {
                tmp = device.createInsecureRfcommSocketToServiceRecord(Constants.MY_UUID);
            } catch (IOException ignored) { }
            mSocket = tmp;
        }

        public void run() {
            mBtAdapter = BluetoothAdapter.getDefaultAdapter();
            mBtAdapter.cancelDiscovery();

            try {
                mSocket.connect();
            } catch (IOException connectException) {
                try {
                    setState(Constants.STATE_CONNECTION_FAILED);
                    mSocket.close();
                } catch (IOException ignored) { }
                return;
            }

            synchronized (BluetoothService.this) {
                mConnectThread = null;
            }
            // Do work to manage the connection (in a separate thread)
            connected(mSocket, mDevice);
        }

        /** Will cancel an in-progress connection, and close the socket */
        public void cancel() {
            try {
                mSocket.close();
            } catch (IOException ignored) { }
        }
    }

    private class ConnectedThread extends Thread {
        private final BluetoothSocket mSocket;
        private final InputStream mInStream;
        private final OutputStream mOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            }
            catch (IOException ignored) {}

            mInStream = tmpIn;
            mOutStream = tmpOut;


        }

        public void run() {
            // Keep listening until an error occurs
            byte[] buffer = new byte[4096];

            int bytes = 0;
            while(true) {
                try {
                    byte b = (byte) mInStream.read();

                    if(Constants.SERIAL_START_FLAG == (char)b) {
                        /* Start of the transmission. Initialize buffer */
                        buffer = new byte[4096];
                        bytes=0;
                    } else if(Constants.SERIAL_END_FLAG == (char)b) {
                        /* End of the transmission */
                        try {
                            sendObjectToUiThread(new String(buffer, "UTF-8").substring(0, bytes));
                        }   catch (IOException ignored) {}
                    } else if(bytes < 4096) {
                        buffer[bytes] = b;
                        bytes++;
                    }

                } catch (IOException e) {
                    cancel();
                    break;
                }




            }

        }

        public void write(byte[] buffer) {
            if(mState == Constants.STATE_CONNECTED && buffer != null) {
                try {
                    mOutStream.write(buffer);
                    mOutStream.flush();
                } catch (IOException ignored) {
                }
            }
        }

        public void cancel() {
            try {
                mInStream.close();
            } catch (IOException ignored) {
            }

            if (mOutStream != null) {
                try {
                    mOutStream.close();
                } catch (IOException ignored) {}
            }
            try {
                mSocket.close();
                setState(Constants.STATE_NONE);

            } catch (IOException ignored) {
            }

        }
    }
}
