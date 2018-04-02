/*******************************************************************************
* File Name: Wheel_InA.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Wheel_InA.h"

static Wheel_InA_BACKUP_STRUCT  Wheel_InA_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Wheel_InA_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Wheel_InA_SUT.c usage_Wheel_InA_Sleep_Wakeup
*******************************************************************************/
void Wheel_InA_Sleep(void)
{
    #if defined(Wheel_InA__PC)
        Wheel_InA_backup.pcState = Wheel_InA_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Wheel_InA_backup.usbState = Wheel_InA_CR1_REG;
            Wheel_InA_USB_POWER_REG |= Wheel_InA_USBIO_ENTER_SLEEP;
            Wheel_InA_CR1_REG &= Wheel_InA_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Wheel_InA__SIO)
        Wheel_InA_backup.sioState = Wheel_InA_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Wheel_InA_SIO_REG &= (uint32)(~Wheel_InA_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Wheel_InA_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Wheel_InA_Sleep() for an example usage.
*******************************************************************************/
void Wheel_InA_Wakeup(void)
{
    #if defined(Wheel_InA__PC)
        Wheel_InA_PC = Wheel_InA_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Wheel_InA_USB_POWER_REG &= Wheel_InA_USBIO_EXIT_SLEEP_PH1;
            Wheel_InA_CR1_REG = Wheel_InA_backup.usbState;
            Wheel_InA_USB_POWER_REG &= Wheel_InA_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Wheel_InA__SIO)
        Wheel_InA_SIO_REG = Wheel_InA_backup.sioState;
    #endif
}


/* [] END OF FILE */
