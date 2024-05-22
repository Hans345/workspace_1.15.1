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

#ifndef PLX_HAL_H_
#define PLX_HAL_H_

#include "pil.h"

extern float PLXHAL_ADC_getInjIn(uint16_t aHandle, uint16_t aChannel);

extern float PLXHAL_ADC_getRegIn(uint16_t aHandle, uint16_t aChannel);

extern void PLXHAL_DAC_setOut(uint16_t aHandle, uint16_t aChannel, float aValue);

extern void PLXHAL_DAC_setChannelOut(uint16_t aHandle, uint16_t aChannel, float aValue, float aMinValue, float aMaxValue);

extern bool PLXHAL_DIO_get(uint16_t aHandle);

extern void PLXHAL_DIO_set(uint16_t aHandle, bool aVal);

extern void PLXHAL_TIM_setDuty(uint16_t aHandle, uint16_t aChannel, float aDuty);

extern void PLXHAL_TIM_setPwmOutputState(uint16_t aHandle, uint16_t aChannel, uint8_t aEnable, uint8_t aEnableN);

extern void PLXHAL_TIM_enablePwmOutputs(uint16_t aHandle);

extern void PLXHAL_TIM_scalePeriod(uint16_t aHandle, float aScalingFactor);

extern void PLXHAL_PWR_setEnableRequest(bool aEnable);

extern bool PLXHAL_PWR_isEnabled();

extern void PLXHAL_PWR_syncdPwmEnable();

extern void PLXHAL_SINCOS_update(uint16_t aHandle, float aAngle);

extern float PLXHAL_SINCOS_getSin(uint16_t aHandle);

extern float PLXHAL_SINCOS_getCos(uint16_t aHandle);

extern void PLXHAL_HRTIM_enablePwmOutputs(uint16_t aHandle);

extern void PLXHAL_HRTIM_enablePwmOutputByMaster(uint16_t aHandle, int16_t aEnable);

extern void PLXHAL_HRTIM_setDuty(uint16_t aHandle, uint16_t aTimer, uint16_t aChannel, float aDuty, int16_t aEnable);

extern void PLXHAL_HRTIM_setDutyAndSampling(uint16_t aHandle, uint16_t aTimer, uint16_t aChannel, float aDuty, int16_t aEnable);

extern void PLXHAL_HRTIM_setDutyComplementary(uint16_t aHandle, uint16_t aTimer, float aDuty, int16_t aEnable);

extern void PLXHAL_HRTIM_setDutyComplementaryAndSampling(uint16_t aHandle, uint16_t aTimer, float aDuty, int16_t aEnable);

extern void PLXHAL_HRTIM_setPhase(uint16_t aHandle, uint16_t aSubtimer, float aPhase);

extern void PLXHAL_HRTIM_scalePeriod(uint16_t aHandle, float aScalingFactor);

extern void PLXHAL_HRTIM_applyPeriod(uint16_t aHandle);

extern void PLXHAL_HRTIM_scalePeriodHrtimUnit(uint16_t aHandle, uint16_t aTimer, float aScalingFactor);

extern void PLXHAL_HRTIM_enableBurstMode(uint16_t aHandle, float aValue);

extern void PLXHAL_HRTIM_setBurstModeCompare(uint16_t aHandle, float aPeriod);

extern void PLXHAL_HRTIM_PCC_setPeakCurrent(uint16_t aHandle, uint16_t aChannel, float aCurrent);

extern bool PLXHAL_SPI_getWords(uint16_t aHandle, uint16_t *aData, uint16_t aLen);

extern bool PLXHAL_SPI_putWords(uint16_t aHandle, uint16_t *aData, uint16_t aLen);

extern uint16_t PLXHAL_SPI_getRxFifoLevel(uint16_t aHandle);

extern bool PLXHAL_SPI_getAndResetRxOverrunFlag(uint16_t aHandle);

extern void PLXHAL_CAP_getData(uint16_t aHandle, uint32_t *aData, bool *aIsValid);

extern void PLXHAL_CAP_getAndClearOverrunFlag(uint16_t aHandle, bool *aOverrunOccurred);

extern uint32_t PLXHAL_QEP_getCounter(uint16_t aChannel);

extern bool PLXHAL_QEP_getAndClearIndexFlag(uint16_t aChannel);

extern bool PLXHAL_QEP_getIndexCaptureDataValid(uint16_t aChannel);

extern uint32_t PLXHAL_QEP_getIndexCaptureData(uint16_t aChannel);

extern uint32_t PLXHAL_CTR_getCounter(uint16_t aCtr);

extern void PLXHAL_CTR_setCounter(uint16_t aCtr, uint32_t aValue);

extern void PLXHAL_CAN_putMessage(uint16_t aChannel, uint16_t aMailBox, uint8_t data[]);

extern bool PLXHAL_CAN_getMessage(uint16_t aChannel, uint16_t aRxFifo, uint8_t *aBuffer, uint8_t *aFlags);

extern uint32_t PLXHAL_CAN_getRxFifoFillLevel(uint16_t aChannel, uint16_t aRxFifo);

extern uint32_t PLXHAL_CAN_getRxCanId(uint16_t aChannel, uint16_t aRxFifo);

extern void PLXHAL_CAN_setBusOn(uint16_t aChannel, bool aBusOn);

extern bool PLXHAL_CAN_getIsBusOn(uint16_t aChannel);

extern bool PLXHAL_CAN_getIsErrorActive(uint16_t aChannel);

extern float PLXHAL_DISPR_getTask0LoadInPercent();

extern uint32_t PLXHAL_DISPR_getTimeStamp0();
extern uint32_t PLXHAL_DISPR_getTimeStamp1();
extern uint32_t PLXHAL_DISPR_getTimeStamp2();
extern uint32_t PLXHAL_DISPR_getTimeStamp3();
extern uint32_t PLXHAL_DISPR_getTimeStampB();
extern uint32_t PLXHAL_DISPR_getTimeStampD();
extern uint32_t PLXHAL_DISPR_getTimeStampP();

#endif // PLX_HAL_H_