/*
   Copyright (c) 2019 by Plexim GmbH
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

#ifndef PLX_UART_H_
#define PLX_UART_H_

#include "plx_uart_impl.h"  // implementation specific

extern PLX_UART_Handle_t PLX_UART_init(void *aMemory, const size_t aNumBytes);
extern void PLX_UART_configure(PLX_UART_Handle_t aHandle, PLX_Usart_Unit_t port);
extern bool PLX_UART_setupPort(PLX_UART_Handle_t aHandle, uint32_t baudrate);
extern void PLX_UART_InitRingBuffer(PLX_UART_Handle_t aHandle, uint16_t* aBuffer, uint16_t aBufferSize);

extern bool PLX_UART_rxReady(PLX_UART_Handle_t aHandle);
extern bool PLX_UART_txIsBusy(PLX_UART_Handle_t aHandle);

extern uint16_t PLX_UART_getChar(PLX_UART_Handle_t aHandle);
extern void PLX_UART_putChar(PLX_UART_Handle_t aHandle, uint16_t aC);

extern bool PLX_UART_breakOccurred(PLX_UART_Handle_t aHandle);
extern void PLX_UART_reset(PLX_UART_Handle_t aHandle);

extern bool PLX_UART_bufferIsEmpty(PLX_UART_Handle_t aHandle);
extern bool PLX_UART_ringBufferPush(PLX_UART_Handle_t aHandle, uint16_t aC);
extern void PLX_UART_ringBufferPop(PLX_UART_Handle_t aHandle, uint16_t* aC);

extern void PLX_UART_IRQHandler(PLX_UART_Handle_t aHandle);

#endif /* PLX_UART_H_ */
