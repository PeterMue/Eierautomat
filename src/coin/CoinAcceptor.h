#pragma once

#include <Arduino.h>

#include "Storage.h"
#include "config.h"

class CoinAcceptor {
   public:
    static CoinAcceptor *const instance;

    void begin();
    void loop();
    unsigned int getBalance();
    bool withdraw(unsigned int amount);

   private:
    CoinAcceptor(uint8_t pulsePin);

    uint8_t pulsePin;
    unsigned int pulseMaxDelay;
    volatile unsigned int pulses;
    volatile unsigned long lastPulseMillis;
    unsigned int balance;
    unsigned int values[7];

    static void pulseHandler();
};