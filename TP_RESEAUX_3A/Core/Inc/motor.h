#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "main.h"

void reset_motor_angle();
int get_motor_angle();
int set_motor_angle(int angle);
#endif
