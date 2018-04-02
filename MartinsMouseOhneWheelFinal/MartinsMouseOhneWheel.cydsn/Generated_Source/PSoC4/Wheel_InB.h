/*******************************************************************************
* File Name: Wheel_InB.h  
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

#if !defined(CY_PINS_Wheel_InB_H) /* Pins Wheel_InB_H */
#define CY_PINS_Wheel_InB_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Wheel_InB_aliases.h"


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
} Wheel_InB_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Wheel_InB_Read(void);
void    Wheel_InB_Write(uint8 value);
uint8   Wheel_InB_ReadDataReg(void);
#if defined(Wheel_InB__PC) || (CY_PSOC4_4200L) 
    void    Wheel_InB_SetDriveMode(uint8 mode);
#endif
void    Wheel_InB_SetInterruptMode(uint16 position, uint16 mode);
uint8   Wheel_InB_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Wheel_InB_Sleep(void); 
void Wheel_InB_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Wheel_InB__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Wheel_InB_DRIVE_MODE_BITS        (3)
    #define Wheel_InB_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Wheel_InB_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Wheel_InB_SetDriveMode() function.
         *  @{
         */
        #define Wheel_InB_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Wheel_InB_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Wheel_InB_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Wheel_InB_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Wheel_InB_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Wheel_InB_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Wheel_InB_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Wheel_InB_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Wheel_InB_MASK               Wheel_InB__MASK
#define Wheel_InB_SHIFT              Wheel_InB__SHIFT
#define Wheel_InB_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Wheel_InB_SetInterruptMode() function.
     *  @{
     */
        #define Wheel_InB_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Wheel_InB_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Wheel_InB_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Wheel_InB_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Wheel_InB__SIO)
    #define Wheel_InB_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Wheel_InB__PC) && (CY_PSOC4_4200L)
    #define Wheel_InB_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Wheel_InB_USBIO_DISABLE              ((uint32)(~Wheel_InB_USBIO_ENABLE))
    #define Wheel_InB_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Wheel_InB_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Wheel_InB_USBIO_ENTER_SLEEP          ((uint32)((1u << Wheel_InB_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Wheel_InB_USBIO_SUSPEND_DEL_SHIFT)))
    #define Wheel_InB_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Wheel_InB_USBIO_SUSPEND_SHIFT)))
    #define Wheel_InB_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Wheel_InB_USBIO_SUSPEND_DEL_SHIFT)))
    #define Wheel_InB_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Wheel_InB__PC)
    /* Port Configuration */
    #define Wheel_InB_PC                 (* (reg32 *) Wheel_InB__PC)
#endif
/* Pin State */
#define Wheel_InB_PS                     (* (reg32 *) Wheel_InB__PS)
/* Data Register */
#define Wheel_InB_DR                     (* (reg32 *) Wheel_InB__DR)
/* Input Buffer Disable Override */
#define Wheel_InB_INP_DIS                (* (reg32 *) Wheel_InB__PC2)

/* Interrupt configuration Registers */
#define Wheel_InB_INTCFG                 (* (reg32 *) Wheel_InB__INTCFG)
#define Wheel_InB_INTSTAT                (* (reg32 *) Wheel_InB__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Wheel_InB_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Wheel_InB__SIO)
    #define Wheel_InB_SIO_REG            (* (reg32 *) Wheel_InB__SIO)
#endif /* (Wheel_InB__SIO_CFG) */

/* USBIO registers */
#if !defined(Wheel_InB__PC) && (CY_PSOC4_4200L)
    #define Wheel_InB_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Wheel_InB_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Wheel_InB_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Wheel_InB_DRIVE_MODE_SHIFT       (0x00u)
#define Wheel_InB_DRIVE_MODE_MASK        (0x07u << Wheel_InB_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Wheel_InB_H */


/* [] END OF FILE */
