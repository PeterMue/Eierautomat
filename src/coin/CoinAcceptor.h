#pragma once

#include <Arduino.h>
#include <arduino-timer.h>

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
    unsigned int balance;
    unsigned int values[7];
    volatile bool enabledInternal;

    bool enabled;

    Timer<10, millis, CoinAcceptor *> balanceResetTimer;
    Timer<10, millis, CoinAcceptor *>::Task balanceResetTask;

    Timer<10, millis, CoinAcceptor *> enableTimer;
    Timer<10, millis, CoinAcceptor *>::Task enableTask;

    void scheduleBalanceReset();

    static void pulseHandler();
};