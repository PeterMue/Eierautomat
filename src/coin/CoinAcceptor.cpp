#include "CoinAcceptor.h"

CoinAcceptor *const CoinAcceptor::instance = new CoinAcceptor(COIN_PIN_PULSE);

CoinAcceptor::CoinAcceptor(uint8_t pulsePin)
    : pulsePin(pulsePin),
      pulseMaxDelay(STORAGE.coinPulseMaxDelay.get()),
      pulses(0),
      lastPulseMillis(0),
      balance(0),
      values{0,  // 0-index is not used
             STORAGE.coin1value.get(),
             STORAGE.coin2value.get(),
             STORAGE.coin3value.get(),
             STORAGE.coin4value.get(),
             STORAGE.coin5value.get(),
             STORAGE.coin6value.get()} {
    pinMode(pulsePin, INPUT);  // TODO: pull-up?
}

void CoinAcceptor::begin() { attachInterrupt(digitalPinToInterrupt(pulsePin), pulseHandler, FALLING); }

void CoinAcceptor::pulseHandler() {
    instance->pulses++;
    instance->lastPulseMillis = millis();
}

void CoinAcceptor::loop() {
    unsigned long currentMillis = millis();
    unsigned int delta = currentMillis - lastPulseMillis;
    if (delta > pulseMaxDelay) {
        if (pulses > 0 && pulses < 7) {
            balance += values[pulses];
        }
        pulses = 0;
    }
}

unsigned int CoinAcceptor::getBalance() { return balance; }

bool CoinAcceptor::withdraw(unsigned int amount) {
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}