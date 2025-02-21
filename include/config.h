#pragma once
#include <Arduino.h>

// Dispenser A
#define DISPENSER_A_REMOVAL_LED_PIN 23

#define DISPENSER_A_EMPTY_SWITCH_PIN 25
#define DISPENSER_A_EMPTY_SWITCH_MODE INPUT
#define DISPENSER_A_EMPTY_SWITCH_POLARITY LOW

#define DISPENSER_A_MIN_LIMIT_SWITCH_PIN 27
#define DISPENSER_A_MIN_LIMIT_SWITCH_MODE INPUT
#define DISPENSER_A_MIN_LIMIT_SWITCH_POLARITY LOW

#define DISPENSER_A_MAX_LIMIT_SWITCH_PIN 29
#define DISPENSER_A_MAX_LIMIT_SWITCH_MODE INPUT
#define DISPENSER_A_MAX_LIMIT_SWITCH_POLARITY LOW

#define DISPENSER_A_MAX_LIMIT_SWITCH2_PIN 31
#define DISPENSER_A_MAX_LIMIT_SWITCH2_MODE INPUT
#define DISPENSER_A_MAX_LIMIT_SWITCH2_POLARITY HIGH

#define DISPENSER_A_MANUAL_SWITCH_PIN 47
#define DISPENSER_A_MANUAL_SWITCH_MODE INPUT
#define DISPENSER_A_MANUAL_SWITCH_POLARITY HIGH

// Dispenser B
#define DISPENSER_B_REMOVAL_LED_PIN 41

#define DISPENSER_B_EMPTY_SWITCH_PIN 39
#define DISPENSER_B_EMPTY_SWITCH_MODE INPUT
#define DISPENSER_B_EMPTY_SWITCH_POLARITY LOW

#define DISPENSER_B_MIN_LIMIT_SWITCH_PIN 37
#define DISPENSER_B_MIN_LIMIT_SWITCH_MODE INPUT
#define DISPENSER_B_MIN_LIMIT_SWITCH_POLARITY LOW

#define DISPENSER_B_MAX_LIMIT_SWITCH_PIN 35
#define DISPENSER_B_MAX_LIMIT_SWITCH_MODE INPUT
#define DISPENSER_B_MAX_LIMIT_SWITCH_POLARITY LOW

#define DISPENSER_B_MAX_LIMIT_SWITCH2_PIN 33
#define DISPENSER_B_MAX_LIMIT_SWITCH2_MODE INPUT
#define DISPENSER_B_MAX_LIMIT_SWITCH2_POLARITY HIGH

#define DISPENSER_B_MANUAL_SWITCH_PIN 49
#define DISPENSER_B_MANUAL_SWITCH_MODE INPUT
#define DISPENSER_B_MANUAL_SWITCH_POLARITY HIGH

// Rotary encoder
#define ROTARY_ENC_PIN_CLK 18
#define ROTARY_ENC_PIN_DT 19
#define ROTARY_ENC_PIN_SW 17
#define ROTARY_ENC_DEBOUNCE_DELAY 10
#define ROTARY_ENC_ERROR_DELAY 250

// Settings
#define SETTINGS_MENU_LONG_PRESS_MS 2000

// Error led
#define ERROR_LED_PIN 43

// Display
#define LCD_I2C_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
#define DISPLAY_FPS 4

// Coin acceptor
#define COIN_ACCEPTOR_PULSE_PIN 2
#define COIN_ACCEPTOR_ENABLE_PIN 45
#define COIN_ACCEPTOR_INIT_DELAY 100
