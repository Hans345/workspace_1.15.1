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

#include "plx_uart.h"

PLX_UART_Handle_t PLX_UART_init(void *aMemory, const size_t aNumBytes)
{
   if(aNumBytes < sizeof(PLX_UART_Obj_t))
   {
      return((PLX_UART_Handle_t)NULL);
   }
   PLX_UART_Handle_t handle = (PLX_UART_Handle_t)aMemory;
   return handle;
}

void PLX_UART_configure(PLX_UART_Handle_t aHandle, PLX_Usart_Unit_t port)
{
   PLX_UART_Obj_t *obj = (PLX_UART_Obj_t *)aHandle;
   obj->port = port;
}

bool PLX_UART_setupPort(PLX_UART_Handle_t aHandle, uint32_t baudrate){
   PLX_UART_Obj_t *obj = (PLX_UART_Obj_t *)aHandle;

   switch(obj->port)
   {
     case PLX_USART1:
       obj->portHandle.Instance = USART1;
       break;

     case PLX_USART2:
       obj->portHandle.Instance = USART2;
       __HAL_RCC_USART2_CLK_ENABLE();
       break;

     case PLX_USART3:
       obj->portHandle.Instance = USART3;
       __HAL_RCC_USART3_CLK_ENABLE();
       break;

     default:
       PLX_ASSERT(0);
   }

   obj->portHandle.Init.BaudRate = baudrate;
   obj->portHandle.Init.WordLength = UART_WORDLENGTH_8B;
   obj->portHandle.Init.StopBits = UART_STOPBITS_1;
   obj->portHandle.Init.Parity = UART_PARITY_NONE;
   obj->portHandle.Init.Mode = UART_MODE_TX_RX;
   obj->portHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   obj->portHandle.Init.OverSampling = UART_OVERSAMPLING_8;
   if(HAL_UART_Init(&obj->portHandle) != HAL_OK){
      PLX_ASSERT(0);
   }
   if (HAL_UARTEx_EnableFifoMode(&obj->portHandle) != HAL_OK)
   {
     PLX_ASSERT(0);
   }
   return true;
}
