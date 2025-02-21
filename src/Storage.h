#pragma once

#include <Arduino.h>
#include <EEPROM-Storage.h>

struct Storage {
   public:
    static Storage *const instance;

   public:
    EEPROMStorage<unsigned int> totalSoldItems;
    EEPROMStorage<unsigned int> soldItems;
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
private:
    Storage();
};