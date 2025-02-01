#pragma once

#include <Arduino.h>

/**
 * Patch for missing std::clamp in Arduino
 */
namespace std {
template <typename T>
T clamp(T val, T min, T max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}
}  // namespace std

class Motor {
   public:
    enum Direction { CW, CCW };

   public:
    Motor(uint8_t dir, uint8_t pwm, uint8_t brake, uint8_t currentSense);

    void enable(Direction dir, float speed);
    void stop();
    float amps();
    float ampsAvg();
    void loop();
    bool blocked();

    void configureBlockDetection(float blockCurent, int inrushWait);

   private:
    const struct Pin {
        uint8_t dir;
        uint8_t pwm;
        uint8_t brake;
        uint8_t current;
    } pin;

    static constexpr float MAX_SPEED_RAW = 255.0;
    static constexpr float MIN_SPEED_RAW = 0;
    static constexpr int MAX_SAMPLES = 10;
    static constexpr float CURRENT_SENSE_AMPS_PER_VOLT = 1.65;
    static constexpr float CURRENT_SENSE_MAX_VOLTAGE = 3.3;

    Direction dir;
    float speed;
    int currentSpeed;

    bool enabled;

    int inrushWait;
    int currentInrushWait;
    float blockCurrent;

    float samples[MAX_SAMPLES];
    int sampleIndex;
    bool samplesRolled;

    unsigned long lastSample;
    unsigned long lastRamp;
};