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

#include "plx_hrtimer.h"
#include "plx_dac.h"

#ifndef PLX_HRTIMER_PCC_H_
#define PLX_HRTIMER_PCC_H_

#ifdef PLX_HRTIM_AVAILABLE

#include "plx_hrtimer_pcc_impl.h" // implementation specific

extern PLX_HRTIM_PCC_Handle_t PLX_HRTIM_PCC_init(void *aMemory, const size_t aNumBytes);

extern void PLX_HRTIM_PCC_configureChannel(PLX_HRTIM_PCC_Handle_t aHandle, uint16_t aChannel,
    PLX_DAC_Handle_t aDacHandle, uint16_t aDacChannel,
    PLX_HRTIM_Handle_t aHrtimHandle, PLX_HRTIM_Timer_t aHrtimTimer);

extern void PLX_HRTIM_PCC_setPeakCurrent(PLX_HRTIM_PCC_Handle_t aHandle, uint16_t aChannel, float aValue);

#endif // PLX_HRTIM_AVAILABLE

#endif /* PLX_HRTIMER_PCC_H_ */
