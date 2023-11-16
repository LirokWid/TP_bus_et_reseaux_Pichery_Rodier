#include "motor.h"
extern CAN_HandleTypeDef hcan1;

int motor_angle = 0; // entre -180 et +180

void reset_motor_angle()
{
	uint32_t mailbox;
	CAN_TxHeaderTypeDef header;
	header.DLC 						= 0;
	header.ExtId					= 0;//message ID extended
	header.IDE						= CAN_ID_STD; 	//Trame standard ou étendue
	header.RTR						= CAN_RTR_DATA; //Trame n'est pas remote
	header.StdId					= 0x62;			//Message ID
	header.TransmitGlobalTime		= DISABLE;		//Message ID étendu
	if (HAL_CAN_AddTxMessage(&hcan1, &header, 0, &mailbox) != HAL_OK)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		Error_Handler ();
	}
}

int get_motor_angle()
{
	return motor_angle;
}
int set_motor_angle(int angle)
{
	uint8_t data[2];
	uint32_t mailbox;
	CAN_TxHeaderTypeDef header;
	header.DLC 						= 2;
	header.ExtId					= 0;//message ID extended
	header.IDE						= CAN_ID_STD; 	//Trame standard ou étendue
	header.RTR						= CAN_RTR_DATA; //Trame n'est pas remote
	header.StdId					= 0x61;			//Message ID
	header.TransmitGlobalTime		= DISABLE;		//Message ID étendu
	if ((angle>180)&&(angle<180))
	{
		return -1;
	}else{
		if(angle>=0)
		{
			data[1]=1;//sens trigo
			data[0]=abs(angle);
		}else{
			data[1]=0;//sens anti-trigo
			data[0]=abs(angle);
		}
		  if (HAL_CAN_AddTxMessage(&hcan1, &header, data, &mailbox) != HAL_OK)
		  {
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		     Error_Handler ();
		  }
		  return 0;
	}
}
