#include "nec_interface.h"

static NEC_Device_t nec_dev;
uint64_t us_ticks;
uint8_t frame_num;

static uint32_t NEC_Decode(NEC_Device_t *p_nec_dev);


__weak void NEC_Full_Frame_Received_Callback(NEC_Device_t *p_nec_dev)
{
	/* Should be redefined by the consumer of this NEC interface */
	printf("Frame: 0x%" PRIx32 "\n", p_nec_dev->last_frame);
}

__weak void NEC_Hold_Frame_Received_Callback(NEC_Device_t *p_nec_dev)
{
	/* Should be redefined by the consumer of this NEC interface */
	printf("Hold frame\n");
}

__weak void NEC_Decode_Error_Callback(NEC_Device_t *p_nec_dev)
{
	/* Should be redefined by the consumer of this NEC interface */
	printf("Error decoding frame.");
}

void NEC_Dev_Initialize(void)
{
	nec_dev.num_edges = -1;
	GPIO_InitTypeDef nec_gpio_init;
	nec_gpio_init.Pin = NEC_GPIO_PIN;
	nec_gpio_init.Mode = NEC_GPIO_MODE;
	nec_gpio_init.Pull = NEC_GPIO_PULL;
	nec_gpio_init.Speed = NEC_GPIO_SPEED;

	HAL_GPIO_Handle_t nec_gpio_handle = {
			.gpio_init_typedef = nec_gpio_init,
			.curr_gpio_state = GPIO_PIN_RESET,
			.gpio_typedef = NEC_GPIO_PORT
	};

	nec_dev.p_gpio_handle = nec_gpio_handle;
	nec_dev.state = NEC_STATE_IDLE;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	us_ticks += 10;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint32_t second_preamble_pulse;

	switch (nec_dev.state)
	{
		case NEC_STATE_IDLE:
			nec_dev.timestamp = us_ticks = 0;
			nec_dev.preamble_edges[0].ts = us_ticks;
			nec_dev.num_edges = 1;
			nec_dev.state = NEC_STATE_PREAMBLE;
			break;

		case NEC_STATE_PREAMBLE:
			if (nec_dev.num_edges == 1)
			{
				nec_dev.preamble_edges[1].ts = us_ticks - nec_dev.timestamp;
				nec_dev.timestamp = us_ticks;
				nec_dev.num_edges += 1;
			}
			else if (nec_dev.num_edges == 2)
			{
				nec_dev.preamble_edges[2].ts = second_preamble_pulse = us_ticks - nec_dev.timestamp;
				nec_dev.timestamp = us_ticks;

				if ((second_preamble_pulse > NEC_HOLD_FRAME_MIN) &&
						(second_preamble_pulse < NEC_HOLD_FRAME_MAX))
				{
					/* This is a hold frame */
					nec_dev.num_edges += 1;
				}
				else if ((second_preamble_pulse > NEC_FULL_FRAME_MIN) &&
						(second_preamble_pulse < NEC_FULL_FRAME_MAX))
				{
					/* This is a full frame */
					nec_dev.num_edges = 0;
					nec_dev.state = NEC_STATE_DATA;
				}
				else
				{
					/* Preamble decode failed, reset state */
					nec_dev.num_edges = -1;
					nec_dev.state = NEC_STATE_IDLE;
					NEC_Decode_Error_Callback(&nec_dev);
				}
			}
			else if (nec_dev.num_edges == 3)
			{
				/* We only get here in a hold frame */
				nec_dev.num_edges = -1;
				nec_dev.state = NEC_STATE_IDLE;
				NEC_Hold_Frame_Received_Callback(&nec_dev);
			}
			break;

		case NEC_STATE_DATA:
			if (nec_dev.num_edges >= NEC_EDGES_PER_READ - 1)
			{
				/* Read complete, enter decode logic */
				nec_dev.last_frame = NEC_Decode(&nec_dev);
				nec_dev.state = NEC_STATE_IDLE;
				nec_dev.num_edges = -1;

				if (nec_dev.last_frame == NEC_FRAME_DECODE_ERROR)
				{
					NEC_Decode_Error_Callback(&nec_dev);
				}
				else
				{
					NEC_Full_Frame_Received_Callback(&nec_dev);
				}
			}
			else
			{
				nec_dev.edges[nec_dev.num_edges].ts = us_ticks - nec_dev.timestamp;
				nec_dev.edges[nec_dev.num_edges].value = HAL_GPIO_ReadPin(nec_dev.p_gpio_handle.gpio_typedef, NEC_IN_Pin);
				nec_dev.timestamp = us_ticks;
				nec_dev.num_edges += 1;
			}
			break;
	}
}

static uint32_t NEC_Decode(NEC_Device_t *p_nec_dev)
{
	uint32_t nec_frame = 0;
	uint32_t timedelta;
	for (int i = 0; i < NEC_BITS_PER_READ; i++)
	{
		timedelta = p_nec_dev->edges[(i * 2) + 1].ts;
		if ((timedelta > NEC_MIN_LOGIC_1) && (timedelta < NEC_MAX_LOGIC_1))
		{
			nec_frame |= (1 << i);
		}
		else if (!((timedelta > NEC_MIN_LOGIC_0) && (timedelta < NEC_MAX_LOGIC_0)))
		{
			/* Pulse not between logic 0 or logic 1, failure to decode */
			return NEC_FRAME_DECODE_ERROR;
		}
	}

	return nec_frame;
}

