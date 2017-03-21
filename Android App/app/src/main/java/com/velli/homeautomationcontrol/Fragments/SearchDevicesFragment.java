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

package com.velli.homeautomationcontrol.Fragments;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.AppCompatButton;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;

import com.velli.homeautomationcontrol.views.RobotoLightTextView;

import java.util.ArrayList;
import java.util.Set;

import com.velli.homeautomationcontrol.collections.ListItemDevice;
import com.velli.homeautomationcontrol.interfaces.OnDeviceClickListener;
import com.velli.homeautomationcontrol.R;

public class SearchDevicesFragment extends Fragment implements View.OnClickListener {
    private static final int REQUEST_ENABLE_BT = 0;

    private BluetoothAdapter mBluetoothAdapter;

    private RecyclerView mList;
    private ProgressBar mProgressBar;
    private AppCompatButton mSearch;
    private DevicesAdapter mAdapter;
    private OnDeviceSelectedListener mListener;

    public interface OnDeviceSelectedListener {
        void onDeviceSelected(String deviceName, String deviceAddress);
    }
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_search_devices, container, false);

        mList = (RecyclerView) view.findViewById(R.id.search_devices_recycler_view);
        mList.setLayoutManager(new LinearLayoutManager(getContext()));

        mProgressBar = (ProgressBar) view.findViewById(R.id.search_devices_progressBar);
        mSearch = (AppCompatButton) view.findViewById(R.id.search_devices_button);
        mSearch.setOnClickListener(this);

        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);


        final AppCompatActivity activity = (AppCompatActivity) getActivity();

        activity.registerReceiver(mReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
        activity.registerReceiver(mReceiver, new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED));
        activity.getSupportActionBar().setTitle(R.string.menu_bluetooth_connect);

        mAdapter = new DevicesAdapter(getContext());
        mAdapter.setOnDeviceClickListener(new OnDeviceClickListener() {
            @Override
            public void onDeviceSelected(ListItemDevice d) {
                if(mListener != null) {
                    mListener.onDeviceSelected(d.name, d.address);
                }
            }
        });

        if(mList != null) {
            mList.setAdapter(mAdapter);
        }

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if(!mBluetoothAdapter.isEnabled()){
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);

            mSearch.setEnabled(false);
        } else {
            getPairedDevices();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        getActivity().unregisterReceiver(mReceiver);

        if (mBluetoothAdapter != null) {
            mBluetoothAdapter.cancelDiscovery();
        }
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onClick(View v) {
        searchForDevices();
    }

    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();

            if(BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);

                if(device.getBondState() != BluetoothDevice.BOND_BONDED){
                    String deviceName = device.getName();
                    mAdapter.addDevice(new ListItemDevice(deviceName == null || deviceName.isEmpty() ?
                            getString(R.string.title_device_no_name) : deviceName, device.getAddress(), false));
                }
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                mSearch.setEnabled(true);
                mSearch.setText(R.string.action_search_devices);
                mProgressBar.setVisibility(View.INVISIBLE);
            }
        }
    };

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch(requestCode){
            case REQUEST_ENABLE_BT:
                if(resultCode == Activity.RESULT_OK){
                    mSearch.setEnabled(true);
                    getPairedDevices();
                }
                break;
        }

    }

    public void setOnDeviceSelectedListener(OnDeviceSelectedListener l) {
        mListener = l;
    }

    private void getPairedDevices() {
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // Get a set of currently paired devices
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        // If there are paired devices, add each one to the ArrayAdapter
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                mAdapter.addDevice(new ListItemDevice(device.getName(), device.getAddress(), true));
            }
        }
    }

    private void searchForDevices() {

        if(mBluetoothAdapter != null && mBluetoothAdapter.isEnabled()) {
            mAdapter.removeDevices();
            mSearch.setText(R.string.action_searching_devices);
            mSearch.setEnabled(false);
            mProgressBar.setVisibility(View.VISIBLE);
            mBluetoothAdapter.startDiscovery();
        }
    }

    private class DevicesAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
        private static final int VIEW_TYPE_TITLE_NEW_DEVICES = 0;
        private static final int VIEW_TYPE_TITLE_BONDED_DEVICES = 1;
        private static final int VIEW_TYPE_BONDED_DEVICE = 2;
        private static final int VIEW_TYPE_NEW_DEVICE = 3;

        private LayoutInflater mInflater;
        private ArrayList<ListItemDevice> mListItemDevices = new ArrayList<>();
        private ArrayList<ListItemDevice> mBondedListItemDevices = new ArrayList<>();
        private OnDeviceClickListener mListener;

        public DevicesAdapter(Context c) {
            mInflater = LayoutInflater.from(c);
        }

        public void addDevice(ListItemDevice listItemDevice) {
            if(listItemDevice.bonded) {
                mBondedListItemDevices.add(listItemDevice);
            } else {
                mListItemDevices.add(listItemDevice);
            }
            notifyDataSetChanged();
        }

        public void removeDevices() {
            mListItemDevices.clear();
        }

        public void setOnDeviceClickListener(OnDeviceClickListener l) {
            mListener = l;
        }

        @Override
        public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            if(viewType == VIEW_TYPE_TITLE_BONDED_DEVICES || viewType == VIEW_TYPE_TITLE_NEW_DEVICES) {
                return new ViewHolderTitle(mInflater.inflate(R.layout.list_item_subtitle, parent, false));
            } else {
                return new ViewHolderDevice(mInflater.inflate(R.layout.list_item_device, parent, false));
            }
        }

        @Override
        public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
            int viewType = getItemViewType(position);

            if(viewType == VIEW_TYPE_TITLE_NEW_DEVICES || viewType == VIEW_TYPE_TITLE_BONDED_DEVICES) {
                ((ViewHolderTitle)holder).mTitle.setText(viewType == VIEW_TYPE_TITLE_NEW_DEVICES ? R.string.title_new_devices : R.string.title_bonded_devices);
                ((ViewHolderTitle)holder).itemView.setClickable(false);
            } else {
                ListItemDevice listItemDevice;
                if(position <= mBondedListItemDevices.size()) {
                    listItemDevice = mBondedListItemDevices.get(position -1);
                } else {
                    listItemDevice = mListItemDevices.get((position - mBondedListItemDevices.size()) -2);
                }

                ((ViewHolderDevice)holder).mName.setText(listItemDevice.name);
                ((ViewHolderDevice)holder).mAddress.setText(listItemDevice.address);
                ((ViewHolderDevice)holder).itemView.setOnClickListener(new ClickListener(listItemDevice));
            }
        }

        public int getActualPosition(int viewType, int position) {
            int bondedCount = mBondedListItemDevices.size();
            int newDevCount = mListItemDevices.size();

            if(bondedCount == 0 || bondedCount > 0 && position == 0) {
                return 0;
            } else if(bondedCount > 0 && position < bondedCount) {
                return position -1;
            } else if(bondedCount == 0 && newDevCount > 0 && position == 0) {
                return 0;
            }

            return position;
        }

        @Override
        public int getItemCount() {
            return 2 + mListItemDevices.size() + mBondedListItemDevices.size();
        }

        @Override
        public int getItemViewType(int position) {
            if(position == 0) {
                return VIEW_TYPE_TITLE_BONDED_DEVICES;
            } else if(position <= mBondedListItemDevices.size()) {
                return VIEW_TYPE_BONDED_DEVICE;
            } else if(position == mBondedListItemDevices.size() +1) {
                return VIEW_TYPE_TITLE_NEW_DEVICES;
            } else {
                return VIEW_TYPE_NEW_DEVICE;
            }
        }

        private class ClickListener implements View.OnClickListener {
            private ListItemDevice d;

            public ClickListener(ListItemDevice listItemDevice) {
                d = listItemDevice;
            }

            @Override
            public void onClick(View v) {
                if(mListener != null) {
                    mListener.onDeviceSelected(d);
                }
            }
        }
    }



    public class ViewHolderTitle extends RecyclerView.ViewHolder {
        protected RobotoLightTextView mTitle;

        public ViewHolderTitle(View itemView) {
            super(itemView);
            mTitle = (RobotoLightTextView) itemView.findViewById(R.id.list_item_subtitle);
        }
    }

    public class ViewHolderDevice extends RecyclerView.ViewHolder {
        protected RobotoLightTextView mName;
        protected RobotoLightTextView mAddress;

        public ViewHolderDevice(View itemView) {
            super(itemView);
            mName = (RobotoLightTextView) itemView.findViewById(R.id.list_item_device_name);
            mAddress = (RobotoLightTextView) itemView.findViewById(R.id.list_item_device_address);
        }
    }
}
