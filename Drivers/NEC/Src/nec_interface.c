#include "nec_interface.h"

/* Private counter variable for NEC devices - counts in us. */
/* Triggers off TIM3, which is configured to produce an interrupt every 10us. */
static uint64_t us_ticks;


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

void NEC_Dev_Initialize(NEC_Device_t *p_nec_dev)
{
	p_nec_dev->state = NEC_STATE_INIT;
	p_nec_dev->num_edges = -1;
	p_nec_dev->gpio_pin_num = NEC_GPIO_PIN;
	p_nec_dev->gpio_port = NEC_GPIO_PORT;
	HAL_TIM_Base_Start_IT(&NEC_TIME_SOURCE);
	p_nec_dev->state = NEC_STATE_IDLE;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	us_ticks += 10;
}

void NEC_Edge_Detected_Callback(NEC_Device_t *p_nec_dev)
{
	uint32_t second_preamble_pulse;
	switch (p_nec_dev->state)
	{
		case NEC_STATE_IDLE:
			p_nec_dev->timestamp = us_ticks = 0;
			p_nec_dev->preamble_edges[0].ts = us_ticks;
			p_nec_dev->num_edges = 1;
			p_nec_dev->state = NEC_STATE_PREAMBLE;
			break;

		case NEC_STATE_PREAMBLE:
			if (p_nec_dev->num_edges == 1)
			{
				p_nec_dev->preamble_edges[1].ts = us_ticks - p_nec_dev->timestamp;
				p_nec_dev->timestamp = us_ticks;
				p_nec_dev->num_edges += 1;
			}
			else if (p_nec_dev->num_edges == 2)
			{
				p_nec_dev->preamble_edges[2].ts = second_preamble_pulse = us_ticks - p_nec_dev->timestamp;
				p_nec_dev->timestamp = us_ticks;

				if ((second_preamble_pulse > NEC_HOLD_FRAME_MIN) &&
						(second_preamble_pulse < NEC_HOLD_FRAME_MAX))
				{
					/* This is a hold frame */
					p_nec_dev->num_edges += 1;
				}
				else if ((second_preamble_pulse > NEC_FULL_FRAME_MIN) &&
						(second_preamble_pulse < NEC_FULL_FRAME_MAX))
				{
					/* This is a full frame */
					p_nec_dev->num_edges = 0;
					p_nec_dev->state = NEC_STATE_DATA;
				}
				else
				{
					/* Preamble decode failed, reset state */
					p_nec_dev->num_edges = -1;
					p_nec_dev->state = NEC_STATE_IDLE;
					NEC_Decode_Error_Callback(p_nec_dev);
				}
			}
			else if (p_nec_dev->num_edges == 3)
			{
				/* We only get here in a hold frame */
				p_nec_dev->num_edges = -1;
				p_nec_dev->state = NEC_STATE_IDLE;
				NEC_Hold_Frame_Received_Callback(p_nec_dev);
			}
			break;

		case NEC_STATE_DATA:
			if (p_nec_dev->num_edges >= NEC_EDGES_PER_READ - 1)
			{
				/* Read complete, enter decode logic */
				p_nec_dev->last_frame = NEC_Decode(p_nec_dev);
				p_nec_dev->state = NEC_STATE_IDLE;
				p_nec_dev->num_edges = -1;

				if (p_nec_dev->last_frame == NEC_FRAME_DECODE_ERROR)
				{
					NEC_Decode_Error_Callback(p_nec_dev);
				}
				else
				{
					NEC_Full_Frame_Received_Callback(p_nec_dev);
				}
			}
			else
			{
				p_nec_dev->edges[p_nec_dev->num_edges].ts = us_ticks - p_nec_dev->timestamp;
				p_nec_dev->edges[p_nec_dev->num_edges].value = HAL_GPIO_ReadPin(p_nec_dev->gpio_port, p_nec_dev->gpio_pin_num);
				p_nec_dev->timestamp = us_ticks;
				p_nec_dev->num_edges += 1;
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

