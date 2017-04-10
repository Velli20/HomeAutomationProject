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

import java.util.UUID;


public class Constants {
    // Unique UUID for this application
    public static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    // Message types sent from the BluetoothService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;

    // Constants that indicate the current connection state
    public static final int STATE_NONE = 0;       // we're doing nothing
    public static final int STATE_LISTEN = 1;     // now listening for incoming connections
    public static final int STATE_CONNECTING = 2; // now initiating an outgoing connection
    public static final int STATE_CONNECTED = 3;  // now connected to a remote device
    public static final int STATE_CONNECTION_FAILED = 4;

    public static final String DEVICE_NAME = "Device name";

    public static final String COMMAND_GET_ROOM_CONFIGURATION = "#<GetRoomConfiguration/>*";


    //XML tags
    public static final String XML_TAG_ROOM_CONFIGURATION = "RoomConfiguration";
    public static final String XML_TAG_ROOM_CONFIGURATION_UPDATE = "RoomConfigurationUpdate";
    public static final String XML_TAG_ROOM = "Room";
    public static final String XML_TAG_WIDGET = "RoomWidget";
    public static final String XML_TAG_ID = "id";
    public static final String XML_TAG_TYPE = "type";
    public static final String XML_TAG_NAME = "name";
    public static final String XML_TAG_INT_VALUE = "intValue";
    public static final String XML_TAG_INT_TARGET_VALUE = "intTargetValue";
    public static final String XML_TAG_BOOL_VALUE = "boolValue";
    public static final String XML_TAG_STATUS = "status";

    public static final String XML_ATTRIBUTE_REQUEST_CODE = "requestCode";
    public static final String XML_ATTRIBUTE_ROOM_ID = "id";
    public static final String XML_ATTRIBUTE_ROOM_NAME= "name";

    /* Response tags */
    public static final String XML_TAG_RESPONSE_COMMAND_UNKNOWN = "UnknownCommand";
    public static final String XML_TAG_RESPONSE_COMMAND_OK = "OK";
    public static final String XML_TAG_RESPONSE_COMMAND_INTERNAL_ERROR = "DeviceInternalError";


    public static final int MAX_TARGET_TEMPERATURE_CELSIUS = 30;
    public static final int MIN_TARGET_TEMPERATURE_CELSIUS = 2;

    public static final char SERIAL_START_FLAG = '#';
    public static final char SERIAL_END_FLAG = '*';
}
