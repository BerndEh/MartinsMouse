/*******************************************************************************
* File Name: Left_Button.h  
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

#if !defined(CY_PINS_Left_Button_H) /* Pins Left_Button_H */
#define CY_PINS_Left_Button_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Left_Button_aliases.h"


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
} Left_Button_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Left_Button_Read(void);
void    Left_Button_Write(uint8 value);
uint8   Left_Button_ReadDataReg(void);
#if defined(Left_Button__PC) || (CY_PSOC4_4200L) 
    void    Left_Button_SetDriveMode(uint8 mode);
#endif
void    Left_Button_SetInterruptMode(uint16 position, uint16 mode);
uint8   Left_Button_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Left_Button_Sleep(void); 
void Left_Button_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Left_Button__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Left_Button_DRIVE_MODE_BITS        (3)
    #define Left_Button_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Left_Button_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Left_Button_SetDriveMode() function.
         *  @{
         */
        #define Left_Button_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Left_Button_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Left_Button_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Left_Button_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Left_Button_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Left_Button_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Left_Button_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Left_Button_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Left_Button_MASK               Left_Button__MASK
#define Left_Button_SHIFT              Left_Button__SHIFT
#define Left_Button_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Left_Button_SetInterruptMode() function.
     *  @{
     */
        #define Left_Button_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Left_Button_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Left_Button_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Left_Button_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Left_Button__SIO)
    #define Left_Button_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Left_Button__PC) && (CY_PSOC4_4200L)
    #define Left_Button_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Left_Button_USBIO_DISABLE              ((uint32)(~Left_Button_USBIO_ENABLE))
    #define Left_Button_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Left_Button_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Left_Button_USBIO_ENTER_SLEEP          ((uint32)((1u << Left_Button_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Left_Button_USBIO_SUSPEND_DEL_SHIFT)))
    #define Left_Button_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Left_Button_USBIO_SUSPEND_SHIFT)))
    #define Left_Button_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Left_Button_USBIO_SUSPEND_DEL_SHIFT)))
    #define Left_Button_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Left_Button__PC)
    /* Port Configuration */
    #define Left_Button_PC                 (* (reg32 *) Left_Button__PC)
#endif
/* Pin State */
#define Left_Button_PS                     (* (reg32 *) Left_Button__PS)
/* Data Register */
#define Left_Button_DR                     (* (reg32 *) Left_Button__DR)
/* Input Buffer Disable Override */
#define Left_Button_INP_DIS                (* (reg32 *) Left_Button__PC2)

/* Interrupt configuration Registers */
#define Left_Button_INTCFG                 (* (reg32 *) Left_Button__INTCFG)
#define Left_Button_INTSTAT                (* (reg32 *) Left_Button__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Left_Button_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Left_Button__SIO)
    #define Left_Button_SIO_REG            (* (reg32 *) Left_Button__SIO)
#endif /* (Left_Button__SIO_CFG) */

/* USBIO registers */
#if !defined(Left_Button__PC) && (CY_PSOC4_4200L)
    #define Left_Button_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Left_Button_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Left_Button_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Left_Button_DRIVE_MODE_SHIFT       (0x00u)
#define Left_Button_DRIVE_MODE_MASK        (0x07u << Left_Button_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Left_Button_H */


/* [] END OF FILE */
