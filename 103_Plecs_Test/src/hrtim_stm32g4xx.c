/*
   Copyright (c) 2020-2021 by Plexim GmbH
   All rights reserved.

   A free license is granted to anyone to use this software for any legal
   non safety-critical purpose, including commercial applications, provided
   that:
   1) IT IS NOT USED TO DIRECTLY OR INDIRECTLY COMPETE WITH PLEXIM, and
   2) THIS COPYRIGHT NOTICE IS PRESERVED in its entirety.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */

#include "plx_hrtimer.h"

#ifdef PLX_HRTIM_AVAILABLE

#include "stm32g4xx_ll_hrtim.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_gpio.h"


static PLX_HRTIM_SObj_t PLX_HRTIM_SObj;

void PLX_HRTIM_sinit()
{
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TM1] = 0;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TM2] = 0;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TA1] = LL_HRTIM_OUTPUT_TA1;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TA2] = LL_HRTIM_OUTPUT_TA2;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TB1] = LL_HRTIM_OUTPUT_TB1;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TB2] = LL_HRTIM_OUTPUT_TB2;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TC1] = LL_HRTIM_OUTPUT_TC1;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TC2] = LL_HRTIM_OUTPUT_TC2;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TD1] = LL_HRTIM_OUTPUT_TD1;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TD2] = LL_HRTIM_OUTPUT_TD2;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TE1] = LL_HRTIM_OUTPUT_TE1;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TE2] = LL_HRTIM_OUTPUT_TE2;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TF1] = LL_HRTIM_OUTPUT_TF1;
  PLX_HRTIM_SObj.channelLookup[PLX_HRTIM_TF2] = LL_HRTIM_OUTPUT_TF2;

   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_HRTIM1);
}

PLX_HRTIM_SObj_t* getSObjPtr( void )
{
   return &PLX_HRTIM_SObj;
}

PLX_HRTIM_Handle_t PLX_HRTIM_init(void *aMemory, const size_t aNumBytes)
{
	PLX_HRTIM_Handle_t handle;

	if(aNumBytes < sizeof(PLX_HRTIM_Obj_t))
		return((PLX_HRTIM_Handle_t)NULL);

	// set handle
	handle = (PLX_HRTIM_Handle_t)aMemory;

   PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)handle;
   obj->activeTimers = 0;
   obj->activeChannels = 0;
   obj->desiredOnChannels = 0;
   obj->allowedOnChannelsByLocalEnable = 0;
   obj->allowedOnChannelsByMaster = 0;
   obj->channelsSyncedToMaster = 0;

   obj->timers[PLX_HRTIM_MASTER] = LL_HRTIM_TIMER_MASTER;
   obj->timers[PLX_HRTIM_TA] = LL_HRTIM_TIMER_A;
   obj->timers[PLX_HRTIM_TB] = LL_HRTIM_TIMER_B;
   obj->timers[PLX_HRTIM_TC] = LL_HRTIM_TIMER_C;
   obj->timers[PLX_HRTIM_TD] = LL_HRTIM_TIMER_D;
   obj->timers[PLX_HRTIM_TE] = LL_HRTIM_TIMER_E;
   obj->timers[PLX_HRTIM_TF] = LL_HRTIM_TIMER_F;

	return handle;
}

void PLX_HRTIM_setup(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Unit_t aUnit)
{
	PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;

	obj->unit = aUnit;
	switch(obj->unit){
		case PLX_HRTIM1:
			obj->instance = HRTIM1;
			break;
		default:
			PLX_ASSERT(0);
	}

	// TODO: Understand impact of continuous calibration - is there a negative impact?
	LL_HRTIM_ConfigDLLCalibration(obj->instance, LL_HRTIM_DLLCALIBRATION_MODE_CONTINUOUS, LL_HRTIM_DLLCALIBRATION_RATE_3);
	while(LL_HRTIM_IsActiveFlag_DLLRDY(obj->instance) == RESET){
		continue;
	}
}

void PLX_HRTIM_timerSetup(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer)
{
	PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
	obj->nominalPeriod[aTimer] = LL_HRTIM_TIM_GetPeriod(obj->instance, obj->timers[aTimer]);
	obj->activeTimers |= obj->timers[aTimer];
	uint32_t prescaler = LL_HRTIM_TIM_GetPrescaler(obj->instance, obj->timers[aTimer]);
	switch (prescaler)
	{
	   case 0:
	      obj->minCmpRegValue[aTimer] = 0x0060;
	      obj->maxCmpRegValue[aTimer] = 0xFFDF;
	      break;
	   case 1:
         obj->minCmpRegValue[aTimer] = 0x0030;
         obj->maxCmpRegValue[aTimer] = 0xFFEF;
	      break;
	   case 2:
         obj->minCmpRegValue[aTimer] = 0x0018;
         obj->maxCmpRegValue[aTimer] = 0xFFF7;
	      break;
	   case 3:
         obj->minCmpRegValue[aTimer] = 0x000C;
         obj->maxCmpRegValue[aTimer] = 0xFFFB;
	      break;
	   case 4:
         obj->minCmpRegValue[aTimer] = 0x0006;
         obj->maxCmpRegValue[aTimer] = 0xFFFD;
	      break;
	   case 5:
	   case 6:
	   case 7:
         obj->minCmpRegValue[aTimer] = 0x0003;
         obj->maxCmpRegValue[aTimer] = 0xFFFD;
	      break;
	   default:
	      PLX_ASSERT(0);
	}

}

void PLX_HRTIM_addPwmChannel(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Channel_t aChannel, uint8_t aIsSyncedToMaster)
{
	PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;

	PLX_ASSERT(aChannel < PLX_HRTIM_CHANNEL_END);
	obj->activeChannels |= PLX_HRTIM_SObj.channelLookup[aChannel];
	obj->allowedOnChannelsByLocalEnable = obj->activeChannels;
	if (aIsSyncedToMaster)
	{
	  obj->channelsSyncedToMaster |= PLX_HRTIM_SObj.channelLookup[aChannel];
	}
	else
	{
	  obj->allowedOnChannelsByMaster |= PLX_HRTIM_SObj.channelLookup[aChannel];
	}
}

void PLX_HRTIM_start(PLX_HRTIM_Handle_t aHandle){
	PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;

	LL_HRTIM_TIM_CounterEnable(obj->instance, obj->activeTimers);
}

void PLX_HRTIM_enableRepIT(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t*)aHandle;

  LL_HRTIM_EnableIT_REP(obj->instance, obj->timers[aTimer]);
}

void PLX_HRTIM_enablePwmOutput(PLX_HRTIM_Handle_t aHandle)
{
	PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
   obj->desiredOnChannels = obj->activeChannels;
	LL_HRTIM_EnableOutput(obj->instance, obj->desiredOnChannels & obj->allowedOnChannelsByLocalEnable & obj->allowedOnChannelsByMaster);
}

void PLX_HRTIM_disablePwmOutput(PLX_HRTIM_Handle_t aHandle)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  obj->desiredOnChannels = 0;
  uint32_t oenr = obj->instance->sCommonRegs.OENR;
  /* only enter IDLE state if at least one channel is in the RUN state*/
  if (oenr != 0UL)
  {
    LL_HRTIM_DisableOutput(obj->instance, obj->activeChannels);
  }
}

void PLX_HRTIM_enablePwmOutputByMaster(PLX_HRTIM_Handle_t aHandle)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  obj->allowedOnChannelsByMaster |= obj->channelsSyncedToMaster;
}

void PLX_HRTIM_disablePwmOutputByMaster(PLX_HRTIM_Handle_t aHandle)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  obj->allowedOnChannelsByMaster &= ~obj->channelsSyncedToMaster;
  LL_HRTIM_DisableOutput(obj->instance, obj->channelsSyncedToMaster);
}

bool PLX_HRTIM_pwmOutputIsEnabled(PLX_HRTIM_Handle_t aHandle)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  HRTIM_TypeDef *HRTIMx = obj->instance;
  bool isEnabled = true;
  /* Check if output is disabled: output not in RUN state */
  uint32_t oenr = HRTIMx->sCommonRegs.OENR;
  uint32_t odsr = HRTIMx->sCommonRegs.ODSR;
  /* At least one output is not in the running state */
  if (oenr != obj->activeChannels)
  {
    /* Output is disabled and in FAULT state */
    if (oenr ^ odsr)
    {
      isEnabled = false;
    }
  }
  return isEnabled;
}

void PLX_HRTIM_activateForcePwmChannelOff(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Channel_t aChannel){
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  PLX_ASSERT(aChannel < PLX_HRTIM_CHANNEL_END);
  uint32_t outLL = PLX_HRTIM_SObj.channelLookup[aChannel];
  LL_HRTIM_DisableOutput(obj->instance, outLL);
  obj->allowedOnChannelsByLocalEnable &= ~outLL;
}

void PLX_HRTIM_releaseForcePwmChannelOff(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Channel_t aChannel){
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  PLX_ASSERT(aChannel < PLX_HRTIM_CHANNEL_END);
  uint32_t outLL = PLX_HRTIM_SObj.channelLookup[aChannel];
  obj->allowedOnChannelsByLocalEnable |= outLL;
  if((obj->desiredOnChannels & obj->allowedOnChannelsByMaster & outLL) != 0)
  {
    LL_HRTIM_EnableOutput(obj->instance, outLL);
  }
}

#endif // PLX_HRTIM_AVAILABLE

