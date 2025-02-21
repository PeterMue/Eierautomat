//#define DEBUG
#ifdef DEBUG
#include <Arduino.h>
#include <avdweb_Switch.h>
#include "config.h"


Switch amin(DISPENSER_A_MIN_LIMIT_SWITCH_PIN, INPUT);
Switch amax(DISPENSER_A_MAX_LIMIT_SWITCH_PIN, INPUT);
Switch amax2(DISPENSER_A_MAX_LIMIT_SWITCH2_PIN, INPUT, HIGH);

void setup() {
    Serial.begin(9600);
}

unsigned long last = 0;

void loop() {
    amin.poll();
    amax.poll();
    amax2.poll();

    unsigned long now = millis();
    if(now - last > 500) {
        last = now;
        Serial.println("Amin: " + String(amin.on()) + " Amax: " + String(amax.on()) + " Amax2: " + String(amax2.on()));
    }
}

#else
#include <Arduino.h>

#include "Display.h"
#include "coin/CoinAcceptor.h"
#include "config.h"
#include "debug.h"
#include "dispenser/Dispensers.h"
#include "motor/Motor.h"
#include "motor/Motorshield.h"

Storage *storage;
Dispensers *dispensers;
CoinAcceptor *coinAcceptor;
Display *display;
EasyLed *errorLed;

enum State { IDLE, AWAITING_PAYMENT, DISPENSING, EMPTY, OUT_OF_ORDER } state;

void setup() {
    Serial.begin(9600);

    // initialization must happen here, otherwise something strange happens
    storage = Storage::instance;
    dispensers = Dispensers::instance;
    coinAcceptor = CoinAcceptor::instance;
    display = Display::instance;

    errorLed = new EasyLed(ERROR_LED_PIN, EasyLed::ActiveLevel::Low);

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
    const unsigned int price = storage->price.get();

    switch (state) {
        case IDLE:
            coinAcceptor->enable();
            errorLed->off();
            display->setText("Betriebsbereit");
            if (!dispensers->ready()) {
                state = dispensers->empty() ? EMPTY : OUT_OF_ORDER;
                Debug::stateTransition("IDLE", state, "Dispenser not ready");
            } else if (balance > 0) {
                state = AWAITING_PAYMENT;
                Debug::stateTransition("IDLE", state, "Balance > 0 (" + String(balance / 100.0) + " EUR)");
            }
            break;
        case AWAITING_PAYMENT:
            coinAcceptor->enable();
            errorLed->off();
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
            errorLed->off();
            coinAcceptor->disable();
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
                        storage->soldItems += 1;
                        storage->totalSoldItems += 1;
                        break;
                    case Dispenser::DISPENSE:
                    case Dispenser::RECOVER:
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
                if(activeDispenser->getState() == Dispenser::REMOVAL) {
                    display->enableProgressIndicator();
                } else {
                    display->disableProgressIndicator();
                }
            }
            break;
        case EMPTY:
            errorLed->on();
            coinAcceptor->disable();
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
            errorLed->on();
            coinAcceptor->disable();
            display->setText("Ausser Betrieb\n+49 171 9422292");
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
#endif