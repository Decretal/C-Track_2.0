//Bibliotheken für LoRa
#include <SPI.h>
#include <LoRa.h>
#include "Receiver.h"

//Pins, an denen das LoRa-Modul am ESP32 angeschlossen ist
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//Frequenzbereich für Europa
#define BAND 866E6

//Counter für die gesendeten Datenpakete
int counter = 0;

//Variable für die empfangenen Daten
String Receiver::LoRaData;

void Receiver::setup_receiver() {

  //Seriellen Monitor starten
  //Serial.begin(115200);

  //Pins für LoRa definieren und Kommunikation starten
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
      Serial.println("LoRa konnte nicht gestartet werden!");
    while (1);
  }
  Serial.println("LoRa  OK! \n");
}

void Receiver::loop_receiver() {

  //LoRaData = "(1, 153810.00, 170323, 4749.35801, 01302.76526)";
  Serial.print(LoRaData + "\n");

  //Datenpaket parsen
  LoRaData = "";
  
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //Serial.print("Paket empfangen ");
      

    //Paket auslesen
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      //LoRaData = "GPS DATEN";
      //Serial.print(LoRaData);
    }
   
   /*
    //RSSI ausgeben
    int rssi = LoRa.packetRssi();
    Serial.print(" mit RSSI ");
    Serial.println(rssi);
    */
  }
  
}

String Receiver::getLoRaData(){
  return LoRaData;
}   
