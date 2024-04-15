#include <Arduino.h>
#include <string>
#include <set>
#include <map>
#include <SPI.h>
#include <LoRa.h>
#include <SparkFun_Ublox_Arduino_Library.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Preferences.h>
#include <iostream>
#include <queue>
#include <esp_sleep.h>

// FIFO-Speicher für die GPS-Daten
std::deque<String> gpsDataQueue;

//std::map<String, String> gpsData;
//std::set<String> gpsFields {"id", "date", "time", "latitude", "longitude"};

#define SERIAL1_RX 34 // GPS_TX -> 34
#define SERIAL1_TX 12 // 12 -> GPS_RX

// Frequenzbereich für Europa
#define BAND 866E6

SFE_UBLOX_GPS NEO6GPS;

// Pins, an denen das LoRa-Modul am ESP32 angeschlossen ist
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

String read_sentence;


void sleepForFiveMinutes() {
  // Deep Sleep für 5 Minuten
  esp_sleep_enable_timer_wakeup(300000000); //300000000us
  esp_deep_sleep_start();
}

void setup() {

  // Seriellen Monitor starten
  Serial.begin(115200);
  delay(2000);
  Serial1.begin(9600, SERIAL_8N1, SERIAL1_RX, SERIAL1_TX);

  // Pins für LoRa definieren und Kommunikation starten
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  while (!Serial);
  Serial1.begin(9600, SERIAL_8N1, SERIAL1_RX, SERIAL1_TX);
  delay(300);
  
  if (!LoRa.begin(BAND)) {
      Serial.println("Connection failed");
  }
  do {
    if (NEO6GPS.begin(Serial1)) {
      Serial.println("Connection is OK");
      NEO6GPS.setUART1Output(COM_TYPE_NMEA);
      NEO6GPS.saveConfiguration();
      NEO6GPS.disableNMEAMessage(UBX_NMEA_GLL, COM_PORT_UART1);
      NEO6GPS.disableNMEAMessage(UBX_NMEA_GSA, COM_PORT_UART1);
      NEO6GPS.disableNMEAMessage(UBX_NMEA_GSV, COM_PORT_UART1);
      NEO6GPS.disableNMEAMessage(UBX_NMEA_VTG, COM_PORT_UART1);
      NEO6GPS.enableNMEAMessage(UBX_NMEA_RMC, COM_PORT_UART1); // benötigtes Paket
      NEO6GPS.disableNMEAMessage(UBX_NMEA_GGA, COM_PORT_UART1);
      NEO6GPS.saveConfiguration();
      break;
    }
    delay(1000);
  } while(1);
}

// Seperieren der Daten
String sentence_sep(String input, int index) {
  int finder = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = input.length() - 1;

  for (int i = 0; i <= maxIndex && finder <= index; i++) {
    if (input.charAt(i) == ',' || i == maxIndex) {  //',' = separator
      finder++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  
  return finder > index ? input.substring(strIndex[0], strIndex[1]) : "";
}

void loop() {
 String read_sentence = Serial1.readStringUntil(13); //13 = return (ASCII)
  read_sentence.trim();

    // Daten in String umwandeln
  if (Serial1.available()) {
    String gps_ID   = sentence_sep(read_sentence, 0); //ID
    String gps_Time = sentence_sep(read_sentence, 1); //Time
    String gps_lat  = sentence_sep(read_sentence, 3); //Latitude
    String gps_long = sentence_sep(read_sentence, 5); //Longitude
    String gps_date = sentence_sep(read_sentence, 9); //Date

    // GPS-Daten in einem JSON-String zusammenfügen
    String gpsDataString = "{\"id\":1,\"date\":\"" + gps_date + "\",\"time\":\"" + gps_Time + "\",\"latitude\":\"" + gps_lat + "\",\"longitude\":\"" + gps_long + "\"}";

    // GPS-Daten in den FIFO-Speicher einfügen
    gpsDataQueue.push_back(gpsDataString);

    // Seriellisierten JSON-String am Monitor ausgeben
    Serial.print("Serialized JSON string: ");
    Serial.println(gpsDataString);

  }

  // Wenn Verbindung besteht und GPS-Daten im FIFO-Speicher vorhanden sind
  if (Serial1.available() && !gpsDataQueue.empty()) {

    // Nächstes GPS-Datenpaket aus dem FIFO-Speicher entnehmen
    String serializedJsonString = gpsDataQueue.front();
    gpsDataQueue.pop_front();

    // Datenpaket senden
    LoRa.beginPacket();
    LoRa.print(serializedJsonString);
    LoRa.endPacket();

    // Seriellisierten JSON-String am Monitor ausgeben
    Serial.print("Sent JSON string: ");
    Serial.println(serializedJsonString);

  }

  /*
//Test um prüfen, ob FIFO funktioniert
gpsDataQueue.push_back("{\"id\":1,\"date\":\"2023-04-15\",\"time\":\"12:34:56\",\"latitude\":\"12.3456\",\"longitude\":\"-123.4567\"}");
gpsDataQueue.push_back("{\"id\":2,\"date\":\"2023-04-15\",\"time\":\"12:34:59\",\"latitude\":\"12.34567\",\"longitude\":\"-123.45678\"}");
gpsDataQueue.push_back("{\"id\":3,\"date\":\"2023-04-15\",\"time\":\"12:35:08\",\"latitude\":\"12.345678\",\"longitude\":\"-123.45678\"}");
gpsDataQueue.push_back("{\"id\":4,\"date\":\"2023-04-15\",\"time\":\"12:35:23\",\"latitude\":\"12.3456789\",\"longitude\":\"-123.456789\"}");

Serial.print("GPS-Datenpaket in Warteschlange: ");
Serial.println(gpsDataQueue.front());

String serializedJsonString = gpsDataQueue.front();
gpsDataQueue.pop_front();

Serial.print("Entnommenes GPS-Datenpaket: ");
Serial.println(serializedJsonString);
*/

 sleepForFiveMinutes();

}