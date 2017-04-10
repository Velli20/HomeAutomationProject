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

import android.support.v7.widget.AppCompatSeekBar;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import com.velli.homeautomationcontrol.R;
import com.velli.homeautomationcontrol.views.RobotoLightTextView;

public class ViewHolderWidgetThermostat extends RecyclerView.ViewHolder {
    public RobotoLightTextView mTitle;
    public RobotoLightTextView mTemperature;
    public RobotoLightTextView mTargetTemperature;
    public AppCompatSeekBar mTargetTemperatureSeekBar;

    public ViewHolderWidgetThermostat(View itemView) {
        super(itemView);

        mTitle = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_temperature_title);
        mTemperature = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_temperature);
        mTargetTemperature = (RobotoLightTextView) itemView.findViewById(R.id.list_item_widget_target_temperature);
        mTargetTemperatureSeekBar = (AppCompatSeekBar) itemView.findViewById(R.id.list_item_widget_temperature_seek_bar);
    }
}