#pragma once

#include "Motor.h"

class Motorshield {
   private:
    // Private constructor to prevent instantiation
    Motorshield() {}

   public:
    // Static constant members for channelA and channelB
    static Motor *const channelA;
    static Motor *const channelB;
};