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

#ifndef PLX_CANBUS_IMPL_H_
#define PLX_CANBUS_IMPL_H_

#include "stm32g4xx_hal_fdcan.h"

typedef enum PLX_CANBUS_UNIT {

    PLX_CAN_CAN1 = 0,
    PLX_CAN_CAN2,
    PLX_CAN_CAN3
} PLX_CANBUS_Unit_t;

typedef enum PLX_CANBUS_RxFIFO {
  PLX_CAN_RX_FIFO_0 = 0,
  PLX_CAN_RX_FIFO_1
} PLX_CANBUS_RxFifo_t;

typedef struct PLX_CANBUS_OBJ
{
  PLX_CANBUS_Unit_t unit;
  FDCAN_HandleTypeDef  canHandle;
} PLX_CANBUS_Obj_t;

typedef PLX_CANBUS_Obj_t *PLX_CANBUS_Handle_t;

typedef struct PLX_CANBUS_TxHEADER_OBJ
{
  FDCAN_TxHeaderTypeDef header;
} PLX_CANBUS_TxHeader_Obj_t;

typedef PLX_CANBUS_TxHeader_Obj_t *PLX_CANBUS_TxHeader_t;

static const uint8_t PLX_DLCtoBytes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

#endif /* PLX_CANBUS_IMPL_H_ */
