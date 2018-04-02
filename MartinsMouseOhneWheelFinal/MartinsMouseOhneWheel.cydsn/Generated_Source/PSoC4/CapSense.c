/*******************************************************************************
* File Name: Mouse.c
* Version 2.60
*
* Description:
*  This file provides the source code for scanning APIs for the Mouse CSD
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Mouse.h"
#include "Mouse_PVT.h"
#include "cypins.h"


#if(0u != Mouse_CSHL_API_GENERATE)
    #include "Mouse_CSHL.h"
#endif /* (0u != Mouse_CSHL_API_GENERATE) */

/* This definition is intended to prevent unexpected linker error.
   For more details please see the IAR Technical Note 49981 */
#if defined(__ICCARM__)
    extern void Mouse_EnableSensor(uint32 sensor);
    extern void Mouse_DisableSensor(uint32 sensor);
#endif /* (__ICCARM__) */

/* SmartSense functions */
#if (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)
    uint8 Mouse_lowLevelTuningDone = 0u;
    uint8 Mouse_scanSpeedTbl[((Mouse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
#endif /* (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) */

#if(Mouse_PRS_OPTIONS != Mouse__PRS_NONE)
    uint8 Mouse_prescalersTuningDone = 0u;
#endif /* (Mouse_PRS_OPTIONS == Mouse__PRS_NONE) */

/* Global software variables */
volatile uint8 Mouse_csdStatusVar = 0u;   /* Mouse CSD status, variable */
volatile uint8 Mouse_sensorIndex = 0u;    /* Index of scannig sensor */

/* Global array of Raw Counts */
uint16 Mouse_sensorRaw[Mouse_TOTAL_SENSOR_COUNT] = {0u};

/* Backup variables for trim registers*/
#if (Mouse_IDAC1_POLARITY == Mouse__IDAC_SINK)
    uint8 Mouse_csdTrim2;
#else
    uint8 Mouse_csdTrim1;
#endif /* (Mouse_IDAC1_POLARITY == Mouse__IDAC_SINK) */

/* Global array of un-scanned sensors state */
uint8 Mouse_unscannedSnsDriveMode[Mouse_PORT_PIN_CONFIG_TBL_ZISE];

/* Backup array for Mouse_sensorEnableMask */
uint8 Mouse_sensorEnableMaskBackup[(((Mouse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];

/* Configured constants and arrays by Customizer */
uint8 Mouse_sensorEnableMask[(((Mouse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)] = {
0x1Fu, };

reg32 * Mouse_pcTable[] = {
    (reg32 *)Mouse_Sns__LinearSlider0_e0__LS__PC, 
    (reg32 *)Mouse_Sns__LinearSlider0_e1__LS__PC, 
    (reg32 *)Mouse_Sns__LinearSlider0_e2__LS__PC, 
    (reg32 *)Mouse_Sns__LinearSlider0_e3__LS__PC, 
    (reg32 *)Mouse_Sns__LinearSlider0_e4__LS__PC, 
};

const uint8 Mouse_portTable[] = {
    Mouse_Sns__LinearSlider0_e0__LS__PORT, 
    Mouse_Sns__LinearSlider0_e1__LS__PORT, 
    Mouse_Sns__LinearSlider0_e2__LS__PORT, 
    Mouse_Sns__LinearSlider0_e3__LS__PORT, 
    Mouse_Sns__LinearSlider0_e4__LS__PORT, 
};

const uint32 Mouse_maskTable[] = {
    Mouse_Sns__LinearSlider0_e0__LS__MASK, 
    Mouse_Sns__LinearSlider0_e1__LS__MASK, 
    Mouse_Sns__LinearSlider0_e2__LS__MASK, 
    Mouse_Sns__LinearSlider0_e3__LS__MASK, 
    Mouse_Sns__LinearSlider0_e4__LS__MASK, 
};

const uint8 Mouse_pinShiftTbl[] = {
    (uint8) Mouse_Sns__LinearSlider0_e0__LS__SHIFT, 
    (uint8) Mouse_Sns__LinearSlider0_e1__LS__SHIFT, 
    (uint8) Mouse_Sns__LinearSlider0_e2__LS__SHIFT, 
    (uint8) Mouse_Sns__LinearSlider0_e3__LS__SHIFT, 
    (uint8) Mouse_Sns__LinearSlider0_e4__LS__SHIFT, 
};

uint8 Mouse_modulationIDAC[Mouse_TOTAL_SENSOR_COUNT];
uint8 Mouse_compensationIDAC[Mouse_TOTAL_SENSOR_COUNT];

uint32 Mouse_widgetResolution[Mouse_RESOLUTIONS_TBL_SIZE] = {
    Mouse_RESOLUTION_12_BITS,
};

uint8 Mouse_senseClkDividerVal[Mouse_TOTAL_SCANSLOT_COUNT];
uint8 Mouse_sampleClkDividerVal[Mouse_TOTAL_SCANSLOT_COUNT];

const uint8 Mouse_widgetNumber[Mouse_TOTAL_SENSOR_COUNT] = {
    0u, 0u, 0u, 0u, 0u, /* LinearSlider0__LS */
};

reg32* const Mouse_prtSelTbl[Mouse_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_HSIOM_PORT_SEL0),
    ((reg32 *) CYREG_HSIOM_PORT_SEL1),
    ((reg32 *) CYREG_HSIOM_PORT_SEL2),
    ((reg32 *) CYREG_HSIOM_PORT_SEL3),
    ((reg32 *) CYREG_HSIOM_PORT_SEL4),
    ((reg32 *) CYREG_HSIOM_PORT_SEL5),
    ((reg32 *) CYREG_HSIOM_PORT_SEL6),
};

reg32* const Mouse_prtCfgTbl[Mouse_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_GPIO_PRT0_PC),
    ((reg32 *) CYREG_GPIO_PRT1_PC),
    ((reg32 *) CYREG_GPIO_PRT2_PC),
    ((reg32 *) CYREG_GPIO_PRT3_PC),
    ((reg32 *) CYREG_GPIO_PRT4_PC),
    ((reg32 *) CYREG_GPIO_PRT5_PC),
    ((reg32 *) CYREG_GPIO_PRT6_PC),
};

reg32* const Mouse_prtDRTbl[Mouse_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_GPIO_PRT0_DR),
    ((reg32 *) CYREG_GPIO_PRT1_DR),
    ((reg32 *) CYREG_GPIO_PRT2_DR),
    ((reg32 *) CYREG_GPIO_PRT3_DR),
    ((reg32 *) CYREG_GPIO_PRT4_DR),
    ((reg32 *) CYREG_GPIO_PRT5_DR),
    ((reg32 *) CYREG_GPIO_PRT6_DR),
};



/*******************************************************************************
* Function Name: Mouse_Init
********************************************************************************
*
* Summary:
*  API initializes default Mouse configuration provided by the customizer that defines
*  the mode of component operations and resets all the sensors to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_immunityIndex - defines immunity level.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_Init(void)
{
    uint32 curSensor;

    #if(Mouse_IS_SHIELD_ENABLE)

        #if((Mouse_CSH_TANK_PREGARGE_OPTION == Mouse__CAPPRIOBUF) || (0u != Mouse_CSH_TANK_ENABLE))
            uint32 newRegValue;

            newRegValue = Mouse_CTANK_CONNECTION_REG;
            newRegValue &= ~(Mouse_CSD_CTANK_CONNECTION_MASK);
            newRegValue |= Mouse_CSD_CTANK_TO_AMUXBUS_B;
            Mouse_CTANK_CONNECTION_REG = newRegValue;

            #if(Mouse_CSH_TANK_PREGARGE_OPTION == Mouse__CAPPRIOBUF)
                newRegValue = Mouse_CTANK_PORT_PC_REG;
                newRegValue &= ~(Mouse_CSD_CTANK_PC_MASK);
                newRegValue |= Mouse_CTANK_PC_STRONG_MODE;
                Mouse_CTANK_PORT_PC_REG = newRegValue;

                newRegValue = Mouse_CTANK_PORT_DR_REG;
                newRegValue &= ~(Mouse_CTANK_DR_MASK);
                newRegValue |= Mouse_CTANK_DR_CONFIG;
                Mouse_CTANK_PORT_DR_REG = newRegValue;
            #endif /* (Mouse_CSH_TANK_PREGARGE_OPTION == Mouse__CAPPRIOBUF) */

        #endif /* ((Mouse_CSH_TANK_PREGARGE_OPTION == Mouse__CAPPRIOBUF) || (Mouse_CSH_TANK_ENABLE)) */

        Mouse_EnableShieldElectrode((uint32)Mouse_SHIELD_PIN_NUMBER, (uint32)Mouse_SHIELD_PORT_NUMBER);

    #endif /* (Mouse_IS_SHIELD_ENABLE) */

    for(curSensor = 0u; curSensor < Mouse_TOTAL_SENSOR_COUNT; curSensor++)
    {
        Mouse_SetUnscannedSensorState(curSensor, Mouse_CONNECT_INACTIVE_SNS);
    }

    Mouse_CsdHwBlockInit();
    Mouse_SetShieldDelay(Mouse_SHIELD_DELAY );

    /* Clear all sensors */
    Mouse_ClearSensors();
}

/*******************************************************************************
* Function Name: Mouse_CsdHwBlockInit
********************************************************************************
*
* Summary:
*  Initialises the hardware parameters of the CSD block
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_csdTrim1 - Contains IDAC trim register value for Sourcing Mode.
*  Mouse_csdTrim2 - Contains IDAC trim register value for Sinking Mode.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_CsdHwBlockInit(void)
{
    uint32 newRegValue;

        /* Set trim registers for CSD Mode */
    #if (Mouse_IDAC1_POLARITY == Mouse__IDAC_SINK)
        /* iDAC1 Sinking Mode */
        Mouse_csdTrim2 = (uint8)Mouse_CSD_TRIM2_REG;
        newRegValue = Mouse_csdTrim2;
        newRegValue &= Mouse_CSD_IDAC1_TRIM_MASK;
        newRegValue |= (uint32)((uint32)Mouse_SFLASH_CSD_TRIM2_REG & (uint32)Mouse_CSFLASH_TRIM_IDAC1_MASK);

        #if (Mouse_IDAC_CNT == 2u)
            /* iDAC2 Sinking Mode */
            newRegValue &= Mouse_CSD_IDAC2_TRIM_MASK;
            newRegValue |= (uint32)((uint32)Mouse_SFLASH_CSD_TRIM2_REG & (uint32)Mouse_CSFLASH_TRIM_IDAC2_MASK);
        #endif /* (Mouse_IDAC_CNT == 2u) */
        Mouse_CSD_TRIM2_REG = newRegValue;
    #else
        /* iDAC1 Sourcing Mode */
        Mouse_csdTrim1 = (uint8)Mouse_CSD_TRIM1_REG;
        newRegValue = Mouse_csdTrim1;
        newRegValue &= Mouse_CSD_IDAC1_TRIM_MASK;
        newRegValue |= (uint32)((uint32)Mouse_SFLASH_CSD_TRIM1_REG & (uint32)Mouse_CSFLASH_TRIM_IDAC1_MASK);
        #if (Mouse_IDAC_CNT == 2u)
             /* iDAC2 Sourcing Mode */
            newRegValue &= Mouse_CSD_IDAC2_TRIM_MASK;
            newRegValue |= (uint32)((uint32)Mouse_SFLASH_CSD_TRIM1_REG & (uint32)Mouse_CSFLASH_TRIM_IDAC2_MASK);
        #endif
        Mouse_CSD_TRIM1_REG = newRegValue;
    #endif /* (Mouse_IDAC1_POLARITY == Mouse__IDAC_SINK) */

    /* Configure CSD and IDAC */
    #if (Mouse_IDAC_CNT > 1u)
        Mouse_CSD_IDAC_REG = Mouse_DEFAULT_CSD_IDAC_CONFIG;
        Mouse_CSD_CFG_REG = Mouse_DEFAULT_CSD_CONFIG;
    #else
        Mouse_CSD_IDAC_REG &= ~(Mouse_CSD_IDAC1_MODE_MASK  | Mouse_CSD_IDAC1_DATA_MASK);
        Mouse_CSD_IDAC_REG |= Mouse_DEFAULT_CSD_IDAC_CONFIG;

        Mouse_CSD_CFG_REG &= ~(Mouse_CSD_CONFIG_MASK);
        Mouse_CSD_CFG_REG |= (Mouse_DEFAULT_CSD_CONFIG);
    #endif /* (Mouse_IDAC_CNT > 1u) */


    /* Connect Cmod to AMUX bus */
    newRegValue = Mouse_CMOD_CONNECTION_REG;
    newRegValue &= ~(Mouse_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Mouse_CSD_CMOD_TO_AMUXBUS_A;
    Mouse_CMOD_CONNECTION_REG = newRegValue;

    /* Configure Dead Band PWM if it is enabled */
    #if(Mouse_CSD_4B_PWM_MODE != Mouse__PWM_OFF)
        Mouse_CSD_4B_PWM_REG = Mouse_DEFAULT_CSD_4B_PWM_CONFIG;
    #endif /* (Mouse_CSD_4B_PWM_MODE != Mouse__PWM_OFF) */

    /* Setup ISR */
    CyIntDisable(Mouse_ISR_NUMBER);
    #if !defined(CY_EXTERNAL_INTERRUPT_CONFIG)
        (void)CyIntSetVector(Mouse_ISR_NUMBER, &Mouse_ISR);
        CyIntSetPriority(Mouse_ISR_NUMBER, Mouse_ISR_PRIORITY);
    #endif /* (CY_EXTERNAL_INTERRUPT_CONFIG) */
}


/*******************************************************************************
* Function Name: Mouse_Enable
********************************************************************************
*
* Summary:
*  Enables the CSD block and related resources to an active mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_Enable(void)
{
    uint32 newRegValue;

    /* Enable Clocks */
    Mouse_SenseClk_Stop();
    Mouse_SampleClk_Stop();

    Mouse_SampleClk_SetDividerValue((uint16)Mouse_INITIAL_CLK_DIVIDER);
    Mouse_SenseClk_SetDividerValue((uint16)Mouse_INITIAL_CLK_DIVIDER);

    #if (0u == Mouse_IS_M0S8PERI_BLOCK)
        Mouse_SenseClk_Start();
        Mouse_SampleClk_Start();
    #else
        Mouse_SampleClk_Start();
        Mouse_SenseClk_StartEx(Mouse_SampleClk__DIV_ID);
    #endif /* (0u == Mouse_IS_M0S8PERI_BLOCK) */

    /* Enable the CSD block */
    newRegValue = Mouse_CSD_CFG_REG;
    newRegValue |= (Mouse_CSD_CFG_ENABLE | Mouse_CSD_CFG_SENSE_COMP_EN
                                                   | Mouse_CSD_CFG_SENSE_EN);
    Mouse_CSD_CFG_REG = newRegValue;

    /* Enable interrupt */
    CyIntEnable(Mouse_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Mouse_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin the component operation. Mouse_Start()
*  calls the Mouse_Init() function, and then calls the Mouse_Enable()
*  function. Initializes the registers and starts the CSD method of the Mouse
*  component. Resets all the sensors to an inactive state. Enables interrupts for
*  sensors scanning. When the SmartSense tuning mode is selected, the tuning procedure
*  is applied for all the sensors. The Mouse_Start() routine must be called before
*  any other API routines.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*   Mouse_initVar - used to check the initial configuration,
*   modified on the first function call.
*  Mouse_lowLevelTuningDone - Used to notify the Tuner GUI that
*   tuning of the scanning parameters is done.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_Start(void)
{

    static uint8 Mouse_initVar = 0u;

    #if((0u != Mouse_AUTOCALIBRATION_ENABLE) && (Mouse_TUNING_METHOD != Mouse__TUNING_AUTO))
        uint32 curSensor;
        uint32 rawLevel;
        uint32 widget;
    #endif /* ((0u != Mouse_AUTOCALIBRATION_ENABLE) && (Mouse_TUNING_METHOD != Mouse__TUNING_AUTO)) */

    if (Mouse_initVar == 0u)
    {
        Mouse_Init();
        Mouse_initVar = 1u;
    }
    Mouse_Enable();

    /* AutoTunning start */
    #if(Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)
        #if(0u != Mouse_CSHL_API_GENERATE)
            Mouse_AutoTune();
            Mouse_lowLevelTuningDone = 1u;
        #endif /* (0u != Mouse_CSHL_API_GENERATE) */
    #else
        #if(0u != Mouse_AUTOCALIBRATION_ENABLE)

            #if(Mouse_IDAC_CNT > 1u)
                Mouse_CSD_IDAC_REG &= ~(Mouse_CSD_IDAC2_MODE_MASK);
            #endif /* (Mouse_IDAC_CNT > 1u) */

            for(curSensor = 0u; curSensor < Mouse_TOTAL_SCANSLOT_COUNT; curSensor++)
            {
                widget = Mouse_widgetNumber[curSensor];
                rawLevel = Mouse_widgetResolution[widget] >> Mouse_RESOLUTION_OFFSET;

                /* Calibration level should be equal to 85% from scanning resolution */
                rawLevel = (rawLevel * 85u) / 100u;

                Mouse_CalibrateSensor(curSensor, rawLevel, Mouse_modulationIDAC);
            }

            #if(0u != Mouse_TOTAL_CENTROIDS_COUNT)
                Mouse_NormalizeWidgets(Mouse_END_OF_WIDGETS_INDEX, Mouse_modulationIDAC);
            #endif /* (0u != Mouse_TOTAL_CENTROIDS_COUNT)  */

            #if(Mouse_IDAC_CNT > 1u)
                for(curSensor = 0u; curSensor < Mouse_TOTAL_SCANSLOT_COUNT; curSensor++)
                {
                    Mouse_compensationIDAC[curSensor] = Mouse_modulationIDAC[curSensor] / 2u;
                    Mouse_modulationIDAC[curSensor] = (Mouse_modulationIDAC[curSensor] + 1u) / 2u;
                }
                Mouse_CSD_IDAC_REG |= Mouse_CSD_IDAC2_MODE_FIXED;
            #endif /* (Mouse_IDAC_CNT > 1u) */

        #endif /* (0u != Mouse_AUTOCALIBRATION_ENABLE) */

    #endif /* ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) */

    /* Initialize Advanced Centroid */
    #if(Mouse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
        Mouse_AdvancedCentroidInit();
    #endif /* (Mouse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */

    /* Connect sense comparator input to AMUXA */
    Mouse_CSD_CFG_REG |= Mouse_CSD_CFG_SENSE_INSEL;
}


/*******************************************************************************
* Function Name: Mouse_Stop
********************************************************************************
*
* Summary:
*  Stops the sensor scanning, disables component interrupts, and resets all the
*  sensors to an inactive state. Disables the Active mode power template bits for
*  the subcomponents used within Mouse.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_csdTrim1 - Contains the IDAC trim register value for the Sourcing Mode.
*  Mouse_csdTrim2 - Contains the IDAC trim register value for vSinking Mode.
*
* Side Effects:
*  This function should be called after scans are completed.
*
*******************************************************************************/
void Mouse_Stop(void)
{
    /* Disable interrupt */
    CyIntDisable(Mouse_ISR_NUMBER);

    Mouse_CSD_CFG_REG &= ~(Mouse_CSD_CFG_SENSE_COMP_EN | Mouse_CSD_CFG_SENSE_EN);

    #if(Mouse_IDAC_CNT == 2u)
        Mouse_CSD_CFG_REG &= ~(Mouse_CSD_CFG_ENABLE);
    #endif /* (Mouse_IDAC_CNT == 2u) */

    /* Disable the Clocks */
    Mouse_SenseClk_Stop();
    Mouse_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: Mouse_FindNextSensor
********************************************************************************
*
* Summary:
*  Finds the next sensor to scan.
*
* Parameters:
*  snsIndex:  Current index of sensor.
*
* Return:
*  Returns the next sensor index to scan.
*
* Global Variables:
*  Mouse_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*
* Side Effects:
*  This function affects the  current scanning and should not
*  be used outside the component.
*
*******************************************************************************/
uint8 Mouse_FindNextSensor(uint8 snsIndex)
{
    uint32 enableFlag;

    /* Check if sensor enabled */
    do
    {
        /* Proceed with next sensor */
        snsIndex++;
        if(snsIndex == Mouse_TOTAL_SENSOR_COUNT)
        {
            break;
        }
        enableFlag = Mouse_GetBitValue(Mouse_sensorEnableMask, (uint32)snsIndex);
    }
    while(enableFlag == 0u);

    return ((uint8)snsIndex);
}


/*******************************************************************************
* Function Name: Mouse_ScanSensor
********************************************************************************
*
* Summary:
*  Sets the scan settings and starts scanning a sensor. After scanning is complete,
*  the ISR copies the measured sensor raw data to the global raw sensor array.
*  Use of the ISR ensures this function is non-blocking.
*  Each sensor has a unique number within the sensor array.
*  This number is assigned by the Mouse customizer in sequence.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Mouse_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and mode of scan as single scan.
*  Mouse_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to the provided sensor argument.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_ScanSensor(uint32 sensor)
{
    /* Clears status/control variable and set sensorIndex */
    Mouse_csdStatusVar = 0u;
    Mouse_sensorIndex = (uint8)sensor;

    /* Start of sensor scan */
    Mouse_csdStatusVar = (Mouse_SW_STS_BUSY | Mouse_SW_CTRL_SINGLE_SCAN);
    Mouse_PreScan(sensor);
}


#if(0u != Mouse_CSHL_API_GENERATE)
/*******************************************************************************
* Function Name: Mouse_ScanWidget
********************************************************************************
*
* Summary:
*  Sets the scan settings and starts scanning a widget.
*
* Parameters:
*  uint32 widget: Widget number.
*
* Return:
*  None
*
* Global Variables:
*  Mouse_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  Mouse_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function Mouse_FindNextSensor or
*  Mouse_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*  Mouse_sensorEnableMaskBackup[] - used to backup bit masks of the enabled
*   sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_ScanWidget(uint32 widget)
{
    uint32 sensorsPerWidget;
    uint32 lastSensor;
    uint32 snsIndex;

    /* Get first sensor in widget */
    Mouse_sensorIndex = Mouse_rawDataIndex[widget];

    /* Get number of sensors in widget */
    sensorsPerWidget = Mouse_numberOfSensors[widget];

    /* Check if generic Sensor */
    if(sensorsPerWidget == 0u)
    {
        sensorsPerWidget = 1u;
    }

    /* Get last sensor in widget */
    lastSensor = (Mouse_rawDataIndex[widget] + sensorsPerWidget) - 1u;

    /* Backup sensorEnableMask array */
    for(snsIndex = 0u; snsIndex < Mouse_TOTAL_SENSOR_MASK; snsIndex++)
    {
        /* Backup sensorEnableMask array */
        Mouse_sensorEnableMaskBackup[snsIndex] = Mouse_sensorEnableMask[snsIndex];
    }

    /* Update sensorEnableMask to scan the sensors that belong to widget */
    for(snsIndex = 0u; snsIndex < Mouse_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        /* Update sensorEnableMask array bits to scan the widget only */
        if((snsIndex >= Mouse_sensorIndex) && (snsIndex <= lastSensor))
        {
            /* Set sensor bit to scan */
            Mouse_SetBitValue(Mouse_sensorEnableMask, snsIndex, 1u);
        }
        else
        {
            /* Reset sensor bit to do not scan */
            Mouse_SetBitValue(Mouse_sensorEnableMask, snsIndex, 0u);
        }
    }

    /* Check end of scan condition */
    if(Mouse_sensorIndex < Mouse_TOTAL_SENSOR_COUNT)
    {
        /* Set widget busy bit in status/control variable*/
        Mouse_csdStatusVar = (Mouse_SW_STS_BUSY | Mouse_SW_CTRL_WIDGET_SCAN);
        /* Scan first sensor of widget*/
        Mouse_PreScan((uint32)Mouse_sensorIndex);
    }
}
#endif /* (0u != Mouse_CSHL_API_GENERATE) */


/*******************************************************************************
* Function Name: Mouse_ScanEnableWidgets
********************************************************************************
*
* Summary:
*  This is the preferred method to scan all of the enabled widgets.
*  The API starts scanning a sensor within the enabled widgets.
*  The ISR continues scanning the sensors until all the enabled widgets are scanned.
*  Use of the ISR ensures this function is non-blocking.
*  All the widgets are enabled by default except proximity widgets.
*  The proximity widgets must be manually enabled as their long scan time
*  is incompatible with a fast response required of other widget types.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  Mouse_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function Mouse_FindNextSensor or
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_ScanEnabledWidgets(void)
{
    /* Clears status/control variable and set sensorIndex */
    Mouse_csdStatusVar = 0u;
    Mouse_sensorIndex = 0xFFu;

    /* Find next sensor */
    Mouse_sensorIndex = (uint8)Mouse_FindNextSensor(Mouse_sensorIndex);

    /* Check end of scan condition */
    if(Mouse_sensorIndex < Mouse_TOTAL_SENSOR_COUNT)
    {
        Mouse_csdStatusVar |= Mouse_SW_STS_BUSY;
        Mouse_PreScan((uint32)Mouse_sensorIndex);
    }
}


/*******************************************************************************
* Function Name: Mouse_IsBusy
********************************************************************************
*
* Summary:
*  Returns the state of the Mouse component. 1 means that scanning in
*  progress and 0 means that scanning is complete.
*
* Parameters:
*  None
*
* Return:
*  Returns the state of scanning. 1 - scanning in progress, 0 - scanning
*  completed.
*
* Global Variables:
*  Mouse_csdStatusVar - used to provide the status and mode of the scanning process.
*  Checks the busy status.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Mouse_IsBusy(void)
{
    return ((uint32)((0u != (Mouse_csdStatusVar & Mouse_SW_STS_BUSY)) ? 1u : 0u));
}


/*******************************************************************************
* Function Name: Mouse_ReadSensorRaw
********************************************************************************
*
* Summary:
*  Returns sensor raw data from the global Mouse_sensorRaw[ ] array.
*  Each scan sensor has a unique number within the sensor array.
*  This number is assigned by the Mouse customizer in sequence.
*  Raw data can be used to perform calculations outside of the Mouse
*  provided framework.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  Returns the current raw data value for a defined sensor number.
*
* Global Variables:
*  Mouse_sensorRaw[] - used to store sensors raw data.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Mouse_ReadSensorRaw(uint32 sensor)
{
    return Mouse_sensorRaw[sensor];
}


/*******************************************************************************
* Function Name: Mouse_WriteSensorRaw
********************************************************************************
*
* Summary:
*  This API writes the raw count value passed as an argument to the sensor Raw count array.
*
* Parameters:
*  sensor:  Sensor number.
*  value: Raw count value.
*
* Global Variables:
*  Mouse_sensorRaw[] - used to store sensors raw data.
*
* Return:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_WriteSensorRaw(uint32 sensor, uint16 value)
{
    Mouse_sensorRaw[sensor] = value;
}


#if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Mouse_SetScanResolution
    ********************************************************************************
    *
    * Summary:
    *  Sets a value of the sensor scan resolution for a widget.
    *  The MIN resolution can be set 8-bit. The MAX Resolution can be set 16 bit.
    *  This function is not available for the tuning mode "None".
    *
    * Parameters:
    *  widget:     Widget index.
    *  resolution: Represents the resolution value. The following defines which are available in the
    *              Mouse.h file should be used:
    *              Mouse_RESOLUTION_6_BITS
    *              Mouse_RESOLUTION_7_BITS
    *              Mouse_RESOLUTION_8_BITS
    *              Mouse_RESOLUTION_9_BITS
    *              Mouse_RESOLUTION_10_BITS
    *              Mouse_RESOLUTION_11_BITS
    *              Mouse_RESOLUTION_12_BITS
    *              Mouse_RESOLUTION_13_BITS
    *              Mouse_RESOLUTION_14_BITS
    *              Mouse_RESOLUTION_15_BITS
    *              Mouse_RESOLUTION_16_BITS
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Mouse_widgetResolution[] - used to store scan resolution of each widget.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Mouse_SetScanResolution(uint32 widget, uint32 resolution)
    {
        Mouse_widgetResolution[widget] = resolution;
    }
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */


/*******************************************************************************
* Function Name: Mouse_GetScanResolution
********************************************************************************
*
* Summary:
*  Returns the resolution value for the appropriate sensor.
*  This function is not available for tuning mode "None".
*
* Parameters:
*  widget:     Widget index.
*
* Return:
*  resolution: Returns the resolution value for the appropriate sensor.
*              The resolution values are defined in the Mouse.h file
*              The defines are encountered below:
*              Mouse_RESOLUTION_6_BITS
*              Mouse_RESOLUTION_7_BITS
*              Mouse_RESOLUTION_8_BITS
*              Mouse_RESOLUTION_9_BITS
*              Mouse_RESOLUTION_10_BITS
*              Mouse_RESOLUTION_11_BITS
*              Mouse_RESOLUTION_12_BITS
*              Mouse_RESOLUTION_13_BITS
*              Mouse_RESOLUTION_14_BITS
*              Mouse_RESOLUTION_15_BITS
*              Mouse_RESOLUTION_16_BITS
*
* Global Variables:
*  Mouse_widgetResolution[] - used to store scan resolution of every widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Mouse_GetScanResolution(uint32 widget)
{
    return(Mouse_widgetResolution[widget]);
}


/*******************************************************************************
* Function Name: Mouse_ClearSensors
********************************************************************************
*
* Summary:
*  Resets all the sensors to the non-sampling state by sequentially disconnecting
*  all the sensors from Analog MUX Bus and putting them to an inactive state.
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
void Mouse_ClearSensors(void)
{
    uint32 snsIndex;

    for (snsIndex = 0u; snsIndex < Mouse_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        Mouse_DisableScanSlot(snsIndex);
    }
}


#if (Mouse_IS_COMPLEX_SCANSLOTS)
    /*******************************************************************************
    * Function Name: Mouse_EnableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Configures the selected slot to measure during the next measurement
    *  cycle. The corresponding pin/pins are set to Analog High-Z mode and
    *  connected to the Analog Mux Bus. This also enables the comparator function.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Constants:
    *  Mouse_portTable[]  - used to store the port number that pin
    *  belongs to for each sensor.
    *  Mouse_maskTable[]  - used to store the pin within the port for
    *  each sensor.
    *  Mouse_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for
    *  complex sensors.
    *  Bit 7 (msb) is used to define the sensor type: single or complex.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Mouse_EnableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;
        /* Read sensor type: single or complex */
        uint8 snsType = Mouse_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & Mouse_COMPLEX_SS_FLAG) == 0u)
        {
            /* Enable sensor (single) */
            Mouse_EnableSensor(slot);
        }
        else
        {
            /* Enable complex sensor */
            snsType &= ~Mouse_COMPLEX_SS_FLAG;
            index = &Mouse_indexTable[snsType];
            snsNumber = Mouse_maskTable[slot];

            for (j = 0u; j < snsNumber; j++)
            {
                Mouse_EnableSensor(index[j]);
            }
        }
    }


    /*******************************************************************************
    * Function Name: Mouse_DisableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Disables the selected slot. The corresponding pin/pins is/are disconnected
    *  from the Analog Mux Bus and connected to GND, High_Z or Shield electrode.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Mouse_portTable[]  - used to store the port number that pin
    *  belongs to for each sensor.
    *  Mouse_maskTable[]  - used to store the pin within the port for
    *  each sensor.
    *  Mouse_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for
    *  complex sensors.
    *  7bit(msb) is used to define the sensor type: single or complex.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Mouse_DisableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;

        /* Read sensor type: single or complex */
        uint8 snsType = Mouse_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & Mouse_COMPLEX_SS_FLAG) == 0u)
        {
            /* Disable sensor (single) */
            Mouse_DisableSensor(slot);
        }
        else
        {
            /* Disable complex sensor */
            snsType &= ~Mouse_COMPLEX_SS_FLAG;
            index = &Mouse_indexTable[snsType];
            snsNumber = Mouse_maskTable[slot];

            for (j=0; j < snsNumber; j++)
            {
                Mouse_DisableSensor(index[j]);
            }
        }
    }
#endif  /* Mouse_IS_COMPLEX_SCANSLOTS */


/*******************************************************************************
* Function Name: Mouse_EnableSensor
********************************************************************************
*
* Summary:
*  Configures the selected sensor to measure during the next measurement cycle.
*  The corresponding pins are set to Analog High-Z mode and connected to the
*  Analog Mux Bus. This also enables the comparator function.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Mouse_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Mouse_pinShiftTbl[] - used to store position of pin that
*  configured as sensor in port.
*  Mouse_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Mouse_PrtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_EnableSensor(uint32 sensor)
{
    uint8  pinModeShift;
    uint8  pinHSIOMShift;
    uint8 interruptState;
    uint32 newRegisterValue;
    uint32 port;

    port = (uint32) Mouse_portTable[sensor];
    pinModeShift = Mouse_pinShiftTbl[sensor]  * Mouse_PC_PIN_CFG_SIZE;
    pinHSIOMShift = Mouse_pinShiftTbl[sensor] * Mouse_HSIOM_PIN_CFG_SIZE;

    interruptState = CyEnterCriticalSection();

    newRegisterValue = *Mouse_prtSelTbl[port];
    newRegisterValue &= ~(Mouse_CSD_HSIOM_MASK << pinHSIOMShift);
    newRegisterValue |= (uint32)((uint32)Mouse_CSD_SENSE_PORT_MODE << pinHSIOMShift);

    *Mouse_prtCfgTbl[port] &= (uint32)~((uint32)Mouse_CSD_PIN_MODE_MASK << pinModeShift);
    *Mouse_prtSelTbl[port] = newRegisterValue;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Mouse_DisableSensor
********************************************************************************
*
* Summary:
*  Disables the selected sensor. The corresponding pin is disconnected from the
*  Analog Mux Bus and connected to GND, High_Z or Shield electrode.
*
* Parameters:
*  sensor:  Sensor number
*
* Return:
*  None
*
* Global Variables:
*  Mouse_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Mouse_pinShiftTbl[] - used to store position of pin that
*  configured as a sensor in the port.
*  Mouse_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Mouse_PrtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_DisableSensor(uint32 sensor)
{
    uint8 interruptState;
    uint32 newRegisterValue;

    uint32 port = (uint32) Mouse_portTable[sensor];
    uint8  pinHSIOMShift = Mouse_pinShiftTbl[sensor] * Mouse_HSIOM_PIN_CFG_SIZE;
    uint8  pinModeShift = Mouse_pinShiftTbl[sensor]  * Mouse_PC_PIN_CFG_SIZE;

    uint32 inactiveConnect = Mouse_SNS_HIZANALOG_CONNECT;
    uint32 sensorState = Mouse_unscannedSnsDriveMode[sensor];

    *Mouse_prtSelTbl[port] &= ~(Mouse_CSD_HSIOM_MASK << pinHSIOMShift);

    #if(Mouse_IS_SHIELD_ENABLE != 0)
    if(sensorState != (uint32)Mouse__SHIELD)
    {
    #else
        /* Connected to Ground if shield is disabled */
        if(sensorState == (uint32)Mouse__SHIELD)
        {
            sensorState = (uint32)Mouse__GROUND;
        }
    #endif /* (Mouse_IS_SHIELD_ENABLE) */
        if(sensorState != (uint32)Mouse__HIZ_ANALOG)
        {
            /* Connected to Ground */
            inactiveConnect = (uint32)Mouse_SNS_GROUND_CONNECT;
        }

        interruptState = CyEnterCriticalSection();

        /* Set drive mode */
        newRegisterValue = *Mouse_prtCfgTbl[port];
        newRegisterValue &= ~(Mouse_CSD_PIN_MODE_MASK << pinModeShift);
        newRegisterValue |=  (uint32)(inactiveConnect << pinModeShift);
        *Mouse_prtCfgTbl[port] =  newRegisterValue;

        CyExitCriticalSection(interruptState);

        *Mouse_prtDRTbl[port]  &=  (uint32)~(uint32)((uint32)1u << Mouse_pinShiftTbl[sensor]);
    #if(Mouse_IS_SHIELD_ENABLE != 0)
    }
    else
    {
        /* Connect to Shield */
        *Mouse_prtSelTbl[port] |= (Mouse_CSD_SHIELD_PORT_MODE << pinHSIOMShift);
    }
    #endif /* (Mouse_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Mouse_SetDriveModeAllPins
********************************************************************************
*
* Summary:
* This API sets the drive mode of port pins used by the Mouse
* component (sensors, guard, shield, shield tank and Cmod) to drive the
* mode specified by the argument.
*
* Parameters:
*  driveMode:  Drive mode definition.
*
* Return:
*  None
*
* Global Variables:
*  Mouse_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Mouse_pinShiftTbl[] - used to store position of pin that
*  configured as a sensor in the port.
*
* Side Effects:
*  This API shall be called only after Mouse component is stopped.
*
*******************************************************************************/
void Mouse_SetDriveModeAllPins(uint32 driveMode)
{
    uint8 curSensor;
    #if (Mouse_IS_COMPLEX_SCANSLOTS)
        uint8 snsNumber;
        uint8 snsType;
    #endif  /* Mouse_IS_COMPLEX_SCANSLOTS */
    uint32 prtNumberTmp;
    uint32 pinNumberTmp;

    for(curSensor = 0u; curSensor < Mouse_TOTAL_SENSOR_COUNT; curSensor++)
    {
        #if (Mouse_IS_COMPLEX_SCANSLOTS)
            /* Read sensor type: single or complex */
            snsType = Mouse_portTable[curSensor];

            /* Check if sensor is complex */
            if ((snsType & Mouse_COMPLEX_SS_FLAG) == 0u)
            {
                /* The sensor is not complex */
                snsNumber = curSensor;
            }
            else
            {
                /* Get dedicated sensor ID of the complex sensor */
                snsType &= ~Mouse_COMPLEX_SS_FLAG;
                snsNumber = Mouse_indexTable[snsType];
            }

            prtNumberTmp = Mouse_portTable[snsNumber];
            pinNumberTmp = Mouse_pinShiftTbl[snsNumber];
        #else
            prtNumberTmp = Mouse_portTable[curSensor];
            pinNumberTmp = Mouse_pinShiftTbl[curSensor];
        #endif  /* Mouse_IS_COMPLEX_SCANSLOTS */

        Mouse_SetPinDriveMode(driveMode, pinNumberTmp, prtNumberTmp);
    }

    Mouse_SetPinDriveMode(driveMode, (uint32)Mouse_CMOD_PIN_NUMBER, (uint32)Mouse_CMOD_PRT_NUMBER);

    #if(0u != Mouse_CSH_TANK_ENABLE)
        Mouse_SetPinDriveMode(driveMode, (uint32)Mouse_CTANK_PIN_NUMBER, (uint32)Mouse_CTANK_PRT_NUMBER);
    #endif /* (0u != Mouse_CSH_TANK_ENABLE) */

    #if(0u != Mouse_IS_SHIELD_ENABLE)
        Mouse_SetPinDriveMode(driveMode, (uint32)Mouse_SHIELD_PIN_NUMBER, (uint32)Mouse_SHIELD_PORT_NUMBER);
    #endif /* (0u != Mouse_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Mouse_RestoreDriveModeAllPins
********************************************************************************
*
* Summary:
*  This API restores the drive for all the Mouse port pins to the original
*  state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*
* Side Effects:
*  This API is called only after the Mouse component is stopped.
*
*******************************************************************************/
void Mouse_RestoreDriveModeAllPins(void)
{
    uint32 newRegisterValue;

    Mouse_SetDriveModeAllPins(CY_SYS_PINS_DM_ALG_HIZ);

    newRegisterValue = Mouse_CMOD_CONNECTION_REG;
    newRegisterValue &= ~(Mouse_CSD_CMOD_CONNECTION_MASK);
    newRegisterValue |= Mouse_CSD_CMOD_TO_AMUXBUS_A;
    Mouse_CMOD_CONNECTION_REG = newRegisterValue;

    #if(0u != Mouse_CSH_TANK_ENABLE)
        newRegisterValue = Mouse_CTANK_CONNECTION_REG;
        newRegisterValue &= ~(Mouse_CSD_CTANK_CONNECTION_MASK);
        newRegisterValue |= Mouse_CSD_CTANK_TO_AMUXBUS_B;
        Mouse_CTANK_CONNECTION_REG = newRegisterValue;
    #endif/* (0u != Mouse_CSH_TANK_ENABLE) */

    #if(0u != Mouse_IS_SHIELD_ENABLE)
        newRegisterValue = *Mouse_prtSelTbl[Mouse_SHIELD_PORT_NUMBER];
        newRegisterValue &= ~(Mouse_CSD_HSIOM_MASK << ((uint32)Mouse_SHIELD_PIN_NUMBER * Mouse_HSIOM_PIN_CFG_SIZE));
        newRegisterValue |= (Mouse_CSD_SHIELD_PORT_MODE << ((uint32)Mouse_SHIELD_PIN_NUMBER * Mouse_HSIOM_PIN_CFG_SIZE));
        *Mouse_prtSelTbl[Mouse_SHIELD_PORT_NUMBER] = newRegisterValue;
    #endif /* (0u != Mouse_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Mouse_SetPinDriveMode
********************************************************************************
*
* Summary:
* This API sets the drive mode for the appropriate port/pin.
*
* Parameters:
*  driveMode:  Drive mode definition.
*  portNumber: contains port number (0, 1, 2).
*  pinNumber: contains pin number (0, 1, 2, ... , 7).
*
* Return:
*  None
*
* Global Variables:
*  Mouse_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Mouse_prtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  This API is called only after Mouse component is stopped.
*
*******************************************************************************/
void Mouse_SetPinDriveMode(uint32 driveMode, uint32 pinNumber, uint32 portNumber)
{
    uint32  pinModeShift;
    uint32 newRegisterValue;

    pinModeShift  = pinNumber * Mouse_PC_PIN_CFG_SIZE;

    newRegisterValue = *Mouse_prtCfgTbl[portNumber];
    newRegisterValue &= ~(Mouse_CSD_PIN_MODE_MASK << pinModeShift);
    newRegisterValue |=  (uint32)((uint32)driveMode << pinModeShift);
    *Mouse_prtCfgTbl[portNumber] =  newRegisterValue;
}


/*******************************************************************************
* Function Name: Mouse_PreScan
********************************************************************************
*
* Summary:
*  Sets required settings, enables a sensor, removes Vref from AMUX and starts the
*  scanning process of the sensor.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
* Mouse_widgetNumber[] - This array contains numbers of widgets for each sensor.
* Mouse_widgetResolution[] - Contains the widget resolution.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_PreScan(uint32 sensor)
{
    uint8 widget;
    uint8 interruptState;
    uint32 newRegValue;
    uint32 counterResolution;

    #if(Mouse_PRS_OPTIONS == Mouse__PRS_AUTO)
        uint8 senseClkDivMath;
        uint8 sampleClkDivMath;
    #endif /* (Mouse_PRS_OPTIONS == Mouse__PRS_AUTO) */

    #if ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) &&\
         (0 != Mouse_IS_OVERSAMPLING_EN))
        uint32 oversamplingFactor;
    #endif /* ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) &&\
               (0 != Mouse_IS_OVERSAMPLING_EN)) */

    /* Define widget sensor belongs to */
    widget = Mouse_widgetNumber[sensor];

    /* Recalculate Counter Resolution to MSB 16 bits */
    counterResolution = Mouse_widgetResolution[widget];

    #if ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) &&\
         (0 != Mouse_IS_OVERSAMPLING_EN))
        oversamplingFactor = Mouse_GetBitValue(Mouse_scanSpeedTbl, sensor);

        if(counterResolution < Mouse_RESOLUTION_16_BITS)
        {
            counterResolution <<= oversamplingFactor;
            counterResolution |= (uint32)(Mouse_RESOLUTION_8_BITS);
        }
    #endif /* ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) &&\
               (0 != Mouse_IS_OVERSAMPLING_EN)) */

    #if (0u != Mouse_IS_M0S8PERI_BLOCK)
        Mouse_SenseClk_Stop();
        Mouse_SampleClk_Stop();
    #endif /* (0u != Mouse_IS_M0S8PERI_BLOCK) */

    #if (0u != Mouse_MULTIPLE_FREQUENCY_SET)
        Mouse_SampleClk_SetDividerValue((uint16)Mouse_sampleClkDividerVal[sensor]);
        Mouse_SenseClk_SetDividerValue((uint16)Mouse_senseClkDividerVal[sensor]);
    #else
        Mouse_SampleClk_SetDividerValue((uint16)Mouse_sampleClkDividerVal);
        Mouse_SenseClk_SetDividerValue((uint16)Mouse_senseClkDividerVal);
    #endif /* (0u != Mouse_MULTIPLE_FREQUENCY_SET) */

    #if (0u != Mouse_IS_M0S8PERI_BLOCK)
        Mouse_SampleClk_Start();
        Mouse_SenseClk_StartEx(Mouse_SampleClk__DIV_ID);

        #if(Mouse_PRS_OPTIONS == Mouse__PRS_NONE)
            CyIntDisable(Mouse_ISR_NUMBER);
            Mouse_CSD_CNT_REG = Mouse_ONE_CYCLE;
            while(0u != (Mouse_CSD_CNT_REG & Mouse_RESOLUTION_16_BITS))
            {
            /* Wait until scanning is complete */
            }
            Mouse_CSD_INTR_REG = 1u;
            CyIntClearPending(Mouse_ISR_NUMBER);
            CyIntEnable(Mouse_ISR_NUMBER);
        #endif /* Mouse_PRS_OPTIONS == Mouse__PRS_NONE */
    #endif /* (0u != Mouse_IS_M0S8PERI_BLOCK) */

#if(Mouse_PRS_OPTIONS != Mouse__PRS_NONE)

    #if(Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)
        if(Mouse_prescalersTuningDone != 0u)
        {
    #endif /* (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) */

            CyIntDisable(Mouse_ISR_NUMBER);

            newRegValue = Mouse_CSD_CFG_REG;
            newRegValue |= Mouse_CSD_CFG_PRS_SELECT;

            #if(Mouse_PRS_OPTIONS == Mouse__PRS_AUTO)

                newRegValue &= ~(Mouse_PRS_MODE_MASK);

                #if (0u != Mouse_MULTIPLE_FREQUENCY_SET)
                    senseClkDivMath = Mouse_senseClkDividerVal[sensor];
                    sampleClkDivMath = Mouse_sampleClkDividerVal[sensor];
                #else
                    senseClkDivMath = Mouse_senseClkDividerVal;
                    sampleClkDivMath = Mouse_sampleClkDividerVal;
                #endif /* ( Mouse_MULTIPLE_FREQUENCY_SET) */

                #if(0u == Mouse_IS_M0S8PERI_BLOCK)
                    senseClkDivMath *= sampleClkDivMath;
                #endif /* (0u == Mouse_IS_M0S8PERI_BLOCK) */

                if((senseClkDivMath * Mouse_RESOLUTION_12_BITS) <
                   (sampleClkDivMath * Mouse_widgetResolution[widget]))
                {
                    newRegValue |= Mouse_CSD_PRS_12_BIT;
                }
            #endif /* (Mouse_PRS_OPTIONS == Mouse__PRS_AUTO) */

            Mouse_CSD_CFG_REG = newRegValue;

            CyIntEnable(Mouse_ISR_NUMBER);

    #if(Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)
        }
    #endif /* (Mouse_PRS_OPTIONS != Mouse__PRS_NONE) */

#endif /* (Mouse_PRS_OPTIONS == Mouse__PRS_NONE) */

    /* Set Idac Value */
    CyIntDisable(Mouse_ISR_NUMBER);
    newRegValue = Mouse_CSD_IDAC_REG;

#if (Mouse_IDAC_CNT == 1u)
    newRegValue &= ~(Mouse_CSD_IDAC1_DATA_MASK);
    newRegValue |= Mouse_modulationIDAC[sensor];
#else
    newRegValue &= ~(Mouse_CSD_IDAC1_DATA_MASK | Mouse_CSD_IDAC2_DATA_MASK);
    newRegValue |= (Mouse_modulationIDAC[sensor] |
                            (uint32)((uint32)Mouse_compensationIDAC[sensor] <<
                            Mouse_CSD_IDAC2_DATA_OFFSET));
#endif /* (Mouse_IDAC_CNT == 1u) */

    Mouse_CSD_IDAC_REG = newRegValue;

#if(Mouse_CMOD_PREGARGE_OPTION == Mouse__CAPPRIOBUF)
    newRegValue = Mouse_CMOD_CONNECTION_REG;
    newRegValue &= ~(Mouse_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Mouse_CSD_CMOD_TO_AMUXBUS_A;
    Mouse_CMOD_CONNECTION_REG = newRegValue;

    newRegValue = Mouse_CMOD_PORT_PC_REG;
    newRegValue &= ~(Mouse_CSD_CMOD_PC_MASK);
    newRegValue |= Mouse_CMOD_PC_HIGH_Z_MODE;
    Mouse_CMOD_PORT_PC_REG = newRegValue;
#endif /* (Mouse_CMOD_PREGARGE_OPTION == Mouse__CAPPRIOBUF) */

    /* Disconnect Vref Buffer from AMUX */
    newRegValue = Mouse_CSD_CFG_REG;
    newRegValue &= ~(Mouse_PRECHARGE_CONFIG_MASK);
    newRegValue |= Mouse_CTANK_PRECHARGE_CONFIG;

    CyIntEnable(Mouse_ISR_NUMBER);

    /* Enable Sensor */
    Mouse_EnableScanSlot(sensor);

    interruptState = CyEnterCriticalSection();
    Mouse_CSD_CFG_REG = newRegValue;

    /* `#START Mouse_PreSettlingDelay_Debug` */

    /* `#END` */

#ifdef Mouse_PRE_SCAN_PRE_SETTLING_DELAY_DEBUG_CALLBACK
    Mouse_PreScan_Pre_SettlingDelay_Debug_Callback();
#endif /* Mouse_PRE_SCAN_PRE_SETTLING_DELAY_DEBUG_CALLBACK */

    CyDelayCycles(Mouse_GLITCH_ELIMINATION_CYCLES);

    /* `#START Mouse_PreScan_Debug` */

    /* `#END` */

#ifdef Mouse_PRE_SCAN_DEBUG_CALLBACK
    Mouse_PreScan_Debug_Callback();
#endif /* Mouse_PRE_SCAN_DEBUG_CALLBACK */

    Mouse_CSD_CNT_REG = counterResolution;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Mouse_PostScan
********************************************************************************
*
* Summary:
*  Stores the results of measurement in the Mouse_sensorRaw[] array,
*  sets the scanning sensor in the non-sampling state, turns off Idac(Current Source IDAC),
*  disconnects the IDAC(Sink mode), and applies Vref on AMUX.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Mouse_sensorRaw[] - used to store sensors raw data.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_PostScan(uint32 sensor)
{
    uint32 newRegValue;
#if ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) &&\
     (0 != Mouse_IS_OVERSAMPLING_EN))
    uint32 oversamplingFactor;
    uint32 widget;
#endif /* ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) &&\
           (0 != Mouse_IS_OVERSAMPLING_EN)) */

    /* `#START Mouse_PostScan_Debug` */

    /* `#END` */

#ifdef Mouse_POST_SCAN_DEBUG_CALLBACK
    Mouse_PostScan_Debug_Callback();
#endif /* Mouse_POST_SCAN_DEBUG_Callback */

    /* Read SlotResult from Raw Counter */
    Mouse_sensorRaw[sensor]  = (uint16)Mouse_CSD_CNT_REG;

#if ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) &&\
     (0 != Mouse_IS_OVERSAMPLING_EN))
    widget = Mouse_widgetNumber[sensor];
    if(Mouse_widgetResolution[widget] < Mouse_RESOLUTION_16_BITS)
    {
        oversamplingFactor = Mouse_GetBitValue(Mouse_scanSpeedTbl, sensor);
        Mouse_sensorRaw[sensor] >>= oversamplingFactor;
    }
#endif /* ((Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) &&\
           (0 != Mouse_IS_OVERSAMPLING_EN)) */

    /* Disable Sensor */
    Mouse_DisableScanSlot(sensor);

    CyIntDisable(Mouse_ISR_NUMBER);

#if(Mouse_CMOD_PREGARGE_OPTION == Mouse__CAPPRIOBUF)
    newRegValue = Mouse_CMOD_CONNECTION_REG;
    newRegValue &= ~(Mouse_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Mouse_CSD_CMOD_TO_AMUXBUS_B;
    Mouse_CMOD_CONNECTION_REG = newRegValue;

    newRegValue = Mouse_CMOD_PORT_PC_REG;
    newRegValue &= ~(Mouse_CSD_CMOD_PC_MASK);
    newRegValue |= Mouse_CMOD_PC_STRONG_MODE;
    Mouse_CMOD_PORT_PC_REG = newRegValue;

    newRegValue = Mouse_CMOD_PORT_DR_REG;
    newRegValue &= ~(Mouse_CMOD_DR_MASK);
    newRegValue |= Mouse_CMOD_DR_CONFIG;
    Mouse_CMOD_PORT_DR_REG = newRegValue;
#endif /* (Mouse_CMOD_PREGARGE_OPTION == Mouse__CAPPRIOBUF) */

    /* Connect Vref Buffer to AMUX bus  */
    newRegValue = Mouse_CSD_CFG_REG;
    newRegValue &= ~(Mouse_PRECHARGE_CONFIG_MASK);
    newRegValue |= Mouse_CMOD_PRECHARGE_CONFIG;
    Mouse_CSD_CFG_REG = newRegValue;

    /* Set Idac Value = 0 */
#if (Mouse_IDAC_CNT == 1u)
    Mouse_CSD_IDAC_REG &= ~(Mouse_CSD_IDAC1_DATA_MASK);
#else
    Mouse_CSD_IDAC_REG &= ~(Mouse_CSD_IDAC1_DATA_MASK | Mouse_CSD_IDAC2_DATA_MASK);
#endif /* (Mouse_IDAC_CNT == 1u) */

    CyIntEnable(Mouse_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Mouse_EnableShieldElectrode
********************************************************************************
*
* Summary:
*  This API enables or disables the shield electrode on a specified port pin.
*
* Parameters:
*  portNumber: contains the shield electrode port number (0, 1, 2).
*  pinNumber: contains the shield electrode pin number (0, 1, 2, ... , 7).
*
* Return:
*  None
*
* Global Variables:
*  Mouse_prtCfgTbl[] - Contains pointers to the port config registers for each port.
*  Mouse_prtSelTbl[] - Contains pointers to the HSIOM registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_EnableShieldElectrode(uint32 pinNumber, uint32 portNumber)
{
    uint32 newRegValue;

    *Mouse_prtCfgTbl[portNumber] &= ~(Mouse_CSD_PIN_MODE_MASK << (pinNumber * Mouse_PC_PIN_CFG_SIZE));
    newRegValue = *Mouse_prtSelTbl[portNumber];
    newRegValue &= ~(Mouse_CSD_HSIOM_MASK << (pinNumber * Mouse_HSIOM_PIN_CFG_SIZE));
    newRegValue |= (Mouse_CSD_SHIELD_PORT_MODE << (pinNumber * Mouse_HSIOM_PIN_CFG_SIZE));
    *Mouse_prtSelTbl[portNumber] = newRegValue;
}


/*******************************************************************************
* Function Name: Mouse_SetShieldDelay
********************************************************************************
*
* Summary:
*  This API sets a shield delay.
*
* Parameters:
*  delay:  shield delay value:
*                               0 - no delay
*                               1 - 1 cycle delay
*                               2 - 2 cycles delay
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
void Mouse_SetShieldDelay(uint32 delay)
{
    uint32 newRegValue;

    delay &= 0x03u;

    newRegValue = Mouse_CSD_CFG_REG;
    newRegValue &= (uint32)(~(uint32)Mouse_SHIELD_DELAY);
    newRegValue |= (delay << Mouse_CSD_CFG_SHIELD_DELAY_POS);
    Mouse_CSD_CFG_REG = newRegValue;
}


/*******************************************************************************
* Function Name: Mouse_ReadCurrentScanningSensor
********************************************************************************
*
* Summary:
*  This API returns scanning sensor number when sensor scan is in progress.
*  When sensor scan is completed the API returns
*  $INSTANCE_NAME`_NOT_SENSOR (0xFFFFFFFF) (when no sensor is scanned).
*
* Parameters:
*   None.
*
* Return:
*  Returns Sensor number if sensor is being scanned and
*  $INSTANCE_NAME`_NOT_SENSOR (0xFFFFFFFF) if scanning is complete.
*
* Global Variables:
*  Mouse_sensorIndex - the sensor number is being scanned.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Mouse_ReadCurrentScanningSensor(void)
{
    return ((uint32)(( 0u != (Mouse_csdStatusVar & Mouse_SW_STS_BUSY)) ?
                     Mouse_sensorIndex : Mouse_NOT_SENSOR));
}


/*******************************************************************************
* Function Name: Mouse_GetBitValue
********************************************************************************
*
* Summary:
*  The API returns a bit status of the bit in the table array which contains the masks.
*
* Parameters:
*  table[] - array with bit masks.
*  position - position of bit in the table[] array.
*
* Return:
*  0 - bit is not set; 1 - bit is set.
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Mouse_GetBitValue(const uint8 table[], uint32 position)
{
    uint32 offset;
    uint8 enMask;

    /* position is divided by 8 to calculate the element of the
       table[] array that contains the enable bit
       for an appropriate sensor.
    */
    offset = (position >> 3u);

    /* The enMask calculation for the appropriate sensor. Operation (position & 0x07u)
       intends to calculate the enable bit offset for the 8-bit element of the
       table[] array.
    */
    enMask = 0x01u << (position & 0x07u);

    return ((0u !=(table[offset] & enMask)) ? 1Lu : 0Lu);
}


/*******************************************************************************
* Function Name: Mouse_SetBitValue
********************************************************************************
*
* Summary:
*  The API sets a bit status of the bit in the table array which contains masks.
*
* Parameters:
*  table[] - array with bit masks.
*  position - position of bit in the table[] array.
*  value: 0 - bit is not set; 1 - bit is set.
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
void Mouse_SetBitValue(uint8 table[], uint32 position, uint32 value)
{
    uint32 offset;
    uint8 enMask;

    /* position is divided by 8 to calculate the element of the
       table[] array that contains the enable bit
       for an appropriate sensor.
    */
    offset = (position >> 3u);

    /* The enMask calculation for the appropriate sensor. Operation (position & 0x07u)
       intends to calculate the enable bit offset for the 8-bit element of the
       table[] array.
    */
    enMask = 0x01u << (position & 0x07u);

    if(0u != value)
    {
        table[offset] |= enMask;
    }
    else
    {
        table[offset] &= (uint8)~(enMask);
    }
}



/*******************************************************************************
* Function Name: Mouse_GetSenseClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the sense clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be
*  from 0 to (Mouse_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the sense clock divider of the sensor.
*
* Global Variables:
*  Mouse_senseClkDividerVal[] - stores the sense clock divider values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Mouse_GetSenseClkDivider(uint32 sensor)
{
    #if(0u != Mouse_MULTIPLE_FREQUENCY_SET)
        return((uint32)Mouse_senseClkDividerVal[sensor]);
    #else
        return((uint32)Mouse_senseClkDividerVal);
    #endif /* (0u != Mouse_MULTIPLE_FREQUENCY_SET) */
}

#if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Mouse_SetSenseClkDivider
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the sense clock divider for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  senseClk: represents the sense clock value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Mouse_senseClkDividerVal[] - stores the sense clock divider values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Mouse_SetSenseClkDivider(uint32 sensor, uint32 senseClk)
    {
        #if(0u != Mouse_MULTIPLE_FREQUENCY_SET)
            Mouse_senseClkDividerVal[sensor] = (uint8)senseClk;
        #else
            Mouse_senseClkDividerVal = (uint8)senseClk;
        #endif /* (0u != Mouse_MULTIPLE_FREQUENCY_SET) */
    }
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */


/*******************************************************************************
* Function Name: Mouse_GetModulatorClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the modulator sample clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The value of index can be
*  from 0 to (Mouse_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulator sample clock divider for the  sensor.
*
* Global Variables:
*  Mouse_sampleClkDividerVal[] - stores the modulator sample divider values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Mouse_GetModulatorClkDivider(uint32 sensor)
{
    #if(0u != Mouse_MULTIPLE_FREQUENCY_SET)
        return((uint32)Mouse_sampleClkDividerVal[sensor]);
    #else
        return((uint32)Mouse_sampleClkDividerVal);
    #endif /* (0u != Mouse_MULTIPLE_FREQUENCY_SET) */
}

#if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Mouse_SetModulatorClkDivider
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the modulator sample clock divider for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  modulatorClk: represents the modulator sample clock value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Mouse_sampleClkDividerVal[] - stores the modulator sample divider values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Mouse_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk)
    {
        #if(0u != Mouse_MULTIPLE_FREQUENCY_SET)
            Mouse_sampleClkDividerVal[sensor] = (uint8)modulatorClk;
        #else
            Mouse_sampleClkDividerVal = (uint8)modulatorClk;
        #endif /* (0u != Mouse_MULTIPLE_FREQUENCY_SET) */
    }
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */

/*******************************************************************************
* Function Name: Mouse_GetModulationIDAC
********************************************************************************
*
* Summary:
*  This API returns a value of the modulation IDAC for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be
*  from 0 to (Mouse_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulation IDAC of the sensor.
*
* Global Variables:
*  Mouse_modulationIDAC[] - stores modulation IDAC values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Mouse_GetModulationIDAC(uint32 sensor)
{
        return((uint32)Mouse_modulationIDAC[sensor]);
}

#if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Mouse_SetModulationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the modulation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  compIdacValue: represents the modulation IDAC data register value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Mouse_modulationIDAC[] - array with modulation IDAC values
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Mouse_SetModulationIDAC(uint32 sensor, uint32 modIdacValue)
    {
        Mouse_modulationIDAC[sensor] = (uint8)modIdacValue;
    }
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */


#if(Mouse_IDAC_CNT > 1u)
    /*******************************************************************************
    * Function Name: Mouse_GetCompensationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API returns a value of the compensation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor: sensor index. The index value can be
    *  from 0 to (Mouse_TOTAL_SENSOR_COUNT-1).
    *
    * Return:
    *  This API returns the compensation IDAC of the sensor.
    *
    * Global Variables:
    *  Mouse_compensationIDAC[] - stores the compensation IDAC values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    uint32 Mouse_GetCompensationIDAC(uint32 sensor)
    {
        return((uint32)Mouse_compensationIDAC[sensor]);
    }
#endif /* (Mouse_IDAC_CNT > 1u) */


#if((Mouse_TUNING_METHOD != Mouse__TUNING_NONE) && (Mouse_IDAC_CNT > 1u))
    /*******************************************************************************
    * Function Name: Mouse_SetCompensationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of compensation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  compIdacValue: represents the compensation IDAC data register value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Mouse_compensationIDAC[] - an array with compensation IDAC values
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Mouse_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue)
    {
        Mouse_compensationIDAC[sensor] = (uint8)compIdacValue;
    }
#endif /* ((Mouse_TUNING_METHOD != Mouse__TUNING_NONE) && (Mouse_IDAC_CNT > 1u)) */

/*******************************************************************************
* Function Name: Mouse_GetIDACRange
********************************************************************************
*
* Summary:
*  This API returns a value that indicates the IDAC range used by the
*  component to scan sensors. The IDAC range is common for all the sensors.
*
* Parameters:
*  None
*
* Return:
*  This API Returns a value that indicates the IDAC range:
*      0 - IDAC range set to 4x
*      1 - IDAC range set to 8x
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Mouse_GetIDACRange(void)
{
    return((0u != (Mouse_CSD_IDAC_REG & Mouse_CSD_IDAC1_RANGE_8X)) ? 1uL : 0uL);
}

#if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Mouse_SetIDACRange
    ********************************************************************************
    *
    * Summary:
    *  This API sets the IDAC range to the 4x (1.2uA/bit) or 8x (2.4uA/bit) mode.
    *  The IDAC range is common for all the sensors and common for the modulation and
    *  compensation IDACs.
    *
    * Parameters:
    *  iDacRange:  represents value for IDAC range
    *  0 -  IDAC range set to 4x (1.2uA/bit)
    *  1 or >1 - IDAC range set to 8x (2.4uA/bit)
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  None
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Mouse_SetIDACRange(uint32 iDacRange)
    {
        if(iDacRange != 0u)
        {
            /*  IDAC range = 8x (2.4uA/bit) */
            Mouse_CSD_IDAC_REG |= (Mouse_CSD_IDAC1_RANGE_8X |\
                                              Mouse_CSD_IDAC2_RANGE_8X);
        }
        else
        {
            /*  IDAC range = 4x (1.2uA/bit) */
            /*  IDAC range = 8x (2.4uA/bit) */
            Mouse_CSD_IDAC_REG &= ~(Mouse_CSD_IDAC1_RANGE_8X |\
                                               Mouse_CSD_IDAC2_RANGE_8X);
        }
    }
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */


#if((0u != Mouse_AUTOCALIBRATION_ENABLE) || (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO))
    /*******************************************************************************
    * Function Name: Mouse_GetSensorRaw
    ********************************************************************************
    *
    * Summary:
    *  The API updates and gets uprated raw data from the sensor.
    *
    * Parameters:
    *  sensor - Sensor number.
    *
    * Return:
    *  Returns the current raw data value for a defined sensor number.
    *
    * Global Variables:
    *  None
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    uint16  Mouse_GetSensorRaw(uint32 sensor)
    {
        uint32 curSample;
        uint32 avgVal = 0u;

        for(curSample = 0u; curSample < Mouse_AVG_SAMPLES_NUM; curSample++)
        {

            Mouse_ScanSensor((uint32)sensor);
            while(Mouse_IsBusy() == 1u)
            {
                /* Wait while sensor is busy */
            }
            avgVal += Mouse_ReadSensorRaw((uint32)sensor);
        }

        return((uint16)(avgVal / Mouse_AVG_SAMPLES_NUM));
    }
#endif /* ((0u != Mouse_AUTOCALIBRATION_ENABLE) && (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)) */


#if((0u != Mouse_AUTOCALIBRATION_ENABLE) && (Mouse_TUNING_METHOD != Mouse__TUNING_AUTO))
    /*******************************************************************************
    * Function Name: Mouse_CalibrateSensor
    ********************************************************************************
    *
    * Summary:
    *  Computes the one sensor IDAC value, which provides the raw signal on
    *  a specified level, with a specified prescaler, speed, and resolution.
    *
    * Parameters:
    *  sensor - Sensor Number.
    *
    *  rawLevel -  Raw data level which should be reached during the calibration
    *              procedure.
    *
    *  idacLevelsTbl - Pointer to the modulatorIdac data register configuration table.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Mouse_CalibrateSensor(uint32 sensor, uint32 rawLevel, uint8 idacLevelsTbl[])
    {
        uint8 mask;
        uint16 rawData;

        rawData = 0u;

        mask = 0x80u;
        /* Init IDAC for null Channel */
        idacLevelsTbl[sensor] = 0x00u;

        do
        {
            /* Set bit for null Channel */
            idacLevelsTbl[sensor] |= mask;

            /* Scan null Channel and get Rawdata */
            rawData = Mouse_GetSensorRaw(sensor);

            /* Decrease IDAC until Rawdata reaches rawLevel */
            if(rawData < rawLevel)
            {
                /* Reset bit for null Channel  */
                idacLevelsTbl[sensor] &= (uint8)~mask;
            }

            mask >>= 1u;
        }
        while(mask > 0u);

    }
#endif /* ((0u != Mouse_AUTOCALIBRATION_ENABLE) && (Mouse_TUNING_METHOD != Mouse__TUNING_AUTO)) */


/*******************************************************************************
* Function Name: Mouse_SetUnscannedSensorState
********************************************************************************
*
* Summary:
*  This API sets a state for un-scanned sensors.
*  It is possible to set the state to Ground, High-Z, or the shield electrode.
*  The un-scanned sensor can be connected to a shield electrode only if the shield is
*  enabled. If the shield is disabled and this API is called with the parameter which
*  indicates the shield state, the un-scanned sensor will be connected to Ground.
*
* Parameters:
*  sensor - Sensor Number.
*  sensorState: This parameter indicates un-scanned sensor state:
*
*     Mouse__GROUND 0
*     Mouse__HIZ_ANALOG 1
*     Mouse__SHIELD 2
*
* Return:
*  None.
*
* Global Variables:
*  Mouse_unscannedSnsDriveMode[] - used to store the state for
*  un-scanned sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_SetUnscannedSensorState(uint32 sensor, uint32 sensorState)
{
    #if(Mouse_IS_COMPLEX_SCANSLOTS)
        uint8 snsType;
        uint8 curSensor;
        uint16 snsNumber;

        /* Read sensor type: single or complex */
        snsType = Mouse_portTable[sensor];

        /* Check if sensor is complex */
        if(0u != (snsType & Mouse_COMPLEX_SS_FLAG))
        {
            snsType &= (uint8)~Mouse_COMPLEX_SS_FLAG;
            snsNumber = (uint16)Mouse_maskTable[sensor];

            for (curSensor=0u; curSensor < snsNumber; curSensor++)
            {
                sensor = Mouse_indexTable[snsType + curSensor];
                Mouse_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
            }
        }
        else
        {
            Mouse_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
        }
    #else
        Mouse_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
    #endif /* (Mouse_IS_COMPLEX_SCANSLOTS) */
}

#if(0u != Mouse_TOTAL_CENTROIDS_COUNT)
/*******************************************************************************
* Function Name: Mouse_NormalizeWidgets
********************************************************************************
*
* Summary:
*  This API aligns all the parameters of the widget to the maximum parameter.
*
* Parameters:
*  uint32 widgetsNum: Number of widgets.
*  uint8 *dataPrt: pointer to array with widget parameters.
*
* Return:
*  None
*
* Global Variables:
*  Mouse_numberOfSensors[] - Number of sensors in the widget.
*  Mouse_rawDataIndex[currentWidget] - Contains the  1st sensor
*  position in the widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_NormalizeWidgets(uint32 widgetsNum, uint8 dataPtr[])
{
    uint32 currentWidget;
    uint32 currentSensor;
    uint32 lastSensor;
    uint32 sensorsPerWidget;
    uint32 maxPerWidget;

    for(currentWidget = 0u; currentWidget < widgetsNum; currentWidget++)
    {
        maxPerWidget = 0u;

        sensorsPerWidget = Mouse_numberOfSensors[currentWidget];
        currentSensor = Mouse_rawDataIndex[currentWidget];
        lastSensor = currentSensor + sensorsPerWidget;

        while(currentSensor < lastSensor)
        {
            if(maxPerWidget < dataPtr[currentSensor])
            {
                maxPerWidget = dataPtr[currentSensor];
            }
            currentSensor++;
        }

        currentSensor = Mouse_rawDataIndex[currentWidget];

        while(currentSensor < lastSensor)
        {
            dataPtr[currentSensor] = (uint8)maxPerWidget;
            currentSensor++;
        }
    }
}
#endif /* (0u != Mouse_TOTAL_CENTROIDS_COUNT) */

/* [] END OF FILE */
