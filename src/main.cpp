#include <Arduino.h>

#include "Display.h"
#include "coin/CoinAcceptor.h"
#include "config.h"
#include "debug.h"
#include "dispenser/Dispensers.h"
#include "motor/Motor.h"
#include "motor/Motorshield.h"

Dispensers *dispensers;
CoinAcceptor *coinAcceptor;
Display *display;

enum State { IDLE, AWAITING_PAYMENT, DISPENSING, EMPTY, OUT_OF_ORDER } state;

void setup() {
    Serial.begin(9600);

    // initialization must happen here, otherwise something strange happens
    dispensers = Dispensers::instance;
    coinAcceptor = CoinAcceptor::instance;
    display = Display::instance;

    display->begin();
    display->setText("Betriebsbereit");
    coinAcceptor->begin();
    dispensers->begin();
    state = IDLE;
    Debug::stateTransition("(none)", state, "Initialization complete");
}

Dispenser *activeDispenser = nullptr;

void loop() {
    display->loop();
    coinAcceptor->loop();
    dispensers->loop();

    unsigned int balance = coinAcceptor->getBalance();
    unsigned int price = STORAGE.price;

    switch (state) {
        case IDLE:
            display->setText("Betriebsbereit");
            if (!dispensers->ready()) {
                state = dispensers->empty() ? EMPTY : OUT_OF_ORDER;
                Debug::stateTransition("IDLE", state, "Dispenser not ready");
                Debug::stateTransition("IDLE", state, "Dispenser not ready");
            } else if (balance > 0) {
                state = AWAITING_PAYMENT;
                Debug::stateTransition("IDLE", state, "Balance > 0");
            }
            break;
        case AWAITING_PAYMENT:
            display->setText("Guthaben: \n" + String(balance / 100.0) + " EUR");
            if (!dispensers->ready()) {
                state = OUT_OF_ORDER;
                Debug::stateTransition("AWAITING_PAYMENT", state, "Dispenser not ready");
            } else if (balance >= price) {
                if (coinAcceptor->withdraw(price)) {
                    activeDispenser = dispensers->dispense();
                    state = DISPENSING;
                    Debug::stateTransition("AWAITING_PAYMENT", state, "Withdraw successful");
                }
            }
            break;
        case DISPENSING:
            if (activeDispenser == nullptr) {
                state = OUT_OF_ORDER;
                Debug::stateTransition("DISPENSING", state, "activeDispenser == nullptr");
            } else {
                switch (activeDispenser->getState()) {
                    case Dispenser::IDLE:
                        state = IDLE;
                        Debug::stateTransition("DISPENSING", state, "Dispenser completed");
                        activeDispenser = nullptr;
                        dispensers->swap();
                        STORAGE.soldItems += 1;
                        STORAGE.totalSoldItems += 1;
                        break;
                    case Dispenser::DISPENSE:
                    case Dispenser::RETURN:
                        display->setText("Warenausgabe");
                        break;
                    case Dispenser::REMOVAL:
                        display->setText("Entnahme");
                        break;
                    case Dispenser::ERROR:
                    default:
                        state = OUT_OF_ORDER;
                        Debug::stateTransition("DISPENSING", state, "Dispenser error");
                        break;
                }
            }
            break;
        case EMPTY:
            display->setText("Leer");
            if (dispensers->ready()) {
                state = IDLE;
                Debug::stateTransition("EMPTY", state, "Dispenser ready");
            } else if (!dispensers->empty()) {
                state = OUT_OF_ORDER;
                Debug::stateTransition("EMPTY", state, "Dispenser not empty");
            }
            break;
        case OUT_OF_ORDER:
        default:
            display->setText("Ausser Betrieb\n+49 123 456 789");
            if (dispensers->ready()) {
                state = IDLE;
                Debug::stateTransition("OUT_OF_ORDER", state, "Dispenser ready");
            } else if (dispensers->empty()) {
                state = EMPTY;
                Debug::stateTransition("OUT_OF_ORDER", state, "Dispenser empty");
            }
            break;
    };
}