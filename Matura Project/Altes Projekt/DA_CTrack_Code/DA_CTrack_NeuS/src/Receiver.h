#ifndef RECEIVER_H
#define RECEIVER_H

#include <string>

using namespace std;

class Receiver
{
    public:
        static void setup_receiver();
        static void loop_receiver();
        static String getLoRaData();

    private:
        static String LoRaData;
};
#endif  