# HomeAutomationProject

The main goal of this project is to create a flexible home automation system that is easy to configure and can be controlled remotely via WiFi/Bluetooth or Ethernet TCP/IP connection. 

Things that are currently working:

- Stable temperature data logging on µSD card
- Reading configuration xml file from µSD card and UI creation based on that configuration file
- Remote controlling over bluetooth connection with Android app
- General UI

<img src="https://github.com/Velli20/HomeAutomationProject/blob/master/UI%20pictures/STM32F746G-DISCO_MENU.jpg?raw=true" width="400" >


## Example of configuration file

```XML
<?xml version="1.0" encoding="utf-8"?>
<RoomConfiguration>
    <!-- -- Room and widget id must be unique <!-- -->
    <Room name="Living room" id="1">

        <RoomWidget>
            <id type="text">11</id>         <!-- -- Widget id<!-- -->
            <type type="text">0</type>      <!-- -- Widget type 0 = lights 1 = temperature controller 2 = door lock <!-- -->
            <name type="text">Living room lights</name> <!-- -- Name of widget that will be displayed in UI <!-- -->
            <pin type="text">3</pin>        <!-- -- GPIO pin where device is connected on board <!-- -->
        </RoomWidget>

        <RoomWidget>
            <id type="text">12</id>
            <type type="text">1</type>
            <name type="text">Living room temperature</name>
            <pin  type="text">4</pin>
        </RoomWidget>
    </Room>

    <Room name="Bedroom" id="2">
        <RoomWidget>
            <id type="text">21</id>
            <type type="text">0</type>
            <name type="text">Bedroom lights</name>
            <pin type="text">9</pin>
        </RoomWidget>
        <RoomWidget>
            <id type="text">22</id>
            <type type="text">1</type>
            <name type="text">Bedroom temperature</name>
            <pin type="text">5</pin>
        </RoomWidget>
    </Room>
</RoomConfiguration>
```
## Pictures of the project

<img src="https://github.com/Velli20/HomeAutomationProject/blob/master/UI%20pictures/IMG_20170410_062019.jpg?raw=true" width="400" >
<img src="https://github.com/Velli20/HomeAutomationProject/blob/master/UI%20pictures/IMG_20170410_062028.jpg?raw=true" width="400" >
<img src="https://github.com/Velli20/HomeAutomationProject/blob/master/UI%20pictures/IMG_20170410_062036.jpg?raw=true" width="400" >
<img src="https://github.com/Velli20/HomeAutomationProject/blob/master/UI%20pictures/IMG_20170410_062048.jpg?raw=true" width="400" >
<img src="https://github.com/Velli20/HomeAutomationProject/blob/master/UI%20pictures/IMG_20170410_063949.jpg?raw=true" width="400" >
<img src="https://github.com/Velli20/HomeAutomationProject/blob/master/UI%20pictures/device-2017-04-10-062618.png?raw=true" width="400" >
