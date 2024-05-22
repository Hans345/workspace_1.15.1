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

#include "includes.h"

#ifndef PLX_SPI_IMPL_H_
#define PLX_SPI_IMPL_H_

#include "stm32g4xx_ll_spi.h"

typedef enum PLX_SPI_UNIT {
    PLX_SPI_SPI1 = 0,
    PLX_SPI_SPI2,
    PLX_SPI_SPI3,
    PLX_SPI_SPI4
} PLX_SPI_Unit_t;

/* convention:
 * pol/phase = 0/0: inactive level low, first bit captured at rising edge of SCK (first edge)
 * pol/phase = 0/1: inactive level low, first bit captured at falling edge of SCK (first edge)
 * pol/phase = 1/0: inactive level high, first bit captured at falling edge of SCK (second edge)
 * pol/phase = 1/1: inactive level high, first bit captured at rising edge of SCK (second edge)
 */

typedef struct PLX_SPI_OBJ {
   PLX_SPI_Unit_t    unit;
   SPI_TypeDef       *instance;
   uint8_t           dataWidth;
} PLX_SPI_Obj_t;

typedef PLX_SPI_Obj_t *PLX_SPI_Handle_t;

__STATIC_INLINE uint32_t PLX_SPI_getTxFifoLevel(PLX_SPI_Handle_t aHandle)
{
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;
  uint32_t level = (obj->instance->SR & SPI_SR_FTLVL_Msk) >> SPI_SR_FTLVL_Pos;
  if ((obj->dataWidth > 8) && (level > 0))
  {
    level--;
  }
  return level;
}

__STATIC_INLINE uint32_t PLX_SPI_getRxFifoLevel(PLX_SPI_Handle_t aHandle)
{
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;
  uint32_t level = (obj->instance->SR & SPI_SR_FRLVL_Msk) >> SPI_SR_FRLVL_Pos;
  if ((obj->dataWidth > 8) && (level > 0))
  {
    level--;
  }
  return level;
}

__STATIC_INLINE bool PLX_SPI_getAndResetRxOverrunFlag(PLX_SPI_Handle_t aHandle)
{
    PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;
    uint32_t flag = LL_SPI_IsActiveFlag_OVR(obj->instance);
    if(flag != 0)
    {
      LL_SPI_ClearFlag_OVR(obj->instance);
      return true;
    }
    else
    {
      return false;
    }
}

__STATIC_INLINE bool PLX_SPI_putWords(PLX_SPI_Handle_t aHandle, uint16_t *aData, uint16_t aLen){
    PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;

    if (PLX_SPI_getTxFifoLevel(aHandle) > 0 )
    {
      return false;
    }

    int i;
    for (i=0; i < aLen; i++)
    {
      if (obj->dataWidth > 8)
      {
        LL_SPI_TransmitData16(obj->instance, aData[i]);
      }
      else
      {
        LL_SPI_TransmitData8(obj->instance, (uint8_t)aData[i]);
      }
    }
    return true;
}

__STATIC_INLINE bool PLX_SPI_getWords(PLX_SPI_Handle_t aHandle, uint16_t *aData, uint16_t aLen){
    PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;

    if (PLX_SPI_getRxFifoLevel(aHandle) < aLen)
    {
      return false;
    }

    int i;
    for (i=0; i<aLen; i++)
    {
      if (obj->dataWidth > 8)
      {
        aData[i] = LL_SPI_ReceiveData16(obj->instance);
      }
      else
      {
        aData[i] = (uint16_t)LL_SPI_ReceiveData8(obj->instance);
      }
    }
    return true;
}

#endif /* PLX_SPI_IMPL_H_ */
