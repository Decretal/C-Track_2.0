#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>

#define SSID    "Htspt"
#define KEY     "f2z56k32tv3kwzx"

#define SERVER_IP "192.168.225.152"
#define SERVER_PORT 8000


WiFiClient client;


JsonDocument data;
int id;
String url;

void setup()
{
    pinMode(38, INPUT_PULLUP);
    Serial.begin(115200);

    WiFi.begin(SSID, KEY);

    while (WiFi.status() != WL_CONNECTED);
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to WiFi");
    }

    data["date"] = "2001-03-16 12:25:45";
    data["latitude"] = String((float) random(1000, 9999) / 100);
    data["longitude"] = String((float) random(1000, 9999) / 100);
    
    id = 1;
    url = "http://192.168.21.152:" + String(SERVER_PORT) + "/api/trackers/" + String(id) + "/positions";
}


void loop()
{
    if (digitalRead(38) == LOW)
    {
        Serial.println("Daten Senden:");


        HTTPClient http;

        data["latitude"] = String((float) random(1000, 9999) / 100);
        data["longitude"] = String((float) random(1000, 9999) / 100);

        String POST_DATA;
        serializeJson(data, POST_DATA);

        if (http.begin(url)) {
            http.addHeader("Content-Type", "application/json");

            int httpResponseCode = http.POST(POST_DATA);

            if (httpResponseCode > 0) {
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);

                String response = http.getString();
                Serial.println(response);
            } else {
                Serial.print("Error in HTTP request. Fehlercode: ");
                Serial.println(httpResponseCode);
            }

            http.end();
        }

        while (digitalRead(38) == LOW);
    }
}