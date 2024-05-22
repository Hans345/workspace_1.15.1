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

#include "plx_trig.h"

#include "stm32g4xx_ll_cordic.h"
#include "stm32g4xx_ll_bus.h"
#include <math.h>


void PLX_TRIG_sinit()
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CORDIC);

    LL_CORDIC_Config(CORDIC, LL_CORDIC_FUNCTION_COSINE,   /* cosine function */
                              LL_CORDIC_PRECISION_6CYCLES, /* max precision for q1.31 cosine */
                              LL_CORDIC_SCALE_0,           /* no scale */
                              LL_CORDIC_NBWRITE_1,         /* One input data: angle. Second input data (modulus) is 1 after cordic reset */
                              LL_CORDIC_NBREAD_2,          /* Two output data: cosine, then sine */
                              LL_CORDIC_INSIZE_32BITS,     /* q1.31 format for input data */
                              LL_CORDIC_OUTSIZE_32BITS);   /* q1.31 format for output data */
}

PLX_TRIG_Handle_t PLX_TRIG_init(void* aMemory, const size_t aNumBytes)
{
    PLX_TRIG_Handle_t handle;

    if (aNumBytes < sizeof(PLX_TRIG_Obj_t))
    {
        return((PLX_TRIG_Handle_t)NULL);
    }

    // set handle
    handle = (PLX_TRIG_Handle_t)aMemory;

    return handle;
}

void PLX_TRIG_updateAngle(PLX_TRIG_Handle_t aHandle, float aAngle)
{
    PLX_TRIG_Obj_t *obj = (PLX_TRIG_Obj_t *)aHandle;

    aAngle = fmodf(aAngle, 2.0f*PLX_TRIG_PI);
    if ((aAngle < 0 && 2.0f*PLX_TRIG_PI > 0) || (aAngle > 0 && 2.0f*PLX_TRIG_PI < 0))
    {
        aAngle += 2.0f*PLX_TRIG_PI;
    }
    aAngle -= PLX_TRIG_PI;

	LL_CORDIC_WriteData(CORDIC, (int32_t)(aAngle/PLX_TRIG_PI * (float)0x80000000));
	obj->cosQ31 = -(int32_t)LL_CORDIC_ReadData(CORDIC);
	obj->sinQ31 = -(int32_t)LL_CORDIC_ReadData(CORDIC);
}

float PLX_TRIG_getSin(PLX_TRIG_Handle_t aHandle)
{
	PLX_TRIG_Obj_t *obj = (PLX_TRIG_Obj_t *)aHandle;
	return (float)(obj->sinQ31)/(float)(0x7FFFFFFF);
}

float PLX_TRIG_getCos(PLX_TRIG_Handle_t aHandle)
{
	PLX_TRIG_Obj_t *obj = (PLX_TRIG_Obj_t *)aHandle;
	return (float)(obj->cosQ31)/(float)(0x7FFFFFFF);
}

