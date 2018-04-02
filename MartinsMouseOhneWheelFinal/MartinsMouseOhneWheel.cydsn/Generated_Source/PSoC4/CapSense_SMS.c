/*******************************************************************************
* File Name: Mouse_SMS.c
* Version 2.60
*
* Description:
*  This file provides the source code of wrapper between Mouse CSD component
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

#include "Mouse_SMS.h"
#include "Mouse_PVT.h"

#include "cytypes.h"

#if (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)


/*  Real Charge Divider values */
#if ((0u != Mouse_IS_M0S8PERI_BLOCK) && (0u == CY_PSOC4_4000))
    /*  Dividers are not chained */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            4u,     4u,     4u,    8u,    8u,    8u,    8u,    16u,    16u,    16u,    16u,    16u,    16u,    16u,    16u,    16u
        };
    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            2u,     2u,     2u,    4u,    4u,    4u,    4u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u
        };
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */

#elif (0u != Mouse_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained (PSoC 4000) */
    #if (CYDEV_BCLK__HFCLK__MHZ >= 12u)
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };

    #elif (CYDEV_BCLK__HFCLK__MHZ >= 6u)
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u
        };
    #else
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */

#else
    /*  Dividers are chained (PSoC 4100, PSoC 4200) */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            2u,     2u,     2u,    4u,    4u,    4u,    4u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u
        };

    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        const uint8 Mouse_prescalerTable[Mouse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
#endif /* ((0u != Mouse_IS_M0S8PERI_BLOCK) && (0u == CY_PSOC4_4000)) */


uint8 Mouse_noiseEnvelopeTbl[Mouse_TOTAL_SENSOR_COUNT];
uint8 Mouse_runningDifferenceTbl[Mouse_TOTAL_SENSOR_COUNT];
uint8 Mouse_signRegisterTbl[Mouse_TOTAL_SENSOR_COUNT];
uint16 Mouse_sampleMinTbl[Mouse_TOTAL_SENSOR_COUNT];
uint16 Mouse_sampleMaxTbl[Mouse_TOTAL_SENSOR_COUNT];
uint16 Mouse_previousSampleTbl[Mouse_TOTAL_SENSOR_COUNT];
uint8 Mouse_kValueTbl[Mouse_TOTAL_SENSOR_COUNT];

uint8 Mouse_sensorSensitivity[] = {
    10u, 10u, 10u, 10u, 10u, 
};

Mouse_CONFIG_TYPE_P4_v2_60 Mouse_config;
Mouse_CONFIG_TYPE_POINTERS_P4_v2_60 const Mouse_configPointers = {
    Mouse_modulationIDAC,
    Mouse_compensationIDAC,
    Mouse_sensorSensitivity,
    Mouse_senseClkDividerVal,
    Mouse_sampleClkDividerVal,
    Mouse_widgetNumber,
    Mouse_widgetResolution,
    Mouse_noiseEnvelopeTbl,
    Mouse_runningDifferenceTbl,
    Mouse_signRegisterTbl,
    Mouse_sampleMinTbl,
    Mouse_sampleMaxTbl,
    Mouse_previousSampleTbl,
    Mouse_kValueTbl,
    Mouse_scanSpeedTbl,
    Mouse_prescalerTable,
    (const uint8  *)Mouse_rawDataIndex,
    (const uint8  *)Mouse_numberOfSensors,
    &Mouse_GetSensorRaw,
    &Mouse_PreScan,
    &Mouse_ReadSensorRaw,
    &Mouse_GetBitValue,
    &Mouse_SetBitValue
};


/*******************************************************************************
* Function Name: Mouse_AutoTune
********************************************************************************
*
* Summary:
*  Provides the tuning procedure for all sensors.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*   Mouse_config: the structure which contains configuration parameters
*   Mouse_senseClkDividerVal[]: used to store the Analog Switch
*   dividers for each sensor.
*   Mouse_senseClkDividerVal[]: used to store the
*    Analog Switch divider for all sensors.
*   Mouse_prescalersTuningDone - used to notify the Tuner GUI that
*   the pre-scalers tuning  is done.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_AutoTune(void)
{
    uint32 curSensor;
    uint32 widget;

    Mouse_config.totalScanslotsNum = Mouse_TOTAL_SCANSLOT_COUNT;
    Mouse_config.totalWidgetsNum = Mouse_END_OF_WIDGETS_INDEX;
    Mouse_config.totalSensorsNum = Mouse_TOTAL_SENSOR_COUNT;

    Mouse_config.hfclkFreq_MHz = CYDEV_BCLK__HFCLK__MHZ;
    Mouse_config.sensetivitySeed = Mouse_SENSETIVITY_FACTOR;

    Mouse_config.pointers = &Mouse_configPointers;

    Mouse_SetAnalogSwitchesSrcDirect();

    for(widget = 0u; widget < Mouse_config.totalWidgetsNum; widget++)
    {
        Mouse_widgetResolution[widget] = Mouse_CALIBRATION_RESOLUTION;
    }


    for(curSensor = 0u; curSensor < Mouse_TOTAL_SCANSLOT_COUNT; curSensor++)
    {
        Mouse_senseClkDividerVal[curSensor] = Mouse_CALIBRATION_ASD;
        Mouse_sampleClkDividerVal[curSensor] = Mouse_CALIBRATION_MD;
    }

    Mouse_DisableBaselineIDAC();

    CalibrateSensors_P4_v2_60(&Mouse_config, Mouse_CAL_RAW_COUNT);

    #if(0u != Mouse_TOTAL_CENTROIDS_COUNT)
        Mouse_NormalizeWidgets(Mouse_END_OF_WIDGETS_INDEX, Mouse_modulationIDAC);
    #endif /* (0u != Mouse_TOTAL_CENTROIDS_COUNT)  */

    TunePrescalers_P4_v2_60(&Mouse_config);

    #if(Mouse_PRS_OPTIONS != Mouse__PRS_NONE)
        Mouse_prescalersTuningDone = 1u;
    #endif /* (Mouse_PRS_OPTIONS == Mouse__PRS_NONE) */

    CalibrateSensors_P4_v2_60(&Mouse_config, Mouse_CAL_RAW_COUNT);
    #if(0u != Mouse_TOTAL_CENTROIDS_COUNT)
        Mouse_NormalizeWidgets(Mouse_END_OF_WIDGETS_INDEX, Mouse_modulationIDAC);
    #endif /* (0u != Mouse_TOTAL_CENTROIDS_COUNT)  */
    TuneSensitivity_P4_v2_60(&Mouse_config);

    Mouse_EnableBaselineIDAC(&Mouse_config);

    for(curSensor = 0u; curSensor < Mouse_TOTAL_SCANSLOT_COUNT; curSensor++)
    {
        Mouse_UpdateThresholds(curSensor);
    }
}


/*******************************************************************************
* Function Name: Mouse_UpdateThresholds
********************************************************************************
*
* Summary:
*  The API updates the Finger Threshold, Hysteresis, Noise Threshold, and
*  Negative Noise Threshold in the AutoTuning (Smartsense) Mode.
*
* Parameters:
*  sensor: sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Mouse_widgetNumber[] - contains widget Number for given sensor.
*  0 through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
*  For other sensors the array element equals to 255.
*  Mouse_numberOfSensors[widget] - Number of sensors in the widget.
*   Mouse_fingerThreshold[] - contains the level of signal for each sensor
*   that determines if a finger present on the sensor.
*   Mouse_negativeNoiseThreshold[] - negative Noise Threshold
*   Mouse_hysteresis[] - the array with hysteresis values.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_UpdateThresholds(uint32 sensor)
{
    uint8 fingerThreshold;
    uint32 widget;

    widget = Mouse_widgetNumber[sensor];

    #if(0u != Mouse_FLEXIBLE_THRESHOLDS_EN)
        fingerThreshold = Mouse_fingerThreshold[widget];
        /* Update Threshold manually (flexible) */
        Mouse_noiseThreshold[widget] = (uint8)(fingerThreshold >> 1u);
        Mouse_negativeNoiseThreshold[widget] = (uint8)(fingerThreshold >> 1u);
        Mouse_hysteresis[widget] = (uint8)(fingerThreshold >> 3u);
    #else
        /* Calculate Finger Threshold and Noise Threshold with Smartsense (automatic) */
        CalculateThresholds_P4_v2_60(&Mouse_config, (uint8)sensor, (uint8)widget, Mouse_fingerThreshold, Mouse_noiseThreshold);

        fingerThreshold = Mouse_fingerThreshold[widget];

        /* Update Threshold based on calculated with Smartsense (automatic) */
        Mouse_negativeNoiseThreshold[widget] = Mouse_noiseThreshold[widget];

        if(fingerThreshold < Mouse_THRESHOLD_1)
        {
            Mouse_hysteresis[widget] = (uint8)(fingerThreshold >> 3u);
        }
        else if(fingerThreshold <  Mouse_THRESHOLD_2)
        {
            Mouse_hysteresis[widget] = (uint8)(fingerThreshold >> 4u);
        }
        else if(fingerThreshold <  Mouse_THRESHOLD_3)
        {
            Mouse_hysteresis[widget] = (uint8)(fingerThreshold >> 5u);
        }
        else if(fingerThreshold <  Mouse_THRESHOLD_4)
        {
            Mouse_hysteresis[widget] = (uint8)(fingerThreshold >> 6u);
        }
        else
        {
            Mouse_hysteresis[widget] = 0u;
        }
    #endif /* (0u != Mouse_FLEXIBLE_THRESHOLDS_EN)  */
}

/*******************************************************************************
* Function Name: Mouse_SetAnalogSwitchesSrcDirect
********************************************************************************
*
* Summary:
*  This API switches the charge clock source to prescaler.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_SetAnalogSwitchesSrcDirect(void)
{
    Mouse_CSD_CFG_REG &= ~(Mouse_CSD_CFG_PRS_SELECT);
}


/*******************************************************************************
* Function Name: Mouse_DisableBaselineIDAC
********************************************************************************
*
* Summary:
*  The API disables the Compensation IDAC.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_DisableBaselineIDAC(void)
{
    uint32 newRegValue;

    newRegValue = Mouse_CSD_IDAC_REG;

    newRegValue &= ~(Mouse_CSD_IDAC1_MODE_MASK | Mouse_CSD_IDAC2_MODE_MASK);
    newRegValue |= Mouse_CSD_IDAC1_MODE_VARIABLE;

    Mouse_CSD_IDAC_REG = newRegValue;
}


/*******************************************************************************
* Function Name: Mouse_EnableBaselineIDAC
********************************************************************************
*
* Summary:
*  The API enables the Compensation IDAC.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_compensationIDAC[] - used to store a 7-bit idac value for all the sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void  Mouse_EnableBaselineIDAC(const Mouse_CONFIG_TYPE_P4_v2_60 *config)
{
    uint32 curSensor;
    uint32 newRegValue;

    for(curSensor = 0u; curSensor < config->totalScanslotsNum; curSensor++)
    {
        Mouse_compensationIDAC[curSensor] = Mouse_modulationIDAC[curSensor] / 2u;
        Mouse_modulationIDAC[curSensor] = (Mouse_modulationIDAC[curSensor] + 1u) / 2u;
    }

    CyIntDisable(Mouse_ISR_NUMBER);

    newRegValue = Mouse_CSD_IDAC_REG;
    newRegValue &= ~(Mouse_CSD_IDAC1_MODE_MASK | Mouse_CSD_IDAC2_MODE_MASK);
    newRegValue |= (Mouse_CSD_IDAC1_MODE_VARIABLE | Mouse_CSD_IDAC2_MODE_FIXED);
    Mouse_CSD_IDAC_REG = newRegValue;

    CyIntEnable(Mouse_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Mouse_SetSensitivity
********************************************************************************
*
* Summary:
*  This API sets the sensitivity value for the sensor. The sensitivity
*  value is used during the auto-tuning algorithm executed as part of the Mouse_Start API.
*  This API is called by the application layer prior to calling the Mouse_Start API.
*  Calling this API after execution of Mouse_Start API has no effect.
*
* Parameters:
*  sensor  Sensor index
*  data    Sensitivity of the sensor. Possible values are below
*  1 - 0.1pf sensitivity
*  2 - 0.2pf sensitivity
*  3 - 0.3pf sensitivity
*  4 - 0.4pf sensitivity
*  5 - 0.5pf sensitivity
*  6 - 0.6pf sensitivity
*  7 - 0.7pf sensitivity
*  8 - 0.8pf sensitivity
*  9 - 0.9pf sensitivity
*  10 - 1.0pf sensitivity
*  All other values, set sensitivity to 1.0pf
*
*  Return Value:
*   None
*
* Global Variables:
* Mouse_sensorSensitivity[] - This 8-bits array contains Sensitivity in the LSB nibble
*                                and Noise Reset Counter value in the MSB nibble.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_SetSensitivity(uint32 sensor, uint32 value)
{
    if(value > 10u)
    {
        value = 10u;
    }

    /* Clear SensorSensitivity value in LSB nibble */
    Mouse_sensorSensitivity[sensor] &= (uint8)~Mouse_SENSITIVITY_MASK;
    /* Set SensorSensitivity value in LSB nibble */
    Mouse_sensorSensitivity[sensor] |= (uint8)value;
}


/*******************************************************************************
* Function Name: Mouse_GetSensitivityCoefficient
********************************************************************************
*
* Summary:
*  This API returns the K coefficient for the appropriate sensor.
*
* Parameters:
*  sensor:  Sensor index
*
*  Return Value:
*   K - value for the appropriate sensor.
*
* Global Variables:
*  Mouse_kValueTbl[] - This 8-bits array contains the K value for each sensor.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint8 Mouse_GetSensitivityCoefficient(uint32 sensor)
{
    return (Mouse_kValueTbl[sensor]);
}


/*******************************************************************************
* Function Name: Mouse_GetNoiseEnvelope
********************************************************************************
*
* Summary:
*  This function returns the noise value of a
*  component.
*
* Parameters:
*  sensor - Sensor number.
*
* Return:
*  The noise envelope value of the sensor
*  indicated by argument.
*
* Global Variables:
*  Mouse_noiseEnvelopeTbl[] - array with Noise Envelope.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Mouse_GetNoiseEnvelope(uint32 sensor)
{
    return((uint16)((uint16)Mouse_noiseEnvelopeTbl[sensor] << 1u) + 1u);
}


/*******************************************************************************
* Function Name: Mouse_GetNormalizedDiffCountData
********************************************************************************
*
* Summary:
*  This API returns normalized difference count data.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  This API returns a normalized count value of the sensor indicated by the
*  argument.
*
* Global Variables:
*  Mouse_sensorSignal[] - array with difference counts for sensors
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Mouse_GetNormalizedDiffCountData(uint32 sensor)
{
    return (uint16)(((uint32)Mouse_sensorSignal[sensor] << 7u) / Mouse_kValueTbl[sensor]);
}

#endif /* (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) */

/* [] END OF FILE */
