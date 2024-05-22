/*
   Copyright (c) 2014-2020 by Plexim GmbH
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

#ifndef PLX_DISPATCHER_H_
#define PLX_DISPATCHER_H_

#ifdef PLX_BARE_METAL_SCHEDULER
typedef bool(*DISPR_TaskPtr_t)(uint16_t);
#else
typedef void(*DISPR_TaskPtr_t)(bool, void * const);
#endif
typedef void(*DISPR_IdleTaskPtr_t)();
typedef void(*DISPR_SyncCallbackPtr_t)();
typedef void(*DISPR_EnableInterruptPtr_t)();

#include "plx_dispatcher_impl.h" // implementation specific

extern void DISPR_sinit();

extern void DISPR_configure(uint32_t aPeriodInSysClkTicks, PIL_Handle_t aPilHandle, DISPR_TaskObj_t *aTskMemory, uint16_t aNumTasks);
#ifdef PLX_BARE_METAL_SCHEDULER
extern void DISPR_sampleScopes();
extern void DISPR_registerTask(uint16_t aTaskId, DISPR_TaskPtr_t aTsk, uint32_t aPeriodInSysClkTicks);
#else
extern void DISPR_registerTask(uint16_t aTaskId, DISPR_TaskPtr_t aTsk, uint32_t aPeriodInSysClkTicks, void * const aParameters);
#endif
extern void DISPR_registerIdleTask(DISPR_IdleTaskPtr_t aTsk);
extern void DISPR_registerSyncCallback(DISPR_SyncCallbackPtr_t aCallback);
extern void DISPR_registerEnableInterrupt(DISPR_EnableInterruptPtr_t aFcn);
extern void DISPR_setPowerupDelay(uint16_t aTask1Ticks);
extern void DISPR_reset();

extern void DISPR_start();
extern void DISPR_dispatch();

extern float DISPR_getTask0LoadInPercent();
extern uint32_t DISPR_getTimeStamp0();
extern uint32_t DISPR_getTimeStamp1();
extern uint32_t DISPR_getTimeStamp2();
extern uint32_t DISPR_getTimeStamp3();
extern uint32_t DISPR_getTimeStampB();
extern uint32_t DISPR_getTimeStampD();
extern uint32_t DISPR_getTimeStampP();

#endif /* PLX_DISPATCHER_H_ */
