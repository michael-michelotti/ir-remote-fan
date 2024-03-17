#ifndef L293D_INC_L293D_DRIVER_H_
#define L293D_INC_L293D_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "main.h"

extern TIM_HandleTypeDef 	htim2;
extern TIM_HandleTypeDef 	htim5;

#define L293D_IN1_TIM_CH	TIM_CHANNEL_1
#define L293D_IN2_TIM_CH	TIM_CHANNEL_4

typedef enum
{
	L293D_DISABLED,
	L293D_ENABLED,
	L293D_ERROR
} L293D_State_t;

typedef struct
{
	TIM_HandleTypeDef 	*in1_pwm;
	TIM_HandleTypeDef	*in2_pwm;
	TIM_HandleTypeDef	*active_channel;
	uint16_t			gpio_pin_num;
	GPIO_TypeDef		*gpio_port;
	L293D_State_t		state;
} L293D_Handle_t;


void L293D_Initialize(L293D_Handle_t *p_l293d);

void L293D_Enable(L293D_Handle_t *p_l293d);
void L293D_Disable(L293D_Handle_t *p_l293d);

void L293D_Set_Direction_CCW(L293D_Handle_t *p_l293d);
void L293D_Set_Direction_CW(L293D_Handle_t *p_l293d);

void L293D_Set_PWM_Duty_Cycle(L293D_Handle_t *p_l293d, uint8_t duty_cycle);
void L293D_Set_PWM_Frequency(L293D_Handle_t *p_l293d, uint32_t freq);

void L293D_Error_Handler(void);

#endif /* L293D_INC_L293D_DRIVER_H_ */
