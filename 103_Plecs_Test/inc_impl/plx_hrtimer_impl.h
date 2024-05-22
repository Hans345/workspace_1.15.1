/*
   Copyright (c) 2020 by Plexim GmbH
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

#ifndef HRTIM_IMPL_H_
#define HRTIM_IMPL_H_

#include "stm32g4xx_hal_hrtim.h"
#include "stm32g4xx_ll_hrtim.h"

typedef enum PLX_HRTIM_UNIT {
	PLX_HRTIM1 = 0,
	PLX_HRTIM2,
} PLX_HRTIM_Unit_t;

typedef enum PLX_HRTIM_TIMER {
  PLX_HRTIM_MASTER = 0,
	PLX_HRTIM_TA,
	PLX_HRTIM_TB,
	PLX_HRTIM_TC,
	PLX_HRTIM_TD,
	PLX_HRTIM_TE,
	PLX_HRTIM_TF,
	PLX_HRTIM_END
} PLX_HRTIM_Timer_t;

typedef enum PLX_HRTIM_CHANNEL {
  PLX_HRTIM_TM1 = 0,
  PLX_HRTIM_TM2,
	PLX_HRTIM_TA1,
	PLX_HRTIM_TA2,
	PLX_HRTIM_TB1,
	PLX_HRTIM_TB2,
	PLX_HRTIM_TC1,
	PLX_HRTIM_TC2,
	PLX_HRTIM_TD1,
	PLX_HRTIM_TD2,
	PLX_HRTIM_TE1,
	PLX_HRTIM_TE2,
	PLX_HRTIM_TF1,
	PLX_HRTIM_TF2,
	PLX_HRTIM_CHANNEL_END
} PLX_HRTIM_Channel_t;

typedef struct PLX_HRTIM_SOBJ
{
  int32_t channelLookup[PLX_HRTIM_CHANNEL_END];
} PLX_HRTIM_SObj_t;

typedef struct PLX_HRTIM_OBJ
{
  PLX_HRTIM_Unit_t unit;
  HRTIM_TypeDef *instance;
  uint32_t timers[PLX_HRTIM_END];
  uint32_t activeTimers;
  uint32_t activeChannels;
  uint32_t desiredOnChannels;
  uint32_t allowedOnChannelsByLocalEnable;
  uint32_t channelsSyncedToMaster;
  uint32_t allowedOnChannelsByMaster;
  uint32_t nominalPeriod[PLX_HRTIM_END];
  uint32_t latchedPeriod;
  uint32_t burstModePeriod;
  uint32_t minCmpRegValue[PLX_HRTIM_END];
  uint32_t maxCmpRegValue[PLX_HRTIM_END];
} PLX_HRTIM_Obj_t;

typedef PLX_HRTIM_Obj_t *PLX_HRTIM_Handle_t;

extern PLX_HRTIM_Handle_t PLX_HRTIM_init(void *aMemory, const size_t aNumBytes);

extern PLX_HRTIM_SObj_t* getSObjPtr( void );

__STATIC_INLINE void PLX_HRTIM_setDuty1(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, PLX_HRTIM_Channel_t aChannel, float aDuty)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  uint32_t period = LL_HRTIM_TIM_GetPeriod(obj->instance, obj->timers[aTimer]);
  uint32_t duty = (uint32_t)((float)period * aDuty);
  bool forceUpdate = false;
  if(duty < obj->minCmpRegValue[aTimer])
  {
    duty = period;   // Force 0% duty cycle
  }
  else if (duty >= period - obj->minCmpRegValue[aTimer])
  {
     if (LL_HRTIM_TIM_GetCountingMode(obj->instance, obj->timers[aTimer]) == LL_HRTIM_COUNTING_MODE_UP_DOWN)
     {
        // Force output to active state for 100 % duty with symmetrical carrier
        PLX_HRTIM_SObj_t* objPtr = getSObjPtr();
        LL_HRTIM_OUT_ForceLevel(obj->instance, objPtr->channelLookup[aChannel], LL_HRTIM_OUT_LEVEL_ACTIVE);
        forceUpdate = true;
     }
     duty = period + 1; // Force 100 % duty cycle
  }
  LL_HRTIM_TIM_SetCompare1(obj->instance, obj->timers[aTimer], duty);
  if (forceUpdate)
  {
     LL_HRTIM_ForceUpdate(obj->instance, obj->timers[aTimer]);
  }
}

__STATIC_INLINE void PLX_HRTIM_setDuty2(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, PLX_HRTIM_Channel_t aChannel, float aDuty)
{
   PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
   uint32_t period = LL_HRTIM_TIM_GetPeriod(obj->instance, obj->timers[aTimer]);
   uint32_t duty = (uint32_t)((float)period * aDuty);
   bool forceUpdate = false;
   if(duty < obj->minCmpRegValue[aTimer])
   {
     duty = period;   // Force 0% duty cycle
   }
   else if (duty >= period - obj->minCmpRegValue[aTimer])
   {
      if (LL_HRTIM_TIM_GetCountingMode(obj->instance, obj->timers[aTimer]) == LL_HRTIM_COUNTING_MODE_UP_DOWN)
      {
         // Force output to active state for 100 % duty with symmetrical carrier
         PLX_HRTIM_SObj_t* objPtr = getSObjPtr();
         LL_HRTIM_OUT_ForceLevel(obj->instance, objPtr->channelLookup[aChannel], LL_HRTIM_OUT_LEVEL_ACTIVE);
         forceUpdate = true;
      }
      duty = period + 1; // Force 100 % duty cycle
   }
   LL_HRTIM_TIM_SetCompare2(obj->instance, obj->timers[aTimer], duty);
   if (forceUpdate)
   {
      LL_HRTIM_ForceUpdate(obj->instance, obj->timers[aTimer]);
   }
}

__STATIC_INLINE void PLX_HRTIM_setDuty3(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, PLX_HRTIM_Channel_t aChannel, float aDuty)
{
   PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
   uint32_t period = LL_HRTIM_TIM_GetPeriod(obj->instance, obj->timers[aTimer]);
   uint32_t duty = (uint32_t)((float)period * aDuty);
   bool forceUpdate = false;
   if(duty < obj->minCmpRegValue[aTimer])
   {
     duty = period;   // Force 0% duty cycle
   }
   else if (duty >= period - obj->minCmpRegValue[aTimer])
   {
      if (LL_HRTIM_TIM_GetCountingMode(obj->instance, obj->timers[aTimer]) == LL_HRTIM_COUNTING_MODE_UP_DOWN)
      {
         // Force output to active state for 100 % duty with symmetrical carrier
         PLX_HRTIM_SObj_t* objPtr = getSObjPtr();
         LL_HRTIM_OUT_ForceLevel(obj->instance, objPtr->channelLookup[aChannel], LL_HRTIM_OUT_LEVEL_ACTIVE);
         forceUpdate = true;
      }
      duty = period + 1; // Force 100 % duty cycle
   }
   LL_HRTIM_TIM_SetCompare3(obj->instance, obj->timers[aTimer], duty);
   if (forceUpdate)
   {
      LL_HRTIM_ForceUpdate(obj->instance, obj->timers[aTimer]);
   }
}

/* CMP4 is only used as ADC trigger (mid-point sampling with sawtooth carrier) */
__STATIC_INLINE void PLX_HRTIM_setDuty4(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, PLX_HRTIM_Channel_t aChannel, float aDuty)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  uint32_t period = LL_HRTIM_TIM_GetPeriod(obj->instance, obj->timers[aTimer]);
  uint32_t duty = (uint32_t)((float)period * aDuty);
  if(duty < obj->minCmpRegValue[aTimer])
  {
    duty = obj->minCmpRegValue[aTimer];
  }
  else if (duty >= period/2)
  {
    duty = period/2;
  }
  LL_HRTIM_TIM_SetCompare4(obj->instance, obj->timers[aTimer], duty);
}

__STATIC_INLINE void PLX_HRTIM_setPhase(PLX_HRTIM_Handle_t aHandle, uint16_t aCmp, float aPhase)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  uint32_t period = LL_HRTIM_TIM_GetPeriod(obj->instance, LL_HRTIM_TIMER_MASTER);
  uint32_t phase = (uint32_t)((float)period * aPhase);
  if(phase < obj->minCmpRegValue[PLX_HRTIM_MASTER])
  {
    phase = obj->minCmpRegValue[PLX_HRTIM_MASTER];
  }
  else if (phase > period)
  {
    phase = period;
  }
  switch(aCmp)
  {
    case(1):
      LL_HRTIM_TIM_SetCompare1(obj->instance, LL_HRTIM_TIMER_MASTER, phase);
      break;
    case(2):
      LL_HRTIM_TIM_SetCompare2(obj->instance, LL_HRTIM_TIMER_MASTER, phase);
      break;
    case(3):
      LL_HRTIM_TIM_SetCompare3(obj->instance, LL_HRTIM_TIMER_MASTER, phase);
      break;
    case(4):
      LL_HRTIM_TIM_SetCompare4(obj->instance, LL_HRTIM_TIMER_MASTER, phase);
      break;
    default:
      PLX_ASSERT(0);
  }
}

__STATIC_INLINE void PLX_HRTIM_scalePeriod(PLX_HRTIM_Handle_t aHandle, float aScalingFactor)
{
   PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
   PLX_ASSERT(aScalingFactor > 0.0f);
   if((aScalingFactor <= (1.0f/65536.0f)) || (aScalingFactor > 65536.0f))
   {
      return;
   }
   uint32_t newPeriod = (uint32_t)obj->nominalPeriod[PLX_HRTIM_MASTER]*1/
                        aScalingFactor;
   if(newPeriod < 1)
   {
      newPeriod = 1;
   }
   else if(newPeriod > obj->maxCmpRegValue[PLX_HRTIM_MASTER])
   {
      newPeriod = obj->maxCmpRegValue[PLX_HRTIM_MASTER];
   }
   obj->latchedPeriod = newPeriod;
}

__STATIC_INLINE void PLX_HRTIM_applyPeriod(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  LL_HRTIM_TIM_SetPeriod(obj->instance, obj->timers[aTimer], obj->latchedPeriod);
}

__STATIC_INLINE void PLX_HRTIM_scalePeriodHrtimUnit(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, float aScalingFactor)
{
   PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
   PLX_ASSERT(aScalingFactor > 0.0f);
   if((aScalingFactor <= (1.0f/65536.0f)) || (aScalingFactor > 65536.0f))
   {
      return;
   }
   uint32_t newPeriod = (uint32_t)obj->nominalPeriod[aTimer]*1/
                        aScalingFactor;
   if(newPeriod < 1)
   {
      newPeriod = 1;
   }
   else if(newPeriod > obj->maxCmpRegValue[aTimer])
   {
      newPeriod = obj->maxCmpRegValue[aTimer];
   }
   LL_HRTIM_TIM_SetPeriod(obj->instance, obj->timers[aTimer], newPeriod);
}

__STATIC_INLINE void PLX_HRTIM_enableBurstMode(PLX_HRTIM_Handle_t aHandle, bool aValue)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  if (aValue)
  {
    LL_HRTIM_BM_Enable(obj->instance);
  }
  else
  {
    LL_HRTIM_BM_Disable(obj->instance);
  }
}

__STATIC_INLINE void PLX_HRTIM_setBurstModeCompare(PLX_HRTIM_Handle_t aHandle, float aValue)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  float duty = aValue;
  if (duty >= 1.0f)
  {
    LL_HRTIM_BM_Disable(obj->instance);
    duty = 1.0f;
  }
  else
  {
    LL_HRTIM_BM_Enable(obj->instance);
  }
  if (duty < 0.0f)
  {
    duty = 0.0f;
  }
  uint32_t period = LL_HRTIM_BM_GetPeriod(obj->instance);
  LL_HRTIM_BM_SetCompare(obj->instance, period - (uint32_t)((float)period * duty));
}

__STATIC_INLINE void PLX_HRTIM_setBurstModePeriod(PLX_HRTIM_Handle_t aHandle, uint32_t aValue)
{
  PLX_HRTIM_Obj_t *obj = (PLX_HRTIM_Obj_t *)aHandle;
  obj->burstModePeriod = aValue;
  LL_HRTIM_BM_SetPeriod(obj->instance, aValue);
}

#endif /* HRTIM_IMPL_H_ */
