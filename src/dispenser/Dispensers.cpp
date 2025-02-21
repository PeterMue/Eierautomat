#include "Dispensers.h"

// Initialize the static instance pointer
Dispensers *const Dispensers::instance = new Dispensers(
    // clang-format off
    new Dispenser("A", Motorshield::channelA,
              new EasyLed(DISPENSER_A_REMOVAL_LED_PIN, EasyLed::ActiveLevel::Low),
              new Switch(DISPENSER_A_EMPTY_SWITCH_PIN, DISPENSER_A_EMPTY_SWITCH_MODE, DISPENSER_A_EMPTY_SWITCH_POLARITY),
              new Switch(DISPENSER_A_MIN_LIMIT_SWITCH_PIN, DISPENSER_A_MIN_LIMIT_SWITCH_MODE, DISPENSER_A_MIN_LIMIT_SWITCH_POLARITY),
              new Switch(DISPENSER_A_MAX_LIMIT_SWITCH_PIN, DISPENSER_A_MAX_LIMIT_SWITCH_MODE, DISPENSER_A_MAX_LIMIT_SWITCH_POLARITY),
              DISPENSER_A_MAX_LIMIT_SWITCH2_PIN > -1
                  ? new Switch(DISPENSER_A_MAX_LIMIT_SWITCH2_PIN, DISPENSER_A_MAX_LIMIT_SWITCH2_MODE, DISPENSER_A_MAX_LIMIT_SWITCH2_POLARITY)
                  : nullptr,
              DISPENSER_A_MANUAL_SWITCH_PIN > -1
                  ? new Switch(DISPENSER_A_MANUAL_SWITCH_PIN, DISPENSER_A_MANUAL_SWITCH_MODE, DISPENSER_A_MANUAL_SWITCH_POLARITY)
                  : nullptr
    ),
    new Dispenser("B", Motorshield::channelB,
              new EasyLed(DISPENSER_B_REMOVAL_LED_PIN, EasyLed::ActiveLevel::Low),
              new Switch(DISPENSER_B_EMPTY_SWITCH_PIN, DISPENSER_B_EMPTY_SWITCH_MODE, DISPENSER_B_EMPTY_SWITCH_POLARITY),
              new Switch(DISPENSER_B_MIN_LIMIT_SWITCH_PIN, DISPENSER_B_MIN_LIMIT_SWITCH_MODE, DISPENSER_B_MIN_LIMIT_SWITCH_POLARITY),
              new Switch(DISPENSER_B_MAX_LIMIT_SWITCH_PIN, DISPENSER_B_MAX_LIMIT_SWITCH_MODE, DISPENSER_B_MAX_LIMIT_SWITCH_POLARITY),
              DISPENSER_B_MAX_LIMIT_SWITCH2_PIN > -1
                  ? new Switch(DISPENSER_B_MAX_LIMIT_SWITCH2_PIN, DISPENSER_B_MAX_LIMIT_SWITCH2_MODE, DISPENSER_B_MAX_LIMIT_SWITCH2_POLARITY)
                  : nullptr,
              DISPENSER_B_MANUAL_SWITCH_PIN > -1
                  ? new Switch(DISPENSER_B_MANUAL_SWITCH_PIN, DISPENSER_B_MANUAL_SWITCH_MODE, DISPENSER_B_MANUAL_SWITCH_POLARITY)
                  : nullptr
    )  // clang-format on
);

Dispensers::Dispensers(Dispenser *const dispenserA, Dispenser *const dispenserB)
    : dispenserA(dispenserA), dispenserB(dispenserB), activeDispenser(nullptr) {}

void Dispensers::begin() {
    dispenserA->begin();
    dispenserB->begin();
    // Set the first ready dispenser as active
    if (dispenserA->isReady()) {
        activeDispenser = dispenserA;
    } else if (dispenserB->isReady()) {
        activeDispenser = dispenserB;
    } else {
        activeDispenser = nullptr;
    }
}

void Dispensers::loop() {
    dispenserA->loop();
    dispenserB->loop();
    if (nullptr == activeDispenser) {
        if (dispenserA->isReady()) {
            activeDispenser = dispenserA;
        } else if (dispenserB->isReady()) {
            activeDispenser = dispenserB;
        }
    }
}

Dispenser *const Dispensers::dispense() {
    if (activeDispenser != nullptr) {
        activeDispenser->dispense();
        return activeDispenser;
    }
    return nullptr;
}

void Dispensers::swap() {
    if (activeDispenser == dispenserA && dispenserB->isReady()) {
        activeDispenser = dispenserB;
    } else if (activeDispenser == dispenserB && dispenserA->isReady()) {
        activeDispenser = dispenserA;
    } else {
        activeDispenser = nullptr;
    }
}

bool Dispensers::ready() { return dispenserA->isReady() || dispenserB->isReady(); }

bool Dispensers::empty() { return dispenserA->isEmpty() && dispenserB->isEmpty(); }
