#pragma once
#include <cstdlib>

struct Credentials {
    char *ssid;
    char *password;
    char *username;
    char *web_password;

    //Destructor
    ~Credentials() {
        free(ssid);
        free(password);
        free(username);
        free(web_password);
    }
};

//AccessPoint get_access_point();
Credentials get_credentials();
