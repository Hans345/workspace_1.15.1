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

#include "plx_comp.h"

#ifdef PLX_COMP_AVAILABLE

void PLX_COMP_sinit()
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
}

PLX_COMP_Handle_t PLX_COMP_init(void *aMemory, const size_t aNumBytes)
{
	PLX_COMP_Handle_t handle;

	if(aNumBytes < sizeof(PLX_COMP_Obj_t))
		return((PLX_COMP_Handle_t)NULL);

	// set handle
	handle = (PLX_COMP_Handle_t)aMemory;

	return handle;
}

void PLX_COMP_setup(PLX_COMP_Handle_t aHandle, PLX_COMP_Unit_t aUnit)
{
  PLX_COMP_Obj_t *obj = (PLX_COMP_Obj_t *)aHandle;

  switch(aUnit){
    case PLX_COMP1:
      obj->instance = COMP1;
      break;
    case PLX_COMP2:
      obj->instance = COMP2;
      break;
    case PLX_COMP3:
      obj->instance = COMP3;
      break;
    case PLX_COMP4:
      obj->instance = COMP4;
      break;
#ifdef COMP5
    case PLX_COMP5:
      obj->instance = COMP5;
      break;
#endif
#ifdef COMP6
    case PLX_COMP6:
      obj->instance = COMP6;
      break;
#endif
#ifdef COMP7
    case PLX_COMP7:
      obj->instance = COMP7;
      break;
#endif
    default:
      PLX_ASSERT(0);
  }
}

#endif // PLX_COMP_AVAILABLE
