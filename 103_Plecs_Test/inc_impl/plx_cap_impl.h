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

#ifndef PLX_CAP_IMPL_H_
#define PLX_CAP_IMPL_H_

#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_gpio.h"

typedef struct PLX_CAP_OBJ
{
   TIM_TypeDef *tim;
} PLX_CAP_Obj_t;

typedef PLX_CAP_Obj_t *PLX_CAP_Handle_t;

typedef enum PLX_CAP_TIM_UNIT {
   PLX_CAP_TIM1 = 0,
   PLX_CAP_TIM2,
   PLX_CAP_TIM3,
   PLX_CAP_TIM4,
   PLX_CAP_TIM5,
   PLX_CAP_TIM6,
   PLX_CAP_TIM7,
   PLX_CAP_TIM8
} PLX_CAP_TimUnit_t;

__STATIC_INLINE void PLX_CAP_sinit()
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

PLX_CAP_Handle_t PLX_CAP_init(void *aMemory, const size_t aNumBytes)
{
   PLX_CAP_Handle_t handle;

   if(aNumBytes < sizeof(PLX_CAP_Obj_t))
      return((PLX_CAP_Handle_t)NULL);

   // set handle
   handle = (PLX_CAP_Handle_t)aMemory;

   return handle;
}

void PLX_CAP_setup(PLX_CAP_Handle_t aHandle, PLX_CAP_TimUnit_t aUnit)
{
   PLX_CAP_Obj_t *obj = (PLX_CAP_Obj_t *)aHandle;

   switch(aUnit){
      case PLX_CAP_TIM1:
         obj->tim = TIM1;
         break;
      case PLX_CAP_TIM2:
         obj->tim = TIM2;
         break;
      case PLX_CAP_TIM3:
         obj->tim = TIM3;
         break;
      case PLX_CAP_TIM4:
         obj->tim = TIM4;
         break;
#ifdef TIM5
      case PLX_CAP_TIM5:
          obj->tim = TIM5;
          break;
#endif
      case PLX_CAP_TIM6:
         obj->tim = TIM6;
         break;
      case PLX_CAP_TIM7:
         obj->tim = TIM7;
         break;
      case PLX_CAP_TIM8:
         obj->tim = TIM8;
         break;
      default:
         PLX_ASSERT(0);
   }
}

__STATIC_INLINE void PLX_CAP_getData(PLX_CAP_Handle_t aHandle, uint32_t *aData, bool *aIsValid)
{
   PLX_CAP_Obj_t *obj = (PLX_CAP_Obj_t *)aHandle;
   aIsValid[0] = LL_TIM_IsActiveFlag_CC1(obj->tim);
   if(aIsValid[0])
   {
      aData[0] = LL_TIM_IC_GetCaptureCH1(obj->tim);
   }
   aIsValid[1] = LL_TIM_IsActiveFlag_CC2(obj->tim);
   if(aIsValid[1])
   {
      aData[1] = LL_TIM_IC_GetCaptureCH2(obj->tim);
   }
   aIsValid[2] = LL_TIM_IsActiveFlag_CC3(obj->tim);
   if(aIsValid[2])
   {
      aData[2] = LL_TIM_IC_GetCaptureCH3(obj->tim);
   }
}

__STATIC_INLINE void PLX_CAP_getAndClearOverrunFlag(PLX_CAP_Handle_t aHandle, bool *aOverrunOccurred)
{
   PLX_CAP_Obj_t *obj = (PLX_CAP_Obj_t *)aHandle;

   aOverrunOccurred[0] = LL_TIM_IsActiveFlag_CC1OVR(obj->tim);
   aOverrunOccurred[1] = LL_TIM_IsActiveFlag_CC2OVR(obj->tim);
   aOverrunOccurred[2] = LL_TIM_IsActiveFlag_CC3OVR(obj->tim);
   LL_TIM_ClearFlag_CC1OVR(obj->tim);
   LL_TIM_ClearFlag_CC2OVR(obj->tim);
   LL_TIM_ClearFlag_CC3OVR(obj->tim);
}

#endif /* PLX_CAP_IMPL_H_ */
