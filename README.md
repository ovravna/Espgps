## Intro
This project is designed to compile on an ESP32.

## PlatformIO
The project is built using [PlatformIO](http://docs.platformio.org/en/latest/platforms/espressif32.html). This allow the code to run on multiple devices and supports both Arduino and native development with ESP-IDF in C. I have choosen to use the Arduino framework for multiple reasons including ease of use, faster development time, library and general support and [cross-compatibility][2]. 
PlatformIO uses normal `.cpp`  files instead of `.ino` files native to the Arduino platfrom, this allows for fully featured usage of the C++ language.  

## Structure
In `src` you will mainly find entry point files identifiable as `main_*.cpp`.
> Note: Only one entry_point file can be compiled at a time, therefore be sure to use `src_filters` in `platformio.ini` to exclude all non-relevant main files.

In `lib` you will find both project code and third-party libraries. The project code is identifiable by only having `.h` and `.cpp` files in each folder and by being separated into folders based on concern.  

## Install, build and upload 
Start by installing PlatformIO. You can install only a CLI, but the simplest approach is to install the VSCode based IDE, read more [here][3]. 

Now [clone][1] the project from https://github.com/ovravna/Espgps.git and test that it builds with `ctrl`+`alt`+`b`. 

All went well if compilation ends with [<span style="color:green">**SUCCESS**</span>].

### Upload
By default PlatformIO will try to upload to any compatible device connected with USB serial. Because of different builds with different entry points this is not practical in this project. Build parameters and port selection is specified in the `platformio.ini` file. 
Select a serial port for each build by changing the `upload_port` parameter in both the `env:esp32client` and `env:esp32server` environments. 

Read more [here][4].

Now you should have a fully running enviorment running and uploading to the ESP32.
## Connect peripheries
The project relies on a [Iridium 9603 transceiver][5] and a GPS-module. Connection must be as follows:

Pin | Wire
---|---
GIO32 | 9603 RXD
GIO33 | 9603 TXD
GIO25 | GPS TX
GIO26 | GPS RX
GIO27 | -
GIO14 | SIGNAL LED

> ESP32 pinout can be found [here][esp32breakout].
> RockBLOCK 9603 pinout [here][9603breakout].


## Other notes
### Antennas
When developing a working antenna is essential, both the GPS and the Iridium chip should have its own external antenna designed for its bandwidth and it should be placed in line-of-sight to the satellites e.i. outdoors. 
### BLE
The code supports BLE (Bluetooth Low Energy), however a stable connection between devices while controlling the peripheries proved to be a difficult task. Thus it need more development before being stable enough for usage.

[1]:https://code.visualstudio.com/docs/editor/versioncontrol#_cloning-a-repository
[2]:http://docs.platformio.org/en/latest/frameworks/arduino.html#platforms
[3]:https://platformio.org/install/ide?install=vscode
[4]:http://docs.platformio.org/en/latest/projectconf/section_env_upload.html#upload-port
[5]:https://www.iridium.com/products/iridium-9603-3/
[esp32breakout]:https://cdn.instructables.com/FKZ/5Y5T/JFIIR5XE/FKZ5Y5TJFIIR5XE.LARGE.jpg
[9603breakout]:https://docs.rockblock.rock7.com/docs/connectors
