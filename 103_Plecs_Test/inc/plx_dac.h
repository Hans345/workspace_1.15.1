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

#if !defined(PLX_DAC_H_) && defined(DAC1)
#define PLX_DAC_H_

#define PLX_DAC_AVAILABLE

#include "plx_dac_impl.h" // implementation specific

extern void PLX_DAC_sinit(float aVref);

extern PLX_DAC_Handle_t PLX_DAC_init(void *aMemory, const size_t aNumBytes);

extern void PLX_DAC_setup(PLX_DAC_Handle_t aHandle, PLX_DAC_Unit_t aUnit);

extern void PLX_DAC_configureChannel(PLX_DAC_Handle_t aHandle, uint16_t aChannel, PLX_DAC_Waveform_t aWaveform, float aScale, float aOffset);

extern void PLX_DAC_activate(PLX_DAC_Handle_t aHandle);

extern void PLX_DAC_setOut(PLX_DAC_Handle_t aHandle, uint16_t aChannel, float aValue);

#endif /* PLX_DAC_H_ */
