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

package com.velli.homeautomationcontrol.collections;

import android.content.Context;
import android.support.v7.widget.AppCompatSeekBar;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.SwitchCompat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.SeekBar;

import com.velli.homeautomationcontrol.interfaces.OnRoomWidgetChangedListener;
import com.velli.homeautomationcontrol.views.RobotoLightTextView;

import java.util.ArrayList;
import java.util.LinkedHashMap;

import com.velli.homeautomationcontrol.R;


public class RoomWidgetsAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    public static final int WIDGET_TYPE_LIGHTS = 0;
    public static final int WIDGET_TYPE_TEMPERATURE = 1;
    public static final int WIDGET_TYPE_AIR_CONDITIONER = 2;

    private LayoutInflater mInflater;
    private LinkedHashMap<Integer, RoomWidget> mWidgets;
    private OnRoomWidgetChangedListener mListener;
    private boolean mWidgetsEnabled = true;

    public RoomWidgetsAdapter(Context c,  LinkedHashMap<Integer, RoomWidget> widgets) {
        mInflater = LayoutInflater.from(c);
        mWidgets = widgets;
    }

    public void setOnRoomWidgetChangedListener(OnRoomWidgetChangedListener l) {
        mListener = l;
    }

    public void setRoomWidgets(LinkedHashMap<Integer, RoomWidget> widgets) {
        mWidgets = widgets;
    }



    public void setWidgetsEnabled(boolean enabled) {
        mWidgetsEnabled = enabled;
    }

    @Override
    public int getItemViewType(int position) {
        return (new ArrayList<>(mWidgets.values()).get(position)).mType;
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        if(viewType == WIDGET_TYPE_LIGHTS) {
            return new ViewHolderWidgetLights(mInflater.inflate(R.layout.list_item_widget_lights, parent, false));
        } else if(viewType == WIDGET_TYPE_TEMPERATURE) {
            return new ViewHolderWidgetTemperature(mInflater.inflate(R.layout.list_item_widget_temperature, parent, false));
        } else if(viewType == WIDGET_TYPE_AIR_CONDITIONER) {
            return new ViewHolderWidgetAirConditioner(mInflater.inflate(R.layout.list_item_widget_air_conditioner, parent, false));
        }
        return null;
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        RoomWidget widget = new ArrayList<>(mWidgets.values()).get(position);

        if(widget.mType == WIDGET_TYPE_LIGHTS) {
            WidgetListener listener = new WidgetListener(widget);
            ((ViewHolderWidgetLights)holder).itemView.setEnabled(mWidgetsEnabled);
            ((ViewHolderWidgetLights) holder).mTitle.setText(widget.mName);
            ((ViewHolderWidgetLights)holder).mSwicth.setOnCheckedChangeListener(null);
            ((ViewHolderWidgetLights)holder).mSwicth.setChecked(widget.mBoolValue);
            ((ViewHolderWidgetLights)holder).mSwicth.setOnCheckedChangeListener(listener);
            ((ViewHolderWidgetLights)holder).mSeekbar.setProgress(widget.mIntValue);
            ((ViewHolderWidgetLights)holder).mSeekbar.setEnabled(widget.mBoolValue);
            ((ViewHolderWidgetLights)holder).mSeekbar.setOnSeekBarChangeListener(listener);
        } else if(widget.mType == WIDGET_TYPE_TEMPERATURE) {
            ((ViewHolderWidgetTemperature)holder).itemView.setEnabled(mWidgetsEnabled);
            ((ViewHolderWidgetTemperature)holder).mTitle.setText(widget.mName);
            ((ViewHolderWidgetTemperature)holder).mTemperature.setText(String.valueOf(widget.mIntValue) + " C°");
        } else if(widget.mType == WIDGET_TYPE_AIR_CONDITIONER) {
            WidgetListener listener = new WidgetListener(widget);
            ((ViewHolderWidgetAirConditioner)holder).itemView.setEnabled(mWidgetsEnabled);
            ((ViewHolderWidgetAirConditioner)holder).mTitle.setText(widget.mName);
            ((ViewHolderWidgetAirConditioner)holder).mAuto.setOnCheckedChangeListener(null);
            ((ViewHolderWidgetAirConditioner)holder).mAuto.setChecked(widget.mBoolValue);
            ((ViewHolderWidgetAirConditioner)holder).mAuto.setOnCheckedChangeListener(listener);
            ((ViewHolderWidgetAirConditioner)holder).mSeekbar.setProgress(widget.mIntValue);
            ((ViewHolderWidgetAirConditioner)holder).mSeekbar.setOnSeekBarChangeListener(listener);
            ((ViewHolderWidgetAirConditioner)holder).mSeekbar.setEnabled(!widget.mBoolValue);
            ((ViewHolderWidgetAirConditioner)holder).mTemperature.setText(String.valueOf(widget.mIntValue) + " C°");
        }
    }

    @Override
    public int getItemCount() {
        return mWidgets.size();
    }




    public class ViewHolderWidgetLights extends RecyclerView.ViewHolder {
        private RobotoLightTextView mTitle;
        private SwitchCompat mSwicth;
        private AppCompatSeekBar mSeekbar;

        public ViewHolderWidgetLights(View itemView) {
            super(itemView);

            mTitle = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_lights_title);
            mSwicth = (SwitchCompat) itemView.findViewById(R.id.list_item_widget_lights_switch);
            mSeekbar = (AppCompatSeekBar) itemView.findViewById(R.id.list_item_widget_lights_bightness);
        }
    }

    public class ViewHolderWidgetTemperature extends RecyclerView.ViewHolder {
        private RobotoLightTextView mTitle;
        private RobotoLightTextView mTemperature;

        public ViewHolderWidgetTemperature(View itemView) {
            super(itemView);

            mTitle = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_temperature_title);
            mTemperature = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_temperature);
        }
    }

    public class ViewHolderWidgetAirConditioner extends RecyclerView.ViewHolder {
        private RobotoLightTextView mTitle;
        private RobotoLightTextView mTemperature;
        private RobotoLightTextView mAirHumidity;

        private CheckBox mAuto;
        private AppCompatSeekBar mSeekbar;

        public ViewHolderWidgetAirConditioner(View itemView) {
            super(itemView);

            mTitle = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_air_cond_title);
            mTemperature = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_air_cond_temp_text);
            mAirHumidity = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_air_humidity_text);

            mAuto = (CheckBox) itemView.findViewById(R.id.list_item_widget_air_cond_auto);
            mSeekbar = (AppCompatSeekBar) itemView.findViewById(R.id.list_item_widget_air_cond_temp_bar);
        }
    }

    private class WidgetListener implements CompoundButton.OnCheckedChangeListener, SeekBar.OnSeekBarChangeListener {
        private RoomWidget mWidget;

        public WidgetListener(RoomWidget widget) {
            mWidget = widget;
        }

        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            if(mListener != null) {
                mWidget.mBoolValue = isChecked;
                mWidget.mStatus = RoomWidget.STATUS_UPDATES_PENDING;
                mListener.onRoomWidgetChanged(mWidget);
            }
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {}

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            if(mListener != null) {
                mWidget.mIntValue = seekBar.getProgress();
                mWidget.mStatus = RoomWidget.STATUS_UPDATES_PENDING;
                mListener.onRoomWidgetChanged(mWidget);
            }
        }
    }
}
