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

#ifndef PLX_DIO_H_
#define PLX_DIO_H_

#include "plx_dio_impl.h" // implementation specific

extern PLX_DIO_Handle_t PLX_DIO_init(void *aMemory, const size_t aNumBytes);
extern void PLX_DIO_configureIn(PLX_DIO_Handle_t aHandle, PLX_GPIO_Port_t aPort, uint16_t aPin, bool activeHigh);
extern void PLX_DIO_configureOut(PLX_DIO_Handle_t aHandle, PLX_GPIO_Port_t aPort, uint16_t aPin, bool activeHigh);

extern bool PLX_DIO_get(PLX_DIO_Handle_t aHandle);
extern void PLX_DIO_set(PLX_DIO_Handle_t aHandle, bool aVal);
extern void PLX_DIO_toggle(PLX_DIO_Handle_t aHandle);

#endif /* PLX_DIO_H_ */
