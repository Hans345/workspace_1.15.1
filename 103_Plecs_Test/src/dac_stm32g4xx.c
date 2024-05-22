/*
   Copyright (c) 2021 by Plexim GmbH
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

#include "plx_dac.h"

#ifdef PLX_DAC_AVAILABLE

#include "stm32g4xx_ll_bus.h"

typedef struct PLX_DAC_SOBJ
{
  float vref;
} PLX_DAC_SObj_t;

static PLX_DAC_SObj_t PLX_DAC_SObj;

void PLX_DAC_sinit(float aVref)
{
  PLX_DAC_SObj.vref = aVref;
#ifdef DAC1
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_DAC1);
#endif
#ifdef DAC2
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_DAC2);
#endif
#ifdef DAC3
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_DAC3);
#endif
#ifdef DAC4
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_DAC4);
#endif
}

PLX_DAC_Handle_t PLX_DAC_init(void *aMemory, const size_t aNumBytes)
{
  PLX_DAC_Handle_t handle;

  if(aNumBytes < sizeof(PLX_DAC_Obj_t))
    return((PLX_DAC_Handle_t)NULL);

  // set handle
  handle = (PLX_DAC_Handle_t)aMemory;

  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)handle;
  obj->channel[0] = LL_DAC_CHANNEL_1;
  obj->channel[1] = LL_DAC_CHANNEL_2;
  obj->configured[0] = false;
  obj->configured[1] = false;
  return handle;
}

void PLX_DAC_setup(PLX_DAC_Handle_t aHandle, PLX_DAC_Unit_t aUnit)
{
  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)aHandle;

  obj->unit = aUnit;
  switch(obj->unit){
#ifdef DAC1
    case PLX_DAC1:
      obj->instance = DAC1;
      break;
#endif
#ifdef DAC2
    case PLX_DAC2:
      obj->instance = DAC2;
      break;
#endif
#ifdef DAC3
    case PLX_DAC3:
      obj->instance = DAC3;
      break;
#endif
#ifdef DAC4
    case PLX_DAC4:
      obj->instance = DAC4;
      break;
#endif
    default:
      PLX_ASSERT(0);
  }
}

void PLX_DAC_configureChannel(PLX_DAC_Handle_t aHandle, uint16_t aChannel, PLX_DAC_Waveform_t aWaveform, float aScale, float aOffset){
  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)aHandle;

  PLX_ASSERT(aChannel < PLX_DAC_MAX_NUM_CHANNELS);
  obj->waveform[aChannel] = aWaveform;
  obj->scale[aChannel] = aScale * 4096.0/PLX_DAC_SObj.vref;
  obj->offset[aChannel] = aOffset * 4096.0/PLX_DAC_SObj.vref;
  obj->configured[aChannel] = true;
}

void PLX_DAC_activate(PLX_DAC_Handle_t aHandle){
  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)aHandle;
  uint16_t i;
  for(i=0; i<PLX_DAC_MAX_NUM_CHANNELS; i++)
  {
    if(obj->configured[i])
    {
      LL_DAC_Enable(obj->instance, obj->channel[i]);
      // should we add a wait here?
      LL_DAC_EnableTrigger(obj->instance, obj->channel[i]);
    }
  }
}

void PLX_DAC_setOut(PLX_DAC_Handle_t aHandle, uint16_t aChannel, float aValue)
{
  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)aHandle;

  float val = aValue * obj->scale[aChannel] + obj->offset[aChannel];
  if(val < 0){
    val = 0;
  } else if (val >= 4096){
    val = 4095;
  }

  if(obj->waveform[aChannel] == PLX_DAC_DC)
  {
    LL_DAC_ConvertData12RightAligned(obj->instance, obj->channel[aChannel], (uint32_t)val);
    LL_DAC_TrigSWConversion(obj->instance, obj->channel[aChannel]);
  }
  else
  {
    LL_DAC_SetWaveSawtoothResetData(obj->instance, obj->channel[aChannel], (uint32_t)val);
  }
}

#endif // PLX_DAC_AVAILABLE
