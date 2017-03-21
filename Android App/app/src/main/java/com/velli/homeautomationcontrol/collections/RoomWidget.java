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


public class RoomWidget {
    public static int STATUS_UPDATES_PENDING = 0;
    public static int STATUS_OK = 1;
    public static int STATUS_FAILED= 2;

    public int mType;
    public int mId;
    public int mStatus = STATUS_OK;

    public String mName;
    public boolean mBoolValue;
    public int mIntValue;


    public RoomWidget() {

    }

    public RoomWidget(int type, int id) {
        mType = type;
        mId = id;
    }

    public RoomWidget(int type, int id, String name, boolean value) {
        mType = type;
        mId = id;
        mName = name;
        mBoolValue = value;
    }

    public RoomWidget(int type, int id, String name, int value) {
        mType = type;
        mId = id;
        mName = name;
        mIntValue = value;
    }

    public void updateValues(RoomWidget widget) {
        this.mBoolValue = widget.mBoolValue;
        this.mIntValue = widget.mIntValue;
        this.mStatus = widget.mStatus;
        this.mType = widget.mType;
        this.mName = widget.mName;
    }

}
