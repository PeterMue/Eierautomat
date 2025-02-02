#pragma once

#include <Arduino.h>
#include <ItemBack.h>
#include <ItemCommand.h>
#include <ItemSubMenu.h>
#include <ItemValue.h>
#include <ItemWidget.h>
#include <LcdMenu.h>
#include <LiquidCrystal.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystalAdapter.h>
#include <input/SimpleRotaryAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <widget/WidgetBool.h>
#include <widget/WidgetList.h>
#include <widget/WidgetRange.h>

#include "Storage.h"
#include "config.h"

class Display {
   public:
    void begin();
    void loop();
    void showMenu();
    void hideMenu();
    void setText(String text);

    static Display* const instance;

   private:
    Display(MenuScreen* screen);
    MenuScreen* screen;

    LiquidCrystal lcd;
    LiquidCrystalAdapter lcdAdapter;
    CharacterDisplayRenderer renderer;

    LcdMenu menu;

    SimpleRotary rotary;
    SimpleRotaryAdapter rotaryAdapter;

    bool menuActive;
    String text;

    unsigned long lastRender;
};
