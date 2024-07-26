#ifndef WifiNetworking_h
#define WifiNetworking_h

#include <WiFi.h>
#include <ArduinoJson.h>

#include <BusinessState.h>
// #include "AsyncJson.h"
// #include "ArduinoJson.h"
// #include <LittleFS.h>

class WifiNetworking {

    public:
        WiFiClass * wifi;
        DynamicJsonDocument * available_networks = nullptr;

        BusinessState * business_state = nullptr;

        WifiNetworking();

        void scanDebug();
        void scan();


        DynamicJsonDocument * getAvailableNetworks();

        void startAP();
        bool connect(String ssid, String password);
        bool isConnected();
        void disconnect();

        void injectBusinessState(BusinessState * business_state);

};

#endif
