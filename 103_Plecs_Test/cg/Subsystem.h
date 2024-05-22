/*
 * Header file for: STM32_Plecs_Test/Subsystem
 * Generated with : PLECS 4.8.3
 *                  STM32G4x 1.4.3
 * Generated on   : 14 May 2024 10:49:36
 */
#ifndef PLECS_HEADER_Subsystem_h_
#define PLECS_HEADER_Subsystem_h_

#include <stdbool.h>
#include <stdint.h>

/* Model floating point type */
typedef float Subsystem_FloatType;

/* Model checksum */
extern const char * const Subsystem_checksum;

/* Model error status */
extern const char * Subsystem_errorStatus;


/* Model sample time */
extern const float Subsystem_sampleTime;


/* Block outputs */
typedef struct
{
   float PulseGenerator;            /* Subsystem/Pulse Generator */
   float PulseGenerator1;           /* Subsystem/Pulse Generator1 */
   float Switch;                    /* Subsystem/Switch */
   float DigitalIn;                 /* Subsystem/Digital In */
} Subsystem_BlockOutputs;
extern Subsystem_BlockOutputs Subsystem_B;

#if defined(EXTERNAL_MODE) && EXTERNAL_MODE
/* External mode signals */
#define Subsystem_NumExtModeSignals 2
extern const float * const Subsystem_ExtModeSignals[];
/* Tunable parameters */
#define Subsystem_NumTunableParameters 0
#endif /* defined(EXTERNAL_MODE) */


/* Entry point functions */
void Subsystem_initialize(void);
void Subsystem_step(void);
void Subsystem_terminate(void);

#endif /* PLECS_HEADER_Subsystem_h_ */
