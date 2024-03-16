#ifndef NEC_INC_NEC_INTERFACE_H_
#define NEC_INC_NEC_INTERFACE_H_

#include "l293d_driver.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdint.h>

#define NEC_BITS_PER_READ				32
#define NEC_EDGES_PER_BIT				2
#define NEC_EDGES_PER_READ				(NEC_BITS_PER_READ) * (NEC_EDGES_PER_BIT) + 1
#define NEC_EDGES_IN_PREAMBLE			3
#define NEC_EDGES_PER_FRAME				(NEC_EDGES_PER_READ) + (NEC_EDGES_IN_PREAMBLE)

#define NEC_MIN_LOGIC_0					500 	/* us */
#define NEC_MAX_LOGIC_0					650 	/* us */
#define NEC_MIN_LOGIC_1					1500 	/* us */
#define NEC_MAX_LOGIC_1					1800 	/* us */

#define nec_frame_t						uint32_t

typedef enum
{
	NEC_STATE_IDLE,
	NEC_STATE_PREAMBLE,
	NEC_STATE_DATA
} NEC_State_t;

typedef struct
{
	NEC_State_t								state;
	HAL_GPIO_Handle_t  						p_gpio_handle;
	uint32_t								timestamp;
	int										num_edges;
	struct { uint32_t ts; uint8_t value; }	edges[NEC_EDGES_PER_READ];
	nec_frame_t								last_frame;
} NEC_Device_t;

#endif /* NEC_INC_NEC_INTERFACE_H_ */
