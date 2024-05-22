/*
 * Implementation file for: STM32_Plecs_Test/Subsystem
 * Generated with         : PLECS 4.8.3
 *                          STM32G4x 1.4.3
 * Generated on           : 14 May 2024 10:49:36
 */
#include "Subsystem.h"
#ifndef PLECS_HEADER_Subsystem_h_
#error The wrong header file "Subsystem.h" was included. Please check your
#error include path to see whether this file name conflicts with the name
#error of another header file.
#endif /* PLECS_HEADER_Subsystem_h_ */
#if defined(__GNUC__) && (__GNUC__ > 4)
#   define _ALIGNMENT 16
#   define _RESTRICT __restrict
#   define _ALIGN __attribute__((aligned(_ALIGNMENT)))
#   if defined(__clang__)
#      if __has_builtin(__builtin_assume_aligned)
#         define _ASSUME_ALIGNED(a) __builtin_assume_aligned(a, _ALIGNMENT)
#      else
#         define _ASSUME_ALIGNED(a) a
#      endif
#   else
#      define _ASSUME_ALIGNED(a) __builtin_assume_aligned(a, _ALIGNMENT)
#   endif
#else
#   ifndef _RESTRICT
#      define _RESTRICT
#   endif
#   ifndef _ALIGN
#      define _ALIGN
#   endif
#   ifndef _ASSUME_ALIGNED
#      define _ASSUME_ALIGNED(a) a
#   endif
#endif
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "plx_hal.h"
#include <stdlib.h>
#define PLECSRunTimeError(msg) Subsystem_errorStatus = msg
static const uint32_t Subsystem_subTaskPeriod[2]= {
   /* [0.05, 0], [0, 0] */
   500,
   /* [0.5, 0], [0, 0] */
   5000
};
static uint32_t Subsystem_subTaskTick[2];
static char Subsystem_subTaskHit[2];
#define Subsystem_UNCONNECTED 0
static uint32_t Subsystem_D_uint32_t[2];
static uint32_t Subsystem_tickLo;
static int32_t Subsystem_tickHi;
Subsystem_BlockOutputs Subsystem_B;
#if defined(EXTERNAL_MODE) && EXTERNAL_MODE
const float * const Subsystem_ExtModeSignals[] = {
   &Subsystem_B.DigitalIn,
   &Subsystem_B.Switch
};
#endif /* defined(EXTERNAL_MODE) */
const char * Subsystem_errorStatus;
const float Subsystem_sampleTime = 0.0001f;
const char * const Subsystem_checksum =
   "53145fa9bdd215dd207b19d272a12f28315d295c";
/* Target declarations */
#define sin sinf
#define cos cosf
extern void Subsystem_initHal();

void Subsystem_initialize(void)
{
   Subsystem_tickHi = 0;
   Subsystem_tickLo = 0;
   /* Initialize sub-task tick counters */
   Subsystem_subTaskTick[0] = 0;    /* [0, 0], [0.05, 0] */
   Subsystem_subTaskTick[1] = 0;    /* [0, 0], [0.5, 0] */


   /* Target pre-initialization */
   Subsystem_initHal();


   /* Initialization for Pulse Generator : 'Subsystem/Pulse\nGenerator' */
   Subsystem_D_uint32_t[0] = 0;

   /* Initialization for Pulse Generator : 'Subsystem/Pulse\nGenerator1' */
   Subsystem_D_uint32_t[1] = 0;
}

void Subsystem_step(void)
{
   if (Subsystem_errorStatus)
   {
      return;
   }
   {
      size_t i;
      for (i = 0; i < 2; ++i)
      {
         Subsystem_subTaskHit[i] = (Subsystem_subTaskTick[i] == 0);
      }
   }

   /* Digital In : 'Subsystem/Digital In' */
   Subsystem_B.DigitalIn = PLXHAL_DIO_get(0);
   if (Subsystem_subTaskHit[0])
   {
      /* Pulse Generator : 'Subsystem/Pulse\nGenerator' */
      Subsystem_B.PulseGenerator = Subsystem_D_uint32_t[0] < 1 ? 1.f : 0.f;
   }
   if (Subsystem_subTaskHit[1])
   {
      /* Pulse Generator : 'Subsystem/Pulse\nGenerator1' */
      Subsystem_B.PulseGenerator1 = Subsystem_D_uint32_t[1] < 1 ? 1.f : 0.f;
   }
   /* Signal Switch : 'Subsystem/Switch' */
   Subsystem_B.Switch =
      (Subsystem_B.DigitalIn !=
    0.f) ? Subsystem_B.PulseGenerator : Subsystem_B.PulseGenerator1;
   /* Digital Out : 'Subsystem/Digital Out' */
   PLXHAL_DIO_set(0, Subsystem_B.Switch);
   if (Subsystem_errorStatus)
   {
      return;
   }
   if (Subsystem_subTaskHit[0])
   {
      /* Update for Pulse Generator : 'Subsystem/Pulse\nGenerator' */
      Subsystem_D_uint32_t[0] += 1;
      if (Subsystem_D_uint32_t[0] > 1)
      {
         Subsystem_D_uint32_t[0] = 0;
      }
   }
   if (Subsystem_subTaskHit[1])
   {
      /* Update for Pulse Generator : 'Subsystem/Pulse\nGenerator1' */
      Subsystem_D_uint32_t[1] += 1;
      if (Subsystem_D_uint32_t[1] > 1)
      {
         Subsystem_D_uint32_t[1] = 0;
      }
   }

   /* Increment sub-task tick counters */
   {
      size_t i;
      for (i = 0; i < 2; ++i)
      {
         Subsystem_subTaskTick[i]++;
         if (Subsystem_subTaskTick[i] >= Subsystem_subTaskPeriod[i])
         {
            Subsystem_subTaskTick[i] = 0;
         }
      }
   }
}

void Subsystem_terminate(void)
{
}
