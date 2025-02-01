#include "Motorshield.h"

Motor::Motor(uint8_t dir, uint8_t pwm, uint8_t brake, uint8_t currentSense)
    : pin{dir, pwm, brake, currentSense},
      dir(CW),
      speed(0),
      currentSpeed(0),
      enabled(false),
      inrushWait(0),
      currentInrushWait(0),
      blockCurrent(0),
      sampleIndex(0),
      samplesRolled(false),
      lastSample(0),
      lastRamp(0) {
    pinMode(pin.dir, OUTPUT);
    pinMode(pin.pwm, OUTPUT);
    pinMode(pin.brake, OUTPUT);
    // pinMode(pin.current, INPUT); // necessary?
}

void Motor::enable(Direction dir, float speed) {
    if (this->enabled && this->dir != dir) {
        stop();
    }
    this->dir = dir;
    this->speed = std::clamp(MAX_SPEED_RAW * speed, MIN_SPEED_RAW, MAX_SPEED_RAW);
    this->enabled = true;
    this->inrushWait = 400;
}

void Motor::stop() {
    this->enabled = false;
    this->currentSpeed = 0;
    this->currentInrushWait = 0;
}

float Motor::amps() {
    float voltage = analogRead(pin.current) * (CURRENT_SENSE_MAX_VOLTAGE / 1023.0);
    return voltage / CURRENT_SENSE_AMPS_PER_VOLT;
}

float Motor::ampsAvg() {
    float sum = 0.0f;
    for (int i = 0; i < MAX_SAMPLES; i++) {
        sum += this->samples[i];
    }
    return sum / MAX_SAMPLES;
}

void Motor::configureBlockDetection(float blockCurrent, int inrushWait) {
    this->blockCurrent = blockCurrent;
    this->inrushWait = inrushWait;
}

bool Motor::blocked() {
    if (this->blockCurrent > 0) {
        return this->currentInrushWait >= this->inrushWait && this->amps() > this->blockCurrent;
    }
    return false;
}

void Motor::loop() {
    unsigned long now = millis();
    // speed ramping
    if (this->enabled) {
        if (now - this->lastRamp > 5) {
            this->lastRamp = now;
            if (this->currentSpeed < this->speed) {
                this->currentSpeed++;
            }
            if (this->currentInrushWait < this->inrushWait) {
                this->currentInrushWait++;
            }
        }
        analogWrite(pin.pwm, this->currentSpeed);
        digitalWrite(pin.dir, this->dir);
        digitalWrite(pin.brake, LOW);
    } else {
        analogWrite(pin.pwm, 0);
        digitalWrite(pin.brake, HIGH);
    }

    // amp sampling
    if (now - this->lastSample > 5) {
        this->lastSample = now;
        samples[sampleIndex] = amps();
        if (++sampleIndex >= MAX_SAMPLES) {
            samplesRolled = true;
            sampleIndex = 0;
        }
    }
}