#pragma once

#include <Arduino.h>
#include <EEPROM-Storage.h>

class Storage {
   public:
    const EEPROMStorage<unsigned int> totalSoldItems;
    const EEPROMStorage<unsigned int> soldItems;
    const EEPROMStorage<float> dispenserSpeed;
    const EEPROMStorage<unsigned int> maxDispenseAttempts;
    const EEPROMStorage<unsigned int> removalWaitTime;
    const EEPROMStorage<float> motorBlockCurrent;
    const EEPROMStorage<unsigned int> motorBlockInrushWait;

    Storage()
        : totalSoldItems(0, 0),
          soldItems(totalSoldItems.nextAddress(), 0),
          dispenserSpeed(soldItems.nextAddress(), 1.0),
          maxDispenseAttempts(dispenserSpeed.nextAddress(), 3),
          removalWaitTime(maxDispenseAttempts.nextAddress(), 3000),
          motorBlockCurrent(removalWaitTime.nextAddress(), 1.5),
          motorBlockInrushWait(motorBlockCurrent.nextAddress(), 400) {}
} const STORAGE;