#pragma once

#include <Arduino.h>

#include "Storage.h"
#include "config.h"
#include "debug.h"

class CoinAcceptor {
   public:
    static CoinAcceptor *const instance;

    void begin();
    void loop();
    unsigned int getBalance();
    bool withdraw(unsigned int amount);

    void enable();
    void disable();

   private:
    CoinAcceptor(uint8_t pulsePin, uint8_t enablePin);

    uint8_t pulsePin;
    uint8_t enablePin;
    unsigned int pulseMaxDelay;
    volatile unsigned int pulses;
    volatile unsigned long lastPulseMillis;
    volatile unsigned int lastPulseCount;
    unsigned int balance;
    unsigned int values[7];
    volatile bool enabled;
    static void pulseHandler();
};