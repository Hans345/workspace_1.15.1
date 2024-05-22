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

#include "includes.h"
#include "pil.h"

#ifndef PLX_CANBUS_H_
#define PLX_CANBUS_H_

#include "plx_canbus_impl.h" // implementation specific

extern void PLX_CANBUS_sinit();

extern PLX_CANBUS_Handle_t PLX_CANBUS_init(void *aMemory, const size_t aNumBytes);

extern PLX_CANBUS_TxHeader_t PLX_CANBUS_TxHeader_init(void *aMemory, const size_t aNumBytes);

extern void PLX_CANBUS_configure(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_Unit_t aUnit, FDCAN_InitTypeDef *aInitDef);

extern void PLX_CANBUS_start(PLX_CANBUS_Handle_t aHandle);

extern void PLX_CANBUS_setBusOn(PLX_CANBUS_Handle_t aHandle, bool busOn);

extern bool PLX_CANBUS_isBusOn(PLX_CANBUS_Handle_t aHandle);

extern bool PLX_CANBUS_isErrorActive(PLX_CANBUS_Handle_t aHandle);

extern FDCAN_HandleTypeDef* PLX_CANBUS_getHandle(PLX_CANBUS_Handle_t aHandle);

extern void PLX_CANBUS_configureTxHeader(PLX_CANBUS_TxHeader_t aHandle, FDCAN_TxHeaderTypeDef aTypeDef);

extern bool PLX_CANBUS_getMessage(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_RxFifo_t aRxFifo, uint8_t *aBuffer, uint8_t *aFlags);

extern bool PLX_CANBUS_putMessage(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_TxHeader_t aHeader, uint8_t data[]);

extern uint32_t PLX_CANBUS_getRxFifoFillLevel(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_RxFifo_t aRxFifo);

extern uint32_t PLX_CANBUS_getRxCanId(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_RxFifo_t aRxFifo);

#endif /* PLX_CANBUS_H_ */
