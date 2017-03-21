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
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.design.widget.NavigationView;
import android.support.design.widget.Snackbar;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import com.afollestad.materialdialogs.DialogAction;
import com.afollestad.materialdialogs.MaterialDialog;
import com.velli.homeautomationcontrol.collections.Room;
import com.velli.homeautomationcontrol.collections.RoomWidget;
import com.velli.homeautomationcontrol.interfaces.OnBtServiceStateChangedListener;

import org.xmlpull.v1.XmlPullParserException;

import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.LinkedHashMap;


import com.velli.homeautomationcontrol.interfaces.OnRoomWidgetDataReceivedListener;

public class ActivityMain extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    private static final String Tag = "ActivityMain";
    private static final int REQUEST_CODE_SCAN_FOR_DEVICES = 0;

    private BluetoothService mBtService;

    private NavigationView mNavigationView;

    private String mBtDeviceName = "";
    private String mBtDeviceAddress = "";

    private boolean mEndNodeRequiresLogin = true;

    private MenuItem mCurrentMenuItem;
    private MaterialDialog mDialog;
    private LinkedHashMap<Integer, OnRoomWidgetDataReceivedListener> mWidgetDataReceivedCallbacks = new LinkedHashMap<>();
    private ArrayList<OnBtServiceStateChangedListener> mBtStateChangedCallbacks = new ArrayList<>();
    private LinkedHashMap<Integer, Room> mRooms;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        final DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);

        setSupportActionBar(toolbar);


        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        mNavigationView = ((NavigationView) findViewById(R.id.nav_view));
        mNavigationView.setNavigationItemSelectedListener(this);

        mRooms = getDummyRooms();
        setData(mRooms);

        mBtService = BluetoothService.getInstance();
        mBtService.setHandler(new BluetoothServiceHandler(this));
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if(mBtService != null) {
            mBtService.disconnect();
            mBtService.setHandler(null);
        }
        mBtService = null;
        mWidgetDataReceivedCallbacks.clear();
        mWidgetDataReceivedCallbacks = null;
        mBtStateChangedCallbacks.clear();
        mBtStateChangedCallbacks = null;

    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_activity_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        if (id == R.id.menu_search_devices) {
            Intent i = new Intent(this, ActivitySearchDevices.class);
            startActivityForResult(i, REQUEST_CODE_SCAN_FOR_DEVICES);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        int id = item.getItemId();

        ((DrawerLayout)findViewById(R.id.drawer_layout)).closeDrawer(GravityCompat.START);

        switch (id) {
            case R.id.nav_settings:
                return true;
        }
        if (mRooms != null && mRooms.containsKey(id)) {
            final Room room = mRooms.get(id);
            final RoomControlFragment fragment = getNewRoomFragment(room.getRoomId(), room.getRoomWidgets());

            getSupportFragmentManager()
                    .beginTransaction()
                    .replace(R.id.activity_main_content, fragment, String.valueOf(room.getRoomId()))
                    .commit();
            if (mCurrentMenuItem != null) {
                mCurrentMenuItem.setChecked(false);
            }
            setTitle(item.getTitle());

            mCurrentMenuItem = item;
            mCurrentMenuItem.setChecked(true);
            return true;
        }

        return false;
    }

    private RoomControlFragment getNewRoomFragment(int id, LinkedHashMap<Integer, RoomWidget> widgets) {
        RoomControlFragment frag = new RoomControlFragment();
        frag.setRoomWidgets(id, widgets);
        return frag;
    }


    private LinkedHashMap<Integer, Room> getDummyRooms() {
        LinkedHashMap<Integer, Room> widgets = null;

        try {
            InputStream is = getAssets().open("room_widget_sceme.xml");

            try {
                widgets = new RoomWidgetParser().parse(is);
            } catch (XmlPullParserException ignored) {}
            is.close();
        } catch (IOException ignored) {}


        return widgets;
    }

    private void setData(LinkedHashMap<Integer, Room> widgets) {

        if(widgets != null) {
            mRooms = widgets;
            notifyWidgetDataReceivedCallbacks(widgets);
            int order = 0;
            Menu menu = mNavigationView.getMenu().findItem(R.id.nav_group_rooms).getSubMenu();
            for(Room room : widgets.values()) {

                if(menu.findItem(room.getRoomId()) == null) {
                    MenuItem item = menu.add(R.id.nav_group_rooms, room.getRoomId(), order, room.getRoomName());
                    item.setIcon(R.mipmap.ic_label_grey);
                    if(mCurrentMenuItem == null) {
                        onNavigationItemSelected(item);
                    }
                }

                order++;
            }
        }
        MenuItem logout = mNavigationView.getMenu().findItem(R.id.nav_log_out);
        if (logout != null) {
            logout.setVisible(mEndNodeRequiresLogin);
        }

    }

    public BluetoothService getBluetoothService() {
        return mBtService;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case REQUEST_CODE_SCAN_FOR_DEVICES:
                if(data != null) {
                    connectToDevice(data.getStringExtra(ActivitySearchDevices.INTENT_EXTRA_DEVICE_NAME),
                            data.getStringExtra(ActivitySearchDevices.INTENT_EXTRA_DEVICE_ADDRESS));
                }
                break;
        }
    }

    private void connectToDevice(String deviceName, String deviceAdress) {
        mBtDeviceName = deviceName;
        mBtDeviceAddress = deviceAdress;

        hideDialog();
        mBtService.connect(BluetoothAdapter.getDefaultAdapter().getRemoteDevice(deviceAdress));
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

    private void showConnectedToDevice() {
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
        private WeakReference<ActivityMain> mActivity;

        public BluetoothServiceHandler(ActivityMain activity) {
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
                    LinkedHashMap<Integer, Room> data = (LinkedHashMap) msg.obj;
                    if(data != null) {
                        mActivity.get().setData(data);
                    }
                    break;
            }
        }
    }

    public void registerOnRoomWidgetDataReceivedListener(int roomId, OnRoomWidgetDataReceivedListener l) {
        if(mWidgetDataReceivedCallbacks != null) {
            mWidgetDataReceivedCallbacks.put(roomId, l);
        }
    }

    public void unregisterOnRoomWidgetDataReceivedListener(int roomId) {
        if(mWidgetDataReceivedCallbacks != null) {
            mWidgetDataReceivedCallbacks.remove(roomId);
        }
    }

    public void registerOnBtServiceStateChangedListener(OnBtServiceStateChangedListener l) {
        if(mBtStateChangedCallbacks != null && !mBtStateChangedCallbacks.contains(l)) {
            mBtStateChangedCallbacks.add(l);
        }
    }

    public void unregisterOnBtServiceStateChangedListener(OnBtServiceStateChangedListener l) {
        if(mBtStateChangedCallbacks != null ) {
            mBtStateChangedCallbacks.remove(l);
        }
    }

    public void notifyWidgetDataReceivedCallbacks(LinkedHashMap<Integer, Room> data) {

        for(Room room : data.values()) {
            OnRoomWidgetDataReceivedListener callback = mWidgetDataReceivedCallbacks.get(room.getRoomId());
            if(callback != null) {
                callback.onRoomWidgetDataReceived(room);
            }
        }

    }

    public void notifyBtStateCallbacks(int newState) {
        if(mBtStateChangedCallbacks == null) {
            return;
        }
        for(OnBtServiceStateChangedListener c : mBtStateChangedCallbacks) {
            c.onBluetoothStateChanged(newState);
        }
    }

    public void commitRoomDataChanges(int roomId, RoomWidget widget) {
        Log.i(Tag, Tag + new RoomWidgetWriter().writeXml(roomId, widget));

        if(mBtService != null && mBtService.getState() == Constants.STATE_CONNECTED) {
            mBtService.write(new RoomWidgetWriter().writeXml(roomId, widget));
        } else {
            showNotConnectedError();
            setData(mRooms);
        }
    }

    public void showNotConnectedError() {
        Snackbar.make(findViewById(android.R.id.content), R.string.error_not_connected, Snackbar.LENGTH_LONG)
                .setAction(R.string.menu_bluetooth_connect, new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Intent i = new Intent(ActivityMain.this, ActivitySearchDevices.class);
                        startActivityForResult(i, REQUEST_CODE_SCAN_FOR_DEVICES);
                    }
                }).show();
    }
}
