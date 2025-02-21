#include "CoinAcceptor.h"

CoinAcceptor *const CoinAcceptor::instance = new CoinAcceptor(COIN_ACCEPTOR_PULSE_PIN, COIN_ACCEPTOR_ENABLE_PIN);

CoinAcceptor::CoinAcceptor(uint8_t pulsePin, uint8_t enable_pin)
    : pulsePin(pulsePin),
      enablePin(enable_pin),
      pulseMaxDelay(200),
      pulses(0),
      lastPulseMillis(0),
      balance(0),
      values{0},
      enabledInternal(false),
      enabled(false) {
    pinMode(pulsePin, INPUT);
    pinMode(enablePin, OUTPUT);
}

void CoinAcceptor::begin() { 
    pulseMaxDelay = Storage::instance->coinPulseMaxDelay.get();
    values[0] = 0;
    values[1] = Storage::instance->coin1value.get();
    values[2] = Storage::instance->coin2value.get();
    values[3] = Storage::instance->coin3value.get();
    values[4] = Storage::instance->coin4value.get();
    values[5] = Storage::instance->coin5value.get();
    values[6] = Storage::instance->coin6value.get();

    attachInterrupt(digitalPinToInterrupt(this->pulsePin), pulseHandler, FALLING);

    enable();

    Debug::log("Coin acceptor initialized");
    Debug::log("Coins: [1] " + String(values[1]) + " ct, [2] " + String(values[2]) + " ct, [3] " + String(values[3]) + " ct, [4] " + String(values[4]) + " ct, [5] " + String(values[5]) + " ct, [6] " + String(values[6]) + " ct");
}

void CoinAcceptor::pulseHandler() {
    if(instance->enabledInternal) {
        instance->pulses++;
        instance->lastPulseMillis = millis();
    }
}

void CoinAcceptor::loop() {
    timer.tick();
    unsigned long currentMillis = millis();
    if(enabledInternal) {
        noInterrupts();
        unsigned int delta = currentMillis - lastPulseMillis;
        if (delta > pulseMaxDelay) {
            if (pulses > 0 && pulses < 7) {
                balance += values[pulses];
                Debug::log("Coin accepted: " + String(values[pulses]) + " ct");
                scheduleBalanceReset();
            }
            pulses = 0;
        }
        interrupts();
    }
}

void CoinAcceptor::scheduleBalanceReset() {
    if(balanceResetTask  != nullptr){
        timer.cancel(balanceResetTask);
    }
    unsigned int delay = Storage::instance->balanceResetDelay.get() * 1000;
    if(delay == 0) {
        return;
    }
    balanceResetTask = timer.in(delay, [](CoinAcceptor *self) -> bool {
        self->withdraw(self->getBalance());
        Debug::log("Balance reset after timeout");
        return false;
    }, this);   
}

unsigned int CoinAcceptor::getBalance() { return balance; }

bool CoinAcceptor::withdraw(unsigned int amount) {
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

void CoinAcceptor::enable() { 
    if(this->enabled) {
        return;
    } 
    this->enabled = true; 
    digitalWrite(enablePin, HIGH);
    if(enableTask != nullptr) {
        timer.cancel(enableTask);
    }
    enableTask = this->timer.in(COIN_ACCEPTOR_INIT_DELAY, [](CoinAcceptor *self) -> bool {
        self->pulses = 0;
        self->enabledInternal = true;
        Debug::log("Coin pulse handler attached");
        return false;
    }, this);
}

void CoinAcceptor::disable() { 
    if(!this->enabled) {
        return;
    }
    this->enabled = false; 
    this->enabledInternal = false;
    digitalWrite(enablePin, LOW);
}