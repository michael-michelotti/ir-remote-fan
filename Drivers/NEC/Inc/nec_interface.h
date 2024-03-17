#ifndef NEC_INC_NEC_INTERFACE_H_
#define NEC_INC_NEC_INTERFACE_H_

#include "l293d_driver.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

extern TIM_HandleTypeDef htim3;
#define NEC_TIME_SOURCE					htim3

#define NEC_GPIO_PIN					NEC_IN_Pin
#define NEC_GPIO_PORT					NEC_IN_GPIO_Port

#define NEC_BITS_PER_READ				32
#define NEC_EDGES_PER_BIT				2
#define NEC_EDGES_PER_READ				((NEC_BITS_PER_READ) * (NEC_EDGES_PER_BIT)) + 1
#define NEC_EDGES_IN_PREAMBLE			3
#define NEC_EDGES_PER_FRAME				(NEC_EDGES_PER_READ) + (NEC_EDGES_IN_PREAMBLE)

#define NEC_MIN_LOGIC_0					450 	/* us */
#define NEC_MAX_LOGIC_0					650 	/* us */
#define NEC_MIN_LOGIC_1					1500 	/* us */
#define NEC_MAX_LOGIC_1					1800 	/* us */

#define NEC_HOLD_FRAME_MIN				2000	/* us */
#define NEC_HOLD_FRAME_MAX				3000	/* us */
#define NEC_FULL_FRAME_MIN				4000	/* us */
#define NEC_FULL_FRAME_MAX				4600	/* us */

#define NEC_FRAME_HOLD_VALUE			0
#define NEC_FRAME_DECODE_ERROR			1

#define nec_frame_t						uint32_t

typedef enum
{
	NEC_STATE_INIT,
	NEC_STATE_IDLE,
	NEC_STATE_PREAMBLE,
	NEC_STATE_DATA
} NEC_State_t;

typedef struct
{
	NEC_State_t								state;
	uint16_t								gpio_pin_num;
	GPIO_TypeDef							*gpio_port;
	uint64_t								timestamp;
	int32_t									num_edges;
	struct { uint64_t ts; uint8_t value; }	edges[NEC_EDGES_PER_READ];
	struct { uint64_t ts; uint8_t value; }  preamble_edges[NEC_EDGES_IN_PREAMBLE];
	nec_frame_t								last_frame;
} NEC_Device_t;

void NEC_Dev_Initialize(NEC_Device_t *p_nec_dev);

void NEC_Full_Frame_Received_Callback(NEC_Device_t *p_nec_dev);
void NEC_Hold_Frame_Received_Callback(NEC_Device_t *p_nec_dev);
void NEC_Decode_Error_Callback(NEC_Device_t *p_nec_dev);
void NEC_Edge_Detected_Callback(NEC_Device_t *p_nec_dev);

#endif /* NEC_INC_NEC_INTERFACE_H_ */
