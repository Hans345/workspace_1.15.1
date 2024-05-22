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

#include "plx_dio.h"

PLX_DIO_Handle_t PLX_DIO_init(void *aMemory, const size_t aNumBytes)
{
	PLX_DIO_Handle_t handle;

	if(aNumBytes < sizeof(PLX_DIO_Obj_t))
		return((PLX_DIO_Handle_t)NULL);

	// set handle
	handle = (PLX_DIO_Handle_t)aMemory;

	return handle;
}

void PLX_DIO_configureIn(PLX_DIO_Handle_t aHandle, PLX_GPIO_Port_t aPort, uint16_t aPin, bool activeHigh)
{
	PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;

	obj->activeHigh = activeHigh;
	obj->gpio = PLX_GPIO_getBase(aPort);
	obj->pin = (uint16_t)(0x0001 << aPin);
}

void PLX_DIO_configureOut(PLX_DIO_Handle_t aHandle, PLX_GPIO_Port_t aPort, uint16_t aPin, bool activeHigh)
{
	PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;

	obj->activeHigh = activeHigh;
	obj->gpio = PLX_GPIO_getBase(aPort);
	obj->pin = (uint16_t)(0x0001 << aPin);

	PLX_DIO_set(aHandle, false);
}
