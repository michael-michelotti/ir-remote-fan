#include "remote.h"


/* Remote will be utilizing my NEC driver through these callbacks */
void NEC_Full_Frame_Received_Callback(NEC_Device_t *p_nec_dev)
{
	/* Extract character code from frame */
	char_code_t frame_char = ((p_nec_dev->last_frame >> 16) & 0xFF);
	Remote_Key_Press_Callback(frame_char);
}

void NEC_Hold_Frame_Received_Callback(NEC_Device_t *p_nec_dev)
{
	Remote_Key_Hold_Callback();
}

void NEC_Decode_Error_Callback(NEC_Device_t *p_nec_dev)
{
	/* Don't want to do anything on decode errors for now. */
	/* Could just mean remote was turned away from receiver mid-frame. */
}

void Remote_Listen(Remote_Device_t *remote)
{
	remote->state = REMOTE_STATE_INIT;
	NEC_Dev_Initialize();
	remote->state = REMOTE_STATE_IDLE;
}

const char *Remote_Decode_Char_Code(char_code_t char_to_decode)
{
	switch (char_to_decode)
	{
	case CHAR_CODE_DOWN: return "DOWN";
	case CHAR_CODE_4: return "4";
	case CHAR_CODE_UP: return "UP";
	case CHAR_CODE_1: return "1";
	case CHAR_CODE_ST: return "ST";
	case CHAR_CODE_VOLM: return "VOL-";
	case CHAR_CODE_0: return "0";
	case CHAR_CODE_2: return "2";
	case CHAR_CODE_EQ: return "EQ";
	case CHAR_CODE_5: return "5";
	case CHAR_CODE_PLAY: return "PLAY";
	case CHAR_CODE_7: return "7";
	case CHAR_CODE_FFW: return ">>";
	case CHAR_CODE_REW: return "<<";
	case CHAR_CODE_PWR: return "PWR";
	case CHAR_CODE_VOLP: return "VOL+";
	case CHAR_CODE_FUNC: return "FUNC";
	case CHAR_CODE_9: return "9";
	case CHAR_CODE_8: return "8";
	case CHAR_CODE_6: return "6";
	case CHAR_CODE_3: return "3";

	}
}

__weak void Remote_Key_Press_Callback(char_code_t pressed_char)
{
	/* Should be defined in the application code */
}

__weak void Remote_Key_Hold_Callback()
{
	/* Should be defined in the application code */
}
