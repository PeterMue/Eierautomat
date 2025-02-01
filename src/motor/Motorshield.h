#pragma once

#include "Motor.h"

class Motorshield {
   private:
    // Private constructor to prevent instantiation
    Motorshield() {}

   public:
    // Static constant members for channelA and channelB
    static const Motor channelA() { return Motor(12, 3, 9, A0); }
    static const Motor channelB() { return Motor(13, 11, 8, A1); }
};