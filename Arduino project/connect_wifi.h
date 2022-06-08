#pragma once

const char *connect_wifi(char *ssid, char *password);
//
//Arduino: 1.8.19 (Windows 10), Board: "ESP32 Dev Module, Disabled, Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS), 240MHz (WiFi/BT), QIO, 80MHz, 4MB (32Mb), 921600, Core 1, Core 1, None"
//
//c:/program files (x86)/arduino/hardware/espressif/esp32/tools/xtensa-esp32-elf/bin/../lib/gcc/xtensa-esp32-elf/8.4.0/../../../../xtensa-esp32-elf/bin/ld.exe: sketch\get_credentials.cpp.o:C:\Users\gmora\Documents\Arduino\pbltest/get_credentials.cpp:7: multiple definition of `server'; sketch\connect_wifi.cpp.o:C:\Users\gmora\Documents\Arduino\pbltest/connect_wifi.cpp:7: first defined here
//collect2.exe: error: ld returned 1 exit status
//exit status 1
//Error compiling for board ESP32 Dev Module.
//
//
//This report would have more information with
//"Show verbose output during compilation"
//option enabled in File -> Preferences.
