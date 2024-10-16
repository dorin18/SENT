#ifndef SENT_h
#define SENT_h

#include <Arduino.h>
#define SEND_SENT 0x0
#define READ_SENT 0x1

// Definirea duratelor minime și maxime pentru LOW
// #define MIN_LOW_DURATION 10 // exemplu de durată minimă în microsecunde
// #define MAX_LOW_DURATION 1000 // exemplu de durată maximă în microsecundeS

class SENT {
  public:
    SENT(uint8_t pin, uint8_t mode = OUTPUT);
    void begin(unsigned long tick = 3);
    void send(uint16_t dataSensor1, uint16_t dataSensor2);
    void send(uint16_t dataSensor1);
    void read(uint16_t *dataSensor1, uint16_t *dataSensor2, uint16_t *sentstatus);
    bool available();
    ///uint16_t available();
    uint8_t receive(uint16_t *receivedData);
    uint8_t correctError(uint8_t data);
    uint8_t encodeData(uint8_t data);
    uint8_t decodeData(uint8_t encodedData);
    void syncPulse();
    void sendNibble(uint8_t nibble);
    void pausePulse();
    uint8_t calculateCRC(uint8_t* data); 

  private:
    uint8_t _pin;
    unsigned long _previousTime;
    unsigned long _bitInterval;
    unsigned long _tick;
    uint8_t _status_bit = 0; //status bit LS nibble first 
};

#endif
