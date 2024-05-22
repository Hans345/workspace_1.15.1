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

#include "plx_spi.h"
#include "stm32g4xx_ll_bus.h"

PLX_SPI_Handle_t PLX_SPI_init(void *aMemory, const size_t aNumBytes)
{
  if(aNumBytes < sizeof(PLX_SPI_Obj_t))
  {
    return((PLX_SPI_Handle_t)NULL);
  }
  PLX_SPI_Handle_t handle = (PLX_SPI_Handle_t)aMemory;
  return handle;
}

void PLX_SPI_setup(PLX_SPI_Handle_t aHandle, PLX_SPI_Unit_t aUnit, uint16_t aDatawidth)
{
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;
  obj->unit = aUnit;
  obj->dataWidth = aDatawidth;
  switch(obj->unit)
  {
    case PLX_SPI_SPI1:
      obj->instance = SPI1;
      LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
      break;
    case PLX_SPI_SPI2:
      obj->instance = SPI2;
      LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
      break;
    case PLX_SPI_SPI3:
      obj->instance = SPI3;
      LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
      break;
#ifdef SPI4
    case PLX_SPI_SPI4:
      obj->instance = SPI4;
      LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI4);
      break;
#endif
    default:
      PLX_ASSERT(0);
  }
}

void PLX_SPI_start(PLX_SPI_Handle_t aHandle)
{
  PLX_SPI_Obj_t *obj = (PLX_SPI_Obj_t *)aHandle;
  LL_SPI_Enable(obj->instance);
}
