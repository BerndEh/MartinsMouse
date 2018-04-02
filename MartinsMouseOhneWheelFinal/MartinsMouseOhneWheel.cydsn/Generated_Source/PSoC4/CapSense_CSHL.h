/*******************************************************************************
* File Name: Mouse_CSHL.h
* Version 2.60
*
* Description:
*  This file provides constants and parameter values for the High Level APIs
*  for Mouse CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_Mouse_CSD_CSHL_Mouse_H)
#define CY_Mouse_CSD_CSHL_Mouse_H

#include "Mouse.h"


/***************************************
*   Condition compilation parameters
***************************************/

#define Mouse_SIGNAL_SIZE                    (8u)
#define Mouse_AUTO_RESET                     (0u)
#define Mouse_RAW_FILTER_MASK                (2u)

/* Signal size definition */
#define Mouse_SIGNAL_SIZE_UINT8              (8u)
#define Mouse_SIGNAL_SIZE_UINT16             (16u)

/* Auto reset definition */
#define Mouse_AUTO_RESET_DISABLE             (0u)
#define Mouse_AUTO_RESET_ENABLE              (1u)

/* Mask for RAW and POS filters */
#define Mouse_MEDIAN_FILTER                  (0x01u)
#define Mouse_AVERAGING_FILTER               (0x02u)
#define Mouse_IIR2_FILTER                    (0x04u)
#define Mouse_IIR4_FILTER                    (0x08u)
#define Mouse_JITTER_FILTER                  (0x10u)
#define Mouse_IIR8_FILTER                    (0x20u)
#define Mouse_IIR16_FILTER                   (0x40u)
#define Mouse_RAW_FILTERS_ENABLED            (0x01u)
#define Mouse_RAW_FILTERS_DISABLED           (0x00u)

/***************************************
*           API Constants
***************************************/

/* Widgets constants definition */
#define Mouse_LINEARSLIDER0__LS        (0u)

#define Mouse_TOTAL_DIPLEXED_SLIDERS_COUNT        (0u)
#define Mouse_TOTAL_LINEAR_SLIDERS_COUNT          (1u)
#define Mouse_TOTAL_RADIAL_SLIDERS_COUNT          (0u)
#define Mouse_TOTAL_TOUCH_PADS_COUNT              (0u)
#define Mouse_TOTAL_TOUCH_PADS_BASIC_COUNT        (0u)
#define Mouse_TOTAL_TRACKPAD_GESTURES_COUNT       (0u)
#define Mouse_TOTAL_BUTTONS_COUNT                 (0u)
#define Mouse_TOTAL_MATRIX_BUTTONS_COUNT          (0u)
#define Mouse_TOTAL_GENERICS_COUNT                (0u)

#define Mouse_POS_FILTERS_MASK                    (0x2u)
#define Mouse_LINEAR_SLIDERS_POS_FILTERS_MASK     (0x2u)
#define Mouse_RADIAL_SLIDERS_POS_FILTERS_MASK     (0x0u)
#define Mouse_TOUCH_PADS_POS_FILTERS_MASK         (0x0u)
#define Mouse_TRACKPAD_GEST_POS_FILTERS_MASK      (0x0u)

#define Mouse_UNUSED_DEBOUNCE_COUNTER_INDEX       (0u)

#define Mouse_TOTAL_PROX_SENSORS_COUNT            (0u)

#define Mouse_END_OF_SLIDERS_INDEX                (0u)
#define Mouse_END_OF_TOUCH_PAD_INDEX              (0u)
#define Mouse_END_OF_BUTTONS_INDEX                (0u)
#define Mouse_END_OF_MATRIX_BUTTONS_INDEX         (0u)
#define Mouse_END_OF_WIDGETS_INDEX                (1u)



#define Mouse_TOTAL_SLIDERS_COUNT            ( Mouse_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Mouse_TOTAL_RADIAL_SLIDERS_COUNT )

#define Mouse_TOTAL_CENTROIDS_COUNT          ( Mouse_TOTAL_SLIDERS_COUNT + \
                                                         (Mouse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u) +\
                                                         (Mouse_TOTAL_TRACKPAD_GESTURES_COUNT * 4u))

#define Mouse_TOTAL_CENTROIDS_BASIC_COUNT    ( Mouse_TOTAL_SLIDERS_COUNT + \
                                                         (Mouse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u))

#define Mouse_TOTAL_CENTROID_AXES_COUNT      ( Mouse_TOTAL_SLIDERS_COUNT + \
                                                         (Mouse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u) +\
                                                         (Mouse_TOTAL_TRACKPAD_GESTURES_COUNT * 2u))

#define Mouse_TOTAL_WIDGET_COUNT             ( Mouse_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Mouse_TOTAL_RADIAL_SLIDERS_COUNT + \
                                                          Mouse_TOTAL_TOUCH_PADS_COUNT + \
                                                          Mouse_TOTAL_BUTTONS_COUNT + \
                                                          Mouse_TOTAL_MATRIX_BUTTONS_COUNT )

#define Mouse_ANY_POS_FILTER                 ( Mouse_MEDIAN_FILTER | \
                                                          Mouse_AVERAGING_FILTER | \
                                                          Mouse_IIR2_FILTER | \
                                                          Mouse_IIR4_FILTER | \
                                                          Mouse_JITTER_FILTER )

#define Mouse_IS_DIPLEX_SLIDER               ( Mouse_TOTAL_DIPLEXED_SLIDERS_COUNT > 0u)

#define Mouse_IS_NON_DIPLEX_SLIDER           ( (Mouse_TOTAL_LINEAR_SLIDERS_COUNT - \
                                                           Mouse_TOTAL_DIPLEXED_SLIDERS_COUNT) > 0u)
#define Mouse_ADD_SLIDER_TYPE                ((Mouse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) ? \
                                                        ((Mouse_TOTAL_TOUCH_PADS_COUNT > 0u) || \
                                                         (Mouse_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) : 0u)

#define Mouse_TOTAL_PROX_SENSOR_COUNT        (Mouse_TOTAL_PROX_SENSORS_COUNT)

#define Mouse_WIDGETS_TBL_SIZE               ( Mouse_TOTAL_WIDGET_COUNT + \
                                                          Mouse_TOTAL_GENERICS_COUNT)

#define Mouse_WIDGET_PARAM_TBL_SIZE          (Mouse_TOTAL_BUTTONS_COUNT + \
                                                         Mouse_TOTAL_SLIDERS_COUNT +\
                                                         Mouse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u + \
                                                         Mouse_TOTAL_TRACKPAD_GESTURES_COUNT * 2u +\
                                                         Mouse_TOTAL_MATRIX_BUTTONS_COUNT * 2u)

#define Mouse_THRESHOLD_TBL_SIZE         (Mouse_WIDGET_PARAM_TBL_SIZE)
#define Mouse_DEBOUNCE_CNT_TBL_SIZE      (Mouse_WIDGET_PARAM_TBL_SIZE)
#define Mouse_RAW_DATA_INDEX_TBL_SIZE    (Mouse_WIDGET_PARAM_TBL_SIZE +\
                                                     Mouse_TOTAL_GENERICS_COUNT)

#define Mouse_RES_MULT                   (256u)


#define Mouse_NOT_WIDGET                     (0xFFFFFFFFu)

/*Types of centroids */
#define Mouse_TYPE_RADIAL_SLIDER             (0x01u)
#define Mouse_TYPE_LINEAR_SLIDER             (0x02u)
#define Mouse_TYPE_GENERIC                   (0xFFu)

/* Defines if sensors or widgets are active */
#define Mouse_SENSOR_IS_ACTIVE               (0x01u)
#define Mouse_SENSOR_1_IS_ACTIVE             (0x01u)
#define Mouse_SENSOR_2_IS_ACTIVE             (0x02u)
#define Mouse_WIDGET_IS_ACTIVE               (0x01u)

/* Defines diplex type of Slider */
#define Mouse_IS_DIPLEX                      (0x80u)

/* Defines fingers positions on Slider  */
#define Mouse_POS_PREV                       (0u)
#define Mouse_POS                            (1u)
#define Mouse_POS_NEXT                       (2u)
#define Mouse_CENTROID_ROUND_VALUE           (0x7F00u)
#define Mouse_MAXIMUM_CENTROID               (0xFFu)

#define Mouse_NEGATIVE_NOISE_THRESHOLD       (20u)
#define Mouse_LOW_BASELINE_RESET             (5u)


/***************************************
*        Function Prototypes
***************************************/

void Mouse_InitializeSensorBaseline(uint32 sensor);
void Mouse_InitializeAllBaselines(void);
void Mouse_InitializeEnabledBaselines(void);
void Mouse_UpdateSensorBaseline(uint32 sensor);
void Mouse_UpdateBaselineNoThreshold(uint32 sensor);
void Mouse_UpdateEnabledBaselines(void);
void Mouse_UpdateWidgetBaseline(uint32 widget);
uint16 Mouse_GetBaselineData(uint32 sensor);
void Mouse_SetBaselineData(uint32 sensor, uint16 data);
void Mouse_BaseInit(uint32 sensor);

#if (Mouse_IS_DIPLEX_SLIDER)
    uint8 Mouse_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold, const uint8 *diplex);
#else
    uint8 Mouse_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold);
#endif /* (Mouse_IS_DIPLEX_SLIDER) */

#if (Mouse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
    uint8 Mouse_CalcCentroid(uint8 maximum, uint8 offset, \
                                    uint8 count, uint32 resolution, uint8 noiseThreshold);
#else
    uint8 Mouse_CalcCentroid(uint8 maximum, uint8 offset, \
                                    uint8 count, uint16 resolution, uint8 noiseThreshold);
#endif /* (Mouse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */


uint8 Mouse_GetFingerThreshold(uint32 widget);
uint8 Mouse_GetNoiseThreshold(uint32 widget);
uint8 Mouse_GetFingerHysteresis(uint32 widget);
uint8 Mouse_GetNegativeNoiseThreshold(uint32 widget);

#if(Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    void Mouse_SetFingerThreshold(uint32 widget, uint8 value);
    void Mouse_SetNoiseThreshold(uint32 widget, uint8 value);
    void Mouse_SetFingerHysteresis(uint32 widget, uint8 value);
    void Mouse_SetNegativeNoiseThreshold(uint32 widget, uint8 value);
    void Mouse_SetDebounce(uint32 widget, uint8 value);
    void Mouse_SetLowBaselineReset(uint32 sensor, uint8 value);
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */

uint8 Mouse_GetDiffCountData(uint32 sensor);
void Mouse_SetDiffCountData(uint32 sensor, uint8 value);

uint32 Mouse_CheckIsSensorActive(uint32 sensor);
uint32 Mouse_CheckIsWidgetActive(uint32 widget);
uint32 Mouse_CheckIsAnyWidgetActive(void);
void Mouse_EnableWidget(uint32 widget);
void Mouse_DisableWidget(uint32 widget);
void Mouse_EnableRawDataFilters(void);
void Mouse_DisableRawDataFilters(void);

#if (Mouse_TOTAL_MATRIX_BUTTONS_COUNT)
    uint32 Mouse_GetMatrixButtonPos(uint32 widget, uint8* pos);
#endif /* (Mouse_TOTAL_MATRIX_BUTTONS_COUNT) */

#if((Mouse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Mouse_TOTAL_LINEAR_SLIDERS_COUNT > 0u))
    uint16 Mouse_GetCentroidPos(uint32 widget);
#endif /* ((Mouse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Mouse_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) */
#if((Mouse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Mouse_TOTAL_LINEAR_SLIDERS_COUNT > 0u))
    uint16 Mouse_GetRadialCentroidPos(uint32 widget);
#endif /* #if((Mouse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Mouse_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) */
#if (Mouse_TOTAL_TOUCH_PADS_COUNT)
    uint32 Mouse_GetTouchCentroidPos(uint32 widget, uint16* pos);
#endif /* (Mouse_TOTAL_TOUCH_PADS_COUNT) */

uint32 Mouse_GetWidgetNumber(uint32 sensor);
uint8 Mouse_GetLowBaselineReset(uint32 sensor);
uint8 Mouse_GetDebounce(uint32 widget);

/* Filter function prototypes for High level APIs */

/* Median filter function prototype */
#if ( (0u != (Mouse_RAW_FILTER_MASK & Mouse_MEDIAN_FILTER)) || \
      (0u != (Mouse_POS_FILTERS_MASK & Mouse_MEDIAN_FILTER)) || \
      ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)) )
    uint16 Mouse_MedianFilter(uint16 x1, uint16 x2, uint16 x3);
#endif /* ( (0u != (Mouse_RAW_FILTER_MASK & Mouse_MEDIAN_FILTER)) || \
      (0u != (Mouse_POS_FILTERS_MASK & Mouse_MEDIAN_FILTER)) || \
      ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)) ) */

/* Averaging filter function prototype */
#if ( (0u != (Mouse_RAW_FILTER_MASK & Mouse_AVERAGING_FILTER)) || \
      (0u != (Mouse_POS_FILTERS_MASK & Mouse_AVERAGING_FILTER)) )
    uint16 Mouse_AveragingFilter(uint16 x1, uint16 x2, uint16 x3);
#endif /* Mouse_RAW_FILTER_MASK && Mouse_POS_FILTERS_MASK */

/* IIR2Filter(1/2prev + 1/2cur) filter function prototype */
#if ( (0u != (Mouse_RAW_FILTER_MASK & Mouse_IIR2_FILTER)) || \
      (0u != (Mouse_POS_FILTERS_MASK & Mouse_IIR2_FILTER)) )
    uint16 Mouse_IIR2Filter(uint16 x1, uint16 x2);
#endif /* Mouse_RAW_FILTER_MASK && Mouse_POS_FILTERS_MASK */

/* IIR4Filter(3/4prev + 1/4cur) filter function prototype */
#if ( (0u != (Mouse_RAW_FILTER_MASK & Mouse_IIR4_FILTER)) || \
      (0u != (Mouse_POS_FILTERS_MASK & Mouse_IIR4_FILTER)) )
    uint16 Mouse_IIR4Filter(uint16 x1, uint16 x2);
#endif /* Mouse_RAW_FILTER_MASK && Mouse_POS_FILTERS_MASK */

/* IIR8Filter(7/8prev + 1/8cur) filter function prototype - RawCounts only */
#if (0u != (Mouse_RAW_FILTER_MASK & Mouse_IIR8_FILTER))
    uint16 Mouse_IIR8Filter(uint16 x1, uint16 x2);
#endif /* Mouse_RAW_FILTER_MASK */

/* IIR16Filter(15/16prev + 1/16cur) filter function prototype - RawCounts only */
#if (0u != (Mouse_RAW_FILTER_MASK & Mouse_IIR16_FILTER))
    uint16 Mouse_IIR16Filter(uint16 x1, uint16 x2);
#endif /* Mouse_RAW_FILTER_MASK */

/* JitterFilter filter function prototype */
#if ( (0u != (Mouse_RAW_FILTER_MASK & Mouse_JITTER_FILTER)) || \
      (0u != (Mouse_POS_FILTERS_MASK & Mouse_JITTER_FILTER)) || \
      (0u != (Mouse_TRACKPAD_GEST_POS_FILTERS_MASK & Mouse_JITTER_FILTER)))
    uint16 Mouse_JitterFilter(uint16 x1, uint16 x2);
#endif /* ( (0u != (Mouse_RAW_FILTER_MASK & Mouse_JITTER_FILTER)) || \
       *    (0u != (Mouse_POS_FILTERS_MASK & Mouse_JITTER_FILTER)) )
       *    (0u != (Mouse_TRACKPAD_GEST_POS_FILTERS_MASK & Mouse_JITTER_FILTER)) )
       */


/***************************************
*     Vars with External Linkage
***************************************/
extern uint16 Mouse_sensorBaseline[Mouse_TOTAL_SENSOR_COUNT];
extern uint8  Mouse_sensorBaselineLow[Mouse_TOTAL_SENSOR_COUNT];
extern uint8 Mouse_sensorSignal[Mouse_TOTAL_SENSOR_COUNT];
extern uint8  Mouse_sensorOnMask[Mouse_TOTAL_SENSOR_MASK];

extern uint8 Mouse_lowBaselineResetCnt[Mouse_TOTAL_SENSOR_COUNT];
extern uint8 Mouse_lowBaselineReset[Mouse_TOTAL_SENSOR_COUNT];

/* Generated by Customizer */
#if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    extern uint8 Mouse_fingerThreshold[Mouse_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Mouse_noiseThreshold[Mouse_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Mouse_negativeNoiseThreshold[Mouse_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Mouse_hysteresis[Mouse_WIDGET_PARAM_TBL_SIZE];
    extern uint8  Mouse_debounce[Mouse_WIDGET_PARAM_TBL_SIZE];
#else
    extern const uint8 Mouse_fingerThreshold[Mouse_WIDGET_PARAM_TBL_SIZE];
    extern const uint8 Mouse_noiseThreshold[Mouse_WIDGET_PARAM_TBL_SIZE];
    extern const uint8 Mouse_hysteresis[Mouse_WIDGET_PARAM_TBL_SIZE];
    extern const uint8  Mouse_debounce[Mouse_WIDGET_PARAM_TBL_SIZE];
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */

extern const uint8 Mouse_rawDataIndex[Mouse_RAW_DATA_INDEX_TBL_SIZE];
extern const uint8 Mouse_numberOfSensors[Mouse_RAW_DATA_INDEX_TBL_SIZE];

#if (0u != Mouse_TOTAL_TRACKPAD_GESTURES_COUNT)
    #if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
        extern uint8 Mouse_posFiltersMask[Mouse_TOTAL_CENTROID_AXES_COUNT];
    #else
        extern const uint8 Mouse_posFiltersMask[Mouse_TOTAL_CENTROID_AXES_COUNT];
    #endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */

    extern uint32 Mouse_centroidMult[Mouse_TOTAL_CENTROID_AXES_COUNT];
#endif /* (0u != Mouse_TOTAL_TRACKPAD_GESTURES_COUNT) */

/***************************************
*        Obsolete Names
***************************************/
#define Mouse_SensorRaw              Mouse_sensorRaw
#define Mouse_SensorEnableMask       Mouse_sensorEnableMask
#define Mouse_SensorBaseline         Mouse_sensorBaseline
#define Mouse_SensorBaselineLow      Mouse_sensorBaselineLow
#define Mouse_SensorSignal           Mouse_sensorSignal
#define Mouse_SensorOnMask           Mouse_sensorOnMask
#define Mouse_LowBaselineResetCnt    Mouse_lowBaselineResetCnt

#endif /* CY_Mouse_CSD_CSHL_Mouse_H */

/* [] END OF FILE */
