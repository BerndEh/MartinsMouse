/*******************************************************************************
* File Name: Buttons.h  
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

#if !defined(CY_PINS_Buttons_H) /* Pins Buttons_H */
#define CY_PINS_Buttons_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Buttons_aliases.h"


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
} Buttons_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Buttons_Read(void);
void    Buttons_Write(uint8 value);
uint8   Buttons_ReadDataReg(void);
#if defined(Buttons__PC) || (CY_PSOC4_4200L) 
    void    Buttons_SetDriveMode(uint8 mode);
#endif
void    Buttons_SetInterruptMode(uint16 position, uint16 mode);
uint8   Buttons_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Buttons_Sleep(void); 
void Buttons_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Buttons__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Buttons_DRIVE_MODE_BITS        (3)
    #define Buttons_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Buttons_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Buttons_SetDriveMode() function.
         *  @{
         */
        #define Buttons_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Buttons_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Buttons_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Buttons_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Buttons_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Buttons_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Buttons_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Buttons_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Buttons_MASK               Buttons__MASK
#define Buttons_SHIFT              Buttons__SHIFT
#define Buttons_WIDTH              2u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Buttons_SetInterruptMode() function.
     *  @{
     */
        #define Buttons_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Buttons_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Buttons_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Buttons_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Buttons__SIO)
    #define Buttons_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Buttons__PC) && (CY_PSOC4_4200L)
    #define Buttons_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Buttons_USBIO_DISABLE              ((uint32)(~Buttons_USBIO_ENABLE))
    #define Buttons_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Buttons_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Buttons_USBIO_ENTER_SLEEP          ((uint32)((1u << Buttons_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Buttons_USBIO_SUSPEND_DEL_SHIFT)))
    #define Buttons_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Buttons_USBIO_SUSPEND_SHIFT)))
    #define Buttons_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Buttons_USBIO_SUSPEND_DEL_SHIFT)))
    #define Buttons_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Buttons__PC)
    /* Port Configuration */
    #define Buttons_PC                 (* (reg32 *) Buttons__PC)
#endif
/* Pin State */
#define Buttons_PS                     (* (reg32 *) Buttons__PS)
/* Data Register */
#define Buttons_DR                     (* (reg32 *) Buttons__DR)
/* Input Buffer Disable Override */
#define Buttons_INP_DIS                (* (reg32 *) Buttons__PC2)

/* Interrupt configuration Registers */
#define Buttons_INTCFG                 (* (reg32 *) Buttons__INTCFG)
#define Buttons_INTSTAT                (* (reg32 *) Buttons__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Buttons_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Buttons__SIO)
    #define Buttons_SIO_REG            (* (reg32 *) Buttons__SIO)
#endif /* (Buttons__SIO_CFG) */

/* USBIO registers */
#if !defined(Buttons__PC) && (CY_PSOC4_4200L)
    #define Buttons_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Buttons_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Buttons_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Buttons_DRIVE_MODE_SHIFT       (0x00u)
#define Buttons_DRIVE_MODE_MASK        (0x07u << Buttons_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Buttons_H */


/* [] END OF FILE */
