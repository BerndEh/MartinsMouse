/*******************************************************************************
* File Name: PWM_WheelLED_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_WheelLED.h"

static PWM_WheelLED_backupStruct PWM_WheelLED_backup;


/*******************************************************************************
* Function Name: PWM_WheelLED_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_WheelLED_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_WheelLED_SaveConfig(void) 
{

    #if(!PWM_WheelLED_UsingFixedFunction)
        #if(!PWM_WheelLED_PWMModeIsCenterAligned)
            PWM_WheelLED_backup.PWMPeriod = PWM_WheelLED_ReadPeriod();
        #endif /* (!PWM_WheelLED_PWMModeIsCenterAligned) */
        PWM_WheelLED_backup.PWMUdb = PWM_WheelLED_ReadCounter();
        #if (PWM_WheelLED_UseStatus)
            PWM_WheelLED_backup.InterruptMaskValue = PWM_WheelLED_STATUS_MASK;
        #endif /* (PWM_WheelLED_UseStatus) */

        #if(PWM_WheelLED_DeadBandMode == PWM_WheelLED__B_PWM__DBM_256_CLOCKS || \
            PWM_WheelLED_DeadBandMode == PWM_WheelLED__B_PWM__DBM_2_4_CLOCKS)
            PWM_WheelLED_backup.PWMdeadBandValue = PWM_WheelLED_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_WheelLED_KillModeMinTime)
             PWM_WheelLED_backup.PWMKillCounterPeriod = PWM_WheelLED_ReadKillTime();
        #endif /* (PWM_WheelLED_KillModeMinTime) */

        #if(PWM_WheelLED_UseControl)
            PWM_WheelLED_backup.PWMControlRegister = PWM_WheelLED_ReadControlRegister();
        #endif /* (PWM_WheelLED_UseControl) */
    #endif  /* (!PWM_WheelLED_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_WheelLED_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_WheelLED_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_WheelLED_RestoreConfig(void) 
{
        #if(!PWM_WheelLED_UsingFixedFunction)
            #if(!PWM_WheelLED_PWMModeIsCenterAligned)
                PWM_WheelLED_WritePeriod(PWM_WheelLED_backup.PWMPeriod);
            #endif /* (!PWM_WheelLED_PWMModeIsCenterAligned) */

            PWM_WheelLED_WriteCounter(PWM_WheelLED_backup.PWMUdb);

            #if (PWM_WheelLED_UseStatus)
                PWM_WheelLED_STATUS_MASK = PWM_WheelLED_backup.InterruptMaskValue;
            #endif /* (PWM_WheelLED_UseStatus) */

            #if(PWM_WheelLED_DeadBandMode == PWM_WheelLED__B_PWM__DBM_256_CLOCKS || \
                PWM_WheelLED_DeadBandMode == PWM_WheelLED__B_PWM__DBM_2_4_CLOCKS)
                PWM_WheelLED_WriteDeadTime(PWM_WheelLED_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_WheelLED_KillModeMinTime)
                PWM_WheelLED_WriteKillTime(PWM_WheelLED_backup.PWMKillCounterPeriod);
            #endif /* (PWM_WheelLED_KillModeMinTime) */

            #if(PWM_WheelLED_UseControl)
                PWM_WheelLED_WriteControlRegister(PWM_WheelLED_backup.PWMControlRegister);
            #endif /* (PWM_WheelLED_UseControl) */
        #endif  /* (!PWM_WheelLED_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_WheelLED_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_WheelLED_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_WheelLED_Sleep(void) 
{
    #if(PWM_WheelLED_UseControl)
        if(PWM_WheelLED_CTRL_ENABLE == (PWM_WheelLED_CONTROL & PWM_WheelLED_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_WheelLED_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_WheelLED_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_WheelLED_UseControl) */

    /* Stop component */
    PWM_WheelLED_Stop();

    /* Save registers configuration */
    PWM_WheelLED_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_WheelLED_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_WheelLED_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_WheelLED_Wakeup(void) 
{
     /* Restore registers values */
    PWM_WheelLED_RestoreConfig();

    if(PWM_WheelLED_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_WheelLED_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
