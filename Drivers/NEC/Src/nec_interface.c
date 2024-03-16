#include "nec_interface.h"


static NEC_Device_t nec_dev;
unsigned long long us_ticks;

static uint32_t NEC_Decode();


void NEC_Dev_Initialize()
{
	nec_dev.num_edges = -1;
	GPIO_InitTypeDef nec_gpio_init;
	nec_gpio_init.Pin = NEC_IN_Pin;
	nec_gpio_init.Mode = GPIO_MODE_IT_RISING_FALLING;
	nec_gpio_init.Pull = GPIO_NOPULL;
	nec_gpio_init.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Handle_t nec_gpio_handle = {
			.gpio_init_typedef = nec_gpio_init,
			.curr_gpio_state = GPIO_PIN_RESET,
			.gpio_typedef = NEC_IN_GPIO_Port
	};

	nec_dev.p_gpio_handle = nec_gpio_handle;
	nec_dev.state = NEC_STATE_IDLE;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	us_ticks += 10;
}

/* this is the callback that will trigger every time the input pin goes either high or low */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t pin_val;

	/* 1. Determine whether pin went high, or went low? */
	if (nec_dev.state == NEC_STATE_IDLE)
	{
		pin_val = HAL_GPIO_ReadPin(NEC_IN_GPIO_Port, NEC_IN_Pin);
		nec_dev.num_edges = 1;
		nec_dev.state = NEC_STATE_PREAMBLE;
	}
	else if (nec_dev.state == NEC_STATE_PREAMBLE)
	{
		nec_dev.num_edges += 1;
		if (nec_dev.num_edges == 2)
		{
			/* Premable complete, entering read stage */
			nec_dev.state = NEC_STATE_DATA;
			nec_dev.num_edges = 0;
		}
	}
	else if (nec_dev.state == NEC_STATE_DATA)
	{
		if (nec_dev.num_edges == NEC_EDGES_PER_READ)
		{
			/* Read complete, enter decode logic */
			nec_dev.last_frame = NEC_Decode();
			nec_dev.state = NEC_STATE_IDLE;
		}

		if (nec_dev.num_edges == 0)
		{
			nec_dev.edges[nec_dev.num_edges].ts = us_ticks;
		}
		else
		{
			nec_dev.edges[nec_dev.num_edges].ts = us_ticks - nec_dev.timestamp;
		}
		nec_dev.edges[nec_dev.num_edges].value = HAL_GPIO_ReadPin(nec_dev.p_gpio_handle.gpio_typedef, NEC_IN_Pin);
		nec_dev.timestamp = us_ticks;
		nec_dev.num_edges += 1;
	}
}

static uint32_t NEC_Decode()
{
	uint32_t nec_frame = 0;
	for (int i = 1; i < NEC_BITS_PER_READ + 1; i++)
	{
		uint32_t timedelta = nec_dev.edges[(i * 2)].ts;
		if ((timedelta > NEC_MIN_LOGIC_0) && (timedelta < NEC_MAX_LOGIC_0))
		{
			nec_frame &= ~(1 << (i - 1));
		}
		else if ((timedelta > NEC_MIN_LOGIC_1) && (timedelta < NEC_MAX_LOGIC_1))
		{
			nec_frame |= (1 << (i - 1));
		}
		else
		{
			/* failure to decode */
			nec_dev.num_edges = -1;
			nec_dev.state = NEC_STATE_IDLE;
			return -1;
		}
	}

	nec_dev.num_edges = -1;
	nec_dev.state = NEC_STATE_IDLE;
	return nec_frame;
}

