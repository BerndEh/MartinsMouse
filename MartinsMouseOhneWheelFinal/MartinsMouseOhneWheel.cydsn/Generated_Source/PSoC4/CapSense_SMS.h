/*******************************************************************************
* File Name: Mouse_SMS.h
* Version 2.60
*
* Description:
*  This file provides the declarations of the wrapper between the Mouse CSD component
*  and Auto Tuning library.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_Mouse_CSD_SMS_Mouse_H)
#define CY_Mouse_CSD_SMS_Mouse_H

#include "Mouse.h"
#include "Mouse_PVT.h"

#if(0u != Mouse_CSHL_API_GENERATE)
    #include "Mouse_CSHL.h"
#endif /* (0u != Mouse_CSHL_API_GENERATE) */

#define Mouse_PRESCALERS_TBL_SIZE        (16u)

#define Mouse_CALIBRATION_FREQ_KHZ       (3000u)
#define Mouse_CALIBRATION_MD             (1u)

#define Mouse_MAX_RESOLUTION         (65535u)
#define Mouse_CALIBRATION_RESOLUTION (0x0FFF0000u)

#define Mouse_PRS_CORRECTION_DIVIDER (4u)
#define Mouse_SENSITIVITY_DIVIDER    (10u)

#define Mouse_SENSITIVITY_MASK           (0x0Fu)
#define Mouse_FLEXIBLE_THRESHOLDS_EN     (0u)
#define Mouse_CAL_RAW_COUNT              (3482u)

#define Mouse_THRESHOLD_1                (220u)
#define Mouse_THRESHOLD_2                (237u)
#define Mouse_THRESHOLD_3                (245u)
#define Mouse_THRESHOLD_4                (250u)


#if (0u != Mouse_IS_M0S8PERI_BLOCK)
    #define Mouse_CALIBRATION_ASD        (CYDEV_BCLK__HFCLK__KHZ /\
                                                     Mouse_CALIBRATION_FREQ_KHZ)
#else
    #define Mouse_MIN_IMO_FREQ_KHZ       (6000u)
    #if(CYDEV_BCLK__HFCLK__KHZ < Mouse_MIN_IMO_FREQ_KHZ)
        #define Mouse_CALIBRATION_ASD            (1u)
    #else
        #define Mouse_CALIBRATION_ASD    (CYDEV_BCLK__HFCLK__KHZ / Mouse_CALIBRATION_MD /\
                                                     Mouse_CALIBRATION_FREQ_KHZ)
    #endif /* (CYDEV_BCLK__HFCLK__KHZ < Mouse_MIN_IMO_FREQ_KHZ) */
#endif /* (0u != Mouse_IS_M0S8PERI_BLOCK) */

#if (0u != Mouse_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained */
    #if(Mouse_PRS_OPTIONS != Mouse__PRS_NONE)
        #define Mouse_SENSETIVITY_FACTOR (1228u)
    #else
        #define Mouse_SENSETIVITY_FACTOR (2456u)
    #endif /* (Mouse_PRS_OPTIONS == Mouse__PRS_NONE) */
#else
    /*  Dividers are chained */
    #if(Mouse_PRS_OPTIONS != Mouse__PRS_NONE)
        #define Mouse_SENSETIVITY_FACTOR (1228u/Mouse_CALIBRATION_MD)
    #else
        #define Mouse_SENSETIVITY_FACTOR (2456u/Mouse_CALIBRATION_MD)
    #endif /* (Mouse_PRS_OPTIONS == Mouse__PRS_NONE) */
#endif /* (0u != Mouse_IS_M0S8PERI_BLOCK) */


/***************************************
*        Function Prototypes
***************************************/
void Mouse_AutoTune(void);
void Mouse_UpdateThresholds(uint32 sensor);
void Mouse_SetSensitivity(uint32 sensor, uint32 value);
void Mouse_SetAnalogSwitchesSrcDirect(void);
uint16 Mouse_GetNoiseEnvelope(uint32 sensor);
uint8 Mouse_GetSensitivityCoefficient(uint32 sensor);
uint16 Mouse_GetNormalizedDiffCountData(uint32 sensor);


/***************************************
*     Vars with External Linkage
***************************************/

/*  Real Charge Divider values */
#if (0u != Mouse_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained */
    #if (CYDEV_BCLK__HFCLK__MHZ >= 12u)
        extern const uint8 prescalerTable[Mouse_PRESCALERS_TBL_SIZE];
    #elif (CYDEV_BCLK__HFCLK__MHZ >= 6u)
        extern const uint8 prescalerTable[Mouse_PRESCALERS_TBL_SIZE];
    #else
        extern const uint8 prescalerTable[Mouse_PRESCALERS_TBL_SIZE];
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
#else
    /*  Dividers are chained */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        extern const uint8 prescalerTable[Mouse_PRESCALERS_TBL_SIZE];
    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        extern const uint8 prescalerTable[Mouse_PRESCALERS_TBL_SIZE];
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        extern const uint8 prescalerTable[Mouse_PRESCALERS_TBL_SIZE];
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
#endif /* (0u != Mouse_IS_M0S8PERI_BLOCK) */


extern uint8 Mouse_noiseEnvelopeTbl[Mouse_TOTAL_SENSOR_COUNT];
extern uint8 Mouse_runningDifferenceTbl[Mouse_TOTAL_SENSOR_COUNT];
extern uint8 Mouse_signRegisterTbl[Mouse_TOTAL_SENSOR_COUNT];
extern uint16 Mouse_sampleMinTbl[Mouse_TOTAL_SENSOR_COUNT];
extern uint16 Mouse_sampleMaxTbl[Mouse_TOTAL_SENSOR_COUNT];
extern uint16 Mouse_previousSampleTbl[Mouse_TOTAL_SENSOR_COUNT];
extern uint8 Mouse_kValueTbl[Mouse_TOTAL_SENSOR_COUNT];


#endif  /* (CY_Mouse_CSD_SMS_Mouse_H) */

/* [] END OF FILE */
