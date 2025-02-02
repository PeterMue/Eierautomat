#pragma once

#include <Arduino.h>
#include <EEPROM-Storage.h>

class Storage {
   public:
    const EEPROMStorage<unsigned int> totalSoldItems;
    const EEPROMStorage<unsigned int> soldItems;
    // dispenser
    const EEPROMStorage<float> dispenserSpeed;
    const EEPROMStorage<unsigned int> maxDispenseAttempts;
    const EEPROMStorage<unsigned int> removalWaitTime;
    // motor current
    const EEPROMStorage<float> motorBlockCurrent;
    const EEPROMStorage<unsigned int> motorBlockInrushWait;
    // coin pulses
    const EEPROMStorage<unsigned int> coinPulseMaxDelay;
    // coin values
    const EEPROMStorage<unsigned int> coin1value;
    const EEPROMStorage<unsigned int> coin2value;
    const EEPROMStorage<unsigned int> coin3value;
    const EEPROMStorage<unsigned int> coin4value;
    const EEPROMStorage<unsigned int> coin5value;
    const EEPROMStorage<unsigned int> coin6value;
    // price
    const EEPROMStorage<unsigned int> price;

    Storage()
        : totalSoldItems(0, 0),
          soldItems(totalSoldItems.nextAddress(), 0),
          // dispenser
          dispenserSpeed(soldItems.nextAddress(), 1.0),
          maxDispenseAttempts(dispenserSpeed.nextAddress(), 3),
          removalWaitTime(maxDispenseAttempts.nextAddress(), 3000),
          // motor current
          motorBlockCurrent(removalWaitTime.nextAddress(), 1.5),
          motorBlockInrushWait(motorBlockCurrent.nextAddress(), 400),
          // coin pulses
          coinPulseMaxDelay(motorBlockCurrent.nextAddress(), 140),
          // coin values
          coin1value(coinPulseMaxDelay.nextAddress(), 200),
          coin2value(coin1value.nextAddress(), 100),
          coin3value(coin2value.nextAddress(), 50),
          coin4value(coin3value.nextAddress(), 20),
          coin5value(coin4value.nextAddress(), 10),
          coin6value(coin5value.nextAddress(), 0),
          // price
          price(coin6value.nextAddress(), 350)
    //
    {
        /* intentionally left blank */
    }
} const STORAGE;