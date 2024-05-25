#include "task_handler.h"


//-----------------------------------------------------------------------------------------------------------------------
/****** VARIABLES ******/
uint16_t time_val = 0;
static int handler_state;
static int prev_handler_state;

void enter_handler_state(int state)
{
    switch(state){
    //--------------------------------------------------------------------------------------
        case IDLE:
			/****** LED2 ******/
        	HAL_GPIO_WritePin(LD2, LD2_PIN, GPIO_PIN_RESET);
        break;
    //--------------------------------------------------------------------------------------
        case LEDON:
			/****** LED2 ******/
        	HAL_GPIO_WritePin(LD2, LD2_PIN, GPIO_PIN_SET);
        break;
    //--------------------------------------------------------------------------------------
        case LEDBLINK:

        break;
    //--------------------------------------------------------------------------------------
        default:
        break;
    //--------------------------------------------------------------------------------------
    }
}

//-----------------------------------------------------------------------------------------------------------------------

void exec_handler_state(int state)
{
    switch(state){
    //--------------------------------------------------------------------------------------
        case IDLE:

        break;
    //--------------------------------------------------------------------------------------
        case LEDON:
        	// LEDON -> IDLE
			if(HAL_GPIO_ReadPin(B1, B1_PIN) && time_val > 1000)
			{
				set_handler_state(IDLE);
			}
			// LEDON -> LEDBlink
			// B1 Interrupt
        break;
    //--------------------------------------------------------------------------------------
        case LEDBLINK:
        	// LEDON -> IDLE
			if(HAL_GPIO_ReadPin(B1, B1_PIN) && time_val > 1000)
			{
				set_handler_state(IDLE);
			}
			// LEDBLINK
			else if (time_val > 1000)
			{
				HAL_GPIO_TogglePin(LD2, LD2_PIN);
				time_val = 0;
			}
        break;
      //--------------------------------------------------------------------------------------
      default:
      break;
      //--------------------------------------------------------------------------------------
    }
}

//-----------------------------------------------------------------------------------------------------------------------

void leave_handler_state(int state)
{
    switch(state){
    //--------------------------------------------------------------------------------------
        case IDLE:
        break;
    //--------------------------------------------------------------------------------------
        case LEDON:
        break;
    //--------------------------------------------------------------------------------------
        case LEDBLINK:
        break;
    //--------------------------------------------------------------------------------------
        default:
        break;
    //--------------------------------------------------------------------------------------
    }
}

//-----------------------------------------------------------------------------------------------------------------------

void handler_task(void)
{
    exec_handler_state(handler_state);
}

void set_handler_state(int state)
{
    prev_handler_state = handler_state;
    handler_state = state;
    leave_handler_state(prev_handler_state);
    enter_handler_state(state);
}

void increase_handler_state(void)
{
	if(handler_state <= LEDON)
	{
	    prev_handler_state = handler_state;
	    handler_state = handler_state + 1;
	    leave_handler_state(prev_handler_state);
	    enter_handler_state(handler_state);
	}
	else
	{
		// Nichts tun
	}
}

uint8_t get_handler_state(void)
{
    return handler_state;
}


void set_handler_currentTime(uint16_t dT)
{
    time_val = time_val + dT;
}

