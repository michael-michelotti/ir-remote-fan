#include "fan.h"
#include <stdio.h>

L293D_Handle_t l293d;

void Fan_Initialize(Fan_Device_t *fan_dev)
{
	fan_dev->state = FAN_STATE_INIT;

	L293D_Initialize(&l293d);
	L293D_Set_Direction_CCW(&l293d);
	L293D_Set_PWM_Duty_Cycle(&l293d, 0);

	fan_dev->direction = FAN_DIRECTION_CCW;
	fan_dev->state = FAN_STATE_OFF;
}

void Fan_Off(Fan_Device_t *fan_dev)
{
	printf("turning fan off\n");
	if (fan_dev->state == FAN_STATE_ON)
	{
		L293D_Disable(&l293d);
		L293D_Set_PWM_Duty_Cycle(&l293d, 0);
		fan_dev->state = FAN_STATE_OFF;
	}
}

void Fan_On(Fan_Device_t *fan_dev)
{
	printf("turning fan on\n");
	if (fan_dev->state == FAN_STATE_OFF)
	{
		L293D_Enable(&l293d);
		fan_dev->state = FAN_STATE_ON;
	}
}

void Fan_Set_Speed(Fan_Device_t *fan_dev, fan_speed_t speed)
{
	printf("Setting fan speed: %u\n", speed);
	L293D_Set_PWM_Duty_Cycle(&l293d, speed);
}
