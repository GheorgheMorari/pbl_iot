#include "wifi_header.h"
#include "connect_wifi.h"
#include <cstring>


const char *connect_wifi(char *ssid, char *password) {
    WiFi.begin(ssid, password);

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        tries++;
        if (tries > 10) {
            Serial.println("Connection timed out");
            return "Invalid credentials";
        }
    }
    return "OK";
}