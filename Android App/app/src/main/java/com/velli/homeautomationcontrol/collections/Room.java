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

import java.util.LinkedHashMap;


public class Room {
    private LinkedHashMap<Integer, RoomWidget> mRoomWidgets;
    private String mRoomName;
    private int mId;

    public Room(int id, String name, LinkedHashMap<Integer, RoomWidget> widgets) {
        mId = id;
        mRoomName = name;
        mRoomWidgets = widgets;
    }

    public void setRoomWidgets(LinkedHashMap<Integer, RoomWidget> widgets) {
        mRoomWidgets = widgets;
    }

    public void addWidget(int id, RoomWidget widget) {
        if(mRoomWidgets == null) {
            mRoomWidgets = new LinkedHashMap<>();
        }
        mRoomWidgets.put(id, widget);
    }

    public LinkedHashMap<Integer, RoomWidget> getRoomWidgets() {
        return mRoomWidgets;
    }

    public String getRoomName() {
        return mRoomName;
    }

    public int getRoomId() {
        return mId;
    }
}
