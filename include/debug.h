#pragma once

#include <Arduino.h>

#define DEBUG

#ifdef DEBUG
// clang-format off
#define STATE_ENUM_NAME(state) (state == 0 ? "IDLE" : \
                          (state == 1 ? "AWAITING_PAYMENT" : \
                          (state == 2 ? "DISPENSING" : \
                          (state == 3 ? "EMPTY" : \
                          (state == 4 ? "OUT_OF_ORDER" : "UNKNOWN")))))
#define DISP_STATE_ENUM_NAME(state) (state == 0 ? "IDLE" : \
                          (state == 1 ? "DISPENSE" : \
                          (state == 2 ? "RETURN" : \
                          (state == 3 ? "RENMOVAL" : \
                          (state == 4 ? "RECOVER" : \
                          (state == 5 ? "ERROR": "UNKNOWN"))))))
// clang-format on
#endif

class Debug {
   public:
#ifdef DEBUG
    static void stateTransition(String from, unsigned int to, String reason) {
        Serial.println(from + " -> " + STATE_ENUM_NAME(to) + ": " + reason);
    }
    static void dispenserStateTransition(String from, unsigned int to, String reason) {
        Serial.println(from + " -> " + DISP_STATE_ENUM_NAME(to) + ": " + reason);
    }
    static void log(String message) { Serial.println(message); }
#else
    static void stateTransition(String from, unsigned int to, String reason) {}
    static void dispenserStateTransition(String from, unsigned int to, String reason) {}
    static void log(String message) {}
#endif
};