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

#ifndef PLX_CTR_IMPL_H_
#define PLX_CTR_IMPL_H_

#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_gpio.h"

typedef struct PLX_CTR_OBJ
{
   TIM_TypeDef *tim;
   uint32_t maxCtrValue;
} PLX_CTR_Obj_t;

typedef PLX_CTR_Obj_t *PLX_CTR_Handle_t;

typedef enum PLX_CTR_TIM_UNIT {
   PLX_CTR_TIM1 = 0,
   PLX_CTR_TIM2,
   PLX_CTR_TIM3,
   PLX_CTR_TIM4,
   PLX_CTR_TIM5,
   PLX_CTR_TIM6,
   PLX_CTR_TIM7,
   PLX_CTR_TIM8
} PLX_CTR_TimUnit_t;

__STATIC_INLINE uint32_t PLX_CTR_getCounter(PLX_CTR_Handle_t aHandle)
{
   PLX_CTR_Obj_t *obj = (PLX_CTR_Obj_t *)aHandle;
   return LL_TIM_GetCounter(obj->tim);
}

__STATIC_INLINE void PLX_CTR_setCounter(PLX_CTR_Handle_t aHandle, uint32_t aValue)
{
   PLX_CTR_Obj_t *obj = (PLX_CTR_Obj_t *)aHandle;
   uint32_t ctrValue = aValue;
   if (ctrValue > obj->maxCtrValue)
   {
      ctrValue = obj->maxCtrValue;
   }
   LL_TIM_SetCounter(obj->tim, ctrValue);
}

#endif /* PLX_CTR_IMPL_H_ */
