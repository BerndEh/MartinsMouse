/*******************************************************************************
* File Name: Mouse_PM.c
* Version 2.60
*
* Description:
*  This file provides Sleep APIs for Mouse CSD Component.
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

static Mouse_BACKUP_STRUCT Mouse_backup =
{
    0x00u, /* enableState; */
};


/*******************************************************************************
* Function Name: Mouse_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the customer configuration of Mouse.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_backup - used to save the component state before entering the  sleep
*  mode and none-retention registers.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Mouse_SaveConfig(void)
{
    if ((Mouse_CSD_CFG_REG & Mouse_CSD_CFG_ENABLE) != 0u)
    {
        Mouse_backup.enableState = 1u;
    }
}


/*******************************************************************************
* Function Name: Mouse_Sleep
********************************************************************************
*
* Summary:
*  Disables the Active mode power.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_backup - used to save the component state before entering the sleep
*  mode.
*
*******************************************************************************/
void Mouse_Sleep(void)
{
    Mouse_SaveConfig();

    /* Disable interrupt */
    CyIntDisable(Mouse_ISR_NUMBER);

    Mouse_CSD_CFG_REG &= ~(Mouse_CSD_CFG_SENSE_COMP_EN | Mouse_CSD_CFG_SENSE_EN);

    #if(Mouse_IDAC_CNT == 2u)
        Mouse_CSD_CFG_REG &= ~(Mouse_CSD_CFG_ENABLE);
    #endif /* (Mouse_IDAC_CNT == 2u) */

    /* Disable Clocks */
    Mouse_SenseClk_Stop();
    Mouse_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: Mouse_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the Mouse configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Must be called only after Mouse_SaveConfig() routine. Otherwise
*  the component configuration will be overwritten with its initial setting.
*
* Global Variables:
*  Mouse_backup - used to save the component state before entering the sleep
*  mode and none-retention registers.
*
*******************************************************************************/
void Mouse_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: Mouse_Wakeup
********************************************************************************
*
* Summary:
*  Restores the Mouse configuration and non-retention register values.
*  Restores the enabled state of the component by setting the Active mode power template
*  bits for a number of components used within Mouse.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_backup - used to save the component state before entering the sleep
*  mode and none-retention registers.
*
*******************************************************************************/
void Mouse_Wakeup(void)
{
    /* Enable the Clocks */
    Mouse_SenseClk_Start();
    Mouse_SampleClk_Start();

    /* Restore Mouse Enable state */
    if (Mouse_backup.enableState != 0u)
    {
        Mouse_Enable();
    }
}


/* [] END OF FILE */
