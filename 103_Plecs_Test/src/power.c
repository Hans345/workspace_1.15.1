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

#include "plx_power.h"
#include "pil.h"

PLX_PWR_Obj_t PLX_PWR_SObj;
PLX_PWR_Handle_t PLX_PWR_SHandle;

static bool PLX_PWR_isSafe();
static void PLX_PWR_disableSwitching();
static bool PLX_PWR_hasTripped();
static void PLX_PWR_reset();

__STATIC_INLINE void PLX_PWR_DINT()
{
    //ASM code from core_cmFunc.h
    //Disable IRQ Interrupts globally
    __asm volatile ("cpsid i");
}

__STATIC_INLINE void PLX_PWR_EINT()
{
    //ASM code from core_cmFunc.h
    //Enable IRQ Interrupts globally
    __asm volatile ("cpsie i");
}

void PLX_PWR_sinit()
{
    PLX_PWR_SHandle = (PLX_PWR_Handle_t)&PLX_PWR_SObj;
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
    obj->enableDelayInTicks = 0;
}

void PLX_PWR_configure(PLX_DIO_Handle_t aGateDrvEnableHandle, uint16_t aFsmExecRateHz)
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
    obj->gdrvEnableHandle = aGateDrvEnableHandle;

    obj->fsmExecRateHz = aFsmExecRateHz;
    obj->enableDelayInTicks = obj->fsmExecRateHz/1000 * 100; // 100 ms = default

    obj->numRegisteredPwmChannels = 0;
#ifdef PLX_HRTIM_AVAILABLE
    obj->numRegisteredHrtimUnits = 0;
#endif

    obj->pilMode = false;

    PLX_PWR_reset();
}

void PLX_PWR_registerPwm(PLX_TIM_Handle_t aPwmHandle)
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    PLX_ASSERT(obj->numRegisteredPwmChannels < PLX_PWR_MAX_PWM_CHANNELS);
    obj->pwmChannels[obj->numRegisteredPwmChannels] = aPwmHandle;
    obj->numRegisteredPwmChannels++;
}

#ifdef PLX_HRTIM_AVAILABLE
void PLX_PWR_registerHrtim(PLX_HRTIM_Handle_t aHrtimHandle)
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    PLX_ASSERT(obj->numRegisteredHrtimUnits < PLX_PWR_MAX_HRTIMS_UNITS);
    obj->hrtimUnits[obj->numRegisteredHrtimUnits] = aHrtimHandle;
    obj->numRegisteredHrtimUnits++;
}
#endif

void PLX_PWR_reset()
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    obj->gatesActive = false;
    obj->enableSwitchingReq = false;
    obj->state = PLX_PWR_STATE_POWERUP;
    obj->enableReq = false;
}

bool PLX_PWR_isReadyForEnable()
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    PLX_PWR_DINT();
    bool isReadyForEnable = (obj->state == PLX_PWR_STATE_DISABLED);
    PLX_PWR_EINT();
    return isReadyForEnable;
}

void PLX_PWR_setEnableRequest(bool aEnable)
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    PLX_PWR_DINT();
    obj->enableReq = aEnable;
    PLX_PWR_EINT();
}

bool PLX_PWR_isEnabled()
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
    PLX_PWR_DINT();
    bool isEnabled = (obj->state == PLX_PWR_STATE_ENABLED);
    PLX_PWR_EINT();
    return isEnabled;
}

void PLX_PWR_setPilMode(bool pilMode)
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    PLX_PWR_DINT();
    obj->pilMode = pilMode;
    PLX_PWR_EINT();
}

void PLX_PWR_runFsm()
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    PLX_PWR_DINT(); // disable interrupts to allow changing enableReq from highest priority task
    uint16_t enableReq = obj->enableReq;
    PLX_PWR_EINT();
    int16_t newState = obj->state;
    switch(obj->state)
    {
        // powerup
        case PLX_PWR_STATE_POWERUP:
            if(!enableReq)
            {
                goto enter_PS_FSM_STATE_DISABLED;
            }
            break;

        // disabled
        enter_PS_FSM_STATE_DISABLED:
			if(obj->gdrvEnableHandle){
				PLX_DIO_set(obj->gdrvEnableHandle, false);
			}
            newState = PLX_PWR_STATE_DISABLED;
            PLX_PWR_disableSwitching();

        break;
        case PLX_PWR_STATE_DISABLED:
            if(enableReq)
            {
                goto enter_PS_FSM_STATE_ENABLING;
            }
            break;

        // enabling
        enter_PS_FSM_STATE_ENABLING:
            newState = PLX_PWR_STATE_ENABLING;
            if(obj->pilMode != true)
            {
            		if(obj->gdrvEnableHandle){
            			PLX_DIO_set(obj->gdrvEnableHandle, true);
            		}
                obj->timer = obj->enableDelayInTicks; // for charge-pump to reach steady-state
                if(obj->timer < 1)
                {
                    obj->timer = 1; // enforce minimal delay
                }
            }
            else
            {
                obj->timer = 0;
            }
            break;
        case PLX_PWR_STATE_ENABLING:
            if(obj->timer == 0)
            {
                goto enter_PS_FSM_STATE_ENABLED;
            }
            else
            {
                obj->timer--;
            }
            break;

        // enabled
        enter_PS_FSM_STATE_ENABLED:
            // enable powerstage
            newState = PLX_PWR_STATE_ENABLED;
            PLX_PWR_DINT();
            obj->enableSwitchingReq = true;
            PLX_PWR_EINT();
        break;
        case PLX_PWR_STATE_ENABLED:
            // check for fault
            if(!enableReq)
            {
                goto enter_PS_FSM_STATE_DISABLED;
            }
            else
            {
                if(PLX_PWR_hasTripped())
                {
                    goto enter_PS_FSM_STATE_FAULT;
                }
            }
            break;

        // fault
        enter_PS_FSM_STATE_FAULT:
            PLX_PWR_disableSwitching();
            if(obj->gdrvEnableHandle){
            		PLX_DIO_set(obj->gdrvEnableHandle, false);
            }
            newState = PLX_PWR_STATE_FAULT;
        break;
        case PLX_PWR_STATE_FAULT:
            if(!enableReq)
            {
                goto enter_PS_FSM_STATE_FAULT_ACKN;
            }
            break;

        // critical fault
        enter_PS_FSM_STATE_CRITICAL_FAULT:
            PLX_PWR_disableSwitching();
            if(obj->gdrvEnableHandle){
            		PLX_DIO_set(obj->gdrvEnableHandle, false);
            }
            newState = PLX_PWR_STATE_CRITICAL_FAULT;
        break;
        case PLX_PWR_STATE_CRITICAL_FAULT:
            // no way out
            break;

        // fault acknowledge
        enter_PS_FSM_STATE_FAULT_ACKN:
            newState = PLX_PWR_STATE_FAULT_ACKN;
            break;
        case PLX_PWR_STATE_FAULT_ACKN:
            // reset gate driver here...
            goto enter_PS_FSM_STATE_DISABLED;
    }
    PLX_PWR_DINT(); // disable interrupts to allow reading state from higher priority tasks
    obj->state = newState;
    PLX_PWR_EINT();
}

static bool PLX_PWR_isSafe()
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    return ((obj->state != PLX_PWR_STATE_ENABLED) && (obj->state != PLX_PWR_STATE_ENABLING));
}

static void PLX_PWR_disableSwitching()
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;

    PLX_PWR_DINT();
    obj->enableSwitchingReq = false;
    PLX_PWR_EINT();

    // disable actuators
    int i;
    for(i=0; i< obj->numRegisteredPwmChannels; i++)
    {
    		PLX_TIM_disablePwmOutput(obj->pwmChannels[i]);
    }
#ifdef PLX_HRTIM_AVAILABLE
    for(i=0; i< obj->numRegisteredHrtimUnits; i++)
    {
      PLX_HRTIM_disablePwmOutput(obj->hrtimUnits[i]);
    }
#endif
    obj->gatesActive = false;
}

static bool PLX_PWR_hasTripped()
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
    if(obj->pilMode){
        return false;
    }
    int i;
    for(i=0; i< obj->numRegisteredPwmChannels; i++)
    {
        if(!PLX_TIM_pwmOutputIsEnabled(obj->pwmChannels[i]))
        {
            return true;
        }
    }
#ifdef PLX_HRTIM_AVAILABLE
    for(i=0; i < obj->numRegisteredHrtimUnits; i++)
    {
      if(!PLX_HRTIM_pwmOutputIsEnabled(obj->hrtimUnits[i]))
      {
        return true;
      }
    }
#endif
    return false;
}
