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


import android.util.Log;
import android.util.Xml;

import com.velli.homeautomationcontrol.collections.RoomWidget;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import java.io.IOException;
import java.io.InputStream;
import java.util.LinkedHashMap;

import com.velli.homeautomationcontrol.collections.Room;

public class RoomWidgetParser {
    private static final String Tag = "RoomWidgetParser ";
    private static final String nameSpace = null;

    public LinkedHashMap<Integer, Room> parse(InputStream in) throws XmlPullParserException, IOException {
        Log.i(Tag, Tag + "parse()");
        try {
            XmlPullParser parser = Xml.newPullParser();
            parser.setFeature(XmlPullParser.FEATURE_PROCESS_NAMESPACES, false);
            parser.setInput(in, null);
            parser.nextTag();
            return readData(parser);
        } finally {
            in.close();
        }
    }

    private LinkedHashMap<Integer, Room> readData(XmlPullParser parser) throws XmlPullParserException, IOException {
        LinkedHashMap<Integer, Room> entries = new LinkedHashMap();
        parser.require(XmlPullParser.START_TAG, nameSpace, "Data");

        while (parser.next() != XmlPullParser.END_TAG) {
            if (parser.getEventType() != XmlPullParser.START_TAG) {
                continue;
            }
            String tag = parser.getName();

            Log.i(Tag, Tag + "readRoom() tag: " + tag);
            if(tag.equals("Room")) {
                String roomName = parser.getAttributeValue(nameSpace, "name");
                int id = Integer.parseInt(parser.getAttributeValue(nameSpace, "id"));

                LinkedHashMap<Integer, RoomWidget> w = readRoom(parser);
                entries.put(id, new Room(id, roomName, w));
            } else {
                skip(parser);
            }
        }
        return entries;
    }

    private LinkedHashMap<Integer, RoomWidget> readRoom(XmlPullParser parser) throws XmlPullParserException, IOException {
        Log.i(Tag, Tag + "readRoom()");
        LinkedHashMap<Integer, RoomWidget> entries = new LinkedHashMap();

        parser.require(XmlPullParser.START_TAG, nameSpace, "Room");
        while (parser.next() != XmlPullParser.END_TAG) {
            if (parser.getEventType() != XmlPullParser.START_TAG) {
                continue;
            }
            String name = parser.getName();

            Log.i(Tag, Tag + "readRoom() tag: " + name);
            if(name.equals("RoomWidget")) {
                RoomWidget w = readWidget(parser);
                entries.put(w.mId, w);
            } else {
                skip(parser);
            }
        }
        return entries;
    }

    private RoomWidget readWidget (XmlPullParser parser) throws XmlPullParserException, IOException {
        parser.require(XmlPullParser.START_TAG, nameSpace, "RoomWidget");
        RoomWidget widget = new RoomWidget();

        while (parser.next() != XmlPullParser.END_TAG) {
            if (parser.getEventType() != XmlPullParser.START_TAG) {
                continue;
            }
            String name = parser.getName();

            if (name.equals("id")) {
                widget.mId = readInt(parser, "id");
            } else if(name.equals("type")) {
                widget.mType = readInt(parser, "type");
            } else if(name.equals("name")) {
                widget.mName = readString(parser, "name");
            } else if(name.equals("intValue")) {
                widget.mIntValue = readInt(parser, "intValue");
            } else if(name.equals("boolValue")) {
                widget.mBoolValue = readBool(parser, "boolValue");
            } else if(name.equals("status")) {
                widget.mStatus = readInt(parser, "status");
            } else {
                skip(parser);
            }
        }

        return widget;
    }

    private int readInt(XmlPullParser parser, String tag) throws IOException, XmlPullParserException {
        parser.require(XmlPullParser.START_TAG, nameSpace, tag);
        int id = Integer.parseInt(readText(parser));
        parser.require(XmlPullParser.END_TAG, nameSpace, tag);
        return id;
    }

    private boolean readBool(XmlPullParser parser, String tag) throws IOException, XmlPullParserException {
        parser.require(XmlPullParser.START_TAG, nameSpace, tag);
        boolean bool = Boolean.parseBoolean(readText(parser));
        parser.require(XmlPullParser.END_TAG, nameSpace, tag);
        return bool;
    }

    private String readString(XmlPullParser parser, String tag) throws IOException, XmlPullParserException {
        parser.require(XmlPullParser.START_TAG, nameSpace, tag);
        String summary = readText(parser);
        parser.require(XmlPullParser.END_TAG, nameSpace, tag);
        return summary;
    }

    private String readText(XmlPullParser parser) throws IOException, XmlPullParserException {
        String result = "";
        if (parser.next() == XmlPullParser.TEXT) {
            result = parser.getText();
            parser.nextTag();
        }
        return result;
    }


    private void skip(XmlPullParser parser) throws XmlPullParserException, IOException {
        if (parser.getEventType() != XmlPullParser.START_TAG) {
            throw new IllegalStateException();
        }
        int depth = 1;
        while (depth != 0) {
            switch (parser.next()) {
                case XmlPullParser.END_TAG:
                    depth--;
                    break;
                case XmlPullParser.START_TAG:
                    depth++;
                    break;
            }
        }
    }
}
