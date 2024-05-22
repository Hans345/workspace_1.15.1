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
#include "plx_opamp.h"

#ifdef PLX_OPAMP_AVAILABLE

#include "stm32g4xx_ll_bus.h"

void PLX_OPAMP_sinit()
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
}

PLX_OPAMP_Handle_t PLX_OPAMP_init(void *aMemory, const size_t aNumBytes)
{
	PLX_OPAMP_Handle_t handle;

	if(aNumBytes < sizeof(PLX_OPAMP_Obj_t))
		return((PLX_OPAMP_Handle_t)NULL);

	// set handle
	handle = (PLX_OPAMP_Handle_t)aMemory;

	return handle;
}

void PLX_OPAMP_setup(PLX_OPAMP_Handle_t aHandle, PLX_OPAMP_Unit_t aUnit)
{
  PLX_OPAMP_Obj_t *obj = (PLX_OPAMP_Obj_t *)aHandle;

  switch(aUnit){
    case PLX_OPAMP1:
      obj->instance = OPAMP1;
      break;
#ifdef OPAMP2
    case PLX_OPAMP2:
      obj->instance = OPAMP2;
      break;
#endif
#ifdef OPAMP3
    case PLX_OPAMP3:
      obj->instance = OPAMP3;
      break;
#endif
#ifdef OPAMP4
    case PLX_OPAMP4:
      obj->instance = OPAMP4;
      break;
#endif
#ifdef OPAMP5
    case PLX_OPAMP5:
      obj->instance = OPAMP5;
      break;
#endif
#ifdef OPAMP6
    case PLX_OPAMP6:
      obj->instance = OPAMP6;
      break;
#endif
    default:
      PLX_ASSERT(0);
  }
}

#endif // PLX_OPAMP_AVAILABLE
