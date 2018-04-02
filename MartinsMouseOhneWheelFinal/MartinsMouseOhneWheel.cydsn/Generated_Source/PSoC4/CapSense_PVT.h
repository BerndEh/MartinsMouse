/*******************************************************************************
* File Name: Mouse_PVT.h
* Version 2.60
*
* Description:
*  This file provides constants and structure declarations for the
*  Mouse CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2014-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_Mouse_CSD_PVT_Mouse_H)
#define CY_Mouse_CSD_PVT_Mouse_H

#include "Mouse.h"

#if(0u != Mouse_CSHL_API_GENERATE)
    #include "Mouse_CSHL.h"
#endif /* (0u != Mouse_CSHL_API_GENERATE) */

#if(Mouse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
    #include "Mouse_GESTURE.h"
#endif /* (Mouse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */

typedef struct
{
    #if (Mouse_IS_COMPLEX_SCANSLOTS)
    uint8 sensor;
    #endif  /* Mouse_IS_COMPLEX_SCANSLOTS */
    uint8 pinNumber;
    uint8 portNumber;
    uint8 idacValue;
    uint16 senseClkDiv;
    uint16 sampleClkDiv;
    uint8 avgSamplesNum;
    uint16 calibrationCount;
    uint32 scanResolution;
}Mouse_CP_MEASURE_CONFIG;

typedef struct
{
    uint8 *idac1LevelsTbl;
    uint8 *idac2LevelsTbl;
    uint8 *sensetivityTbl;
    uint8 *chargeDivTbl;
    uint8 *modDivTbl;
    const uint8 *widgetNubmersTbl;
    uint32 *resolutionsTbl;
    uint8 *noiseEnvelopeTbl;
    uint8 *runningDifferenceTbl;
    uint8 *signRegisterTbl;
    uint16 *sampleMinTbl;
    uint16 *sampleMaxTbl;
    uint16 *previousSampleTbl;
    uint8 *kValueTbl;
    uint8 *scanSpeedTbl;
    const uint8 *prescalerTable;
    uint8 const *rawDataIndex;
    uint8 const *numberOfSensors;
    uint16 (*ptrGetRaw)(uint32 sensor);
    void (*ptrPreScan)(uint32 sensor);
    uint16 (*ptrReadSensorRaw)(uint32 sensor);
    uint32 (*ptrGetBitValue)(const uint8 table[], uint32 position);
    void (*ptrSetBitValue)(uint8 table[], uint32 position, uint32 value);
}Mouse_CONFIG_TYPE_POINTERS_P4_v2_60;

typedef struct
{
    uint8 totalScanslotsNum;
    uint8 totalWidgetsNum;
    uint8 totalSensorsNum;
    uint16 sensetivitySeed;
    uint16 hfclkFreq_MHz;
    Mouse_CONFIG_TYPE_POINTERS_P4_v2_60 const * pointers;
}Mouse_CONFIG_TYPE_P4_v2_60;

#define Mouse_AVG_SAMPLES_NUM        (1u)


/***************************************
*        Function Prototypes
***************************************/
void Mouse_PreScan(uint32 sensor);
void Mouse_PostScan(uint32 sensor);

void Mouse_EnableSensor(uint32 sensor);
void Mouse_DisableSensor(uint32 sensor);

#if (Mouse_IS_COMPLEX_SCANSLOTS)
    void Mouse_EnableScanSlot(uint32 slot);
    void Mouse_DisableScanSlot(uint32 slot);
#else
    #define Mouse_EnableScanSlot(slot)   Mouse_EnableSensor(slot)
    #define Mouse_DisableScanSlot(slot)  Mouse_DisableSensor(slot)
#endif  /* End Mouse_IS_COMPLEX_SCANSLOTS */

void Mouse_EnableShieldElectrode(uint32 pinNumber, uint32 portNumber);

uint32 Mouse_GetBitValue(const uint8 table[], uint32 position);
void Mouse_SetBitValue(uint8 table[], uint32 position, uint32 value);

/* Find next sensor for One Channel design */
uint8 Mouse_FindNextSensor(uint8 snsIndex);

void Mouse_SetPinDriveMode(uint32 driveMode, uint32 pinNumber, uint32 portNumber);
void Mouse_CalibrateSensor(uint32 sensor, uint32 rawLevel, uint8 idacLevelsTbl[]);
uint16  Mouse_GetSensorRaw(uint32 sensor);

void Mouse_DisableBaselineIDAC(void);
void Mouse_EnableBaselineIDAC(const Mouse_CONFIG_TYPE_P4_v2_60 *config);

uint32 Mouse_GetLowCp(Mouse_CP_MEASURE_CONFIG *config);
uint32 Mouse_GetHighCp(const Mouse_CP_MEASURE_CONFIG *config);
uint32 Mouse_CalibrateIDAC(Mouse_CP_MEASURE_CONFIG *config);
uint16 Mouse_ScanSensorCp(const Mouse_CP_MEASURE_CONFIG *config);

void Mouse_ConnectElectrode(uint32 pinNumber, uint32 portNumber);
void Mouse_DisconnectElectrode(uint32 pinNumber, uint32 portNumber);

void TunePrescalers_P4_v2_60(void *config);
void TuneSensitivity_P4_v2_60(void *config);
void MeasureNoiseEnvelope_P4_v2_60(void *config, const uint8 sensorNumber, const uint16 *rawData);
void CalculateThresholds_P4_v2_60(void *config, const uint8 sensorNumber, const uint8 widget, uint8 *fingerThres, uint8 *noiseThres);
void CalibrateSensors_P4_v2_60(void *config, uint16 rawLevel);
void CalibrateOneDAC_P4_v2_60(void *config, uint16 rawLevel, uint32 curSensor, uint8 *idacLevelsTbl);


#endif /* CY_Mouse_CSD_PVT_Mouse_H */
