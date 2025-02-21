#include "Dispenser.h"

#include "Storage.h"

Dispenser::Dispenser(String id, Motor *const motor, EasyLed *removalLed,
                     Switch *emptySwitch, Switch *minLimitSwitch,
                     Switch *maxLimitSwitch, Switch *maxLimitSwitch2 = nullptr,
                     Switch *manualSwitch = nullptr)
    : maxDispenseAttempts(3),
      dispenserSpeed(1.0),
      id(id),
      motor(motor),
      minLimitSwitch(minLimitSwitch),
      maxLimitSwitch(maxLimitSwitch),
      maxLimitSwitch2(maxLimitSwitch2),
      emptySwitch(emptySwitch),
      manualSwitch(manualSwitch),
      removalLed(removalLed),
      state(IDLE) {
    this->removalLed->off();
}

void Dispenser::begin() {
    this->motor->configureBlockDetection(
        Storage::instance->motorBlockCurrent.get(),
        Storage::instance->motorBlockInrushWait.get());
    this->dispenserSpeed = Storage::instance->dispenserSpeed.get();
    this->maxDispenseAttempts = Storage::instance->maxDispenseAttempts.get();
}

void Dispenser::updateComponents() {
    minLimitSwitch->poll();
    maxLimitSwitch->poll();
    if (nullptr != maxLimitSwitch2) {
        maxLimitSwitch2->poll();
    }
    emptySwitch->poll();
    if ((nullptr != manualSwitch)) {
        manualSwitch->poll();
    }
    motor->loop();
    timer.tick();
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
                Debug::dispenserStateTransition("RECOVER", state,
                                                "Block detected");
            } else {
                motor->stop();
                state = (++dispenseAttempt > maxDispenseAttempts) ? ERROR
                                                                  : DISPENSE;
                Debug::dispenserStateTransition(
                    "RECOVER", state, "Attempt " + String(dispenseAttempt));
            }
            break;
        case DISPENSE:
            if (isMaxLimitSwitchOn()) {
                motor->stop();
                state = RETURN;
                Debug::dispenserStateTransition("DISPENSE", state,
                                                "Max limit switch on");
            } else if (blockDetected) {
                motor->stop();
                state = RECOVER;
                Debug::dispenserStateTransition("DISPENSE", state,
                                                "Block detected");
            } else {
                motor->enable(Motor::Direction::CW, dispenserSpeed);
            }
            break;
        case RETURN:
            if (isMinLimitSwitchOn()) {
                motor->stop();
                state = REMOVAL;
                Debug::dispenserStateTransition("RETURN", state,
                                                "Min limit switch on");
            } else if (blockDetected) {
                motor->stop();
                state = ERROR;
                Debug::dispenserStateTransition("RETURN", state,
                                                "Block detected");
            } else {
                motor->enable(Motor::Direction::CCW, dispenserSpeed);
            }
            break;
        case REMOVAL:
            removalLed->on();
            timer.in(
                Storage::instance->removalWaitTime.get(),
                [](Dispenser *self) -> bool {
                    self->removalLed->off();
                    self->state = IDLE;
                    Debug::dispenserStateTransition("REMOVAL", self->state,
                                                    "Removal Timeout");
                    return false;
                },
                this);
            break;
        case IDLE:
            dispenseAttempt = 0;
            if (nullptr != manualSwitch) {
                if (manualSwitch->singleClick()) {
                    dispense();
                    break;
                } else if (manualSwitch->longPress()) {
                    dispense(true);
                    break;
                }
            }
        default:
            if (!isMinLimitSwitchOn()) {
                motor->enable(Motor::Direction::CCW, dispenserSpeed);
            } else {
                motor->stop();
                if (state != IDLE) {
                    state = IDLE;
                    Debug::dispenserStateTransition("IDLE", state,
                                                    "Min limit switch on");
                }
            }
            break;
    }
}

void Dispenser::dispense(bool force) {
    if (force || (state == IDLE && !emptySwitch->on())) {
        dispenseAttempt = 1;
        state = DISPENSE;
        Debug::dispenserStateTransition(
            "IDLE", state, "Dispense requested on dispenser " + id);
    }
}

bool Dispenser::isReady() { return state == IDLE && !isEmpty(); }

bool Dispenser::isEmpty() { return emptySwitch->on(); }

bool Dispenser::isError() { return state == ERROR; }

void Dispenser::reset() { state = IDLE; }

Dispenser::State Dispenser::getState() { return state; }

Dispenser::Info Dispenser::getInfo() {
    return {id, Storage::instance->soldItems.get()};
}

bool Dispenser::isMinLimitSwitchOn() { return minLimitSwitch->on(); }

bool Dispenser::isMaxLimitSwitchOn() {
    return maxLimitSwitch->on() ||
           (nullptr != maxLimitSwitch2 && maxLimitSwitch2->on());
}