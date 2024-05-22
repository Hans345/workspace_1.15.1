#include "task_handler.h"


uint8_t handler_state, prev_handler_state;
uint32_t handler_timeout;
uint32_t cycle_counter;

//-----------------------------------------------------------------------------------------------------------------------
/****** VARIABLES ******/
uint16_t time_val = 0;
uint16_t dT = 0;

void enter_handler_state(uint8_t state)
{
    switch(state){
    //--------------------------------------------------------------------------------------
        case IDLE:

        break;
    //--------------------------------------------------------------------------------------
        case LEDON:
			/****** Timer ******/
			// Start Timer
			HAL_TIM_Base_Start(&htim16);
			// Get current time
			time_val = __HAL_TIM_GET_COUNTER(&htim16);
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

void exec_handler_state(uint8_t state)
{
    switch(state){
    //--------------------------------------------------------------------------------------
        case IDLE:

        break;
    //--------------------------------------------------------------------------------------
        case LEDON:
		if(HAL_GPIO_ReadPin(B1, B1_PIN))
		{
			dT = __HAL_TIM_GET_COUNTER(&htim16) - time_val; // 100us
			time_val = __HAL_TIM_GET_COUNTER(&htim16);
			if(time_val > 10000)
			{
				set_handler_state(IDLE);
			}
		}
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

void leave_handler_state(uint8_t state)
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

void handler_rt_task(void)
{
    if(handler_timeout > 0){
        handler_timeout--;
    }
}

void handler_task(void)
{
    exec_handler_state(handler_state);
}

void set_handler_state(uint8_t state)
{
    prev_handler_state = handler_state;
    handler_state = state;
    leave_handler_state(prev_handler_state);
    enter_handler_state(state);
}

uint8_t get_handler_state(void)
{
    return handler_state;
}


void set_handler_timeout(uint32_t timeout)
{
    handler_timeout = timeout;
}

