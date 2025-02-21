#include "Display.h"

unsigned int infoScreen_sold;
unsigned int infoScreen_total;

// clang-format off
MENU_SCREEN(infoScreen, infoScreenItems,
    //        |>--------------+|
    ITEM_VALUE("Sold", infoScreen_sold, "%d"),
    //        |>--------------+|
    ITEM_COMMAND("Reset sold", []() { 
        Debug::log("Resetting sold items");
        Storage::instance->sold.set(0);
    }),
    //        |>--------------+|
    ITEM_VALUE("Total", infoScreen_total, "%d"),
    //        |>--------------+|
    ITEM_BASIC(GIT_VERSION),
    //        |>--------------+|
    ITEM_BACK()
);
MENU_SCREEN(coinScreen, coinScreenItems,
    //         |>--------------+|
    ITEM_WIDGET("interval",
        [](unsigned int delay) { Storage::instance->coinPulseMaxDelay.set(delay); },
        WIDGET_RANGE(Storage::instance->coinPulseMaxDelay.get(), 10U, 10U, 500U, "%d ms")),
    //         |>--------------+|
    ITEM_WIDGET("1 pulse",
        [](unsigned int value) { Storage::instance->coin1value.set(value); },
        WIDGET_RANGE(Storage::instance->coin1value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("2 pulse",
        [](unsigned int value) { Storage::instance->coin2value.set(value); },
        WIDGET_RANGE(Storage::instance->coin2value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("3 pulse",
        [](unsigned int value) { Storage::instance->coin3value.set(value); },
        WIDGET_RANGE(Storage::instance->coin3value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("4 pulse",
        [](unsigned int value) { Storage::instance->coin4value.set(value); },
        WIDGET_RANGE(Storage::instance->coin4value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("5 pulse",
        [](unsigned int value) { Storage::instance->coin5value.set(value); },
        WIDGET_RANGE(Storage::instance->coin5value.get(), 5U, 0U, 200U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("6 pulse",
        [](unsigned int value) { Storage::instance->coin6value.set(value); },
        WIDGET_RANGE(Storage::instance->coin6value.get(), 5U, 0U, 200U, "%03d ct")),
    //          |>--------------+|
    ITEM_BACK()
);
MENU_SCREEN(factoryResetScreen, factoryResetScreenItems,
    //         |>--------------+|
    ITEM_COMMAND("Reset",
        []() {
            Storage::instance->reset();
            Display::instance->hideMenu();
        }),
    //         |>--------------+|
    ITEM_BACK()
);

MENU_SCREEN(settingsScreen, settingsScreenItems,
    //         |>--------------+|
    ITEM_WIDGET("price",
        [](unsigned int price) { Storage::instance->price.set(price); },
         WIDGET_RANGE(Storage::instance->price.get(), 5U, 0U, 2500U, "%03d ct")),
    //         |>--------------+|
    ITEM_WIDGET("balance rst",
        [](unsigned int delay) { Storage::instance->balanceResetDelay.set(delay); },
        WIDGET_RANGE(Storage::instance->balanceResetDelay.get(), 5U, 0U, 600U, "%d s")),
    //         |>--------------+|
    ITEM_WIDGET("speed",
        [](float speed) { Storage::instance->dispenserSpeed.set(speed); },
         WIDGET_RANGE(Storage::instance->dispenserSpeed.get(), 0.1f, 0.0f, 1.0f, "%.1f")),
    //         |>--------------+|
    ITEM_WIDGET("retries",
        [](unsigned int attempts) { Storage::instance->maxDispenseAttempts.set(attempts); },
        WIDGET_RANGE(Storage::instance->maxDispenseAttempts.get(), 1U, 1U, 10U, "%d")),
    //         |>--------------+|
    ITEM_WIDGET("wait",
        [](unsigned int time) { Storage::instance->removalWaitTime.set(time); },
        WIDGET_RANGE(Storage::instance->removalWaitTime.get(), 250U, 1000U, 10000U, "%d ms")),
    //         |>--------------+|
    ITEM_WIDGET("block",
        [](float current) { Storage::instance->motorBlockCurrent.set(current); },
        WIDGET_RANGE(Storage::instance->motorBlockCurrent.get(), 0.05f, 0.0f, 2.0f, "%.2f A")),
    //         |>--------------+|
    ITEM_WIDGET("inrush",
        [](unsigned int time) { Storage::instance->motorBlockInrushWait.set(time); },
        WIDGET_RANGE(Storage::instance->motorBlockInrushWait.get(), 5U, 0U, 250U, "%d ms")),
    //         |>--------------+|
    ITEM_SUBMENU("Coin config", coinScreen),
    ITEM_SUBMENU("Factory reset", factoryResetScreen),
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
      lcd(LiquidCrystal_I2C(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS)),
      lcdAdapter(LiquidCrystal_I2CAdapter(&lcd)),
      renderer(CharacterDisplayRenderer(&lcdAdapter, LCD_COLS, LCD_ROWS)),
      menu(LcdMenu(renderer)),
      rotary(SimpleRotary(ROTARY_ENC_PIN_CLK, ROTARY_ENC_PIN_DT,
                          ROTARY_ENC_PIN_SW)),
      rotaryAdapter(SimpleRotaryAdapter(&menu, &rotary)),
      menuActive(false),
      lastRender(0),
      progressIndicatorEnabled(false),
      progressBarIndex(0) {
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
    infoScreen_sold = Storage::instance->sold.get();
    infoScreen_total = Storage::instance->total.get();
    if (menuActive) {
        rotaryAdapter.observe();
        menu.poll(1000);
    } else if (rotary.pushLong(SETTINGS_MENU_LONG_PRESS_MS)) {
        showMenu();
    } else {
        unsigned long now = millis();
        if (now - lastRender > (1000 / DISPLAY_FPS)) {
            lcd.setCursor(0, 0);
            int newLine = text.indexOf('\n');
            if (newLine != -1) {
                lcd.printstr(text.substring(0, newLine).c_str());
                lcd.setCursor(0, 1);
                lcd.printstr(text.substring(newLine + 1).c_str());
            } else {
                lcd.printstr(text.c_str());
            }
            if (progressIndicatorEnabled) {
                for (int i = 0; i < LCD_COLS; i++) {
                    progressBar[i] = ' ';
                }
                progressBar[LCD_COLS] = '\0';
                progressBar[progressBarIndex] = '.';
                progressBar[(progressBarIndex + 1) % LCD_COLS] = '.';
                progressBar[(progressBarIndex + 2) % LCD_COLS] = '.';
                progressBarIndex = (progressBarIndex + 1) % LCD_COLS;
                lcd.setCursor(0, 1);
                lcd.printstr(progressBar);
            }
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

void Display::enableProgressIndicator() {
    if (progressIndicatorEnabled) {
        return;
    }
    progressIndicatorEnabled = true;
    progressBarIndex = 0;
    lastRender = 0;
}

void Display::disableProgressIndicator() {
    if (!progressIndicatorEnabled) {
        return;
    }
    progressIndicatorEnabled = false;
    lcd.clear();
    lastRender = 0;
}