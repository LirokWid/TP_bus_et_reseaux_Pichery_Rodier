/**
 * @file motor.h
 * @brief Header file for motor control functions using CAN communication.
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "main.h"

/**
 * @brief Resets the motor angle to zero.
 *
 * This function sends a CAN message to reset the motor angle.
 * @note The motor angle is set to zero after calling this function.
 */
void reset_motor_angle();

/**
 * @brief Gets the current motor angle.
 *
 * @return Current motor angle in degrees.
 */
int get_motor_angle();

/**
 * @brief Sets the motor angle.
 *
 * This function sets the motor angle and sends a CAN message with the new angle.
 *
 * @param angle The desired motor angle in degrees (-180 to +180).
 * @return 0 on success, -1 if the angle is out of range.
 */
int set_motor_angle(int angle);

#endif /* MOTOR_H */
