#include "Dispenser.h"

#include "Storage.h"

Dispenser::Dispenser(Motor *const motor, EasyLed *removalLed, Switch *emptySwitch, Switch *minLimitSwitch,
                     Switch *maxLimitSwitch, Switch *maxLimitSwitch2 = nullptr)
    : maxDispenseAttempts(STORAGE.maxDispenseAttempts.get()),
      dispenserSpeed(STORAGE.dispenserSpeed.get()),
      motor(motor),
      minLimitSwitch(minLimitSwitch),
      maxLimitSwitch(maxLimitSwitch),
      maxLimitSwitch2(maxLimitSwitch2),
      emptySwitch(emptySwitch),
      removalLed(removalLed),
      state(IDLE) {
    this->removalLed->off();
    this->motor->configureBlockDetection(STORAGE.motorBlockCurrent.get(), STORAGE.motorBlockInrushWait.get());
}

void Dispenser::updateComponents() {
    minLimitSwitch->poll();
    maxLimitSwitch->poll();
    if (nullptr != maxLimitSwitch2) {
        maxLimitSwitch2->poll();
    }
    emptySwitch->poll();
    motor->loop();
}

void Dispenser::loop() {
    updateComponents();

    // block detection
    bool blockDetected = motor->blocked();

    // state machine
    switch (state) {
        case ERROR:
            motor->stop();
            break;
        case RECOVER:
            if (!isMinLimitSwitchOn()) {
                motor->enable(Motor::Direction::CCW, dispenserSpeed);
            } else if (blockDetected) {
                motor->stop();
                state = ERROR;
            } else {
                motor->stop();
                state = (++dispenseAttempt > maxDispenseAttempts) ? ERROR : DISPENSE;
            }
            break;
        case DISPENSE:
            if (isMaxLimitSwitchOn()) {
                motor->stop();
                state = RETURN;
            } else if (blockDetected) {
                motor->stop();
                state = RECOVER;
            } else {
                motor->enable(Motor::Direction::CW, dispenserSpeed);
            }
            break;
        case RETURN:
            if (isMinLimitSwitchOn()) {
                motor->stop();
                state = REMOVAL;
            } else if (blockDetected) {
                motor->stop();
                state = ERROR;
            } else {
                motor->enable(Motor::Direction::CCW, dispenserSpeed);
            }
            break;
        case REMOVAL:
            removalLed->on();
            timer.in(
                STORAGE.removalWaitTime.get(),
                [](Dispenser *self) -> bool {
                    self->removalLed->off();
                    self->state = IDLE;
                    return false;
                },
                this);
            break;
        case IDLE:
            dispenseAttempt = 0;
            STORAGE.soldItems += 1;
        default:
            if (!isMinLimitSwitchOn()) {
                motor->enable(Motor::Direction::CCW, dispenserSpeed);
            } else {
                motor->stop();
                state = IDLE;
            }
            break;
    }
}

void Dispenser::dispense() {
    if (state == IDLE && !emptySwitch->on()) {
        dispenseAttempt = 1;
        state = DISPENSE;
    }
}

bool Dispenser::isError() { return state == ERROR; }

void Dispenser::reset() { state = IDLE; }

Dispenser::Info Dispenser::getInfo() { return {STORAGE.soldItems.get()}; }

bool Dispenser::isMinLimitSwitchOn() { return minLimitSwitch->on(); }

bool Dispenser::isMaxLimitSwitchOn() {
    return maxLimitSwitch->on() || (nullptr != maxLimitSwitch2 && maxLimitSwitch2->on());
}