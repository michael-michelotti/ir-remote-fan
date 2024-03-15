#ifndef L293D_INC_L293D_DRIVER_H_
#define L293D_INC_L293D_DRIVER_H_

#include "stm32f4xx_hal.h"

#define L293D_IN1_TIM_CH	TIM_CHANNEL_1
#define L293D_IN2_TIM_CH	TIM_CHANNEL_4

/* return status typedef? */

/* state typedef? */
typedef enum
{
	L293D_DISABLED,
	L293D_ENABLED_CCW,
	L293D_ENABLED_CW,
} L293D_State_t;

typedef struct
{
	GPIO_InitTypeDef 	gpio_init_typedef;
	GPIO_TypeDef		*gpio_typedef;
	GPIO_PinState		curr_gpio_state;
} HAL_GPIO_Handle_t;

/* handle typedef? */
typedef struct
{
	TIM_HandleTypeDef 	*in1_pwm;
	TIM_HandleTypeDef	*in2_pwm;
	TIM_HandleTypeDef	*active_channel;
	HAL_GPIO_Handle_t	*en_gpio;
	L293D_State_t		state;
} L293D_Handle_t;


void L293D_Enable(L293D_Handle_t *p_l293d);
void L293D_Disable(L293D_Handle_t *p_l293d);

void L293D_Set_Direction_CCW(L293D_Handle_t *p_l293d);
void L293D_Set_Direction_CW(L293D_Handle_t *p_l293d);

void L293D_Set_PWM_Duty_Cycle(L293D_Handle_t *p_l293d, uint8_t duty_cycle);
void L293D_Set_PWM_Frequency(L293D_Handle_t *p_l293d, uint32_t freq);

#endif /* L293D_INC_L293D_DRIVER_H_ */
