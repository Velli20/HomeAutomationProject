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

import android.util.Xml;

import com.velli.homeautomationcontrol.collections.RoomWidget;

import org.xmlpull.v1.XmlSerializer;

import java.io.IOException;
import java.io.StringWriter;
import java.util.LinkedHashMap;

import com.velli.homeautomationcontrol.collections.Room;


public class RoomWidgetWriter {
    private static final String namespace = "";

    public byte[] writeXml(LinkedHashMap<Integer, Room> widgets) {
        XmlSerializer serializer = Xml.newSerializer();
        StringWriter writer = new StringWriter();
        try {
            serializer.setOutput(writer);
            serializer.startDocument("UTF-8", true);
            serializer.startTag(namespace, Constants.XML_TAG_DATA);

            for(Room room : widgets.values()) {
                writeRoom(room, serializer);
            }

            serializer.endTag(namespace, Constants.XML_TAG_DATA);
            serializer.endDocument();
        } catch (Exception ignored) {}

        return writer.toString().getBytes();
    }

    public byte[] writeXml(Room room) {
        XmlSerializer serializer = Xml.newSerializer();
        StringWriter writer = new StringWriter();
        try {
            serializer.setOutput(writer);
            serializer.startDocument("UTF-8", true);
            serializer.startTag(namespace, Constants.XML_TAG_DATA);

            writeRoom(room, serializer);

            serializer.endTag(namespace, Constants.XML_TAG_DATA);
            serializer.endDocument();
        } catch (Exception ignored) {}

        return writer.toString().getBytes();
    }

    public byte[] writeXml(int roomId, RoomWidget widget) {
        XmlSerializer serializer = Xml.newSerializer();
        StringWriter writer = new StringWriter();
        try {
            serializer.setOutput(writer);
            serializer.startDocument("UTF-8", true);
            serializer.startTag(namespace, Constants.XML_TAG_DATA);
            serializer.startTag(namespace, Constants.XML_TAG_ROOM);
            serializer.attribute(namespace, Constants.XML_TAG_ID, String.valueOf(roomId));

            writeWidget(widget, serializer);

            serializer.endTag(namespace, Constants.XML_TAG_ROOM);
            serializer.endTag(namespace, Constants.XML_TAG_DATA);
            serializer.endDocument();
        } catch (Exception ignored) {}
        writer.append("\r");
        return writer.toString().getBytes();
    }

    private void writeRoom(Room room, XmlSerializer serializer) throws IOException {
        serializer.startTag(namespace, Constants.XML_TAG_ROOM);
        serializer.attribute(namespace, Constants.XML_TAG_NAME, room.getRoomName());
        serializer.attribute(namespace, Constants.XML_TAG_ID, String.valueOf(room.getRoomId()));

        for(RoomWidget widget : room.getRoomWidgets().values()) {
            writeWidget(widget, serializer);
        }
        serializer.endTag(namespace, Constants.XML_TAG_ROOM);
    }

    private void writeWidget(RoomWidget widget, XmlSerializer serializer) throws IOException{
        serializer.startTag(namespace, Constants.XML_TAG_WIDGET);

        //ID
        serializer.startTag(namespace, Constants.XML_TAG_ID);
        serializer.text(String.valueOf(widget.mId));
        serializer.endTag(namespace, Constants.XML_TAG_ID);

        //Type
        serializer.startTag(namespace, Constants.XML_TAG_TYPE);
        serializer.text(String.valueOf(widget.mType));
        serializer.endTag(namespace, Constants.XML_TAG_TYPE);

        //Name
        if(widget.mName != null) {
            serializer.startTag(namespace, Constants.XML_TAG_NAME);
            serializer.text(String.valueOf(widget.mName));
            serializer.endTag(namespace, Constants.XML_TAG_NAME);
        }

        //Integer value
        serializer.startTag(namespace, Constants.XML_TAG_INT_VALUE);
        serializer.text(String.valueOf(widget.mIntValue));
        serializer.endTag(namespace, Constants.XML_TAG_INT_VALUE);

        //Bool value
        serializer.startTag(namespace, Constants.XML_TAG_BOOL_VALUE);
        serializer.text(String.valueOf(widget.mBoolValue));
        serializer.endTag(namespace, Constants.XML_TAG_BOOL_VALUE);

        //Status
        serializer.startTag(namespace, Constants.XML_TAG_STATUS);
        serializer.text(String.valueOf(widget.mStatus));
        serializer.endTag(namespace, Constants.XML_TAG_STATUS);

        serializer.endTag(namespace, Constants.XML_TAG_WIDGET);
    }
}
