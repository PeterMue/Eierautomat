#pragma once

#include <Arduino.h>
#include <EEPROM-Storage.h>

struct Storage {
   public:
    static Storage *const instance;

   public:
    // dispenser
    EEPROMStorage<float> dispenserSpeed;
    EEPROMStorage<unsigned int> maxDispenseAttempts;
    EEPROMStorage<unsigned int> removalWaitTime;
    // motor current
    EEPROMStorage<float> motorBlockCurrent;
    EEPROMStorage<unsigned int> motorBlockInrushWait;
    // coin pulses
    EEPROMStorage<unsigned int> coinPulseMaxDelay;
    // coin values
    EEPROMStorage<unsigned int> coin1value;
    EEPROMStorage<unsigned int> coin2value;
    EEPROMStorage<unsigned int> coin3value;
    EEPROMStorage<unsigned int> coin4value;
    EEPROMStorage<unsigned int> coin5value;
    EEPROMStorage<unsigned int> coin6value;
    // price
    EEPROMStorage<unsigned int> price;
    EEPROMStorage<unsigned int> balanceResetDelay;
    // statistics
    EEPROMStorage<unsigned int> sold;
    EEPROMStorage<unsigned int> total;
private:
    Storage();

public:
    void reset() {
        dispenserSpeed.unset();
        maxDispenseAttempts.unset();
        removalWaitTime.unset();
        motorBlockCurrent.unset();
        motorBlockInrushWait.unset();
        coinPulseMaxDelay.unset();
        coin1value.unset();
        coin2value.unset();
        coin3value.unset();
        coin4value.unset();
        coin5value.unset();
        coin6value.unset();
        price.unset();
        balanceResetDelay.unset();
        sold.unset();
        total.unset();
    }
};