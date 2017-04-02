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

import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import java.util.LinkedHashMap;


import com.velli.homeautomationcontrol.collections.Room;
import com.velli.homeautomationcontrol.collections.RoomWidget;
import com.velli.homeautomationcontrol.collections.RoomWidgetsAdapter;
import com.velli.homeautomationcontrol.collections.SpacesItemDecoration;
import com.velli.homeautomationcontrol.interfaces.OnBtServiceStateChangedListener;
import com.velli.homeautomationcontrol.interfaces.OnRoomWidgetChangedListener;
import com.velli.homeautomationcontrol.interfaces.OnRoomWidgetDataReceivedListener;


public class RoomControlFragment extends Fragment implements OnRoomWidgetChangedListener, OnBtServiceStateChangedListener, OnRoomWidgetDataReceivedListener, SwipeRefreshLayout.OnRefreshListener {
    private RecyclerView mList;
    private SwipeRefreshLayout mRefreshLayout;
    private LinkedHashMap<Integer, RoomWidget> mWidgets;
    private RoomWidgetsAdapter mAdapter;
    private int mRoomId = -1;
    private boolean mRefreshing = false;
    private Handler mRefreshHandler;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.fragment_room_control, container, false);
        mList = (RecyclerView) v.findViewById(R.id.fragment_room_recycler);
        mRefreshLayout = (SwipeRefreshLayout) v.findViewById(R.id.fragment_room_swipe_refresh_layout);
        return v;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        mList.setLayoutManager(new LinearLayoutManager(getActivity()));
        mList.addItemDecoration(new SpacesItemDecoration(getResources().getDimensionPixelSize(R.dimen.widget_card_padding)));

        if(mWidgets != null) {
            mAdapter = new RoomWidgetsAdapter(getActivity(), mWidgets);
            mAdapter.setOnRoomWidgetChangedListener(this);
            mList.setAdapter(mAdapter);
        }
        mRefreshHandler = new Handler();
        mRefreshLayout.setOnRefreshListener(this);
        ((ActivityMain)getActivity()).registerOnRoomWidgetDataReceivedListener(mRoomId, this);
        ((ActivityMain)getActivity()).registerOnBtServiceStateChangedListener(this);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        ((ActivityMain)getActivity()).unregisterOnRoomWidgetDataReceivedListener(mRoomId);
        ((ActivityMain)getActivity()).unregisterOnBtServiceStateChangedListener(this);
    }

    public void setRoomWidgets(int roomId, LinkedHashMap<Integer, RoomWidget> widgets) {
        mRoomId = roomId;
        mWidgets = widgets;

        if(mWidgets != null) {
            if(mAdapter == null && mList != null) {
                mAdapter = new RoomWidgetsAdapter(getActivity(), mWidgets);
                mAdapter.setOnRoomWidgetChangedListener(this);
                mAdapter.setWidgetsEnabled(((ActivityMain) getActivity()).getBluetoothService().getState() == Constants.STATE_CONNECTED);
                mList.setAdapter(mAdapter);
            } else if(mAdapter != null) {
                mAdapter.setRoomWidgets(mWidgets);
                mAdapter.notifyDataSetChanged();
            }
        }
    }


    @Override
    public void onRoomWidgetChanged(RoomWidget widget) {
        ((ActivityMain)getActivity()).commitRoomDataChanges(mRoomId, widget);
    }

    @Override
    public void onBluetoothStateChanged(int newState) {
        if(newState == Constants.STATE_CONNECTED && mAdapter != null) {
            mAdapter.setWidgetsEnabled(true);
        }
    }


    @Override
    public void onRoomWidgetDataReceived(Room data) {
        setRoomWidgets(data.getRoomId(), data.getRoomWidgets());
        if(mRefreshing) {
            mRefreshLayout.setRefreshing(false);
        }
    }

    @Override
    public void onRefresh() {
        /* User performed a swipe-to-refresh gesture. Request to update data */
        mRefreshing = true;
        ((ActivityMain)getActivity()).requestRoomConfiguration();
        mRefreshHandler.postDelayed(mRefreshRunnable, 1000);
    }

    private Runnable mRefreshRunnable= new Runnable() {
        @Override
        public void run() {
            if(mRefreshing && mRefreshLayout != null) {
                mRefreshLayout.setRefreshing(false);
            }
        }
    };
}
