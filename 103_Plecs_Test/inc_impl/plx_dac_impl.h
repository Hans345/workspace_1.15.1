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

#ifndef PLX_DAC_IMPL_H_
#define PLX_DAC_IMPL_H_

#include "stm32g4xx_ll_dac.h"

#define PLX_DAC_MAX_NUM_CHANNELS 2

typedef enum PLX_DAC_UNIT {
	PLX_DAC1 = 0,
	PLX_DAC2,
	PLX_DAC3,
	PLX_DAC4
} PLX_DAC_Unit_t;

typedef enum PLX_DAC_WAVEFORM {
  PLX_DAC_DC = 0,
  PLX_DAC_SAWTOOTH
} PLX_DAC_Waveform_t;

typedef struct PLX_DAC_OBJ
{
   PLX_DAC_Unit_t unit;
   DAC_TypeDef *instance;
   uint32_t channel[PLX_DAC_MAX_NUM_CHANNELS];
   float scale[PLX_DAC_MAX_NUM_CHANNELS];
   float offset[PLX_DAC_MAX_NUM_CHANNELS];
   bool configured[PLX_DAC_MAX_NUM_CHANNELS];
   PLX_DAC_Waveform_t waveform[PLX_DAC_MAX_NUM_CHANNELS];
} PLX_DAC_Obj_t;

typedef PLX_DAC_Obj_t *PLX_DAC_Handle_t;

#endif /* PLX_DAC_IMPL_H_ */
