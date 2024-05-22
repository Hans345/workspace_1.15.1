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

#ifndef PLX_TRIG_H_
#define PLX_TRIG_H_

#include "plx_trig_impl.h"

extern void PLX_TRIG_sinit();
extern PLX_TRIG_Handle_t PLX_TRIG_init(void *aMemory, const size_t aNumBytes);

extern void PLX_TRIG_updateAngle(PLX_TRIG_Handle_t aHandle, float aAngle);
extern float PLX_TRIG_getSin(PLX_TRIG_Handle_t aHandle);
extern float PLX_TRIG_getCos(PLX_TRIG_Handle_t aHandle);

#endif /* PLX_TRIG_H_ */
