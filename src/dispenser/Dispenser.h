#pragma once

#include <Arduino.h>
#include <EasyLed.h>
#include <arduino-timer.h>
#include <avdweb_Switch.h>

#include "motor/Motorshield.h"
#include "debug.h"

class Dispenser {
   public:
    enum State { IDLE, DISPENSE, RETURN, REMOVAL, RECOVER, ERROR };
    struct Info {
        String id;
        unsigned int dispensedItems;
    };

   public:
    Dispenser(String id, Motor *const motor, EasyLed *indicatorLed, Switch *emptySwitch, Switch *minLimitSwitch,
              Switch *maxLimitSwitch, Switch *maxLimitSwitch2, Switch *manualSwitch);

    void begin();
    void loop();
    void dispense(bool force = false);

    bool isError();
    void reset();

    bool isReady();

    bool isEmpty();

    State getState();
    Info getInfo();

   private:
    unsigned short maxDispenseAttempts;
    float dispenserSpeed;

    const String id;

    Motor *const motor;
    Switch *const minLimitSwitch;
    Switch *const maxLimitSwitch;
    Switch *const maxLimitSwitch2;
    Switch *const emptySwitch;
    Switch *const manualSwitch;
    EasyLed *const removalLed;

    State state;

    Timer<2, millis, Dispenser *> timer;
    unsigned short dispenseAttempt;

    void updateComponents();
    bool isMinLimitSwitchOn();
    bool isMaxLimitSwitchOn();
};