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
#include "pil.h"

#ifndef PLX_ADC_H_
#define PLX_ADC_H_

#include "plx_adc_impl.h" // implementation specific

extern void PLX_ADC_sinit(float aVref);

extern PLX_ADC_Handle_t PLX_ADC_init(void *aMemory, const size_t aNumBytes);

extern void PLX_ADC_setup(PLX_ADC_Handle_t aHandle, PLX_ADC_Unit_t aUnit);

extern void PLX_ADC_addChannel(PLX_ADC_Handle_t aHandle, /*ADC_ChannelConfTypeDef *aConfig,*/ float aScale, float aOffset);

extern void PLX_ADC_addInjectedChannel(PLX_ADC_Handle_t aHandle, /*ADC_InjectionConfTypeDef *aConfig,*/ float aScale, float aOffset);

extern void PLX_ADC_start(PLX_ADC_Handle_t aHandle);

extern void PLX_ADC_updateInjected(PLX_ADC_Handle_t aHandle);

extern float PLX_ADC_getIn(PLX_ADC_Handle_t aHandle, uint16_t aChannel);

#endif /* PLX_ADC_H_ */
