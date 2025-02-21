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
      enabled(false) {
    pinMode(pulsePin, INPUT);
    pinMode(enablePin, OUTPUT);
}

void CoinAcceptor::begin() { 
    attachInterrupt(digitalPinToInterrupt(pulsePin), pulseHandler, FALLING); 
    pulseMaxDelay = Storage::instance->coinPulseMaxDelay.get();
    values[0] = 0;
    values[1] = Storage::instance->coin1value.get();
    values[2] = Storage::instance->coin2value.get();
    values[3] = Storage::instance->coin3value.get();
    values[4] = Storage::instance->coin4value.get();
    values[5] = Storage::instance->coin5value.get();
    values[6] = Storage::instance->coin6value.get();

    enable();

    Debug::log("Coin acceptor initialized");
    Debug::log("Coins: [1] " + String(values[1]) + " EUR, [2] " + String(values[2]) + " EUR, [3] " + String(values[3]) + " EUR, [4] " + String(values[4]) + " EUR, [5] " + String(values[5]) + " EUR, [6] " + String(values[6]) + " EUR");
}

void CoinAcceptor::pulseHandler() {
    if(instance->enabled) {
        instance->pulses++;
        instance->lastPulseMillis = millis();
    }
}

void CoinAcceptor::loop() {
    unsigned long currentMillis = millis();
    digitalWrite(enablePin, enabled ? HIGH : LOW);
    if(enabled) {
        noInterrupts();
        unsigned int delta = currentMillis - lastPulseMillis;
        if (delta > pulseMaxDelay) {
            if (pulses > 0 && pulses < 7) {
                balance += values[pulses];
                Debug::log("Coin accepted: " + String(values[lastPulseCount]) + " EUR");
            }
            pulses = 0;
        }
        interrupts();
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

void CoinAcceptor::enable() { 
    if(this->enabled) {
        return;
    } 
    this->enabled = true; 
    this->pulses = 0;
    this->lastPulseCount = 0;
}

void CoinAcceptor::disable() { this->enabled = false; }