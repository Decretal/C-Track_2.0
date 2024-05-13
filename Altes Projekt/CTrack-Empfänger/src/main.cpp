#include <Arduino.h>
#include "Socket.h"
#include "Receiver.h"


void setup()
{
    Serial.begin(9600);
    Serial.print("Starting....................\n");

    Socket::start_socket();
    Receiver::setup_receiver();
}

void loop()
{
   Socket::send_data();
   Receiver::loop_receiver();
   
}