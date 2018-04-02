/*******************************************************************************
* File Name: Mid_Button.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Mid_Button_H) /* Pins Mid_Button_H */
#define CY_PINS_Mid_Button_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Mid_Button_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} Mid_Button_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Mid_Button_Read(void);
void    Mid_Button_Write(uint8 value);
uint8   Mid_Button_ReadDataReg(void);
#if defined(Mid_Button__PC) || (CY_PSOC4_4200L) 
    void    Mid_Button_SetDriveMode(uint8 mode);
#endif
void    Mid_Button_SetInterruptMode(uint16 position, uint16 mode);
uint8   Mid_Button_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Mid_Button_Sleep(void); 
void Mid_Button_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Mid_Button__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Mid_Button_DRIVE_MODE_BITS        (3)
    #define Mid_Button_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Mid_Button_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Mid_Button_SetDriveMode() function.
         *  @{
         */
        #define Mid_Button_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Mid_Button_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Mid_Button_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Mid_Button_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Mid_Button_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Mid_Button_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Mid_Button_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Mid_Button_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Mid_Button_MASK               Mid_Button__MASK
#define Mid_Button_SHIFT              Mid_Button__SHIFT
#define Mid_Button_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Mid_Button_SetInterruptMode() function.
     *  @{
     */
        #define Mid_Button_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Mid_Button_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Mid_Button_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Mid_Button_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Mid_Button__SIO)
    #define Mid_Button_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Mid_Button__PC) && (CY_PSOC4_4200L)
    #define Mid_Button_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Mid_Button_USBIO_DISABLE              ((uint32)(~Mid_Button_USBIO_ENABLE))
    #define Mid_Button_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Mid_Button_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Mid_Button_USBIO_ENTER_SLEEP          ((uint32)((1u << Mid_Button_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Mid_Button_USBIO_SUSPEND_DEL_SHIFT)))
    #define Mid_Button_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Mid_Button_USBIO_SUSPEND_SHIFT)))
    #define Mid_Button_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Mid_Button_USBIO_SUSPEND_DEL_SHIFT)))
    #define Mid_Button_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Mid_Button__PC)
    /* Port Configuration */
    #define Mid_Button_PC                 (* (reg32 *) Mid_Button__PC)
#endif
/* Pin State */
#define Mid_Button_PS                     (* (reg32 *) Mid_Button__PS)
/* Data Register */
#define Mid_Button_DR                     (* (reg32 *) Mid_Button__DR)
/* Input Buffer Disable Override */
#define Mid_Button_INP_DIS                (* (reg32 *) Mid_Button__PC2)

/* Interrupt configuration Registers */
#define Mid_Button_INTCFG                 (* (reg32 *) Mid_Button__INTCFG)
#define Mid_Button_INTSTAT                (* (reg32 *) Mid_Button__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Mid_Button_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Mid_Button__SIO)
    #define Mid_Button_SIO_REG            (* (reg32 *) Mid_Button__SIO)
#endif /* (Mid_Button__SIO_CFG) */

/* USBIO registers */
#if !defined(Mid_Button__PC) && (CY_PSOC4_4200L)
    #define Mid_Button_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Mid_Button_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Mid_Button_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Mid_Button_DRIVE_MODE_SHIFT       (0x00u)
#define Mid_Button_DRIVE_MODE_MASK        (0x07u << Mid_Button_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Mid_Button_H */


/* [] END OF FILE */
