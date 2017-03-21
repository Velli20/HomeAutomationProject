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

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.widget.Toolbar;


import com.velli.homeautomationcontrol.Fragments.LoginFragment;
import com.velli.homeautomationcontrol.Fragments.SearchDevicesFragment;



public class ActivityStartUp extends ConnectionActivity implements SearchDevicesFragment.OnDeviceSelectedListener, LoginFragment.OnDoLoginListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_up);

        final Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FragmentTransaction ft = getFragmentTransaction();
        ft.setCustomAnimations(R.anim.slide_in_from_right, R.anim.slide_out_to_left, R.anim.slide_out_to_left, R.anim.slide_out_to_right);

        if(BluetoothService.getInstance().getState() != Constants.STATE_CONNECTED) {
            SearchDevicesFragment frag = new SearchDevicesFragment();
            frag.setOnDeviceSelectedListener(this);
            ft.replace(R.id.activity_start_up_content, frag).commit();
        }

    }

    private FragmentTransaction getFragmentTransaction() {
        FragmentTransaction ft = getSupportFragmentManager().beginTransaction();
        ft.setCustomAnimations(R.anim.slide_in_from_right, R.anim.slide_out_to_left, R.anim.slide_in_from_left, R.anim.slide_out_to_right);

        return ft;
    }

    @Override
    public void onDeviceSelected(String deviceName, String deviceAddress) {
        connectToDevice(deviceName, deviceAddress);
    }

    @Override
    public void notifyBtStateCallbacks(int newState) {
        super.notifyBtStateCallbacks(newState);

        if(newState == Constants.STATE_CONNECTED) {
            LoginFragment frag = new LoginFragment();
            frag.setOnDoLoginListener(this);

            getFragmentTransaction().addToBackStack(null).replace(R.id.activity_start_up_content, frag).commit();
        }
    }

    @Override
    public void doLogin(String username, String password) {
        Intent i = new Intent(this, ActivityMain.class);
        startActivity(i);
    }
}
