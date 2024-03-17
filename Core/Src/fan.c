#include "fan.h"
#include <stdio.h>

void Fan_Initialize(Fan_Device_t *fan_dev)
{
	fan_dev->state = FAN_STATE_INIT;

	l293d.state = L293D_DISABLED;
	l293d.in1_pwm = &htim2;
	l293d.in2_pwm = &htim5;
	l293d.en_gpio = &en_gpio_handle;

	L293D_Set_Direction_CCW(&l293d);
	L293D_Set_PWM_Duty_Cycle(&l293d, 50);

	fan_dev->direction = FAN_DIRECTION_CCW;
	fan_dev->state = FAN_STATE_OFF;
}

void Fan_Off(Fan_Device_t *fan_dev)
{
	if (fan_dev->state == FAN_STATE_ON)
	{
		L293D_Disable(&l293d);
		fan_dev->state = FAN_STATE_OFF;
	}
	printf("turning fan off\n");
}

void Fan_On(Fan_Device_t *fan_dev)
{
	if (fan_dev->state == FAN_STATE_OFF)
	{
		L293D_Enable(&l293d);
		fan_dev->state = FAN_STATE_ON;
	}
	printf("turning fan on\n");
}

void Fan_Set_Speed(Fan_Device_t *fan_dev, fan_speed_t speed)
{
	printf("setting fan speed: %u\n", speed);
	L293D_Set_PWM_Duty_Cycle(&l293d, speed);
}
