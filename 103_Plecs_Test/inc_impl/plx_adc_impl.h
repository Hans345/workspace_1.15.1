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

#ifndef PLX_ADC_IMPL_H_
#define PLX_ADC_IMPL_H_

#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_adc.h"

/* Delay between ADC end of calibration and ADC enable.                     */
/* Delay estimation in CPU cycles: Case of ADC enable done                  */
/* immediately after ADC calibration, ADC clock setting slow                */
/* (LL_ADC_CLOCK_ASYNC_DIV32). Use a higher delay if ratio                  */
/* (CPU clock / ADC clock) is above 32.                                     */
#define PLX_ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)

typedef enum PLX_ADC_UNIT {
	PLX_ADC1 = 0,
	PLX_ADC2,
	PLX_ADC3,
	PLX_ADC4,
	PLX_ADC5
} PLX_ADC_Unit_t;

typedef struct PLX_ADC_OBJ
{
  ADC_TypeDef *adcInstance;
	uint16_t numRegularConversions;
	uint16_t numInjectedConversions;
	uint16_t results[32];
	float scale[32];
	float offset[32];
} PLX_ADC_Obj_t;

typedef PLX_ADC_Obj_t *PLX_ADC_Handle_t;

__STATIC_INLINE void PLX_ADC_updateInjected(PLX_ADC_Handle_t aHandle)
{
	PLX_ADC_Obj_t *obj = (PLX_ADC_Obj_t *)aHandle;

	obj->results[0] = LL_ADC_INJ_ReadConversionData12(obj->adcInstance, LL_ADC_INJ_RANK_1);
  obj->results[1] = LL_ADC_INJ_ReadConversionData12(obj->adcInstance, LL_ADC_INJ_RANK_2);
  obj->results[2] = LL_ADC_INJ_ReadConversionData12(obj->adcInstance, LL_ADC_INJ_RANK_3);
  obj->results[3] = LL_ADC_INJ_ReadConversionData12(obj->adcInstance, LL_ADC_INJ_RANK_4);
}

__STATIC_INLINE float PLX_ADC_getIn(PLX_ADC_Handle_t aHandle, uint16_t aChannel)
{
    PLX_ADC_Obj_t *obj = (PLX_ADC_Obj_t *)aHandle;
    return ((float)obj->results[aChannel] * obj->scale[aChannel] + obj->offset[aChannel]);

    // NOTE: some internal channels require specific post-processing:
    //return (float)(__LL_ADC_CALC_VREFANALOG_VOLTAGE(obj->results[aChannel], ADC_RESOLUTION_12B))/1000.0;
 	//return (float)(__LL_ADC_CALC_TEMPERATURE(3300, obj->results[aChannel], ADC_RESOLUTION_12B));
 }

#endif /* PLX_ADC_IMPL_H_ */
