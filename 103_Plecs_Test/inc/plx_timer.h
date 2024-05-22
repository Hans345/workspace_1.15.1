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

#include "includes.h"

#ifndef PLX_TIMER_H_
#define PLX_TIMER_H_

#include "plx_timer_impl.h" // implementation specific

extern void PLX_TIM_sinit();

extern PLX_TIM_Handle_t PLX_TIM_init(void *aMemory, const size_t aNumBytes);

extern void PLX_TIM_setup(PLX_TIM_Handle_t aHandle, PLX_TIM_Unit_t aUnit, LL_TIM_InitTypeDef *aInitDef, uint8_t aUseEnable, float aNomFreq);

extern TIM_TypeDef* PLX_TIM_getStmLLHandle(PLX_TIM_Handle_t aHandle);

extern void PLX_TIM_addPwmChannel(PLX_TIM_Handle_t aHandle, PLX_TIM_Channel_t aChannel, LL_TIM_OC_InitTypeDef *aConfig,
		PLX_TIM_PwmMode_t aMode, PLX_TIM_PwmPolarity_t aPolarity);

extern void PLX_TIM_start(PLX_TIM_Handle_t aHandle, bool aDelayedRepetitionCounter);

extern void PLX_TIM_enablePwmOutput(PLX_TIM_Handle_t aHandle);

extern void PLX_TIM_disablePwmOutput(PLX_TIM_Handle_t aHandle);

extern bool PLX_TIM_pwmOutputIsEnabled(PLX_TIM_Handle_t aHandle);

extern void PLX_TIM_setDuty(PLX_TIM_Handle_t aHandle, uint16_t aChannel, float aDuty);

extern void PLX_TIM_setPwmOutputState(PLX_TIM_Handle_t aHandle, PLX_TIM_Channel_t aChannel, uint8_t aEnable, uint8_t aEnableN);

extern bool PLX_TIM_processInt(PLX_TIM_Handle_t aHandle);

extern void PLX_TIM_generateCommutationEvent(PLX_TIM_Handle_t aHandle);

extern void PLX_TIM_scalePeriod(PLX_TIM_Handle_t aHandle, float aScalingFactor);

extern void PLX_TIM_enableAllClocks();

extern void PLX_TIM_disableAllClocks();

#endif /* PLX_TIMER_H_ */
