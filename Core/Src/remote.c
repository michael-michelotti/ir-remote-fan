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
	case 0x7: return "DOWN";
	case 0x8: return "4";
	case 0x9: return "UP";
	case 0xC: return "1";
	case 0xD: return "ST";
	case 0x15: return "VOL-";
	case 0x16: return "0";
	case 0x18: return "2";
	case 0x19: return "EQ";
	case 0x1C: return "5";
	case 0x40: return "PLAY";
	case 0x42: return "7";
	case 0x43: return ">>";
	case 0x44: return "<<";
	case 0x45: return "PWR";
	case 0x46: return "VOL+";
	case 0x47: return "FUNC";
	case 0x4A: return "9";
	case 0x52: return "8";
	case 0x5A: return "6";
	case 0x5E: return "3";

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
