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

#include "plx_timer.h"

#include "stm32g4xx_ll_tim.h"

void PLX_TIM_sinit()
{
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();
#ifdef TIM5
	__HAL_RCC_TIM5_CLK_ENABLE();
#endif
	__HAL_RCC_TIM6_CLK_ENABLE();
	__HAL_RCC_TIM7_CLK_ENABLE();
	__HAL_RCC_TIM8_CLK_ENABLE();
#ifdef TIM20
	__HAL_RCC_TIM20_CLK_ENABLE();
#endif
}

PLX_TIM_Handle_t PLX_TIM_init(void *aMemory, const size_t aNumBytes)
{
	PLX_TIM_Handle_t handle;

	if(aNumBytes < sizeof(PLX_TIM_Obj_t))
		return((PLX_TIM_Handle_t)NULL);

	// set handle
	handle = (PLX_TIM_Handle_t)aMemory;

	return handle;
}

void PLX_TIM_setup(PLX_TIM_Handle_t aHandle, PLX_TIM_Unit_t aUnit, LL_TIM_InitTypeDef *aInitDef, uint8_t aUseEnable, float aNomFreq)
{
	PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;

	obj->unit = aUnit;
	switch(obj->unit){
		case PLX_TIM1:
			obj->instance = TIM1;
			break;
		case PLX_TIM2:
			obj->instance = TIM2;
			break;
		case PLX_TIM3:
			obj->instance = TIM3;
			break;
		case PLX_TIM4:
			obj->instance = TIM4;
			break;
#ifdef TIM5
		case PLX_TIM5:
		  obj->instance = TIM5;
		  break;
#endif
		case PLX_TIM6:
			obj->instance = TIM6;
			break;
		case PLX_TIM7:
			obj->instance = TIM7;
			break;
		case PLX_TIM8:
			obj->instance = TIM8;
			break;
#ifdef TIM20
    case PLX_TIM20:
      obj->instance = TIM20;
      break;
#endif
		default:
			PLX_ASSERT(0);
	}

	obj->period = aInitDef->Autoreload;
	LL_TIM_Init(obj->instance, aInitDef);
	LL_TIM_EnableARRPreload(obj->instance);
	if (aUseEnable || !IS_TIM_BREAK_INSTANCE(obj->instance))
	{
	  obj->instance->CR2 |= TIM_CR2_CCPC;
	  obj->instance->CR2 &= ~TIM_CR2_CCUS;
	  obj->instance->CR2 |= TIM_COMMUTATION_SOFTWARE;
	}

	obj->counterMode = aInitDef->CounterMode;
	obj->repetitionCounter = aInitDef->RepetitionCounter;

	obj->configuredPwmChannelsMask = 0;
	obj->configuredPwmNChannelsMask = 0;

	obj->timerClk = HAL_RCC_GetPCLK2Freq();
	obj->nominalFrequency = aNomFreq;

#if 0
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&obj->timHandle, &sClockSourceConfig) != HAL_OK)
	{
		PLX_ASSERT(0);
	}
#endif
}

void PLX_TIM_addPwmChannel(PLX_TIM_Handle_t aHandle, PLX_TIM_Channel_t aChannel, LL_TIM_OC_InitTypeDef *aConfig, PLX_TIM_PwmMode_t aMode, PLX_TIM_PwmPolarity_t aPolarity)
{
	PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;

	uint32_t channel;
	switch(aChannel)
	{
		case PLX_TIM_CHANNEL1:
			channel = LL_TIM_CHANNEL_CH1;
			obj->configuredPwmChannelsMask |= 1;
			if(aMode == PLX_TIM_PWM_COMPLEMENTARY)
			{
				obj->configuredPwmNChannelsMask |= 1;
			}
			if(aPolarity == PLX_TIM_PWM_ACTIVE_LOW)
			{
			   obj->channelPolarityMask |= 1;
			}
			break;
		case PLX_TIM_CHANNEL2:
			channel = LL_TIM_CHANNEL_CH2;
			obj->configuredPwmChannelsMask |= 2;
			if(aMode == PLX_TIM_PWM_COMPLEMENTARY)
			{
				obj->configuredPwmNChannelsMask |= 2;
			}
         if(aPolarity == PLX_TIM_PWM_ACTIVE_LOW)
         {
            obj->channelPolarityMask |= 2;
         }
			break;
		case PLX_TIM_CHANNEL3:
			channel = LL_TIM_CHANNEL_CH3;
			obj->configuredPwmChannelsMask |= 4;
			if(aMode == PLX_TIM_PWM_COMPLEMENTARY)
			{
				obj->configuredPwmNChannelsMask |= 4;
			}
         if(aPolarity == PLX_TIM_PWM_ACTIVE_LOW)
         {
            obj->channelPolarityMask |= 4;
         }
			break;
    case PLX_TIM_CHANNEL4:
         channel = LL_TIM_CHANNEL_CH4;
         obj->configuredPwmChannelsMask |= 8;
         if(aMode == PLX_TIM_PWM_COMPLEMENTARY)
         {
           obj->configuredPwmNChannelsMask |= 8;
         }
         if(aPolarity == PLX_TIM_PWM_ACTIVE_LOW)
         {
            obj->channelPolarityMask |= 8;
         }
         break;
    default:
			PLX_ASSERT(0);
	}

    LL_TIM_OC_EnablePreload(obj->instance, channel);

    LL_TIM_OC_Init(obj->instance, channel, aConfig);
    LL_TIM_OC_DisableFast(obj->instance, channel);
}

void PLX_TIM_start(PLX_TIM_Handle_t aHandle, bool aDelayedRepetitionCounter){
	PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;


	if(obj->configuredPwmChannelsMask & 0x1)
	{
		LL_TIM_CC_EnableChannel(obj->instance, LL_TIM_CHANNEL_CH1);
	}
	if(obj->configuredPwmNChannelsMask & 0x1)
	{
		LL_TIM_CC_EnableChannel(obj->instance, LL_TIM_CHANNEL_CH1N);
	}
	if(obj->configuredPwmChannelsMask & 0x2)
	{
		LL_TIM_CC_EnableChannel(obj->instance, LL_TIM_CHANNEL_CH2);
	}
	if(obj->configuredPwmNChannelsMask & 0x2)
	{
		LL_TIM_CC_EnableChannel(obj->instance, LL_TIM_CHANNEL_CH2N);
	}
	if(obj->configuredPwmChannelsMask & 0x4)
	{
		LL_TIM_CC_EnableChannel(obj->instance, LL_TIM_CHANNEL_CH3);
	}
	if(obj->configuredPwmNChannelsMask & 0x4)
	{
		LL_TIM_CC_EnableChannel(obj->instance, LL_TIM_CHANNEL_CH3N);
	}
  if(obj->configuredPwmChannelsMask & 0x8)
  {
    LL_TIM_CC_EnableChannel(obj->instance, LL_TIM_CHANNEL_CH4);
  }
  if(obj->configuredPwmNChannelsMask & 0x8)
  {
    LL_TIM_CC_EnableChannel(obj->instance, LL_TIM_CHANNEL_CH4N);
  }

	LL_TIM_EnableIT_UPDATE(obj->instance);

	if(aDelayedRepetitionCounter)
	{
		LL_TIM_SetRepetitionCounter(obj->instance, 0);
	}
	LL_TIM_GenerateEvent_UPDATE(obj->instance);
}

bool PLX_TIM_processInt(PLX_TIM_Handle_t aHandle){

	PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;

	if (LL_TIM_IsActiveFlag_UPDATE(obj->instance))
	{
		if (LL_TIM_IsEnabledIT_UPDATE(obj->instance))
		{
			LL_TIM_ClearFlag_UPDATE(obj->instance);
			return true;
		}
	}
	return false;
}

void PLX_TIM_generateCommutationEvent(PLX_TIM_Handle_t aHandle)
{
  PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;
  LL_TIM_GenerateEvent_COM(obj->instance);
}

void PLX_TIM_setDuty(PLX_TIM_Handle_t aHandle, uint16_t aChannel, float aDuty)
{
	PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;
  if(aDuty < 0.0f)
  {
    aDuty = 0.0f;
  }
  else if (aDuty > 1.0f)
  {
    aDuty = 1.0f;
  }
	switch(aChannel)
	{
		case 1:
			LL_TIM_OC_SetCompareCH1(obj->instance, (uint32_t)((float)obj->period * aDuty));
			break;
		case 2:
			LL_TIM_OC_SetCompareCH2(obj->instance, (uint32_t)((float)obj->period * aDuty));
			break;
		case 3:
			LL_TIM_OC_SetCompareCH3(obj->instance, (uint32_t)((float)obj->period * aDuty));
			break;
    case 4:
      LL_TIM_OC_SetCompareCH4(obj->instance, (uint32_t)((float)obj->period * aDuty));
      break;
	}
}

void PLX_TIM_enablePwmOutput(PLX_TIM_Handle_t aHandle)
{
	PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;
	if (IS_TIM_BREAK_INSTANCE(obj->instance))
	{
	  LL_TIM_EnableAllOutputs(obj->instance);
	}
	else
	{
	  LL_TIM_OC_SetMode(obj->instance, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
	  LL_TIM_OC_SetMode(obj->instance, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);
	  LL_TIM_OC_SetMode(obj->instance, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
	  LL_TIM_OC_SetMode(obj->instance, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
	}
}

void PLX_TIM_disablePwmOutput(PLX_TIM_Handle_t aHandle)
{
	PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;
  if (IS_TIM_BREAK_INSTANCE(obj->instance))
  {
    LL_TIM_DisableAllOutputs(obj->instance);
  }
  else
  {
    LL_TIM_OC_SetMode(obj->instance, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_FORCED_INACTIVE);
    LL_TIM_OC_SetMode(obj->instance, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_FORCED_INACTIVE);
    LL_TIM_OC_SetMode(obj->instance, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_FORCED_INACTIVE);
    LL_TIM_OC_SetMode(obj->instance, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_FORCED_INACTIVE);
  }
}

bool PLX_TIM_pwmOutputIsEnabled(PLX_TIM_Handle_t aHandle)
{
	PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;
  if (IS_TIM_BREAK_INSTANCE(obj->instance))
  {
    return LL_TIM_IsEnabledAllOutputs(obj->instance);
  }
  else
  {
    return true;
  }
}

void PLX_TIM_setPwmOutputState(PLX_TIM_Handle_t aHandle, PLX_TIM_Channel_t aChannel, uint8_t aEnable, uint8_t aEnableN)
{
   PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;
   switch(aChannel)
   {
   case PLX_TIM_CHANNEL1:
      if(aEnable)
      {
         obj->instance->CCER |= (uint32_t)(TIM_CCER_CC1E);
      }
      else
      {
         obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC1E);
      }
      if(obj->configuredPwmNChannelsMask & 0x1)
      {
         if(aEnable)
         {
            if(obj->channelPolarityMask & 0x1)
            {
               obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC1NP);
            }
            else
            {
               obj->instance->CCER |= (uint32_t)(TIM_CCER_CC1NP);
            }
         }
         else
         {
            if(obj->channelPolarityMask & 0x1)
            {
               obj->instance->CCER |= (uint32_t)(TIM_CCER_CC1NP);
            }
            else
            {
               obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC1NP);
            }
         }
         if(aEnableN)
         {
            obj->instance->CCER |= (uint32_t)(TIM_CCER_CC1NE);
         }
         else
         {
            obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC1NE);
         }
      }
      break;
   case PLX_TIM_CHANNEL2:
      if(aEnable)
      {
         obj->instance->CCER |= (uint32_t)(TIM_CCER_CC2E);
      }
      else
      {
         obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC2E);
      }
      if(obj->configuredPwmNChannelsMask & 0x2)
      {
         if(aEnable)
         {
            if(obj->channelPolarityMask & 0x2)
            {
               obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC2NP);
            }
            else
            {
               obj->instance->CCER |= (uint32_t)(TIM_CCER_CC2NP);
            }
         }
         else
         {
            if(obj->channelPolarityMask & 0x2)
            {
               obj->instance->CCER |= (uint32_t)(TIM_CCER_CC2NP);
            }
            else
            {
               obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC2NP);
            }
         }
         if(aEnableN)
         {
            obj->instance->CCER |= (uint32_t)(TIM_CCER_CC2NE);
         }
         else
         {
            obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC2NE);
         }
      }
      break;
   case PLX_TIM_CHANNEL3:
      if(aEnable)
      {
         obj->instance->CCER |= (uint32_t)(TIM_CCER_CC3E);
      }
      else
      {
         obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC3E);
      }
      if(obj->configuredPwmNChannelsMask & 0x4)
      {
         if(aEnable)
         {
            if(obj->channelPolarityMask & 0x4)
            {
               obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC3NP);
            }
            else
            {
               obj->instance->CCER |= (uint32_t)(TIM_CCER_CC3NP);
            }
         }
         else
         {
            if(obj->channelPolarityMask & 0x4)
            {
               obj->instance->CCER |= (uint32_t)(TIM_CCER_CC3NP);
            }
            else
            {
               obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC3NP);
            }
         }
         if(aEnableN)
         {
            obj->instance->CCER |= (uint32_t)(TIM_CCER_CC3NE);
         }
         else
         {
            obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC3NE);
         }
      }
      break;
   case PLX_TIM_CHANNEL4:
      if(aEnable)
      {
         obj->instance->CCER |= (uint32_t)(TIM_CCER_CC4E);
      }
      else
      {
         obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC4E);
      }
      if(obj->configuredPwmNChannelsMask & 0x8)
      {
         if(aEnable)
         {
            if(obj->channelPolarityMask & 0x8)
            {
               obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC4NP);
            }
            else
            {
               obj->instance->CCER |= (uint32_t)(TIM_CCER_CC4NP);
            }
         }
         else
         {
            if(obj->channelPolarityMask & 0x8)
            {
               obj->instance->CCER |= (uint32_t)(TIM_CCER_CC4NP);
            }
            else
            {
               obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC4NP);
            }
         }
         if(aEnableN)
         {
            obj->instance->CCER |= (uint32_t)(TIM_CCER_CC4NE);
         }
         else
         {
            obj->instance->CCER &= (uint32_t)~(TIM_CCER_CC4NE);
         }
      }
   }
   LL_TIM_GenerateEvent_COM(obj->instance);
}

void PLX_TIM_scalePeriod(PLX_TIM_Handle_t aHandle, float aScalingFactor)
{
  PLX_TIM_Obj_t *obj = (PLX_TIM_Obj_t *)aHandle;
  if (aScalingFactor <= 0.0f)
  {
    return;
  }
  uint16_t prescaler = LL_TIM_GetPrescaler(obj->instance);
  uint32_t clk = obj->timerClk;
  if (LL_TIM_GetCounterMode(obj->instance) != LL_TIM_COUNTERMODE_UP)
  {
    aScalingFactor = aScalingFactor * 2.0f;
  }
  float scaledFreq = obj->nominalFrequency * aScalingFactor;
  uint32_t newARR = __LL_TIM_CALC_ARR(clk, prescaler, scaledFreq);

  if (newARR < 2000 || newARR > 65535)
  {

    prescaler = __LL_TIM_CALC_PSC(clk, (uint32_t)(scaledFreq * 4000.0f));
    LL_TIM_SetPrescaler(obj->instance, prescaler);
    newARR = __LL_TIM_CALC_ARR(clk, prescaler, scaledFreq);
  }
  obj->period = newARR;
  LL_TIM_SetAutoReload(obj->instance, newARR);
}
