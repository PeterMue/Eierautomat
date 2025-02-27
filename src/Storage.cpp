#include "Storage.h"

Storage *const Storage::instance = new Storage();

Storage::Storage() :
          // dispenser
          dispenserSpeed(1024, 1.0),
          maxDispenseAttempts(dispenserSpeed.nextAddress(), 3),
          removalWaitTime(maxDispenseAttempts.nextAddress(), 3000),
          // motor current
          motorBlockCurrent(removalWaitTime.nextAddress(), 1.65),
          motorBlockInrushWait(motorBlockCurrent.nextAddress(), 200),
          // coin pulses
          coinPulseMaxDelay(motorBlockCurrent.nextAddress(), 250),
          // coin values
          coin1value(coinPulseMaxDelay.nextAddress(), 200),
          coin2value(coin1value.nextAddress(), 100),
          coin3value(coin2value.nextAddress(), 50),
          coin4value(coin3value.nextAddress(), 20),
          coin5value(coin4value.nextAddress(), 10),
          coin6value(coin5value.nextAddress(), 0),
          // price
          price(coin6value.nextAddress(), 350),
          balanceResetDelay(price.nextAddress(), 25),
          // statistics
          sold(balanceResetDelay.nextAddress(), 0),
          total(sold.nextAddress(), 0)
    //
    {
        /* intentionally left blank */
    }
