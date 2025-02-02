#pragma once

#include "Dispenser.h"
#include "config.h"

class Dispensers {
   public:
    static Dispensers *const instance;

    void begin();
    void loop();

    bool ready();
    bool empty();

    void swap();

    Dispenser *const dispense();

   private:
    Dispensers(Dispenser *const dispensersA, Dispenser *const dispensersB);

    Dispenser *const dispenserA;
    Dispenser *const dispenserB;
    Dispenser *activeDispenser;
};