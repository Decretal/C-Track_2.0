#include <Arduino.h>
#include <WiFi.h>
#include <string.h>
#include <iostream> // std::cout
#include <string>
#include <cstring>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include "Socket.h"
#include "html_page.h"
#include "Receiver.h"

using namespace std;
AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");
String jsonString = "";

// const char *ssid = "FORTI_INTERNET_2G4";
// const char *password = "HTLInternet";

const char *ssid = "Htspt";
const char *password = "f2z56k32tv3kwzx";

//const char *ssid = "cablelink_0792054";
//const char *password = "4fxvxXP!++YS8WAh";

//const char *ssid = "TP-Link_4C54";
//const char *password = "06630539";

String LoRaData = Receiver::getLoRaData();
//String ID;

StaticJsonDocument<200> doc;

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}
void onEvent(AsyncWebSocket *server,
             AsyncWebSocketClient *client,
             AwsEventType type,
             void *arg,
             uint8_t *data,
             size_t len)
{

    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void Socket::start_socket()
{

    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", webpageCode); });

    server.onNotFound(notFound);
    server.begin();

    webSocket.onEvent(onEvent);
    server.addHandler(&webSocket);
}

// Funktion um json Werte zu "send_data" bekommen
void update_webpage(String LoRaData) 
{
    webSocket.cleanupClients();

    JsonObject object = doc.to<JsonObject>();


    Serial.println(LoRaData);
    //object["LoRaData"] = LoRaData;
    object["LoRaData_received"] = String(LoRaData);


    serializeJson(object, jsonString);

    // Serial.print("Send: \n");
    Serial.println(jsonString);

    webSocket.textAll(jsonString); // send the JSON object through the websocket
    jsonString = ""; 
}

// Sende Daten zu main.cpp
void Socket::send_data()
{

   LoRaData = Receiver::getLoRaData();
   update_webpage(LoRaData);
   //delay(500);
   
}