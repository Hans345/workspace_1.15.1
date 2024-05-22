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

#include "plx_ctr.h"

#include "stm32g4xx_ll_tim.h"

void PLX_CTR_sinit()
{
   static bool firstCall = true;
   if(firstCall){
      firstCall = false;
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
   }
}

PLX_CTR_Handle_t PLX_CTR_init(void *aMemory, const size_t aNumBytes)
{
   PLX_CTR_Handle_t handle;

   if(aNumBytes < sizeof(PLX_CTR_Obj_t))
      return((PLX_CTR_Handle_t)NULL);

   // set handle
   handle = (PLX_CTR_Handle_t)aMemory;

   return handle;
}

void PLX_CTR_setup(PLX_CTR_Handle_t aHandle, PLX_CTR_TimUnit_t aUnit, uint32_t aMaxValue)
{
   PLX_CTR_Obj_t *obj = (PLX_CTR_Obj_t *)aHandle;

   switch(aUnit){
      case PLX_CTR_TIM1:
         obj->tim = TIM1;
         break;
      case PLX_CTR_TIM2:
         obj->tim = TIM2;
         break;
      case PLX_CTR_TIM3:
         obj->tim = TIM3;
         break;
      case PLX_CTR_TIM4:
         obj->tim = TIM4;
         break;
#ifdef TIM5
      case PLX_CTR_TIM5:
         obj->tim = TIM5;
         break;
#endif
      case PLX_CTR_TIM6:
         obj->tim = TIM6;
         break;
      case PLX_CTR_TIM7:
         obj->tim = TIM7;
         break;
      case PLX_CTR_TIM8:
         obj->tim = TIM8;
         break;
      default:
         PLX_ASSERT(0);
   }
   obj->maxCtrValue = aMaxValue;
}
