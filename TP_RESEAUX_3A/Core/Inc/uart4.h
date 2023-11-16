/*
 * uart4.h
 *
 *  Created on: Oct 17, 2023
 *      Author: maxim
 */

#include "main.h"
#include "stdio.h"
#include "string.h"

#define RX_BUFF_SIZE 6
extern UART_HandleTypeDef huart4;
#define UART_TX_BUFFER_SIZE 64

enum CMD_type{Null, GET_T, GET_P, SET_K, GET_K, GET_A, SET_A};

#ifndef INC_UART4_H_
#define INC_UART4_H_

void Prepare_Receive(enum CMD_type *pCMD);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Print_RESULT_float(enum CMD_type CMD, float val);


#endif /* INC_UART4_H_ */
