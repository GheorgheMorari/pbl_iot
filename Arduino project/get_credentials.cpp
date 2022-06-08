#include "wifi_header.h"
#include "get_credentials.h"
#include <cstring>
#include <string>

// Set web server port number to 80
WiFiServer server(80);

// Replace with your network credentials
const char *ssid = "SmartShowerConfig";
const char *password = "";

// Variable to store the HTTP request
std::string http_request;
enum class Status {
    REQUEST, CONTENT_LENGTH, EMPTY_LINE, BODY
};
const char index_html[] = R"(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/submit" method="GET">
    <h2> Internet credentials </h2>
    SSID: <input type="text" name="ssid">
    PASSWORD: <input type="password" name="password">
    <h2> Website credentials </h2>
    USERNAME: <input type="text" name="username">
    PASSWORD: <input type="password" name="web_password">
    <input type="submit" value="Submit">
  </form>
</body></html>)";

Credentials get_credentials() {
    Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();
    Serial.println("Server begin.");
    while (true) {
        WiFiClient client = server.available();
        if (!client) {
            continue;
        }
        String currentLine;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
//                    Serial.write(c);
                http_request += c;
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println(index_html);
                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
//        Serial.println(http_request.c_str());
        auto index_of_submission = http_request.find("/submit?", 0);
        if (index_of_submission != -1) {
            index_of_submission += 8;
            auto ssid_start = strstr(http_request.c_str(), "ssid=") + 5;
            auto ssid_end = strstr(ssid_start, "&");
            auto ssid = (char *) malloc(ssid_end - ssid_start + 1);
            strncpy(ssid, ssid_start, ssid_end - ssid_start);
            ssid[ssid_end - ssid_start] = '\0';

            auto password_start = ssid_end + 10;
            auto password_end = strstr(ssid_end + 11, "&");
            auto password = (char *) malloc(password_end - password_start + 1);
            strncpy(password, password_start, password_end - password_start);
            password[password_end - password_start] = '\0';

            auto username_start = password_end + 10;
            auto username_end = strstr(password_end + 11, "&");
            auto username = (char *) malloc(username_end - username_start + 1);
            strncpy(username, username_start, username_end - username_start);
            username[username_end - username_start] = '\0';

            auto web_password_start = username_end + 14;
            auto web_password_end = strstr(username_end + 14, "HTTP") - 1;
            auto web_password = (char *) malloc(web_password_end - web_password_start + 1);
            strncpy(web_password, web_password_start, web_password_end - web_password_start);
            web_password[web_password_end - web_password_start] = '\0';
            client.stop();

            //Kill the server
            server.stop();

            //Kill the Wi-Fi get_credentials point
            WiFi.softAPdisconnect(true);
            return {ssid, password, username, web_password};

        }
//      Clear the header variable
        http_request = "";
//      Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}




