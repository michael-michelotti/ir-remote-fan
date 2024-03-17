#ifndef INC_FAN_H_
#define INC_FAN_H_

#include "l293d_driver.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef 	htim2;
extern TIM_HandleTypeDef 	htim5;
extern L293D_Handle_t 		l293d;
extern HAL_GPIO_Handle_t	en_gpio_handle;

#define fan_speed_t			uint8_t

/* fan_speed_t macros to feed into Fan_Set_Speed. */
#define FAN_SPEED_0			0
#define FAN_SPEED_1			10
#define FAN_SPEED_2			20
#define FAN_SPEED_3			30
#define FAN_SPEED_4			40
#define FAN_SPEED_5			50
#define FAN_SPEED_6			60
#define FAN_SPEED_7 		70
#define FAN_SPEED_8			80
#define FAN_SPEED_9			90

typedef enum
{
	FAN_STATE_OFF,
	FAN_STATE_INIT,
	FAN_STATE_ON
} Fan_State_t;

typedef enum
{
	FAN_DIRECTION_CW,
	FAN_DIRECTION_CCW
} Fan_Direction_t;


typedef struct
{
	Fan_State_t				state;
	Fan_Direction_t			direction;
	fan_speed_t				curr_speed;
} Fan_Device_t;

void Fan_Initialize(Fan_Device_t *fan_dev);
void Fan_Off(Fan_Device_t *fan_dev);
void Fan_On(Fan_Device_t *fan_dev);
void Fan_Set_Speed(Fan_Device_t *fan_dev, fan_speed_t speed);

#endif /* INC_FAN_H_ */
