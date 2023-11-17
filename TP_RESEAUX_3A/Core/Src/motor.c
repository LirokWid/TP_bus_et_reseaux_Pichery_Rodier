/**
 * @file motor.c
 * @brief Motor control functions using CAN communication.
 */

#include "motor.h"

extern CAN_HandleTypeDef hcan1; /**< External CAN handle. */

int motor_angle = 0; /**< Motor angle in degrees (-180 to +180). */

/**
 * @brief Resets the motor angle to zero.
 *
 * This function sends a CAN message to reset the motor angle.
 * @note The motor angle is set to zero after calling this function.
 */
void reset_motor_angle()
{
    uint32_t mailbox;
    CAN_TxHeaderTypeDef header;

    header.DLC                    = 0;
    header.ExtId                  = 0; /**< Message ID extended. */
    header.IDE                    = CAN_ID_STD; /**< Standard or extended frame. */
    header.RTR                    = CAN_RTR_DATA; /**< Data frame, not remote. */
    header.StdId                  = 0x62; /**< Message ID. */
    header.TransmitGlobalTime     = DISABLE; /**< Extended message ID. */

    if (HAL_CAN_AddTxMessage(&hcan1, &header, 0, &mailbox) != HAL_OK)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
        Error_Handler();
    }
}

/**
 * @brief Gets the current motor angle.
 *
 * @return Current motor angle in degrees.
 */
int get_motor_angle()
{
    return motor_angle;
}

/**
 * @brief Sets the motor angle.
 *
 * This function sets the motor angle and sends a CAN message with the new angle.
 *
 * @param angle The desired motor angle in degrees (-180 to +180).
 * @return 0 on success, -1 if the angle is out of range.
 */
int set_motor_angle(int angle)
{
    uint8_t data[2];
    uint32_t mailbox;
    CAN_TxHeaderTypeDef header;

    header.DLC                    = 2;
    header.ExtId                  = 0; /**< Message ID extended. */
    header.IDE                    = CAN_ID_STD; /**< Standard or extended frame. */
    header.RTR                    = CAN_RTR_DATA; /**< Data frame, not remote. */
    header.StdId                  = 0x61; /**< Message ID. */
    header.TransmitGlobalTime     = DISABLE; /**< Extended message ID. */

    if ((angle > 180) || (angle < -180))
    {
        return -1; /**< Invalid angle range. */
    }
    else
    {
        if (angle >= 0)
        {
            data[1] = 1; /**< Trigonometric direction. */
            data[0] = abs(angle);
        }
        else
        {
            data[1] = 0; /**< Anti-trigonometric direction. */
            data[0] = abs(angle);
        }

        if (HAL_CAN_AddTxMessage(&hcan1, &header, data, &mailbox) != HAL_OK)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
            Error_Handler();
        }
        return 0;
    }
}
