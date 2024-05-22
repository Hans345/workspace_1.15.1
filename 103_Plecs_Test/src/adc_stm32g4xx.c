/*
   Copyright (c) 2020-2021 by Plexim GmbH
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

#include "plx_adc.h"
#include "plx_dio.h"

typedef struct PLX_ADC_SOBJ
{
   float vref;
   bool adc12CommonInitPerformed;
   bool adc345CommonInitPerformed;
} PLX_ADC_SObj_t;

static PLX_ADC_SObj_t PLX_ADC_SObj;

void PLX_ADC_sinit(float aVref)
{
	PLX_ADC_SObj.vref = aVref;
  PLX_ADC_SObj.adc12CommonInitPerformed = false;
  PLX_ADC_SObj.adc345CommonInitPerformed = false;

	__HAL_RCC_ADC12_CLK_ENABLE();
#ifdef ADC345_COMMON
 __HAL_RCC_ADC345_CLK_ENABLE();
#endif

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
#ifdef GPIOE
	__HAL_RCC_GPIOE_CLK_ENABLE();
#endif
	__HAL_RCC_GPIOF_CLK_ENABLE();
#ifdef GPIOG
	__HAL_RCC_GPIOG_CLK_ENABLE();
#endif
#ifdef GPIOH
    __HAL_RCC_GPIOH_CLK_ENABLE();
#endif

}

PLX_ADC_Handle_t PLX_ADC_init(void *aMemory, const size_t aNumBytes)
{
	PLX_ADC_Handle_t handle;

	if(aNumBytes < sizeof(PLX_ADC_Obj_t))
		return((PLX_ADC_Handle_t)NULL);

	// set handle
	handle = (PLX_ADC_Handle_t)aMemory;

	return handle;
}

void PLX_ADC_setup(PLX_ADC_Handle_t aHandle, PLX_ADC_Unit_t aUnit)
{
   PLX_ADC_Obj_t *obj = (PLX_ADC_Obj_t *)aHandle;

   switch(aUnit){
      case PLX_ADC1:
      case PLX_ADC2:
      {
         if(!PLX_ADC_SObj.adc12CommonInitPerformed)
         {
            LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
            ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
            ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
            LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);

            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VBAT);
            PLX_ADC_SObj.adc12CommonInitPerformed = true;
         }
         if(aUnit == PLX_ADC1)
         {
            obj->adcInstance = ADC1;
         }
         else
         {
            obj->adcInstance = ADC2;
         }
      }
      break;
#ifdef ADC345_COMMON
      case PLX_ADC3:
      case PLX_ADC4:
      case PLX_ADC5:
      {
        if(!PLX_ADC_SObj.adc345CommonInitPerformed)
        {
           LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
           ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
           ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
           LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC3), &ADC_CommonInitStruct);

           LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC3), LL_ADC_PATH_INTERNAL_VREFINT);
           LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC3), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
           LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC3), LL_ADC_PATH_INTERNAL_VBAT);
           PLX_ADC_SObj.adc345CommonInitPerformed = true;
        }
        if(aUnit == PLX_ADC3)
        {
           obj->adcInstance = ADC3;
        }
        else if (aUnit == PLX_ADC4)
        {
           obj->adcInstance = ADC4;
        }
        else
        {
          obj->adcInstance = ADC5;
        }
      }
      break;
#endif
      default:
         PLX_ASSERT(0);
   }
   obj->numRegularConversions = 0;
   obj->numInjectedConversions = 0;
}

void PLX_ADC_addChannel(PLX_ADC_Handle_t aHandle, float aScale, float aOffset){
	PLX_ADC_Obj_t *obj = (PLX_ADC_Obj_t *)aHandle;

	obj->scale[4+obj->numRegularConversions] = aScale * PLX_ADC_SObj.vref/4096.0;
	obj->offset[4+obj->numRegularConversions] = aOffset;
	obj->numRegularConversions++;
}

void PLX_ADC_addInjectedChannel(PLX_ADC_Handle_t aHandle, float aScale, float aOffset){
	PLX_ADC_Obj_t *obj = (PLX_ADC_Obj_t *)aHandle;

	obj->scale[obj->numInjectedConversions] = aScale * PLX_ADC_SObj.vref/4096.0;
	obj->offset[obj->numInjectedConversions] = aOffset;
	obj->numInjectedConversions++;
}

void PLX_ADC_start(PLX_ADC_Handle_t aHandle){
  PLX_ADC_Obj_t *obj = (PLX_ADC_Obj_t *)aHandle;

  if(obj->numRegularConversions > 0)
  {
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    uint32_t dmaChannel = 0;
    uint32_t periphRequest = 0;
    if(obj->adcInstance == ADC1)
    {
      dmaChannel = LL_DMA_CHANNEL_1;
      periphRequest = LL_DMAMUX_REQ_ADC1;
    }
    else if (obj->adcInstance == ADC2)
    {
      dmaChannel = LL_DMA_CHANNEL_2;
      periphRequest = LL_DMAMUX_REQ_ADC2;
    }
#ifdef ADC345_COMMON
    else if (obj->adcInstance == ADC3)
    {
      dmaChannel = LL_DMA_CHANNEL_3;
      periphRequest = LL_DMAMUX_REQ_ADC3;
    }
    else if (obj->adcInstance == ADC4)
    {
      dmaChannel = LL_DMA_CHANNEL_4;
      periphRequest = LL_DMAMUX_REQ_ADC4;
    }
    else
    {
      dmaChannel = LL_DMA_CHANNEL_5;
      periphRequest = LL_DMAMUX_REQ_ADC5;
    }
#endif
    LL_DMA_ConfigTransfer(DMA1, dmaChannel,
        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
        LL_DMA_PRIORITY_MEDIUM            |
        LL_DMA_MODE_CIRCULAR              |
        LL_DMA_PERIPH_NOINCREMENT         |
        LL_DMA_MEMORY_INCREMENT           |
        LL_DMA_PDATAALIGN_HALFWORD        |
        LL_DMA_MDATAALIGN_HALFWORD);
    LL_DMA_ConfigAddresses(DMA1, dmaChannel,
        LL_ADC_DMA_GetRegAddr(obj->adcInstance, LL_ADC_DMA_REG_REGULAR_DATA),
        (uint32_t)&obj->results[4],
        LL_DMA_GetDataTransferDirection(DMA1, dmaChannel));
    LL_DMA_SetDataLength(DMA1, dmaChannel, obj->numRegularConversions);
    LL_DMA_SetPeriphRequest(DMA1, dmaChannel, periphRequest);
    LL_DMA_EnableChannel(DMA1, dmaChannel);
  }

  if (LL_ADC_IsEnabled(obj->adcInstance) == 0)
  {
    LL_ADC_DisableDeepPowerDown(obj->adcInstance);
    LL_ADC_EnableInternalRegulator(obj->adcInstance);

    __IO uint32_t waitLoopCounts = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(waitLoopCounts != 0)
    {
      waitLoopCounts--;
    }

    // run ADC self calibration
    LL_ADC_StartCalibration(obj->adcInstance, LL_ADC_SINGLE_ENDED);
    while (LL_ADC_IsCalibrationOnGoing(obj->adcInstance) != 0)
    {
      continue;
    }

    // Delay between ADC end of calibration and ADC enable.
    waitLoopCounts = (PLX_ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
    while(waitLoopCounts != 0)
    {
      waitLoopCounts--;
    }
    LL_ADC_Enable(obj->adcInstance);

    while (LL_ADC_IsActiveFlag_ADRDY(obj->adcInstance) == 0)
    {
      continue;
    }
  }

  if(obj->numRegularConversions > 0){
    LL_ADC_REG_StartConversion(obj->adcInstance);
  }

  if(obj->numInjectedConversions > 0){
    LL_ADC_INJ_StartConversion(obj->adcInstance);
    LL_ADC_EnableIT_JEOS(obj->adcInstance);
  }
}

