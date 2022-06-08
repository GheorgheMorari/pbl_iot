#include "login.h"
#include "wifi_header.h"
#include <HTTPClient.h>

const char *login(char *username, char *web_password, std::string &body) {
// Perform a get request with username and web_password parameters to "http://gmorari404.pythonanywhere.com/login"
//    WiFiClient client;

    HTTPClient client;
    std::string full_url =
            "http://gmorari404.pythonanywhere.com/attach/?username=" + std::string(username) + "&password=" +
            std::string(web_password);
    full_url[full_url.length()] = '\0';

    client.begin(full_url.c_str());

    int httpCode = client.GET();
    String local_body;
    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
            local_body = client.getString();
            local_body[local_body.length()] = '\0';
            body = std::string(local_body.c_str());
            return "OK";
        } else {
            return "Invalid credentials";
        }
    }

    return "Invalid credentials";
}