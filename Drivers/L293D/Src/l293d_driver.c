#include "l293d_driver.h"

static TIM_OC_InitTypeDef L293D_Get_New_OC_Init(TIM_HandleTypeDef *p_tim, uint8_t duty_cycle);

void L293D_Enable(L293D_Handle_t *p_l293d)
{
	HAL_GPIO_WritePin(p_l293d->en_gpio->gpio_typedef,
			p_l293d->en_gpio->gpio_init_typedef.Pin,
			GPIO_PIN_SET);
}

void L293D_Disable(L293D_Handle_t *p_l293d)
{
	HAL_GPIO_WritePin(p_l293d->en_gpio->gpio_typedef,
			p_l293d->en_gpio->gpio_init_typedef.Pin,
			GPIO_PIN_RESET);
}

void L293D_Set_Direction_CCW(L293D_Handle_t *p_l293d)
{
	HAL_TIM_PWM_Stop(p_l293d->in2_pwm, L293D_IN2_TIM_CH);
	HAL_TIM_PWM_Start(p_l293d->in1_pwm, L293D_IN1_TIM_CH);
	p_l293d->active_channel = p_l293d->in1_pwm;
}

void L293D_Set_Direction_CW(L293D_Handle_t *p_l293d)
{
	HAL_TIM_PWM_Stop(p_l293d->in1_pwm, L293D_IN1_TIM_CH);
	HAL_TIM_PWM_Start(p_l293d->in2_pwm, L293D_IN2_TIM_CH);
	p_l293d->active_channel = p_l293d->in2_pwm;
}

void L293D_Set_PWM_Duty_Cycle(L293D_Handle_t *p_l293d, uint8_t duty_cycle)
{
	TIM_OC_InitTypeDef in1_oc_init = L293D_Get_New_OC_Init(p_l293d->in1_pwm, duty_cycle);
	TIM_OC_InitTypeDef in2_oc_init = L293D_Get_New_OC_Init(p_l293d->in2_pwm, duty_cycle);


	if (HAL_TIM_PWM_ConfigChannel(p_l293d->in1_pwm, &in1_oc_init, L293D_IN1_TIM_CH) != HAL_OK)
	{
		__disable_irq();
		while (1) {}
	}

	if (HAL_TIM_PWM_ConfigChannel(p_l293d->in2_pwm, &in2_oc_init, L293D_IN2_TIM_CH) != HAL_OK)
	{
		__disable_irq();
		while (1) {}
	}
}

static TIM_OC_InitTypeDef L293D_Get_New_OC_Init(TIM_HandleTypeDef *p_tim, uint8_t duty_cycle)
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	uint32_t curr_arr = p_tim->Init.Period;
	uint32_t new_pulse = (curr_arr * duty_cycle) / 100;
	sConfigOC.Pulse = new_pulse;
	return sConfigOC;
}
