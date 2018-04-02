/*******************************************************************************
* File Name: Mouse.h
* Version 2.60
*
* Description:
*  This file provides constants and parameter values for the Mouse CSD
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

#if !defined(CY_Mouse_CSD_Mouse_H)
#define CY_Mouse_CSD_Mouse_H

#include "cytypes.h"
#include "cyfitter.h"
#include "core_cm0_psoc4.h"
#include "CyLib.h"

#include "Mouse_IP.h"
#include "Mouse_SenseClk.h"
#include "Mouse_SampleClk.h"

/* Constants set by Customizer */
#define Mouse_TOTAL_SENSOR_COUNT            (5u)
#define Mouse_TOTAL_SCANSLOT_COUNT          (5u)
#define Mouse_INDEX_TABLE_SIZE              (0u)

/* Define Sensors */
#define Mouse_SENSOR_LINEARSLIDER0_E0__LS    (0u)
#define Mouse_SENSOR_LINEARSLIDER0_E1__LS    (1u)
#define Mouse_SENSOR_LINEARSLIDER0_E2__LS    (2u)
#define Mouse_SENSOR_LINEARSLIDER0_E3__LS    (3u)
#define Mouse_SENSOR_LINEARSLIDER0_E4__LS    (4u)

#define Mouse_TOTAL_SENSOR_MASK (((Mouse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)

/* Interrupt handler */
CY_ISR_PROTO(Mouse_ISR);

/* ISR Number and Priority to work with CyLib functions */
#define Mouse_ISR_NUMBER        (Mouse_ISR__INTC_NUMBER)
#define Mouse_ISR_PRIORITY      (Mouse_ISR__INTC_PRIOR_NUM)

/***************************************
*   Condition compilation parameters
***************************************/

#define Mouse_CONNECT_INACTIVE_SNS       (0u)
#define Mouse_IS_COMPLEX_SCANSLOTS       (0u)
#define Mouse_COMPLEX_SCANSLOTS_NUM      (0u)
#define Mouse_DEDICATED_SENSORS_NUM      (0u)
#define Mouse_PORT_PIN_CONFIG_TBL_ZISE   (5u)

#define Mouse_IDAC_CNT                   (2u)
#define Mouse_IDAC1_POLARITY             (0u)
#define Mouse_IDAC1_RANGE_VALUE          (0u)
#define Mouse_IDAC2_RANGE_VALUE          (0u)

#define Mouse_MULTIPLE_FREQUENCY_SET     (1u)
#define Mouse_FILTER_ENABLE              (0u)
#define Mouse_PRS_OPTIONS                (0u)

#define Mouse_WATER_PROOF                (0u)

#define Mouse_TUNING_METHOD              (2u)
#define Mouse_TUNER_API_GENERATE         (0u)

#define Mouse_CSHL_API_GENERATE          (1u)

#define Mouse_CMOD_PREGARGE_OPTION       (1u)
#define Mouse_CSH_TANK_PREGARGE_OPTION   (1u)
#define Mouse_IS_SHIELD_ENABLE           (0u)
#define Mouse_CSH_TANK_ENABLE            (0u)

#define Mouse_SHIELD_DELAY               (0u)
#define Mouse_AUTOCALIBRATION_ENABLE     (1u)

#define Mouse_IS_OVERSAMPLING_EN         (1u)

#define Mouse_CSD_4B_PWM_MODE            (0u)
#define Mouse_CSD_4B_PWM_COUNT           (0u)

/**************************************
*    Enumerated Types and Parameters
**************************************/

/* Current source definitions */
#define Mouse__IDAC_SOURCE 0
#define Mouse__IDAC_SINK 1

#define Mouse__IDAC_4X 0
#define Mouse__IDAC_8X 1

#define Mouse__PRS_NONE 0
#define Mouse__PRS_8BITS 1
#define Mouse__PRS_12BITS 2
#define Mouse__PRS_AUTO 3

/* Connection of inactive sensors definitions */
#define Mouse__GROUND 0
#define Mouse__HIZ_ANALOG 1
#define Mouse__SHIELD 2

/* Precharge options definitions */
#define Mouse__CAPPRIOBUF 0
#define Mouse__CAPPRVREF 1

/* Method of tuning */
#define Mouse__TUNING_NONE 0
#define Mouse__TUNING_MANUAL 1
#define Mouse__TUNING_AUTO 2

/* Dead band PWM modulator options */
#define Mouse__PWM_OFF 0
#define Mouse__PWM_HIGH 2
#define Mouse__PWM_LOW 3


#define Mouse_DELAY_EXTRACYCLES_NUM          (9u)
#define Mouse_GLITCH_ELIMINATION_TIMEOUT     (0u)
#define Mouse_GLITCH_ELIMINATION_CYCLES_CALC (Mouse_GLITCH_ELIMINATION_TIMEOUT * CYDEV_BCLK__SYSCLK__MHZ)

#if(Mouse_GLITCH_ELIMINATION_CYCLES_CALC >= Mouse_DELAY_EXTRACYCLES_NUM)
    #define Mouse_GLITCH_ELIMINATION_CYCLES (Mouse_GLITCH_ELIMINATION_CYCLES_CALC -\
                                                        Mouse_DELAY_EXTRACYCLES_NUM)
#else
    #define Mouse_GLITCH_ELIMINATION_CYCLES (Mouse_GLITCH_ELIMINATION_CYCLES_CALC)
#endif /* (Mouse_GLITCH_ELIMINATION_CYCLES_CALC >= Mouse_DELAY_EXTRACYCLES_NUM) */

#define Mouse_INITIAL_CLK_DIVIDER            (CYDEV_BCLK__HFCLK__KHZ / CYDEV_BCLK__SYSCLK__KHZ)


/* Structure to save registers before going to sleep */
typedef struct
{
    uint8 enableState;
} Mouse_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/
void Mouse_Init(void);
void Mouse_CsdHwBlockInit(void);
void Mouse_Enable(void);
void Mouse_Start(void);
void Mouse_Stop(void);
void Mouse_SaveConfig(void);
void Mouse_Sleep(void);
void Mouse_RestoreConfig(void);
void Mouse_Wakeup(void);
uint32 Mouse_IsBusy(void);
void Mouse_ScanSensor(uint32 sensor);
void Mouse_ScanWidget(uint32 widget);
void Mouse_ScanEnabledWidgets(void);
uint16 Mouse_ReadSensorRaw(uint32 sensor);
void Mouse_WriteSensorRaw(uint32 sensor, uint16 value);
void Mouse_ClearSensors(void);
void Mouse_SetShieldDelay(uint32 delay);
uint32 Mouse_ReadCurrentScanningSensor(void);

uint32 Mouse_GetScanResolution(uint32 widget);
uint32 Mouse_GetSenseClkDivider(uint32 sensor);
uint32 Mouse_GetModulatorClkDivider(uint32 sensor);
uint32 Mouse_GetModulationIDAC(uint32 sensor);
uint32 Mouse_GetCompensationIDAC(uint32 sensor);
uint32 Mouse_GetIDACRange(void);

#if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    void Mouse_SetScanResolution(uint32 widget, uint32 resolution);
    void Mouse_SetSenseClkDivider(uint32 sensor, uint32 senseClk);
    void Mouse_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk);
    void Mouse_SetModulationIDAC(uint32 sensor, uint32 modIdacValue);
    void Mouse_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue);
    void Mouse_SetIDACRange(uint32 iDacRange);
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE) */


void Mouse_SetDriveModeAllPins(uint32 driveMode);
void Mouse_RestoreDriveModeAllPins(void);
void Mouse_SetUnscannedSensorState(uint32 sensor, uint32 sensorState);
void Mouse_NormalizeWidgets(uint32 widgetsNum, uint8 dataPtr[]);

/***************************************
*           API Constants
***************************************/

/* PWM Resolution */
#define Mouse_CNT_MSB_RESOLUTION_8_BITS  (0x00u)
#define Mouse_CNT_MSB_RESOLUTION_9_BITS  (0x01u)
#define Mouse_CNT_MSB_RESOLUTION_10_BITS (0x03u)
#define Mouse_CNT_MSB_RESOLUTION_11_BITS (0x07u)
#define Mouse_CNT_MSB_RESOLUTION_12_BITS (0x0Fu)
#define Mouse_CNT_MSB_RESOLUTION_13_BITS (0x1Fu)
#define Mouse_CNT_MSB_RESOLUTION_14_BITS (0x3Fu)
#define Mouse_CNT_MSB_RESOLUTION_15_BITS (0x7Fu)
#define Mouse_CNT_MSB_RESOLUTION_16_BITS (0xFFu)

#define Mouse_ONE_CYCLE                  (0x00010000Lu)

#define Mouse_WINDOW_APPEND              (0xFFFF0000Lu)
#define Mouse_RESOLUTION_6_BITS          (0x003F0000Lu)
#define Mouse_RESOLUTION_7_BITS          (0x007F0000Lu)
#define Mouse_RESOLUTION_8_BITS          (0x00FF0000Lu)
#define Mouse_RESOLUTION_9_BITS          (0x01FF0000Lu)
#define Mouse_RESOLUTION_10_BITS         (0x03FF0000Lu)
#define Mouse_RESOLUTION_11_BITS         (0x07FF0000Lu)
#define Mouse_RESOLUTION_12_BITS         (0x0FFF0000Lu)
#define Mouse_RESOLUTION_13_BITS         (0x1FFF0000Lu)
#define Mouse_RESOLUTION_14_BITS         (0x3FFF0000Lu)
#define Mouse_RESOLUTION_15_BITS         (0x7FFF0000Lu)
#define Mouse_RESOLUTION_16_BITS         (0xFFFF0000Lu)

#define Mouse_RESOLUTION_OFFSET          (16u)
#define Mouse_MSB_RESOLUTION_OFFSET      (22u)
#define Mouse_RESOLUTIONS_TBL_SIZE       (1u)

/* Software Status Register Bit Masks */
#define Mouse_SW_STS_BUSY                (0x01u)
/* Software Control Register Bit Masks */
#define Mouse_SW_CTRL_SINGLE_SCAN        (0x80u)

/* Software Control Register Bit Masks for scanning one widget */
#define Mouse_SW_CTRL_WIDGET_SCAN        (0x40u)

/* Flag for complex scan slot */
#define Mouse_COMPLEX_SS_FLAG            (0x80u)

/* Flag for scanning is complete */
#define Mouse_NOT_SENSOR                 (0xFFFFFFFFLu)

/* Number of bits for each pin in PC register. */
#define Mouse_PC_PIN_CFG_SIZE            (0x03u)

/* Number of bits for each pin in HSIOM register. */
#define Mouse_HSIOM_PIN_CFG_SIZE         (0x04u)

#define Mouse_CFG_REG_TBL_SIZE           (7u)

/***************************************
*             Registers
***************************************/

#define Mouse_CSD_ID_REG             (*(reg32 *)  Mouse_CSD_FFB__CSD_ID)
#define Mouse_CSD_ID_PTR             ( (reg32 *)  Mouse_CSD_FFB__CSD_ID)

#define Mouse_CSD_CFG_REG            (*(reg32 *)  Mouse_CSD_FFB__CSD_CONFIG)
#define Mouse_CSD_CFG_PTR            ( (reg32 *)  Mouse_CSD_FFB__CSD_CONFIG)

#define Mouse_CSD_IDAC_REG           (*(reg32 *)  Mouse_IDAC1_cy_psoc4_idac__CSD_IDAC)
#define Mouse_CSD_IDAC_PTR           ( (reg32 *)  Mouse_IDAC1_cy_psoc4_idac__CSD_IDAC)

#define Mouse_CSD_CNT_REG            (*(reg32 *) Mouse_CSD_FFB__CSD_COUNTER)
#define Mouse_CSD_CNT_PTR            ( (reg32 *) Mouse_CSD_FFB__CSD_COUNTER)

#define Mouse_CSD_STAT_REG           (*(reg32 *) Mouse_CSD_FFB__CSD_STATUS)
#define Mouse_CSD_STAT_PTR           ( (reg32 *) Mouse_CSD_FFB__CSD_STATUS)

#define Mouse_CSD_INTR_REG           (*(reg32 *) Mouse_CSD_FFB__CSD_INTR)
#define Mouse_CSD_INTR_PTR           ( (reg32 *) Mouse_CSD_FFB__CSD_INTR)

#define Mouse_CSD_INTR_SET_REG       (*(reg32 *) Mouse_CSD_FFB__CSD_INTR_SET)
#define Mouse_CSD_INTR_SET_PTR       ( (reg32 *) Mouse_CSD_FFB__CSD_INTR_SET)

#define Mouse_CSD_4B_PWM_REG         (*(reg32 *) Mouse_CSD_FFB__CSD_PWM)
#define Mouse_CSD_4B_PWM_PTR         ( (reg32 *) Mouse_CSD_FFB__CSD_PWM)

#define Mouse_CSD_TRIM1_REG          (*(reg32 *) Mouse_IDAC1_cy_psoc4_idac__CSD_TRIM1)
#define Mouse_CSD_TRIM1_PTR          ( (reg32 *) Mouse_IDAC1_cy_psoc4_idac__CSD_TRIM1)

#define Mouse_CSD_TRIM2_REG          (*(reg32 *) Mouse_IDAC1_cy_psoc4_idac__CSD_TRIM2)
#define Mouse_CSD_TRIM2_PTR          ( (reg32 *) Mouse_IDAC1_cy_psoc4_idac__CSD_TRIM2)

#if (2u == Mouse_M0S8CSD_BLOCK_CNT)
    #if (Mouse_CSD_FFB__CSD_NUMBER != 1u)
        #define Mouse_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
        #define Mouse_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

        #define Mouse_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
        #define Mouse_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
    #else
        #define Mouse_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)
        #define Mouse_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)

        #define Mouse_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
        #define Mouse_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
    #endif /* (Mouse_CSD_FFB__CSD_NUMBER != 1u) */
#else
    #define Mouse_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
    #define Mouse_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

    #define Mouse_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
    #define Mouse_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
#endif /* (2u == Mouse_M0S8CSD_BLOCK_CNT) */

/* Port function select */
#define Mouse_PORT_SEL0_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL0 )
#define Mouse_PORT_SEL0_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL0 )

#define Mouse_PORT_SEL1_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL1 )
#define Mouse_PORT_SEL1_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL1 )

#define Mouse_PORT_SEL2_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL2 )
#define Mouse_PORT_SEL2_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL2 )

#define Mouse_PORT_SEL3_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL3 )
#define Mouse_PORT_SEL3_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL3 )

#define Mouse_PORT_SEL4_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL4 )
#define Mouse_PORT_SEL4_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL4 )

#define Mouse_PORT_SEL5_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL5 )
#define Mouse_PORT_SEL5_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL5 )

#define Mouse_PORT_SEL6_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL6 )
#define Mouse_PORT_SEL6_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL6 )


#define Mouse_PORT0_PC_REG          (*(reg32 *) CYREG_GPIO_PRT0_PC )
#define Mouse_PORT0_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT0_PC )

#define Mouse_PORT1_PC_REG          (*(reg32 *) CYREG_GPIO_PRT1_PC )
#define Mouse_PORT1_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT1_PC )

#define Mouse_PORT2_PC_REG          (*(reg32 *) CYREG_GPIO_PRT2_PC )
#define Mouse_PORT2_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT2_PC )

#define Mouse_PORT3_PC_REG          (*(reg32 *) CYREG_GPIO_PRT3_PC )
#define Mouse_PORT3_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT3_PC )

#define Mouse_PORT4_PC_REG          (*(reg32 *) CYREG_GPIO_PRT4_PC )
#define Mouse_PORT4_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT4_PC )

#define Mouse_PORT5_PC_REG          (*(reg32 *) CYREG_GPIO_PRT5_PC )
#define Mouse_PORT5_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT5_PC )

#define Mouse_PORT6_PC_REG          (*(reg32 *) CYREG_GPIO_PRT6_PC )
#define Mouse_PORT6_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT6_PC )


#define Mouse_PORT0_DR_REG          (*(reg32 *) CYREG_GPIO_PRT0_DR )
#define Mouse_PORT0_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT0_DR )

#define Mouse_PORT1_DR_REG          (*(reg32 *) CYREG_GPIO_PRT1_DR )
#define Mouse_PORT1_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT1_DR )

#define Mouse_PORT2_DR_REG          (*(reg32 *) CYREG_GPIO_PRT2_DR )
#define Mouse_PORT2_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT2_DR )

#define Mouse_PORT3_DR_REG          (*(reg32 *) CYREG_GPIO_PRT3_DR )
#define Mouse_PORT3_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT3_DR )

#define Mouse_PORT4_DR_REG          (*(reg32 *) CYREG_GPIO_PRT4_DR )
#define Mouse_PORT4_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT4_DR )

#define Mouse_PORT5_DR_REG          (*(reg32 *) CYREG_GPIO_PRT5_DR )
#define Mouse_PORT5_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT5_DR )

#define Mouse_PORT6_DR_REG          (*(reg32 *) CYREG_GPIO_PRT6_DR )
#define Mouse_PORT6_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT6_DR )



#define Mouse_CMOD_CONNECTION_REG        (*(reg32 *) Mouse_Cmod__0__HSIOM)
#define Mouse_CMOD_PORT_PC_REG           (*(reg32 *) Mouse_Cmod__0__PC)
#define Mouse_CMOD_PORT_DR_REG           (*(reg32 *) Mouse_Cmod__0__DR)

#define Mouse_CTANK_CONNECTION_REG       (*(reg32 *) Mouse_Csh_tank__0__HSIOM)
#define Mouse_CTANK_PORT_PC_REG          (*(reg32 *) Mouse_Csh_tank__0__PC)
#define Mouse_CTANK_PORT_DR_REG          (*(reg32 *) Mouse_Csh_tank__0__DR)

/***************************************
*       Register Constants
***************************************/

/* Port configuration defines */
#define Mouse_CSD_HSIOM_MASK                 (0x0000000FLu)

#define Mouse_CSD_SENSE_PORT_MODE            (0x00000004Lu)
#define Mouse_CSD_SHIELD_PORT_MODE           (0x00000005Lu)

#define Mouse_AMUXA_CONNECTION_MODE          (0x00000006Lu)
#define Mouse_AMUXB_CONNECTION_MODE          (0x00000007Lu)

#define Mouse_CSD_PIN_MODE_MASK              (0x00000007Lu)
#define Mouse_CSD_PIN_DRIVE_MASK             (0x00000001Lu)

#define Mouse_CSD_PIN_DM_STRONG              (0x00000006Lu)
#define Mouse_CSD_PIN_DM_HIGH_Z              (0x00000000Lu)

#define Mouse_CSD_CMOD_CONNECTION_MASK   (uint32)(Mouse_Cmod__0__HSIOM_MASK)
#define Mouse_CSD_CMOD_TO_AMUXBUS_A      (uint32)(Mouse_AMUXA_CONNECTION_MODE << Mouse_Cmod__0__HSIOM_SHIFT)
#define Mouse_CSD_CMOD_TO_AMUXBUS_B      (uint32)(Mouse_AMUXB_CONNECTION_MODE << Mouse_Cmod__0__HSIOM_SHIFT)

#if(0u != Mouse_CSH_TANK_ENABLE)
    #define Mouse_CSD_CTANK_CONNECTION_MASK  (uint32)(Mouse_Csh_tank__0__HSIOM_MASK)
    #define Mouse_CSD_CTANK_TO_AMUXBUS_A     (uint32)(Mouse_AMUXA_CONNECTION_MODE << Mouse_Csh_tank__0__HSIOM_SHIFT)
    #define Mouse_CSD_CTANK_TO_AMUXBUS_B     (uint32)(Mouse_AMUXB_CONNECTION_MODE << Mouse_Csh_tank__0__HSIOM_SHIFT)
#endif /* (0u != Mouse_CSH_TANK_ENABLE) */

#define  Mouse_CSD_CMOD_PC_MASK          (uint32)(Mouse_CSD_PIN_MODE_MASK <<\
                                                            ((uint32)Mouse_Cmod__0__SHIFT * Mouse_PC_PIN_CFG_SIZE))
#define  Mouse_CMOD_PC_STRONG_MODE       (uint32)(Mouse_CSD_PIN_DM_STRONG <<\
                                                            ((uint32)Mouse_Cmod__0__SHIFT * Mouse_PC_PIN_CFG_SIZE))
#define  Mouse_CMOD_PC_HIGH_Z_MODE       (uint32)(Mouse_CSD_PIN_DM_HIGH_Z <<\
                                                            ((uint32)Mouse_Cmod__0__SHIFT * Mouse_PC_PIN_CFG_SIZE))

#if(0u != Mouse_CSH_TANK_ENABLE)
    #define  Mouse_CSD_CTANK_PC_MASK         (uint32)(Mouse_CSD_PIN_MODE_MASK <<\
                                                                ((uint32)Mouse_Csh_tank__0__SHIFT * Mouse_PC_PIN_CFG_SIZE))
    #define  Mouse_CTANK_PC_STRONG_MODE      (uint32)(Mouse_CSD_PIN_DM_STRONG <<\
                                                                ((uint32)Mouse_Csh_tank__0__SHIFT * Mouse_PC_PIN_CFG_SIZE))
    #define  Mouse_CTANK_PC_HIGH_Z_MODE      (uint32)(Mouse_CSD_PIN_DM_HIGH_Z <<\
                                                                ((uint32)Mouse_Csh_tank__0__SHIFT * Mouse_PC_PIN_CFG_SIZE))
#endif /* (0u != Mouse_CSH_TANK_ENABLE) */

#define  Mouse_CMOD_DR_VSSIO             (uint32)(Mouse_Cmod__0__MASK)
#define  Mouse_CMOD_DR_VDDIO             (0x00000000Lu)
#define  Mouse_CMOD_DR_MASK              (uint32)(Mouse_Cmod__0__MASK)

#if(0u != Mouse_CSH_TANK_ENABLE)
    #define  Mouse_CTANK_DR_VSSIO            (uint32)(Mouse_Csh_tank__0__MASK)
    #define  Mouse_CTANK_DR_VDDIO            (0x00000000Lu)
    #define  Mouse_CTANK_DR_MASK             (uint32)(Mouse_Csh_tank__0__MASK)
#endif /* (0u != Mouse_CSH_TANK_ENABLE) */

#define  Mouse_CMOD_PIN_NUMBER               (Mouse_Cmod__0__SHIFT)
#define  Mouse_CMOD_PRT_NUMBER               (Mouse_Cmod__0__PORT)

#if(0u != Mouse_CSH_TANK_ENABLE)
    #define  Mouse_CTANK_PIN_NUMBER          (Mouse_Csh_tank__0__SHIFT)
    #define  Mouse_CTANK_PRT_NUMBER          (Mouse_Csh_tank__0__PORT)
#endif /* (0u != Mouse_CSH_TANK_ENABLE) */

/* ID register fields position */
#define Mouse_CSD_ID_ID__POS                 (0x00u)
#define Mouse_CSD_ID_REV__POS                (0x10u)

#define Mouse_CSD_ID_ID                      (0xFFLu << Mouse_CSD_ID_ID__POS)
#define Mouse_CSD_ID_REV                     (0xFFLu << Mouse_CSD_ID_REV__POS)

/* CSD_CONFIG register fields position */
#define Mouse_CSD_CFG_DSI_SAMPLE_EN_POS      (0x00u)
#define Mouse_CSD_CFG_SAMPLE_SYNC_POS        (0x01u)
#define Mouse_CSD_CFG_FILTER_EN_POS          (0x03u)
#define Mouse_CSD_CFG_PRS_CLEAR_POS          (0x05u)
#define Mouse_CSD_CFG_PRS_SELECT_POS         (0x06u)
#define Mouse_CSD_CFG_PRS_12_8_POS           (0x07u)
#define Mouse_CSD_CFG_DSI_SENSE_EN_POS       (0x08u)
#define Mouse_CSD_CFG_SHIELD_DELAY_POS       (0x09u)
#define Mouse_CSD_CFG_SENSE_COMP_BW_POS      (0x0Bu)
#define Mouse_CSD_CFG_SENSE_EN_POS           (0x0Cu)
#define Mouse_CSD_CFG_REFBUF_EN_POS          (0x0Du)
#define Mouse_CSD_CFG_CHARGE_IO_POS          (0x0Eu)
#define Mouse_CSD_CFG_COMP_PIN_POS           (0x0Fu)
#define Mouse_CSD_CFG_POLARITY_POS           (0x10u)
#define Mouse_CSD_CFG_MUTUAL_CAP_POS         (0x12u)
#define Mouse_CSD_CFG_SENSE_COMP_EN_POS      (0x13u)
#define Mouse_CSD_CFG_REFBUF_OUTSEL_POS      (0x15u)
#define Mouse_CSD_CFG_SENSE_INSEL_POS        (0x16u)
#define Mouse_CSD_REFBUF_DRV_POS             (0x17u)
#define Mouse_CSD_CFG_DDFTSEL_POS            (0x1Au)
#define Mouse_CSD_CFG_ADFTEN_POS             (0x1Du)
#define Mouse_CSD_CFG_DDFTCOMP_POS           (0x1Eu)
#define Mouse_CSD_CFG_ENABLE_POS             (0x1Fu)

/* CSD_STATUS register fields position */
#define Mouse_CSD_STAT_CSD_CHARGE_POS        (0x00u)
#define Mouse_CSD_STAT_CSD_SENSE_POS         (0x01u)
#define Mouse_CSD_STAT_COMP_OUT_POS          (0x02u)
#define Mouse_CSD_STAT_SAMPLE_POS            (0x03u)

/* CSD_IDAC control register fields position */
#define Mouse_CSD_IDAC1_MODE_MASK_POS        (0x08u)
#define Mouse_CSD_IDAC1_MODE_FIXED_POS       (0x08u)
#define Mouse_CSD_IDAC1_MODE_VARIABLE_POS    (0x08u)
#define Mouse_CSD_IDAC1_MODE_DSI_POS         (0x08u)
#define Mouse_CSD_IDAC1_RANGE_8X_POS         (0x0Au)
#define Mouse_CSD_IDAC1_DATA_MASK_POS        (0x00u)

#define Mouse_CSD_IDAC2_MODE_MASK_POS        (0x18u)
#define Mouse_CSD_IDAC2_MODE_FIXED_POS       (0x18u)
#define Mouse_CSD_IDAC2_MODE_VARIABLE_POS    (0x18u)
#define Mouse_CSD_IDAC2_MODE_DSI_POS         (0x18u)
#define Mouse_CSD_IDAC2_RANGE_8X_POS         (0x1Au)
#define Mouse_CSD_IDAC2_DATA_MASK_POS        (0x10u)

/* CSD_COUNTER register fields position */
#define Mouse_CSD_CNT_COUNTER_POS            (0x00u)
#define Mouse_CSD_CNT_PERIOD_POS             (0x10u)

/* CSD_CONFIRG register definitions */
#define Mouse_CSD_CFG_DSI_SAMPLE_EN          (0x01Lu << Mouse_CSD_CFG_DSI_SAMPLE_EN_POS)
#define Mouse_CSD_CFG_SAMPLE_SYNC            (0x01Lu << Mouse_CSD_CFG_SAMPLE_SYNC_POS)
#define Mouse_CSD_CFG_FILTER_EN              (0x01Lu << Mouse_CSD_CFG_FILTER_EN_POS)
#define Mouse_CSD_CFG_PRS_CLEAR              (0x01Lu << Mouse_CSD_CFG_PRS_CLEAR_POS)
#define Mouse_CSD_CFG_PRS_SELECT             (0x01Lu << Mouse_CSD_CFG_PRS_SELECT_POS)
#define Mouse_CSD_CFG_PRS_12_8               (0x01Lu << Mouse_CSD_CFG_PRS_12_8_POS)
#define Mouse_CSD_CFG_DSI_SENSE_EN           (0x01Lu << Mouse_CSD_CFG_DSI_SENSE_EN_POS)
#define Mouse_CSD_CFG_SHIELD_DELAY_MASK      (0x03Lu << Mouse_CSD_CFG_SHIELD_DELAY_POS)
#define Mouse_CSD_CFG_SENSE_COMP_BW          (0x01Lu << Mouse_CSD_CFG_SENSE_COMP_BW_POS)
#define Mouse_CSD_CFG_SENSE_EN               (0x01Lu << Mouse_CSD_CFG_SENSE_EN_POS)
#define Mouse_CSD_CFG_REFBUF_EN              (0x01Lu << Mouse_CSD_CFG_REFBUF_EN_POS)
#define Mouse_CSD_CFG_CHARGE_IO              (0x01Lu << Mouse_CSD_CFG_CHARGE_IO_POS)
#define Mouse_CSD_CFG_COMP_PIN_CH2           (0x01Lu << Mouse_CSD_CFG_COMP_PIN_POS)
#define Mouse_CSD_CFG_POLARITY_VDDIO         (0x01Lu << Mouse_CSD_CFG_POLARITY_POS)
#define Mouse_CSD_CFG_MUTUAL_CAP             (0x01Lu << Mouse_CSD_CFG_MUTUAL_CAP_POS)
#define Mouse_CSD_CFG_SENSE_COMP_EN          (0x01Lu << Mouse_CSD_CFG_SENSE_COMP_EN_POS)
#define Mouse_CSD_CFG_REFBUF_OUTSEL          (0x01Lu << Mouse_CSD_CFG_REFBUF_OUTSEL_POS)
#define Mouse_CSD_CFG_SENSE_INSEL            (0x01Lu << Mouse_CSD_CFG_SENSE_INSEL_POS)
#define Mouse_CSD_REFBUF_DRV_MASK            (0x03Lu << Mouse_CSD_REFBUF_DRV_POS)
#define Mouse_CSD_REFBUF_DRV_LOW             (0x01Lu << Mouse_CSD_REFBUF_DRV_POS)
#define Mouse_CSD_REFBUF_DRV_MEDIUM          (0x02Lu << Mouse_CSD_REFBUF_DRV_POS)
#define Mouse_CSD_REFBUF_DRV_HIGH            (0x03Lu << Mouse_CSD_REFBUF_DRV_POS)
#define Mouse_CSD_CFG_DDFTSEL_MASK           (0x07Lu << Mouse_CSD_CFG_DDFTSEL_POS)
#define Mouse_CSD_CFG_ADFTEN                 (0x01Lu << Mouse_CSD_CFG_ADFTEN_POS)
#define Mouse_CSD_CFG_DDFTCOMP_MASK          (0x03Lu << Mouse_CSD_CFG_DDFTCOMP_POS)
#define Mouse_CSD_CFG_DDFTCOMP_REFCOMP       (0x01Lu << Mouse_CSD_CFG_DDFTCOMP_POS)
#define Mouse_CSD_CFG_DDFTCOMP_SENSECOMP     (0x02Lu << Mouse_CSD_CFG_DDFTCOMP_POS)
#define Mouse_CSD_CFG_ENABLE                 (0x01Lu << Mouse_CSD_CFG_ENABLE_POS)

/* CSD_STATUS register definitions */
#define Mouse_CSD_STAT_CSD_CHARGE            (0x01Lu << Mouse_CSD_STAT_CSD_CHARGE_POS)
#define Mouse_CSD_STAT_CSD_SENSE             (0x01Lu << Mouse_CSD_STAT_CSD_SENSE_POS)
#define Mouse_CSD_STAT_COMP_OUT              (0x01Lu << Mouse_CSD_STAT_COMP_OUT_POS)
#define Mouse_CSD_STAT_SAMPLE                (0x01Lu << Mouse_CSD_STAT_SAMPLE_POS)

/* CSD_IDAC control register definitions */
#define Mouse_CSD_IDAC1_MODE_MASK            (0x03Lu << Mouse_CSD_IDAC1_MODE_MASK_POS)
#define Mouse_CSD_IDAC1_MODE_FIXED           (0x01Lu << Mouse_CSD_IDAC1_MODE_FIXED_POS)
#define Mouse_CSD_IDAC1_MODE_VARIABLE        (0x02Lu << Mouse_CSD_IDAC1_MODE_VARIABLE_POS)
#define Mouse_CSD_IDAC1_MODE_DSI             (0x03Lu << Mouse_CSD_IDAC1_MODE_DSI_POS)
#define Mouse_CSD_IDAC1_RANGE_8X             (0x01Lu << Mouse_CSD_IDAC1_RANGE_8X_POS)
#define Mouse_CSD_IDAC1_DATA_MASK            (0xFFLu << Mouse_CSD_IDAC1_DATA_MASK_POS)

#define Mouse_CSD_IDAC2_MODE_MASK            (0x03Lu << Mouse_CSD_IDAC2_MODE_MASK_POS)
#define Mouse_CSD_IDAC2_MODE_FIXED           (0x01Lu << Mouse_CSD_IDAC2_MODE_FIXED_POS)
#define Mouse_CSD_IDAC2_MODE_VARIABLE        (0x02Lu << Mouse_CSD_IDAC2_MODE_VARIABLE_POS)
#define Mouse_CSD_IDAC2_MODE_DSI             (0x03Lu << Mouse_CSD_IDAC2_MODE_DSI_POS)
#define Mouse_CSD_IDAC2_RANGE_8X             (0x01Lu << Mouse_CSD_IDAC2_RANGE_8X_POS)
#define Mouse_CSD_IDAC2_DATA_MASK            (0x7FLu << Mouse_CSD_IDAC2_DATA_MASK_POS)

#define Mouse_CSD_IDAC2_DATA_OFFSET          (16u)

#define Mouse_CSD_IDAC1_TRIM_MASK            (0xFFFFFFF0u)
#define Mouse_CSD_IDAC2_TRIM_MASK            (0xFFFFFF0Fu)

#define Mouse_CSFLASH_TRIM_IDAC1_MASK        (0x0Fu)
#define Mouse_CSFLASH_TRIM_IDAC2_MASK        (0xF0)

#define Mouse_CSD_4B_PWM_MODE_OFFSET         (4u)
#define Mouse_CSD_4B_PWM_MODE_DEFAULT        (Mouse_CSD_4B_PWM_MODE << Mouse_CSD_4B_PWM_MODE_OFFSET)

#define Mouse_CSD_4B_PWM_COUNT_MASK          (0x0000000Fu)
#define Mouse_CSD_4B_PWM_MODE_MASK           (0x00000030u)

#define Mouse_CSD_IDAC_PRECHARGE_CONFIG      (Mouse_CSD_IDAC1_MODE_FIXED |\
                                                        Mouse_PRECHARGE_IDAC1_VAL)

/* CSD_COUNTER register definitions */
#define Mouse_CSD_CNT_COUNTER                (0xFFLu << Mouse_CSD_CNT_COUNTER_POS )
#define Mouse_CSD_CNT_PERIOD                 (0xFFLu << Mouse_CSD_CNT_PERIOD_POS)

#define Mouse_CSD_PRS_8_BIT                  (0x00000000u)
#define Mouse_CSD_PRS_12_BIT                 (Mouse_CSD_CFG_PRS_12_8)
#define Mouse_PRS_MODE_MASK                  (Mouse_CSD_CFG_PRS_12_8)

/***************************************
*    Initial Parameter Constants
***************************************/

#if (0u != Mouse_FILTER_ENABLE)
    #define Mouse_DEFAULT_FILTER_STATE       (Mouse_CSD_CFG_FILTER_EN)

#else
    #define Mouse_DEFAULT_FILTER_STATE       (0u)

#endif /* (Mouse_PRS_OPTIONS == Mouse_PRS_8BITS) */

#define Mouse_DEFAULT_CSD_4B_PWM_CONFIG      (Mouse_CSD_4B_PWM_MODE_DEFAULT | Mouse_CSD_4B_PWM_COUNT)

/* Set PRS */
#if (Mouse_PRS_OPTIONS == Mouse__PRS_8BITS)
    #define Mouse_DEFAULT_MODULATION_MODE    Mouse_CSD_CFG_PRS_SELECT

#elif (Mouse_PRS_OPTIONS == Mouse__PRS_12BITS)
    #define Mouse_DEFAULT_MODULATION_MODE    (Mouse_CSD_CFG_PRS_12_8 |\
                                                        Mouse_CSD_CFG_PRS_SELECT)
#else
    #define Mouse_DEFAULT_MODULATION_MODE    (0u)
#endif /* (Mouse_PRS_OPTIONS == Mouse_PRS_8BITS) */

/* Set IDAC ranges */
#if (Mouse_IDAC1_RANGE_VALUE == Mouse__IDAC_8X)
    #define Mouse_DEFAULT_IDAC1_RANGE        Mouse_CSD_IDAC1_RANGE_8X
    #define Mouse_DEFAULT_IDAC2_RANGE        Mouse_CSD_IDAC2_RANGE_8X
#else
    #define Mouse_DEFAULT_IDAC1_RANGE        (0u)
    #define Mouse_DEFAULT_IDAC2_RANGE        (0u)
#endif /* (Mouse_IDAC1_RANGE_VALUE == Mouse_IDAC_RANGE_8X) */

/* Set IDAC polarity */
#if(Mouse_IDAC1_POLARITY == Mouse__IDAC_SINK)

    #define Mouse_DEFAULT_IDAC_POLARITY      Mouse_CSD_CFG_POLARITY_VDDIO
    #define Mouse_CMOD_DR_CONFIG             Mouse_CMOD_DR_VDDIO
    #define Mouse_CTANK_DR_CONFIG            Mouse_CTANK_DR_VDDIO
#else

    #define Mouse_DEFAULT_IDAC_POLARITY      (0u)
    #define Mouse_CMOD_DR_CONFIG             Mouse_CMOD_DR_VSSIO
    #define Mouse_CTANK_DR_CONFIG            Mouse_CTANK_DR_VSSIO
#endif /* (Mouse_IDAC_OPTIONS == Mouse_IDAC_SINK) */

#define Mouse_SNS_GROUND_CONNECT             (6u)
#define Mouse_SNS_HIZANALOG_CONNECT          (0u)

/* Inactive sensors connection configuration */
#if (Mouse_CONNECT_INACTIVE_SNS == Mouse__GROUND)
    #define Mouse_CSD_INACTIVE_CONNECT       (Mouse_SNS_GROUND_CONNECT)
#else
    #define Mouse_CSD_INACTIVE_CONNECT       (Mouse_SNS_HIZANALOG_CONNECT)
#endif /* Mouse_CONNECT_INACTIVE_SNS == Mouse__GROUND */

#if(Mouse_IS_SHIELD_ENABLE)
    #define Mouse_SHIELD_PORT_NUMBER Mouse_Shield__PORT
    #define Mouse_SHIELD_PIN_NUMBER  Mouse_Shield__SHIFT
#endif /* (Mouse_IS_SHIELD_ENABLE) */

/* Defining default CSD configuration according to settings in customizer. */
#define Mouse_DEFAULT_CSD_CONFIG             (Mouse_DEFAULT_FILTER_STATE |\
                                                         Mouse_DEFAULT_MODULATION_MODE |\
                                                         Mouse_CSD_CFG_SENSE_COMP_BW |\
                                                         Mouse_DEFAULT_IDAC_POLARITY |\
                                                         Mouse_CSD_CFG_REFBUF_OUTSEL |\
                                                         Mouse_CSD_REFBUF_DRV_HIGH)

#define Mouse_CSD_CONFIG_MASK                (Mouse_CSD_CFG_FILTER_EN |\
                                                         Mouse_CSD_CFG_PRS_SELECT |\
                                                         Mouse_CSD_CFG_PRS_12_8 |\
                                                         Mouse_CSD_CFG_SENSE_COMP_BW |\
                                                         Mouse_CSD_CFG_REFBUF_EN |\
                                                         Mouse_CSD_CFG_REFBUF_OUTSEL |\
                                                         Mouse_CSD_CFG_POLARITY_VDDIO |\
                                                         Mouse_CSD_CFG_REFBUF_OUTSEL  |\
                                                         Mouse_CSD_REFBUF_DRV_MASK)


/* Defining the default IDACs configuration according to settings in customizer. */
#if (Mouse_IDAC_CNT == 1u)
    #define Mouse_DEFAULT_CSD_IDAC_CONFIG    (Mouse_CSD_IDAC1_MODE_VARIABLE |\
                                                             Mouse_DEFAULT_IDAC1_RANGE)
#else
    #define Mouse_DEFAULT_CSD_IDAC_CONFIG    (Mouse_CSD_IDAC1_MODE_VARIABLE |\
                                                             Mouse_CSD_IDAC2_MODE_FIXED |\
                                                             Mouse_DEFAULT_IDAC1_RANGE |\
                                                             Mouse_DEFAULT_IDAC2_RANGE)
#endif /* (Mouse_IDAC_CNT == 1u) */

/* Defining mask intended for clearing bits related to pre-charging options. */
#define Mouse_PRECHARGE_CONFIG_MASK          (Mouse_CSD_CFG_REFBUF_EN |\
                                                         Mouse_CSD_CFG_CHARGE_IO |\
                                                         Mouse_CSD_CFG_COMP_PIN_CH2  |\
                                                         Mouse_CSD_CFG_REFBUF_OUTSEL)

/* Cmod the precharge mode configuration */
#if(Mouse_CMOD_PREGARGE_OPTION == Mouse__CAPPRVREF)

    #define Mouse_CMOD_PRECHARGE_CONFIG      (Mouse_CSD_CFG_REFBUF_EN |\
                                                         Mouse_CSD_CFG_COMP_PIN_CH2)

#else

    #define Mouse_CMOD_PRECHARGE_CONFIG      (Mouse_CSD_CFG_REFBUF_OUTSEL |\
                                                         Mouse_CSD_CFG_REFBUF_EN |\
                                                         Mouse_CSD_CFG_CHARGE_IO)

#endif /* (Mouse_CMOD_PREGARGE_OPTION == Mouse__CAPPRVREF) */

/* Ctank the precharge mode configuration */
#if(Mouse_CSH_TANK_PREGARGE_OPTION == Mouse__CAPPRVREF)

    #if(0u != Mouse_IS_SHIELD_ENABLE)
        #define  Mouse_CTANK_PRECHARGE_CONFIG    (Mouse_CSD_CFG_REFBUF_EN |\
                                                             Mouse_CSD_CFG_PRS_CLEAR |\
                                                             Mouse_CSD_CFG_REFBUF_OUTSEL)
    #else
        #define  Mouse_CTANK_PRECHARGE_CONFIG    (Mouse_CSD_CFG_REFBUF_OUTSEL |\
                                                             Mouse_CSD_CFG_PRS_CLEAR)
    #endif /* (0u != Mouse_IS_SHIELD_ENABLE) */

#else

    #define  Mouse_CTANK_PRECHARGE_CONFIG    (Mouse_CSD_CFG_REFBUF_OUTSEL |\
                                                         Mouse_CSD_CFG_REFBUF_EN |\
                                                         Mouse_CSD_CFG_CHARGE_IO |\
                                                         Mouse_CSD_CFG_PRS_CLEAR |\
                                                         Mouse_CSD_CFG_COMP_PIN_CH2)
#endif /* (Mouse_CSH_TANK_PREGARGE_OPTION == Mouse__CAPPRIOBUF) */

#define Mouse_MAX_UINT_8                     (0xFFu)
#define Mouse_MAX_UINT_16                    (0xFFFFu)
#define Mouse_MAX_UINT_32                    (0xFFFFFFFFLu)

/***************************************
*     Vars with External Linkage
***************************************/
/* SmartSense functions */
#if (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO)
    extern uint8 Mouse_lowLevelTuningDone;
    extern uint8 Mouse_scanSpeedTbl[((Mouse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
    extern void Mouse_AutoTune(void);
#endif /* (Mouse_TUNING_METHOD == Mouse__TUNING_AUTO) */

#if(Mouse_PRS_OPTIONS != Mouse__PRS_NONE)
    extern uint8 Mouse_prescalersTuningDone;
#endif /* (Mouse_PRS_OPTIONS == Mouse__PRS_NONE) */

/* Global software variables */
extern volatile uint8 Mouse_csdStatusVar;
extern volatile uint8 Mouse_sensorIndex;
extern uint16 Mouse_sensorRaw[Mouse_TOTAL_SENSOR_COUNT];
extern uint8 Mouse_unscannedSnsDriveMode[Mouse_PORT_PIN_CONFIG_TBL_ZISE];
extern uint8 Mouse_sensorEnableMaskBackup[Mouse_TOTAL_SENSOR_MASK];
extern uint8 Mouse_sensorEnableMask[Mouse_TOTAL_SENSOR_MASK];

#if ((Mouse_TUNING_METHOD != Mouse__TUNING_NONE) || (0u != Mouse_AUTOCALIBRATION_ENABLE))
    extern uint8 Mouse_modulationIDAC[Mouse_TOTAL_SENSOR_COUNT];
    extern uint8 Mouse_compensationIDAC[Mouse_TOTAL_SENSOR_COUNT];
#else
    extern const uint8 Mouse_modulationIDAC[Mouse_TOTAL_SENSOR_COUNT];
    extern const uint8 Mouse_compensationIDAC[Mouse_TOTAL_SENSOR_COUNT];
#endif /* ((Mouse_TUNING_METHOD != Mouse__TUNING_NONE) || (0u != Mouse_AUTOCALIBRATION_ENABLE)) */

#if (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)
    extern uint32 Mouse_widgetResolution[Mouse_RESOLUTIONS_TBL_SIZE];
    #if (0u != Mouse_MULTIPLE_FREQUENCY_SET)
        extern uint8 Mouse_senseClkDividerVal[Mouse_TOTAL_SCANSLOT_COUNT];
        extern uint8 Mouse_sampleClkDividerVal[Mouse_TOTAL_SCANSLOT_COUNT];
    #else
        extern uint8 Mouse_senseClkDividerVal;
        extern uint8 Mouse_sampleClkDividerVal;
    #endif /* (0u != Mouse_MULTIPLE_FREQUENCY_SET) */
#else
    extern const uint32 Mouse_widgetResolution[Mouse_RESOLUTIONS_TBL_SIZE];
    #if (0u != Mouse_MULTIPLE_FREQUENCY_SET)
        extern const uint8 Mouse_senseClkDividerVal[Mouse_TOTAL_SCANSLOT_COUNT];
        extern const uint8 Mouse_sampleClkDividerVal[Mouse_TOTAL_SCANSLOT_COUNT];
    #else
        extern const uint8 Mouse_senseClkDividerVal;
        extern const uint8 Mouse_sampleClkDividerVal;
    #endif /* (0u != Mouse_MULTIPLE_FREQUENCY_SET) */
#endif /* (Mouse_TUNING_METHOD != Mouse__TUNING_NONE)  */

extern const uint8 Mouse_widgetNumber[Mouse_TOTAL_SENSOR_COUNT];

extern reg32* const Mouse_prtSelTbl[Mouse_CFG_REG_TBL_SIZE];
extern reg32* const Mouse_prtCfgTbl[Mouse_CFG_REG_TBL_SIZE];
extern reg32* const Mouse_prtDRTbl[Mouse_CFG_REG_TBL_SIZE];
extern reg32 * Mouse_pcTable[Mouse_PORT_PIN_CONFIG_TBL_ZISE];

extern const uint8  Mouse_portTable[Mouse_PORT_PIN_CONFIG_TBL_ZISE];
extern const uint32 Mouse_maskTable[Mouse_PORT_PIN_CONFIG_TBL_ZISE];
extern const uint8  Mouse_pinShiftTbl[Mouse_PORT_PIN_CONFIG_TBL_ZISE];

#if (0u != Mouse_INDEX_TABLE_SIZE)
extern const uint8 CYCODE Mouse_indexTable[Mouse_INDEX_TABLE_SIZE];
#endif /* (0u != Mouse_INDEX_TABLE_SIZE)) */

/***************************************************
*    Obsolete Names (will be removed in future)
***************************************************/
#define  Mouse_PrescalersTuningDone         Mouse_prescalersTuningDone
#define  Mouse_SensorRaw                    Mouse_sensorRaw
#define  Mouse_PRSResolutionTbl             Mouse_prsResolutionTbl
#define  Mouse_SensorEnableMask             Mouse_sensorEnableMask
#define  Mouse_Clk1DividerVal               Mouse_senseClkDividerVal
#define  Mouse_Clk2DividerVal               Mouse_sampleClkDividerVal
#define  Mouse_PrtSelTbl                    Mouse_prtSelTbl
#define  Mouse_PrtCfgTbl                    Mouse_prtCfgTbl
#define  Mouse_PrtDRTbl                     Mouse_prtDRTbl
#define  Mouse_idac1Settings                Mouse_modulationIDAC
#define  Mouse_idac2Settings                Mouse_compensationIDAC

#endif /* CY_Mouse_CSD_Mouse_H */


 /* [] END OF FILE */
