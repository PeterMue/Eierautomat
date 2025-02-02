#pragma once

#include <Arduino.h>
#include <EasyLed.h>
#include <arduino-timer.h>
#include <avdweb_Switch.h>

#include "motor/Motorshield.h"

class Dispenser {
   public:
    enum State { IDLE, DISPENSE, RETURN, REMOVAL, RECOVER, ERROR };
    struct Info {
        unsigned int dispensedItems;
    };

   public:
    Dispenser(Motor *const motor, EasyLed *indicatorLed, Switch *emptySwitch, Switch *minLimitSwitch,
              Switch *maxLimitSwitch, Switch *maxLimitSwitch2);
    void loop();
    void dispense();

    bool isError();
    void reset();

    bool isReady();

    bool isEmpty();

    State getState();
    Info getInfo();

   private:
    const unsigned short maxDispenseAttempts;
    const float dispenserSpeed;

    Motor *const motor;
    Switch *const minLimitSwitch;
    Switch *const maxLimitSwitch;
    Switch *const maxLimitSwitch2;
    Switch *const emptySwitch;
    EasyLed *const removalLed;

    State state;

    Timer<1, millis, Dispenser *> timer;
    unsigned short dispenseAttempt;

    void updateComponents();
    bool isMinLimitSwitchOn();
    bool isMaxLimitSwitchOn();
};