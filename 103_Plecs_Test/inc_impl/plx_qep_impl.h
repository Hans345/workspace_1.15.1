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

#ifndef PLX_QEP_IMPL_H_
#define PLX_QEP_IMPL_H_

#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_gpio.h"

typedef struct PLX_QEP_OBJ
{
   TIM_TypeDef *tim;
   char sign;
} PLX_QEP_Obj_t;

typedef PLX_QEP_Obj_t *PLX_QEP_Handle_t;

typedef enum PLX_QEP_TIM_UNIT {
   PLX_QEP_TIM1 = 0,
   PLX_QEP_TIM2,
   PLX_QEP_TIM3,
   PLX_QEP_TIM4,
   PLX_QEP_TIM5,
   PLX_QEP_TIM6,
   PLX_QEP_TIM7,
   PLX_QEP_TIM8
} PLX_QEP_TimUnit_t;

__STATIC_INLINE void PLX_QEP_sinit()
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

PLX_QEP_Handle_t PLX_QEP_init(void *aMemory, const size_t aNumBytes)
{
   PLX_QEP_Handle_t handle;

   if(aNumBytes < sizeof(PLX_QEP_Obj_t))
      return((PLX_QEP_Handle_t)NULL);

   // set handle
   handle = (PLX_QEP_Handle_t)aMemory;

   return handle;
}

void PLX_QEP_setup(PLX_QEP_Handle_t aHandle, PLX_QEP_TimUnit_t aUnit, char aSign)
{
   PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;

   switch(aUnit){
      case PLX_QEP_TIM1:
         obj->tim = TIM1;
         break;
      case PLX_QEP_TIM2:
         obj->tim = TIM2;
         break;
      case PLX_QEP_TIM3:
         obj->tim = TIM3;
         break;
      case PLX_QEP_TIM4:
         obj->tim = TIM4;
         break;
#ifdef TIM5
      case PLX_QEP_TIM5:
         obj->tim = TIM5;
         break;
#endif
      case PLX_QEP_TIM6:
         obj->tim = TIM6;
         break;
      case PLX_QEP_TIM7:
         obj->tim = TIM7;
         break;
      case PLX_QEP_TIM8:
         obj->tim = TIM8;
         break;
      default:
         PLX_ASSERT(0);
   }
   obj->sign = aSign;
}

__STATIC_INLINE uint32_t PLX_QEP_invertCount(PLX_QEP_Handle_t aHandle, uint32_t count){
   PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;
   if (count == 0){
      return 0;
   } else {
      return LL_TIM_GetAutoReload(obj->tim) - count;
   }
}

__STATIC_INLINE uint32_t PLX_QEP_getCounter(PLX_QEP_Handle_t aHandle)
{
    PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;
    uint32_t count = LL_TIM_GetCounter(obj->tim);
    if (obj->sign > 0){
       return count;
    } else {
       return PLX_QEP_invertCount(aHandle, count);
    }
}

__STATIC_INLINE bool PLX_QEP_getAndClearIndexFlag(PLX_QEP_Handle_t aHandle)
{
    PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;
    uint32_t flag = LL_TIM_IsActiveFlag_IDX(obj->tim);
    LL_TIM_ClearFlag_IDX(obj->tim);
    return flag;
}

__STATIC_INLINE bool PLX_QEP_getIndexCaptureDataValid(PLX_QEP_Handle_t aHandle)
{
    PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;
    return LL_TIM_IsActiveFlag_CC3(obj->tim);
}

__STATIC_INLINE uint32_t PLX_QEP_getIndexCaptureData(PLX_QEP_Handle_t aHandle)
{
    PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;

    uint32_t count = LL_TIM_IC_GetCaptureCH3(obj->tim);
    if (obj->sign > 0){
       return count;
    } else {
       return PLX_QEP_invertCount(aHandle, count);
    }
}

#endif /* PLX_QEP_IMPL_H_ */
