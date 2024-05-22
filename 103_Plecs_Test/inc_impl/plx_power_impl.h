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

#ifndef PLX_PWR_IMPL_H_
#define PLX_PWR_IMPL_H_

#define PLX_PWR_MAX_PWM_CHANNELS 12
#define PLX_PWR_MAX_HRTIMS_UNITS 1

typedef enum
{
    PLX_PWR_STATE_POWERUP,
    PLX_PWR_STATE_DISABLED,
    PLX_PWR_STATE_ENABLING,
    PLX_PWR_STATE_ENABLED,
    PLX_PWR_STATE_FAULT,
    PLX_PWR_STATE_FAULT_ACKN,
    PLX_PWR_STATE_CRITICAL_FAULT
} PLX_PWR_FsmState_t;

typedef struct PLX_PWR_OBJ
{
	PLX_DIO_Handle_t gdrvEnableHandle;
    uint16_t timer;
    uint16_t fsmExecRateHz;
    uint16_t enableDelayInTicks;

    uint16_t numRegisteredPwmChannels;
    PLX_TIM_Handle_t pwmChannels[PLX_PWR_MAX_PWM_CHANNELS];
#ifdef PLX_HRTIM_AVAILABLE
    uint16_t numRegisteredHrtimUnits;
    PLX_HRTIM_Handle_t hrtimUnits[PLX_PWR_MAX_HRTIMS_UNITS];
#endif
    uint16_t enableSwitchingReq;
    int16_t pilMode;
    int16_t state;

    uint16_t enableReq;
    uint16_t gatesActive;
} PLX_PWR_Obj_t;

typedef PLX_PWR_Obj_t *PLX_PWR_Handle_t;
extern PLX_PWR_Handle_t PLX_PWR_SHandle;

__STATIC_INLINE void PLX_PWR_setEnableDelay(int16_t aDelayInMs){
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
    obj->enableDelayInTicks = obj->fsmExecRateHz/1000 * aDelayInMs;
}

__STATIC_INLINE void PLX_PWR_syncdSwitchingEnable()
{
    PLX_PWR_Obj_t *obj = (PLX_PWR_Obj_t *)PLX_PWR_SHandle;
    if(obj->enableSwitchingReq)
    {
        if(obj->pilMode == false)
        {
         // enable actuators
            int i;
            for(i=0; i< obj->numRegisteredPwmChannels; i++)
            {
            		PLX_TIM_enablePwmOutput(obj->pwmChannels[i]);
            }
#ifdef PLX_HRTIM_AVAILABLE
            for(i=0; i< obj->numRegisteredHrtimUnits; i++)
            {
              PLX_HRTIM_enablePwmOutput(obj->hrtimUnits[i]);
            }
#endif
        }
        obj->gatesActive = true;
        obj->enableSwitchingReq = false;
    }
}

#endif /* PLX_PWR_IMPL_H_ */
