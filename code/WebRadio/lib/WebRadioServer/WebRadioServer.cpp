#include "WebRadioServer.h"

struct TaskTwoParameters {
  void * param_1;
  void * param_2;
};


WebRadioServer::WebRadioServer() {
}


void WebRadioServer::begin() {
    this->server->begin();
}

void WebRadioServer::init() {

  this->server = new AsyncWebServer(port);

  LittleFS.begin();
  

  this->server->onNotFound(
    [](AsyncWebServerRequest *request){
      request->send(404, "text/html", "Not found");
    }
  );

  this->server->on(
    "/api/ping",
    HTTP_GET,
    [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", "Ok");
    }
  );


  this->server->on(
    "/api/available-networks",
    HTTP_GET,
    [&](AsyncWebServerRequest *request) {

      TaskTwoParameters * task_two_parameters = new TaskTwoParameters();
      task_two_parameters->param_1 = (void*)request;
      task_two_parameters->param_2 = (void*)this;

      xTaskCreate(
        [](void *arg){
          {
            TaskTwoParameters* params = (TaskTwoParameters*)arg;
            AsyncWebServerRequest * request = (AsyncWebServerRequest *)params->param_1;
            WebRadioServer * web_radio_server = (WebRadioServer *)params->param_2;

            request->client()->setRxTimeout(50);
            String response;
            web_radio_server->wifi_networking->scan();
            DynamicJsonDocument * available_networks = web_radio_server->wifi_networking->getAvailableNetworks();
            serializeJson(*available_networks, response);

            request->send(200, "application/json", response);

            delete params;
          }
          vTaskDelete(NULL);
        },
        "handler",
        5000,
        (void*)task_two_parameters,
        1,
        NULL
      );
    }
  );


  this->server->on(
    "/api/credentials",
    HTTP_GET,
    [&](AsyncWebServerRequest *request) {
      String response;
      DynamicJsonDocument * network_credential_list = this->network_credential->network_credential_list;
      serializeJson(*network_credential_list, response);
      request->send(200, "application/json", response);
    }
  );




  this->server->on(
    "/api/credentials",
    HTTP_POST,
    [&](AsyncWebServerRequest *request) {

      String ssid = "";
      String password = "";
      if (request->hasParam("ssid", true)) {
        ssid = request->getParam("ssid", true)->value();
      }

      if (request->hasParam("password", true)) {
        password = request->getParam("password", true)->value();
      }

      Serial.print("Post ssid : ");
      Serial.println(ssid);
      Serial.print("password : ");
      Serial.println(password);

      request->send(201, "text/html", "OK");
    }
  );



  this->server->on(
    "/api/credentials",
    HTTP_DELETE,
    [&](AsyncWebServerRequest *request) {

      String ssid = "";
      if (request->hasParam("ssid", true)) {
        ssid = request->getParam("ssid", true)->value();
      }

      Serial.print("Delete ssid : ");
      Serial.println(ssid);
      request->send(200, "text/html", "OK");
    }
  );


  this->server->serveStatic("/", LittleFS, "/webinterface").setDefaultFile("index.html");
  this->server->serveStatic("/api/streams.json", LittleFS, "/streams.json");

}


void WebRadioServer::injectWifiNetworking(WifiNetworking * wifi_networking) {
    this->wifi_networking = wifi_networking;
}

void WebRadioServer::injectNetworkCredential(NetworkCredential * network_credential) {
    this->network_credential = network_credential;
}
