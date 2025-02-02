#include "Display.h"

// clang-format off
MENU_SCREEN(infoScreen, infoScreenItems,
    //        |>--------------+|
    ITEM_BASIC("Version 1.0"),
    //        |>--------------+|
    ITEM_VALUE("Total sold", STORAGE.totalSoldItems, "%d"),
    //        |>--------------+|
    ITEM_VALUE("Sold items", STORAGE.soldItems, "%d"),
    //        |>--------------+|
    ITEM_BACK()
);
MENU_SCREEN(coinScreen, coinScreenItems,
    //         |>--------------+|
    ITEM_WIDGET("interval",
        [](unsigned int delay) { STORAGE.coinPulseMaxDelay = delay; },
        WIDGET_RANGE(STORAGE.coinPulseMaxDelay.get(), 10U, 10U, 500U, "%d ms")),
    //         |>--------------+|
    ITEM_WIDGET("1 pulse",
        [](unsigned int value) { STORAGE.coin1value = value; },
        WIDGET_RANGE(STORAGE.coin1value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("2 pulse",
        [](unsigned int value) { STORAGE.coin2value = value; },
        WIDGET_RANGE(STORAGE.coin2value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("3 pulse",
        [](unsigned int value) { STORAGE.coin3value = value; },
        WIDGET_RANGE(STORAGE.coin3value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("4 pulse",
        [](unsigned int value) { STORAGE.coin4value = value; },
        WIDGET_RANGE(STORAGE.coin4value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("5 pulse",
        [](unsigned int value) { STORAGE.coin5value = value; },
        WIDGET_RANGE(STORAGE.coin5value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("6 pulse",
        [](unsigned int value) { STORAGE.coin6value = value; },
        WIDGET_RANGE(STORAGE.coin6value.get(), 5U, 0U, 200U, "%03d ct")),
    //          |>--------------+|
    ITEM_BACK()
);
MENU_SCREEN(settingsScreen, settingsScreenItems,
    //         |>--------------+|
    ITEM_WIDGET("speed",
        [](float speed) { STORAGE.dispenserSpeed = speed; },
         WIDGET_RANGE(STORAGE.dispenserSpeed.get(), 0.1f, 0.0f, 1.0f, "%.1f")),
    //         |>--------------+|
    ITEM_WIDGET("retries",
        [](unsigned int attempts) { STORAGE.maxDispenseAttempts = attempts; },
        WIDGET_RANGE(STORAGE.maxDispenseAttempts.get(), 1U, 1U, 10U, "%d")),
    //         |>--------------+|
    ITEM_WIDGET("wait",
        [](unsigned int time) { STORAGE.removalWaitTime = time; },
        WIDGET_RANGE(STORAGE.removalWaitTime.get(), 250U, 1000U, 10000U, "%d ms")),
    //         |>--------------+|
    ITEM_WIDGET("block",
        [](float current) { STORAGE.motorBlockCurrent = current; },
        WIDGET_RANGE(STORAGE.motorBlockCurrent.get(), 0.05f, 0.0f, 2.0f, "%.2f A")),
    //         |>--------------+|
    ITEM_WIDGET("inrush",
        [](unsigned int time) { STORAGE.motorBlockInrushWait = time; },
        WIDGET_RANGE(STORAGE.motorBlockInrushWait.get(), 5U, 0U, 250U, "%d ms")),
    //         |>--------------+|
    ITEM_SUBMENU("Coin config", coinScreen),
    ITEM_BACK()
);
MENU_SCREEN( mainScreen, mainItems,
    ITEM_SUBMENU("Info", infoScreen),
    ITEM_SUBMENU("Settings", settingsScreen),
    ITEM_COMMAND("Exit", []() { Display::instance->hideMenu(); })
);
// clang-format on

// Initialize the static instance
Display* const Display::instance = new Display(mainScreen);

Display::Display(MenuScreen* screen)
    : screen(screen),
      lcd(LiquidCrystal(LCD_PIN_RS, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7)),
      lcdAdapter(LiquidCrystalAdapter(&lcd, LCD_COLS, LCD_ROWS)),
      renderer(CharacterDisplayRenderer(&lcdAdapter, LCD_COLS, LCD_ROWS)),
      menu(LcdMenu(renderer)),
      rotary(SimpleRotary(ROTARY_ENC_PIN_CLK, ROTARY_ENC_PIN_DT, ROTARY_ENC_PIN_SW)),
      rotaryAdapter(SimpleRotaryAdapter(&menu, &rotary)),
      menuActive(false) {
    rotary.setDebounceDelay(ROTARY_ENC_DEBOUNCE_DELAY);
    rotary.setErrorDelay(ROTARY_ENC_ERROR_DELAY);
    rotary.setTrigger(HIGH);
}

void Display::begin() {
    renderer.begin();
    menu.setScreen(screen);
    if (menuActive) {
        menu.show();
    } else {
        menu.hide();
    }
}

void Display::loop() {
    if (menuActive) {
        rotaryAdapter.observe();
        menu.poll(1000);
    } else if (rotary.pushLong(SETTINGS_MENU_LONG_PRESS_MS)) {
        showMenu();
    } else {
        unsigned long now = millis();
        if (now - lastRender > (1000 / DISPLAY_FPS)) {
            lcd.setCursor(0, 0);
            lcd.write(text.c_str());
            lastRender = now;
        }
    }
}

void Display::setText(String text) {
    if (text == this->text) {
        return;
    }
    this->text = text;
    if (!menuActive) {
        lcd.clear();
        lastRender = 0;
    }
}

void Display::showMenu() {
    if (!menuActive) {
        menuActive = true;
        menu.show();
        menu.reset();
    }
}

void Display::hideMenu() {
    if (menuActive) {
        menuActive = false;
        menu.hide();
    }
}
