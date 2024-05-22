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

#include "plx_hrtimer_pcc.h"

#ifdef PLX_HRTIM_AVAILABLE

PLX_HRTIM_PCC_Handle_t PLX_HRTIM_PCC_init(void *aMemory, const size_t aNumBytes)
{
	PLX_HRTIM_PCC_Handle_t handle;

	if(aNumBytes < sizeof(PLX_HRTIM_PCC_Obj_t))
		return((PLX_HRTIM_PCC_Handle_t)NULL);

	// set handle
	handle = (PLX_HRTIM_PCC_Handle_t)aMemory;

	return handle;
}

void PLX_HRTIM_PCC_configureChannel(PLX_HRTIM_PCC_Handle_t aHandle, uint16_t aChannel,
    PLX_DAC_Handle_t aDacHandle, uint16_t aDacChannel,
    PLX_HRTIM_Handle_t aHrtimHandle, PLX_HRTIM_Timer_t aHrtimTimer
    )
{
  PLX_HRTIM_PCC_Obj_t *obj = (PLX_HRTIM_PCC_Obj_t *)aHandle;

  PLX_ASSERT(aChannel < HRTIM_PCC_MAX_NUM_CHANNELS);
  obj->dacHandles[aChannel] = aDacHandle;
  obj->dacChannels[aChannel] = aDacChannel;
  obj->hrtimHandles[aChannel] = aHrtimHandle;
  obj->hrtimTimers[aChannel] = aHrtimTimer;
}

void PLX_HRTIM_PCC_setPeakCurrent(PLX_HRTIM_PCC_Handle_t aHandle, uint16_t aChannel, float aValue)
{
  PLX_HRTIM_PCC_Obj_t *obj = (PLX_HRTIM_PCC_Obj_t *)aHandle;

  if(aValue <= 0)
  {
    PLX_HRTIM_activateForcePwmChannelOff(obj->hrtimHandles[aChannel], (PLX_HRTIM_Channel_t)(2*obj->hrtimTimers[aChannel] + 0));
    PLX_HRTIM_activateForcePwmChannelOff(obj->hrtimHandles[aChannel], (PLX_HRTIM_Channel_t)(2*obj->hrtimTimers[aChannel] + 1));
  }
  else
  {
    PLX_DAC_setOut(obj->dacHandles[aChannel], obj->dacChannels[aChannel], aValue);
    PLX_HRTIM_releaseForcePwmChannelOff(obj->hrtimHandles[aChannel], (PLX_HRTIM_Channel_t)(2*obj->hrtimTimers[aChannel] + 0));
    PLX_HRTIM_releaseForcePwmChannelOff(obj->hrtimHandles[aChannel], (PLX_HRTIM_Channel_t)(2*obj->hrtimTimers[aChannel] + 1));
  }
}

#endif // PLX_HRTIM_AVAILABLE
