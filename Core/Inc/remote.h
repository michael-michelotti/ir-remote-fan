#ifndef INC_REMOTE_H_
#define INC_REMOTE_H_

#include "nec_interface.h"
#include "stm32f4xx_hal.h"
#include <inttypes.h>

#define CHAR_CODE_DOWN	0x7
#define CHAR_CODE_4		0x8
#define CHAR_CODE_UP	0x9
#define CHAR_CODE_1		0xC
#define CHAR_CODE_ST	0xD
/* VOLM = volume minus */
#define CHAR_CODE_VOLM	0x15
#define CHAR_CODE_0		0x16
#define CHAR_CODE_2		0x18
#define CHAR_CODE_EQ	0x19
#define CHAR_CODE_5		0x1C
#define CHAR_CODE_PLAY	0x40
#define CHAR_CODE_7		0x42
#define CHAR_CODE_FFW	0x43
#define CHAR_CODE_REW	0x44
#define CHAR_CODE_PWR	0x45
/* VOLP = volume plus */
#define CHAR_CODE_VOLP	0x46
#define CHAR_CODE_FUNC	0x47
#define CHAR_CODE_9		0x4A
#define CHAR_CODE_8		0x52
#define CHAR_CODE_6		0x5A
#define CHAR_CODE_3		0x5E

#define char_code_t		uint8_t
#define MAX_CHARS		4

typedef enum
{
	REMOTE_STATE_INIT,
	REMOTE_STATE_IDLE,
	REMOTE_STATE_ERROR,
	REMOTE_STATE_RECEIVING
} Remote_State_t;

typedef struct
{
	Remote_State_t 		state;
	char_code_t			last_char_code;
	const char			*last_char_str;
} Remote_Device_t;

void Remote_Listen(Remote_Device_t *remote);

void Remote_Key_Press_Callback(char_code_t pressed_char);
const char *Remote_Decode_Char_Code(char_code_t char_to_decode);
void Remote_Key_Hold_Callback();

#endif /* INC_REMOTE_H_ */
