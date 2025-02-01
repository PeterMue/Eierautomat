#include "Motorshield.h"

Motor *const Motorshield::channelA = new Motor(12, 3, 9, A0);
Motor *const Motorshield::channelB = new Motor(13, 11, 8, A1);