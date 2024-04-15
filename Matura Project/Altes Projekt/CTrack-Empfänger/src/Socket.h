#ifndef SOCKET_H
#define SOCKET_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>

using namespace std;

class Socket
{
private:
    int var;
    char* socket_message;
    const char* PARAM_MESSAGE = "message";
    
public:
    static String processor();
    static void send_data();
    static void start_socket();
};
#endif

