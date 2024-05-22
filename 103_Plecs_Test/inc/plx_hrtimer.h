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

#if !defined(PLX_HRTIMER_H_) && defined(HRTIM1)
#define PLX_HRTIMER_H_

#define PLX_HRTIM_AVAILABLE

#include "plx_hrtimer_impl.h" // implementation specific

extern void PLX_HRTIM_sinit();

extern PLX_HRTIM_Handle_t PLX_HRTIM_init(void *aMemory, const size_t aNumBytes);

extern void PLX_HRTIM_setup(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Unit_t aUnit);

extern void PLX_HRTIM_addPwmChannel(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Channel_t aChannel, uint8_t aIsSyncedToMaster);

extern void PLX_HRTIM_start(PLX_HRTIM_Handle_t aHandle);

extern void PLX_HRTIM_enableRepIT(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer);

extern void PLX_HRTIM_timerSetup(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer);

extern void PLX_HRTIM_setDuty1(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, PLX_HRTIM_Channel_t aChannel, float aDuty);
extern void PLX_HRTIM_setDuty2(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, PLX_HRTIM_Channel_t aChannel, float aDuty);
extern void PLX_HRTIM_setDuty3(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, PLX_HRTIM_Channel_t aChannel, float aDuty);
extern void PLX_HRTIM_setDuty4(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, PLX_HRTIM_Channel_t aChannel, float aDuty);

extern void PLX_HRTIM_setPhase(PLX_HRTIM_Handle_t aHandle, uint16_t aCmp, float aPhase);

extern void PLX_HRTIM_scalePeriod(PLX_HRTIM_Handle_t aHandle, float aScalingFactor);

extern void PLX_HRTIM_applyPeriod(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer);

extern void PLX_HRTIM_scalePeriodHrtimUnit(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Timer_t aTimer, float aScalingFactor);

extern void PLX_HRTIM_enablePwmOutput(PLX_HRTIM_Handle_t aHandle);

extern void PLX_HRTIM_disablePwmOutput(PLX_HRTIM_Handle_t aHandle);

extern void PLX_HRTIM_enablePwmOutputByMaster(PLX_HRTIM_Handle_t aHandle);

extern void PLX_HRTIM_disablePwmOutputByMaster(PLX_HRTIM_Handle_t aHandle);

extern bool PLX_HRTIM_pwmOutputIsEnabled(PLX_HRTIM_Handle_t aHandle);

extern void PLX_HRTIM_activateForcePwmChannelOff(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Channel_t aChannel);

extern void PLX_HRTIM_releaseForcePwmChannelOff(PLX_HRTIM_Handle_t aHandle, PLX_HRTIM_Channel_t aChannel);

extern void PLX_HRTIM_enableBurstMode(PLX_HRTIM_Handle_t aHandle, bool aValue);
extern void PLX_HRTIM_setBurstModePeriod(PLX_HRTIM_Handle_t aHandle, uint32_t aValue);
extern void PLX_HRTIM_setBurstModeCompare(PLX_HRTIM_Handle_t aHandle, float aValue);

#endif /* PLX_HRTIMER_H_ */
