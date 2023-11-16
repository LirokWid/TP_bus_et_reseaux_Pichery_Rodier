/*
 * uart4.c
 *
 *  Created on: Oct 17, 2023
 *      Author: maxim
 */

#include "uart4.h"


char RxBuff[RX_BUFF_SIZE] = {0};

uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];

enum CMD_type *pointerCMD = Null;

void Prepare_Receive(enum CMD_type *pCMD)
{
	HAL_UART_Receive_IT(&huart4, RxBuff, RX_BUFF_SIZE);
	pointerCMD = pCMD;
	int uartTxStringLength = snprintf((char *)uartTxBuffer,UART_TX_BUFFER_SIZE, "Ready\r\n");
	HAL_UART_Transmit(&huart4, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
	printf("Ready\r\n");
}

void Print_RESULT_float(enum CMD_type CMD, float val)
{
	switch(CMD)
	{
	int uartTxStringLength = 0;
	case GET_T:
		uartTxStringLength = snprintf((char *)uartTxBuffer,UART_TX_BUFFER_SIZE, "T = + %2.2f_C\r\n", val);
		HAL_UART_Transmit(&huart4, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
		break;
	case GET_P:
		uartTxStringLength = snprintf((char *)uartTxBuffer,UART_TX_BUFFER_SIZE, "T = +%8.2f_Pa\r\n", val);
		HAL_UART_Transmit(&huart4, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
		break;
	case GET_A:
		uartTxStringLength = snprintf((char *)uartTxBuffer,UART_TX_BUFFER_SIZE, "A = %3.0f_Degree\r\n", val);
		HAL_UART_Transmit(&huart4, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
		break;
	case GET_K:
		uartTxStringLength = snprintf((char *)uartTxBuffer,UART_TX_BUFFER_SIZE, "GET_K Cmd is good\r\n");
		HAL_UART_Transmit(&huart4, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
		break;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if     ((strcmp(RxBuff, "GET_T\r"))==0) *pointerCMD = GET_T;
	else if((strcmp(RxBuff, "GET_P\r"))==0) *pointerCMD = GET_P;
	else if((strcmp(RxBuff, "SET_K=" ))==0) *pointerCMD = SET_K;
	else if((strcmp(RxBuff, "GET_K\r"))==0) *pointerCMD = GET_K;
	else if((strcmp(RxBuff, "GET_A\r"))==0) *pointerCMD = GET_A;
	else
	{
		int uartTxStringLength = snprintf((char *)uartTxBuffer,UART_TX_BUFFER_SIZE, "Cmd not found\r\n");
		HAL_UART_Transmit(&huart4, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
		printf("Cmd not found\r\n");
	}

	HAL_UART_Receive_IT(&huart4, RxBuff, RX_BUFF_SIZE);
}
