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

#ifndef PLX_SPI_H_
#define PLX_SPI_H_

#include "plx_spi_impl.h" // implementation specific

extern PLX_SPI_Handle_t PLX_SPI_init(void *aMemory, const size_t aNumBytes);
extern void PLX_SPI_setup(PLX_SPI_Handle_t aHandle, PLX_SPI_Unit_t aUnit, uint16_t aCharlen);
extern void PLX_SPI_start(PLX_SPI_Handle_t aHandle);

extern bool PLX_SPI_getAndResetRxOverrunFlag(PLX_SPI_Handle_t aHandle);

extern uint32_t PLX_SPI_getTxFifoLevel(PLX_SPI_Handle_t aHandle);
extern uint32_t PLX_SPI_getRxFifoLevel(PLX_SPI_Handle_t aHandle);

extern bool PLX_SPI_putWords(PLX_SPI_Handle_t aHandle, uint16_t *aData, uint16_t aLen);
extern bool PLX_SPI_getWords(PLX_SPI_Handle_t aHandle, uint16_t *aData, uint16_t aLen);

#endif /* PLX_SPI_H_ */
