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

#ifndef PLX_DIO_IMPL_H_
#define PLX_DIO_IMPL_H_

typedef struct PLX_DIO_OBJ
{
   GPIO_TypeDef *gpio;
   uint16_t pin;
   bool activeHigh;
} PLX_DIO_Obj_t;

typedef PLX_DIO_Obj_t *PLX_DIO_Handle_t;

__STATIC_INLINE bool PLX_DIO_get(PLX_DIO_Handle_t aHandle)
{
	PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;
	if(obj->activeHigh)
	{
		return ((obj->gpio->IDR & obj->pin) != 0x00U) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	}
	else
	{
		return ((obj->gpio->IDR & obj->pin) != 0x00U) ? GPIO_PIN_RESET : GPIO_PIN_SET;
	}
}

__STATIC_INLINE void PLX_DIO_set(PLX_DIO_Handle_t aHandle, bool aVal)
{
	PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;

	if(obj->activeHigh)
	{
	  if(aVal != GPIO_PIN_RESET)
	  {
	    obj->gpio->BSRR = (uint32_t)obj->pin;
	  }
	  else
	  {
	    obj->gpio->BRR = (uint32_t)obj->pin;
	  }
	}
	else
	{
	  if(aVal == GPIO_PIN_RESET)
	  {
	    obj->gpio->BSRR = (uint32_t)obj->pin;
	  }
	  else
	  {
	    obj->gpio->BRR = (uint32_t)obj->pin;
	  }
	}
}

__STATIC_INLINE void PLX_DIO_toggle(PLX_DIO_Handle_t aHandle)
{
	PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;
	uint32_t odr;
	odr = obj->gpio->ODR;
	obj->gpio->BSRR = ((odr & obj->pin) << 16U) | (~odr & obj->pin);
}

#endif /* PLX_DIO_IMPL_H_ */
