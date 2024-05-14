#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LoRa.h>
#include <sstream>
#include <SPI.h>

#define SSID    "Htspt"
#define KEY     "f2z56k32tv3kwzx"

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26


WiFiClient client;


JsonDocument data;
String url;

typedef struct Point
{
    byte id;
	double lat, lng;

	byte day, month, year;
	byte hour, minutes, seconds;
} Point;


HTTPClient http;
Point* position;


bool parseString(const char *str, struct Point *position) {
    char *token;
    
    // Parse id
    token = strtok((char *)str, " ");
    if (token == NULL) return false;
    position->id = atoi(token);
    
    // Parse hour
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->hour = atoi(token);
    
    // Parse minutes
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->minutes = atoi(token);
    
    // Parse seconds
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->seconds = atoi(token);
    
    // Parse day
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->day = atoi(token);
    
    // Parse month
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->month = atoi(token);
    
    // Parse year
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->year = atoi(token);
    
    // Parse latitude
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->lat = atof(token) / 100000.0;
    
    // Parse longitude
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->lng = atof(token) / 100000.0;
    
    return true;
}

void setup()
{
    Serial.begin(115200);

    position = new Point;

    WiFi.begin(SSID, KEY);

    while (WiFi.status() != WL_CONNECTED);
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to WiFi");
    }

    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(868.3E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    LoRa.setSpreadingFactor(7);
    LoRa.setTxPower(0);
    LoRa.setSignalBandwidth(125e3);
    //LoRa.setCodingRate4(8);
}

void loop()
{
    int packetSize = LoRa.parsePacket();

    if (packetSize) {

        String rec = "";
        while (LoRa.available()) {
            rec += (char)LoRa.read();
        }

        // print RSSI of packet
        Serial.print(rec);
        Serial.print(" with RSSI ");
        Serial.println(LoRa.packetRssi());

        long double lat, lng;


        if (!parseString(rec.c_str(), position)) return;


        data["latitude"] = position->lat;
        data["longitude"] = position->lng;

        char phpDateTime[20];
        sprintf(phpDateTime, "20%02d-%02d-%02d %02d:%02d:%02d", position->year, position->month, position->day, position->hour, position->minutes, position->seconds);

        data["date"] = String(phpDateTime);

        serializeJsonPretty(data, Serial);
        Serial.println("\n\n");

        url = "http://fast-woodland-47484-eef5313e6f25.herokuapp.com/api/trackers/" + String(position->id) + "/positions";

        Serial.println(url);

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
    }
}