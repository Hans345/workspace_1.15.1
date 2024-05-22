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

#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "stm32g4xx.h"
#include "plx_gpio.h"

#include "pil.h"

#define THIS_TSP_VER 0x0104

#define PLX_ASSERT(x) do {\
   if(!(x)){\
	  asm("bkpt 255");\
      for(;;);\
   }\
} while(0)

//#define PARALLEL_COM_PROTOCOL 3
#define PARALLEL_COM_BUF_ADDR 0x20000000
#define PARALLEL_COM_BUF_LEN  0x100

#endif // _INCLUDES_H_
