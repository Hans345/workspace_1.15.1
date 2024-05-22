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

#include "plx_fdcanbus.h"

void PLX_CANBUS_sinit()
{
   static bool firstCall = true;
   if(firstCall){
      firstCall = false;
      __HAL_RCC_FDCAN_CLK_ENABLE();
   }
}

PLX_CANBUS_Handle_t PLX_CANBUS_init(void *aMemory, const size_t aNumBytes)
{
  PLX_CANBUS_Handle_t handle;

  if(aNumBytes < sizeof(PLX_CANBUS_Obj_t))
    return((PLX_CANBUS_Handle_t)NULL);

  // set handle
  handle = (PLX_CANBUS_Handle_t)aMemory;

  return handle;
}

PLX_CANBUS_TxHeader_t PLX_CANBUS_TxHeader_init(void *aMemory, const size_t aNumBytes)
{
  PLX_CANBUS_TxHeader_t handle;

  if(aNumBytes < sizeof(PLX_CANBUS_TxHeader_Obj_t))
    return((PLX_CANBUS_TxHeader_t)NULL);

  // set handle
  handle = (PLX_CANBUS_TxHeader_t)aMemory;

  return handle;
}

void PLX_CANBUS_configure(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_Unit_t aUnit, FDCAN_InitTypeDef *aInitDef)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  obj->unit = aUnit;

  switch(obj->unit)
  {
    case PLX_CAN_CAN1:
      obj->canHandle.Instance = FDCAN1;
      break;

#ifdef FDCAN2
    case PLX_CAN_CAN2:
      obj->canHandle.Instance = FDCAN2;
      break;
#endif

#ifdef FDCAN3
    case PLX_CAN_CAN3:
      obj->canHandle.Instance = FDCAN3;
      break;
#endif

    default:
      PLX_ASSERT(0);
  }
  obj->canHandle.Init = *aInitDef;
  if (HAL_FDCAN_Init(&obj->canHandle) != HAL_OK)
  {
    PLX_ASSERT(0);
  }

  if (HAL_FDCAN_ConfigGlobalFilter(&obj->canHandle, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    PLX_ASSERT(0);
  }
}

void PLX_CANBUS_configureTxHeader(PLX_CANBUS_TxHeader_t aHandle, FDCAN_TxHeaderTypeDef aTypeDef)
{
  PLX_CANBUS_TxHeader_Obj_t *obj = (PLX_CANBUS_TxHeader_Obj_t*)aHandle;
  obj->header = aTypeDef;
}

FDCAN_HandleTypeDef* PLX_CANBUS_getHandle(PLX_CANBUS_Handle_t aHandle)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  return &obj->canHandle;
}

void PLX_CANBUS_start(PLX_CANBUS_Handle_t aHandle)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  if (HAL_FDCAN_Start(&obj->canHandle) != HAL_OK)
  {
    PLX_ASSERT(0);
  }
}

void PLX_CANBUS_setBusOn(PLX_CANBUS_Handle_t aHandle, bool busOn)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  if (busOn)
  {
    HAL_FDCAN_Start(&obj->canHandle);
  }
  else
  {
    HAL_FDCAN_Stop(&obj->canHandle);
  }
}

bool PLX_CANBUS_isBusOn(PLX_CANBUS_Handle_t aHandle)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  if (obj->canHandle.State == HAL_FDCAN_STATE_BUSY)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool PLX_CANBUS_putMessage(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_TxHeader_t aHeader, uint8_t data[])
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  bool success;
  if (HAL_FDCAN_AddMessageToTxFifoQ(&obj->canHandle, &aHeader->header, data) != HAL_OK)
  {
    success = false;
  }
  else
  {
    success = true;
  }
  return success;
}

bool PLX_CANBUS_getMessage(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_RxFifo_t aRxFifo, uint8_t *aBuffer, uint8_t *aFlags)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  FDCAN_RxHeaderTypeDef RxHeader;
  uint32_t rxFifo;
  bool success = false;

  if (aRxFifo == PLX_CAN_RX_FIFO_0)
  {
    rxFifo = FDCAN_RX_FIFO0;
  }
  // PLX_CAN_RX_FIFO_1
  else
  {
    rxFifo = FDCAN_RX_FIFO1;
  }
  aFlags[0] = 0;
  aFlags[1] = 0;
  aFlags[2] = 0;
  if (HAL_FDCAN_GetRxMessage(&obj->canHandle, rxFifo, &RxHeader, aBuffer) == HAL_OK)
  {
    success = true;
    if (RxHeader.ErrorStateIndicator)
    {
      aFlags[0] = 1;
    }
    if (RxHeader.FDFormat)
    {
      aFlags[1] = 1;
    }
    if (RxHeader.BitRateSwitch)
    {
      aFlags[2] = 1;
    }
  }
  return success;
}

uint32_t PLX_CANBUS_getRxFifoFillLevel(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_RxFifo_t aRxFifo)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  uint32_t rxFifo;

  if (aRxFifo == PLX_CAN_RX_FIFO_0)
  {
    rxFifo = FDCAN_RX_FIFO0;
  }
  // PLX_CAN_RX_FIFO_1
  else
  {
    rxFifo = FDCAN_RX_FIFO1;
  }

  return HAL_FDCAN_GetRxFifoFillLevel(&obj->canHandle, rxFifo);
}

uint32_t PLX_CANBUS_getRxCanId(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_RxFifo_t aRxFifo)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  uint32_t *RxAddress;
  uint32_t GetIndex;
  uint32_t canId = 0;

  if (aRxFifo == PLX_CAN_RX_FIFO_0) /* Rx element is assigned to the Rx FIFO 0 */
  {
    /* Calculate Rx FIFO 0 element address */
    GetIndex = ((obj->canHandle.Instance->RXF0S & FDCAN_RXF0S_F0GI) >> FDCAN_RXF0S_F0GI_Pos);
    RxAddress = (uint32_t *)(obj->canHandle.msgRam.RxFIFO0SA + (GetIndex * (18U * 4U)));
  }
  else /* Rx element is assigned to the Rx FIFO 1 */
  {
    /* Calculate Rx FIFO 1 element address */
    GetIndex = ((obj->canHandle.Instance->RXF1S & FDCAN_RXF1S_F1GI) >> FDCAN_RXF1S_F1GI_Pos);
    RxAddress = (uint32_t *)(obj->canHandle.msgRam.RxFIFO1SA + (GetIndex * (18U * 4U)));
  }
  /* Retrieve Identifier */
  if ((*RxAddress & ((uint32_t)0x40000000U)) == FDCAN_STANDARD_ID) /* Standard ID element */
  {
    canId = (((*RxAddress & ((uint32_t)0x1FFC0000U)) >> 18U));
  }
  else /* Extended ID element */
  {
    canId = (*RxAddress & ((uint32_t)0x1FFFFFFFU));
  }
  return canId;
}

bool PLX_CANBUS_isErrorActive(PLX_CANBUS_Handle_t aHandle)
{
  PLX_CANBUS_Obj_t *obj = (PLX_CANBUS_Obj_t*)aHandle;
  bool hasError;
  if (obj->canHandle.ErrorCode == 0)
  {
    hasError = false;
  }
  else
  {
    hasError = true;
  }
  return hasError;
}
