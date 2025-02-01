#pragma once

#include <Arduino.h>
#include <EEPROM-Storage.h>

class Storage {
   public:
    const EEPROMStorage<unsigned int> totalSoldItems;
    const EEPROMStorage<unsigned int> soldItems;
    const EEPROMStorage<float> dispenserSpeed;
    const EEPROMStorage<unsigned short> maxDispenseAttempts;
    const EEPROMStorage<unsigned int> removalWaitTime;

    Storage()
        : totalSoldItems(0, 0),
          soldItems(totalSoldItems.nextAddress(), 0),
          dispenserSpeed(soldItems.nextAddress(), 1.0),
          maxDispenseAttempts(dispenserSpeed.nextAddress(), 3),
          removalWaitTime(maxDispenseAttempts.nextAddress(), 3000) {}
} const STORAGE;