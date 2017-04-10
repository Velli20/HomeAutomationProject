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
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.SeekBar;

import com.velli.homeautomationcontrol.Constants;
import com.velli.homeautomationcontrol.interfaces.OnUserUpdatedWidgetStateListener;
import com.velli.homeautomationcontrol.views.RobotoLightTextView;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Locale;

import com.velli.homeautomationcontrol.R;


public class RoomWidgetsAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    public static final int WIDGET_TYPE_LIGHTS = 0;
    public static final int WIDGET_TYPE_TEMPERATURE = 1;
    public static final int WIDGET_TYPE_AIR_CONDITIONER = 2;

    private LayoutInflater mInflater;
    private LinkedHashMap<Integer, RoomWidget> mWidgets;
    private OnUserUpdatedWidgetStateListener mListener;
    private boolean mWidgetsEnabled = true;

    public RoomWidgetsAdapter(Context c,  LinkedHashMap<Integer, RoomWidget> widgets) {
        mInflater = LayoutInflater.from(c);
        mWidgets = widgets;
    }

    public void setOnRoomWidgetChangedListener(OnUserUpdatedWidgetStateListener l) {
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
            return new ViewHolderWidgetThermostat(mInflater.inflate(R.layout.list_item_widget_temperature, parent, false));
        } else if(viewType == WIDGET_TYPE_AIR_CONDITIONER) {
            return new ViewHolderWidgetAirConditioner(mInflater.inflate(R.layout.list_item_widget_air_conditioner, parent, false));
        }
        return null;
    }

    private String getCurrentTemperatureString(RoomWidget widget) {
        return String.format(Locale.getDefault(), "%d C°", widget.mIntValue);
    }

    private String getTargetTemperatureString(RoomWidget widget) {
        return getTargetTemperatureString(widget.mIntTargetValue);
    }

    private String getTargetTemperatureString(int value) {
        return String.format(Locale.getDefault(), "/ %d C°", value);
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        RoomWidget widget = new ArrayList<>(mWidgets.values()).get(position);
        WidgetListener listener = new WidgetListener(widget);
        if(widget.mType == WIDGET_TYPE_LIGHTS) {
            ((ViewHolderWidgetLights)holder).itemView.setEnabled(mWidgetsEnabled);
            ((ViewHolderWidgetLights) holder).mTitle.setText(widget.mName);
            ((ViewHolderWidgetLights)holder).mSwitch.setOnCheckedChangeListener(null);
            ((ViewHolderWidgetLights)holder).mSwitch.setChecked(widget.mBoolValue);
            ((ViewHolderWidgetLights)holder).mSwitch.setOnCheckedChangeListener(listener);
            ((ViewHolderWidgetLights)holder).mSeekBar.setProgress(widget.mIntTargetValue);
            ((ViewHolderWidgetLights)holder).mSeekBar.setEnabled(widget.mBoolValue);
            ((ViewHolderWidgetLights)holder).mSeekBar.setOnSeekBarChangeListener(listener);
        } else if(widget.mType == WIDGET_TYPE_TEMPERATURE) {
            listener.setTargetTemperatureText(((ViewHolderWidgetThermostat)holder).mTargetTemperature);
            ((ViewHolderWidgetThermostat)holder).itemView.setEnabled(mWidgetsEnabled);
            ((ViewHolderWidgetThermostat)holder).mTitle.setText(widget.mName);
            ((ViewHolderWidgetThermostat)holder).mTemperature.setText(getCurrentTemperatureString(widget));
            ((ViewHolderWidgetThermostat)holder).mTargetTemperature.setText(getTargetTemperatureString(widget));
            ((ViewHolderWidgetThermostat)holder).mTargetTemperatureSeekBar.setProgress(widget.mIntTargetValue);
            ((ViewHolderWidgetThermostat)holder).mTargetTemperatureSeekBar.setMax(Constants.MAX_TARGET_TEMPERATURE_CELSIUS);
            ((ViewHolderWidgetThermostat)holder).mTargetTemperatureSeekBar.setOnSeekBarChangeListener(listener);

        } else if(widget.mType == WIDGET_TYPE_AIR_CONDITIONER) {
            ((ViewHolderWidgetAirConditioner)holder).itemView.setEnabled(mWidgetsEnabled);
            ((ViewHolderWidgetAirConditioner)holder).mTitle.setText(widget.mName);
            ((ViewHolderWidgetAirConditioner)holder).mAuto.setOnCheckedChangeListener(null);
            ((ViewHolderWidgetAirConditioner)holder).mAuto.setChecked(widget.mBoolValue);
            ((ViewHolderWidgetAirConditioner)holder).mAuto.setOnCheckedChangeListener(listener);
            ((ViewHolderWidgetAirConditioner)holder).mSeekbar.setProgress(widget.mIntTargetValue);
            ((ViewHolderWidgetAirConditioner)holder).mSeekbar.setOnSeekBarChangeListener(listener);
            ((ViewHolderWidgetAirConditioner)holder).mSeekbar.setEnabled(!widget.mBoolValue);
            ((ViewHolderWidgetAirConditioner)holder).mTemperature.setText(getCurrentTemperatureString(widget));
        }
    }

    @Override
    public int getItemCount() {
        return mWidgets.size();
    }





    private class WidgetListener implements CompoundButton.OnCheckedChangeListener, SeekBar.OnSeekBarChangeListener {
        private RoomWidget mWidget;
        private RobotoLightTextView mTargetTemperatureText;

        public WidgetListener(RoomWidget widget) {
            mWidget = widget;
        }

        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            if(mListener != null) {
                mWidget.mBoolValue = isChecked;
                mWidget.mStatus = RoomWidget.STATUS_UPDATES_PENDING;
                mListener.onUserUpdatedWidgetState(mWidget);
            }
        }

        public void setTargetTemperatureText(RobotoLightTextView text) {
            mTargetTemperatureText = text;
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            if(mWidget.mType == WIDGET_TYPE_TEMPERATURE) {
                if(progress < Constants.MIN_TARGET_TEMPERATURE_CELSIUS) {
                    seekBar.setProgress(Constants.MIN_TARGET_TEMPERATURE_CELSIUS);
                } else if(progress > Constants.MAX_TARGET_TEMPERATURE_CELSIUS) {
                    seekBar.setProgress(Constants.MAX_TARGET_TEMPERATURE_CELSIUS);
                }

                if(mTargetTemperatureText != null) {
                    mTargetTemperatureText.setText(getTargetTemperatureString(progress));
                }
            }
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {}

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            if(mListener != null) {
                mWidget.mIntTargetValue = seekBar.getProgress();
                mWidget.mStatus = RoomWidget.STATUS_UPDATES_PENDING;
                mListener.onUserUpdatedWidgetState(mWidget);
            }
        }
    }
}
