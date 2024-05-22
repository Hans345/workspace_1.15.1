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

#include "plx_gpio.h"

void PLX_GPIO_sinit()
{
   static bool firstCall = true;
   if(firstCall){
      firstCall = false;
      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOD_CLK_ENABLE();
    #ifdef GPIOE
      __HAL_RCC_GPIOE_CLK_ENABLE();
    #endif
      __HAL_RCC_GPIOF_CLK_ENABLE();
    #ifdef  GPIOG
      __HAL_RCC_GPIOG_CLK_ENABLE();
    #endif
    #ifdef GPIOH
      __HAL_RCC_GPIOH_CLK_ENABLE();
    #endif
   }
}

void PLX_GPIO_setGpioConfig(PLX_GPIO_Port_t aPort, LL_GPIO_InitTypeDef *aInitStruct)
{
  LL_GPIO_Init(PLX_GPIO_getBase(aPort), aInitStruct);
}

GPIO_TypeDef *PLX_GPIO_getBase(PLX_GPIO_Port_t aPort){
  switch(aPort)
  {
  case PLX_PORTA:
    return ((GPIO_TypeDef *) GPIOA_BASE);

  case PLX_PORTB:
    return ((GPIO_TypeDef *) GPIOB_BASE);

  case PLX_PORTC:
    return ((GPIO_TypeDef *) GPIOC_BASE);

  case PLX_PORTD:
    return ((GPIO_TypeDef *) GPIOD_BASE);

#ifdef  GPIOE
  case PLX_PORTE:
    return ((GPIO_TypeDef *) GPIOE_BASE);
#endif
  case PLX_PORTF:
    return ((GPIO_TypeDef *) GPIOF_BASE);
#ifdef  GPIOG
  case PLX_PORTG:
    return ((GPIO_TypeDef *) GPIOG_BASE);
#endif

#ifdef  GPIOH
  case PLX_PORTH:
    return ((GPIO_TypeDef *) GPIOH_BASE);
#endif
  default:
    PLX_ASSERT(0);
  }
  return (GPIO_TypeDef *) 0;
}
