/*
   Copyright (c) 2019 by Plexim GmbH
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

#include "plx_dispatcher.h"

#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_tim.h"
#include "stm32g4xx_hal_adc.h"

#include "stm32g4xx_ll_tim.h"

#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)  // Debug Exception and Monitor Control Register
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)  // Control Register
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)  // Cycle Count Register

/*
 * Assumptions/simplifications:
 */

// singleton (for now)
DISPR_Obj_t DisprObj;
DISPR_Handle_t DisprHandle;

// for diagnostics purposes
#ifndef PIL_PREP_TOOL
#endif

void PIL_SCOPE_sample(PIL_Handle_t aPilHandle);

#ifndef PLX_BARE_METAL_SCHEDULER
static void DISPR_tasks(void*);
#endif

void DISPR_sinit()
{
   DisprHandle = (DISPR_Handle_t)&DisprObj;

   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   obj->numTasks = 0;
   obj->idleTask = (DISPR_IdleTaskPtr_t)0;
   obj->syncCallback = (DISPR_SyncCallbackPtr_t)0;
   obj->enableInterrupt = (DISPR_EnableInterruptPtr_t)0;
}

void DISPR_configure(uint32_t aPeriodInSysClkTicks, PIL_Handle_t aPilHandle, DISPR_TaskObj_t *aTskMemory, uint16_t aNumTasks)
{

   if (ARM_CM_DWT_CTRL != 0)  // See if DWT is available
   {
      ARM_CM_DEMCR      |= 1 << 24;  // Set bit 24 to enable DWT (and ITM) features
      ARM_CM_DWT_CYCCNT  = 0;
      ARM_CM_DWT_CTRL   |= 1 << 0;   // Set bit 0 to enable cycle count register
   }

   PLX_ASSERT(aNumTasks >= 1);
   PLX_ASSERT(aNumTasks <= 16); // number of tasks limited by size of tasksReadyFlags
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   obj->periodInSysClkTicks = aPeriodInSysClkTicks;
   obj->pilHandle = aPilHandle;
   obj->tskMemory = aTskMemory;
   obj->numTasks = aNumTasks;
}

void DISPR_registerIdleTask(DISPR_IdleTaskPtr_t aTsk)
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   obj->idleTask = aTsk;
}

void DISPR_registerSyncCallback(DISPR_SyncCallbackPtr_t aCallback)
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   obj->syncCallback = aCallback;
}

void DISPR_registerEnableInterrupt(DISPR_EnableInterruptPtr_t aFcn)
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   obj->enableInterrupt = aFcn;
}

void DISPR_setPowerupDelay(uint16_t aTask1Ticks)
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   obj->powerupDelayIntTask1Ticks = aTask1Ticks;
}

void DISPR_reset()
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   uint16_t i;
   for(i=0; i<obj->numTasks; i++){
      obj->tskMemory[i].timer = 0;
   }
}

#ifdef PLX_BARE_METAL_SCHEDULER

void DISPR_registerTask(uint16_t aTaskId, DISPR_TaskPtr_t aTsk, uint32_t aPeriodInTimerTicks)
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

   PLX_ASSERT(aTaskId < obj->numTasks);
   if(aTaskId == 0)
   {
      // task 0 always called at dispatcher rate
      PLX_ASSERT(aPeriodInTimerTicks == obj->periodInSysClkTicks);
   }
   obj->tskMemory[aTaskId].periodInSysClkTicks = aPeriodInTimerTicks;
   obj->tskMemory[aTaskId].tsk = aTsk;
   obj->tskMemory[aTaskId].mask = (1 << aTaskId);
   obj->tskMemory[aTaskId].timer = 0;
   obj->tskMemory[aTaskId].taskId = aTaskId;
   obj->tskMemory[aTaskId].periodInDisprTicks = (uint16_t)(aPeriodInTimerTicks/obj->periodInSysClkTicks);
   // only exact multiples allowed
   PLX_ASSERT(((uint32_t)obj->tskMemory[aTaskId].periodInDisprTicks*obj->periodInSysClkTicks) == aPeriodInTimerTicks);
}

#else

void DISPR_registerTask(uint16_t aTaskId, DISPR_TaskPtr_t aTsk, uint32_t aPeriodInSysClkTicks, void * const aParameters)
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

   PLX_ASSERT(aTaskId < obj->numTasks);

   obj->tskMemory[aTaskId].periodInSysClkTicks = aPeriodInSysClkTicks;
   obj->tskMemory[aTaskId].taskId = aTaskId;
   obj->tskMemory[aTaskId].timer = 0;
   obj->tskMemory[aTaskId].params = aParameters;
   obj->tskMemory[aTaskId].tsk = aTsk;

   obj->tskMemory[aTaskId].periodInDisprTicks = obj->tskMemory[aTaskId].periodInSysClkTicks/obj->periodInSysClkTicks;
   PLX_ASSERT(obj->tskMemory[aTaskId].periodInDisprTicks * obj->periodInSysClkTicks == obj->tskMemory[aTaskId].periodInSysClkTicks);
   xTaskCreate(DISPR_tasks, "Task", 128, (void *)&obj->tskMemory[aTaskId], obj->numTasks-aTaskId, NULL); // priority, lower value = lower priority
}

#endif

static void DISPR_background()
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   __asm volatile ("cpsid i");
   int32_t ts0 = (int32_t)obj->timeStamp0;
   int32_t ts1 = (int32_t)obj->timeStamp1;
   int32_t ts2 = (int32_t)obj->timeStamp2;
   int32_t ts3 = (int32_t)obj->timeStamp3;
   __asm volatile ("cpsie i");
   float load = 0;
   int32_t tsD = ts2 - ts1; // base task duration
   int32_t tsP = ts3 - ts1; // base task period
   if(tsP > 0)
   {
      load = (float)(100*tsD)/(float)(tsP);
   }

   // start of base task
   int32_t tsB = ts1;

   if(tsB >= (int32_t)obj->periodInSysClkTicks){
      tsB -= obj->periodInSysClkTicks;
   }
   __asm volatile ("cpsid i");
   obj->timeStamp0Latched = ts0;
   obj->timeStamp1Latched = ts1;
   obj->timeStamp2Latched = ts2;
   obj->timeStamp3Latched = ts3;
   obj->timeStampPLatched = tsP;
   obj->timeStampDLatched = tsD;
   obj->timeStampBLatched = tsB;
   obj->task0LoadInPercent = load;
   __asm volatile ("cpsie i");
}

#ifndef PLX_BARE_METAL_SCHEDULER

static void DISPR_tasks(void *pvParameters)
{
   DISPR_TaskObj_t *tskRecord = (DISPR_TaskObj_t *)pvParameters;
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

   tskRecord->xTaskToNotify = xTaskGetCurrentTaskHandle();

   if(tskRecord->taskId == 0){
      tskRecord->tsk(true, tskRecord->params);
   }
   for( ;; ){
      uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); //  returns 0 if timeout
      UNUSED(ulNotificationValue);

      tskRecord->tsk(false, tskRecord->params);
      if(tskRecord->taskId == 0){
         if(obj->pilHandle){
            PIL_SCOPE_sample(obj->pilHandle);
         }
         obj->timeStamp2Last = ARM_CM_DWT_CYCCNT; // end of task
      }
   }
}

// do not rename function
void vApplicationIdleHook( void )
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   if(obj->pilHandle)
   {
      PIL_backgroundCall(obj->pilHandle);
   }
   DISPR_background();
   if(obj->idleTask)
   {
      obj->idleTask();
   }
}

#endif

#ifdef PLX_BARE_METAL_SCHEDULER

void DISPR_start()
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

   PLX_ASSERT(obj->numTasks > 0);

   DISPR_reset();

   obj->powerupCountdown = obj->powerupDelayIntTask1Ticks;

   __asm volatile ("cpsid i");
   if(obj->enableInterrupt)
   {
      obj->enableInterrupt();
   }
   __asm volatile ("cpsie i");

   if(obj->syncCallback)
   {
      obj->syncCallback();
   }

   while(obj->powerupCountdown > 0)
   {
      // wait to, e.g., allow gatedrive charge-pump to stabilize
      continue;
   }

   for(;;)
   {
      if(obj->pilHandle != 0)
      {
         PIL_backgroundCall(obj->pilHandle);
      }
      DISPR_background();
      if(obj->idleTask)
      {
         obj->idleTask();
      }
   }
}

#else

void DISPR_start()
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

   if(obj->syncCallback)
   {
      obj->syncCallback();
   }

   vTaskStartScheduler();
}

#endif

#ifdef PLX_BARE_METAL_SCHEDULER

void DISPR_sampleScopes()
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;
   if(obj->pilHandle != 0)
   {
      PIL_SCOPE_sample(obj->pilHandle);
   }
   obj->timeStamp2Last = ARM_CM_DWT_CYCCNT; // end of task
}

void DISPR_dispatch()
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

   // we return immediately if power-up delay has not yet expired
   if(obj->powerupCountdown > 0)
   {
      obj->powerupCountdown--;
      return;
   }

   if(obj->pilHandle != 0)
   {
      PIL_beginInterruptCall(obj->pilHandle);
   }

   obj->timeStamp1 = obj->timeStamp3; // last start of period
   obj->timeStamp2 = obj->timeStamp2Last; // last end of task timestamp
   obj->timeStamp3 = ARM_CM_DWT_CYCCNT; // start of new period
   // always dispatch base task since it has the same rate as the interrupt

   // check if interrupt is still active or pending, if yes --> overrun
   bool overrun;
   overrun = obj->tskMemory[0].tsk(obj->tskMemory[0].taskId);
   PLX_ASSERT(overrun == 0);
   int i;
   // determine which tasks should be dispatched
   for(i=1; i<obj->numTasks; i++)
   {
      if(obj->tskMemory[i].timer == 0)
      {
         overrun = obj->tskMemory[i].tsk(obj->tskMemory[i].taskId);
         PLX_ASSERT(overrun == 0);
         obj->tskMemory[i].timer = obj->tskMemory[i].periodInDisprTicks;
      }
      obj->tskMemory[i].timer--;
   }
}

#else

void DISPR_dispatch()
{
   DISPR_Obj_t *obj = (DISPR_Obj_t *)DisprHandle;

   if(obj->pilHandle)
   {
      PIL_beginInterruptCall(obj->pilHandle);
   }

   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

   for(int i=0; i< obj->numTasks; i++)
   {
      obj->tskMemory[i].timer++;
      if(obj->tskMemory[i].timer == obj->tskMemory[i].periodInDisprTicks)
      {
         obj->tskMemory[i].timer = 0;
         if(obj->tskMemory[i].taskId == 0)
         {
            obj->timeStamp1 = obj->timeStamp3; // last start of period
            obj->timeStamp2 = obj->timeStamp2Last; // last end of task timestamp
            obj->timeStamp3 = ARM_CM_DWT_CYCCNT; // start of new period
         }
         vTaskNotifyGiveFromISR(obj->tskMemory[i].xTaskToNotify, &xHigherPriorityTaskWoken);
      }
   }
   // same as, if xHigherPriorityTaskWoken was set to true (by vTaskNotifyGiveFromISR),
   // then portYIELD() is called
   portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

#endif

// For STM HAL tick - not sure if needed
static TIM_HandleTypeDef    DISPR_Tim7Handle;
// do not rename function - MAY BE CALLED MULTIPLE TIMES!
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
#if defined(STM32G431xx)
   HAL_NVIC_SetPriority(TIM7_IRQn, TickPriority, 0);
   HAL_NVIC_EnableIRQ(TIM7_IRQn);
#elif defined(STM32G474xx)
   HAL_NVIC_SetPriority(TIM7_DAC_IRQn, TickPriority, 0);
   HAL_NVIC_EnableIRQ(TIM7_DAC_IRQn);
#else
#error Unsupported chip.
#endif

   __HAL_RCC_TIM7_CLK_ENABLE();

   // compute the prescaler value to have TIMx counter clock equal to 1000 Hz
   uint32_t uwTimclock = HAL_RCC_GetPCLK1Freq();
   uint32_t uwPrescalerValue = (uint32_t)(uwTimclock / 10000) - 1;
   DISPR_Tim7Handle.Instance = TIM7;
   DISPR_Tim7Handle.Init.Period            = 10 - 1;
   DISPR_Tim7Handle.Init.Prescaler         = uwPrescalerValue;
   DISPR_Tim7Handle.Init.ClockDivision     = 0;
   DISPR_Tim7Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
   DISPR_Tim7Handle.Init.RepetitionCounter = 0;
   DISPR_Tim7Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

   if(HAL_TIM_Base_Init(&DISPR_Tim7Handle) == HAL_OK)
   {
      // start the TIM time Base generation in interrupt mode
      return HAL_TIM_Base_Start_IT(&DISPR_Tim7Handle);
   }
   return HAL_ERROR;
}

// do not rename function
void HAL_SuspendTick(void)
{
   __HAL_TIM_DISABLE_IT(&DISPR_Tim7Handle, TIM_IT_UPDATE);
}

// do not rename function
void HAL_ResumeTick(void)
{
   __HAL_TIM_ENABLE_IT(&DISPR_Tim7Handle, TIM_IT_UPDATE);
}

// do not rename function
#if defined(STM32G431xx)
void TIM7_IRQHandler(void)
#endif
#if defined(STM32G474xx)
void TIM7_DAC_IRQHandler(void)
#endif
{
   if (__HAL_TIM_GET_FLAG(&DISPR_Tim7Handle, TIM_FLAG_UPDATE) != RESET)
   {
      if (__HAL_TIM_GET_IT_SOURCE(&DISPR_Tim7Handle, TIM_IT_UPDATE) != RESET)
      {
         __HAL_TIM_CLEAR_IT(&DISPR_Tim7Handle, TIM_IT_UPDATE);
         HAL_IncTick();
      }
   }
}
