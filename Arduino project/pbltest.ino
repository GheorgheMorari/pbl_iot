#include <cstring>
#include "get_credentials.h"
#include "connect_wifi.h"
#include "login.h"
#include <string>
#include "wifi_header.h"
#include <MQTTPubSubClient.h>

MQTTPubSubClient mqtt;
WiFiClient client;
int i;
std::string body;
int temperature_value = 20;

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("Starting...");
    const char *wifi_status = "";
    const char *login_status = "";
    while ((strcmp(wifi_status, "OK") != 0) && (strcmp(login_status, "OK") != 0)) {
        Serial.println("Getting credentials");
        auto credentials = get_credentials();

        Serial.println("Connecting to wifi");
        wifi_status = connect_wifi(credentials.ssid, credentials.password);
        Serial.println("Logging in");
        login_status = login(credentials.username, credentials.web_password, body);

        Serial.println("Wifi status: ");
        Serial.println(wifi_status);
        Serial.println("Login status: ");
        Serial.println(login_status);
        Serial.println("Body: ");
        Serial.println(body.c_str());

        client.connect("node02.myqtthub.com", 1883);
        mqtt.begin(client);
        mqtt.connect("pbl_device", "pbl", "13243546q"); // client_id, username, password

        std::string unique_name_control = body + "/control";
        unique_name_control[unique_name_control.length()] = '\0';

        mqtt.subscribe(unique_name_control.c_str(),
                       [](const String &payload, const size_t size) {
                           Serial.println("Received message:");
                           Serial.println(payload);
                           int int_payload = std::stoi(std::string(payload.c_str()));
                           temperature_value += int_payload;
                       });

    }
    Serial.println("Done");
}

void loop() {
    mqtt.update();
    //Send status every 3 seconds
    i++;
    delay(1000);
    if (i % 3 == 0) {
        std::string unique_name_status = body + "/data";
        unique_name_status[unique_name_status.length()] = '\0';
        mqtt.publish(unique_name_status.c_str(), std::to_string(temperature_value).c_str());
    }
}
