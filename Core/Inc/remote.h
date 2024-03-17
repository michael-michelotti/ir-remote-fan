#ifndef INC_REMOTE_H_
#define INC_REMOTE_H_

#include "nec_interface.h"
#include "stm32f4xx_hal.h"
#include <inttypes.h>


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
