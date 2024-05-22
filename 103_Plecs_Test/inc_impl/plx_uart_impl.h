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

#include "includes.h"

#ifndef PLX_UART_IMPL_H_
#define PLX_UART_IMPL_H_

#include "stm32g4xx_hal_uart.h"

typedef enum PLX_USART_UNIT {
   PLX_USART1 = 0,
   PLX_USART2,
   PLX_USART3
} PLX_Usart_Unit_t;

typedef struct PLX_UART_OBJ {
   PLX_Usart_Unit_t port;
   UART_HandleTypeDef portHandle;
} PLX_UART_Obj_t;

typedef PLX_UART_Obj_t *PLX_UART_Handle_t;

__STATIC_INLINE void PLX_UART_reset(PLX_UART_Handle_t aHandle)
{
   UNUSED(aHandle);
}

__STATIC_INLINE uint16_t PLX_UART_getChar(PLX_UART_Handle_t aHandle)
{
    PLX_UART_Obj_t *obj = (PLX_UART_Obj_t *)aHandle;
    return obj->portHandle.Instance->RDR;
}

__STATIC_INLINE void PLX_UART_putChar(PLX_UART_Handle_t aHandle, uint16_t c)
{
    PLX_UART_Obj_t *obj = (PLX_UART_Obj_t *)aHandle;
    obj->portHandle.Instance->TDR = (c & 0xFF);
}

__STATIC_INLINE bool PLX_UART_rxReady(PLX_UART_Handle_t aHandle)
{
    PLX_UART_Obj_t *obj = (PLX_UART_Obj_t *)aHandle;
    return (__HAL_UART_GET_FLAG(&obj->portHandle, UART_FLAG_RXNE) == 1);
}

__STATIC_INLINE bool PLX_UART_txIsBusy(PLX_UART_Handle_t aHandle)
{
    PLX_UART_Obj_t *obj = (PLX_UART_Obj_t *)aHandle;
    return (__HAL_UART_GET_FLAG(&obj->portHandle, UART_FLAG_TXE) == 0);
}

__STATIC_INLINE bool PLX_UART_breakOccurred(PLX_UART_Handle_t aHandle)
{
   UNUSED(aHandle);
   return 0;
}

#endif /* PLX_UART_IMPL_H_ */
