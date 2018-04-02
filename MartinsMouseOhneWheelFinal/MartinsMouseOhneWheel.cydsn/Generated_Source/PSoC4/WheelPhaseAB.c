/*******************************************************************************
* File Name: WheelPhaseAB.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "WheelPhaseAB.h"

#if !defined(WheelPhaseAB_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: WheelPhaseAB_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 WheelPhaseAB_Read(void) 
{ 
    return WheelPhaseAB_Status;
}


/*******************************************************************************
* Function Name: WheelPhaseAB_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void WheelPhaseAB_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    WheelPhaseAB_Status_Aux_Ctrl |= WheelPhaseAB_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: WheelPhaseAB_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void WheelPhaseAB_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    WheelPhaseAB_Status_Aux_Ctrl &= (uint8)(~WheelPhaseAB_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: WheelPhaseAB_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void WheelPhaseAB_WriteMask(uint8 mask) 
{
    #if(WheelPhaseAB_INPUTS < 8u)
    	mask &= ((uint8)(1u << WheelPhaseAB_INPUTS) - 1u);
	#endif /* End WheelPhaseAB_INPUTS < 8u */
    WheelPhaseAB_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: WheelPhaseAB_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 WheelPhaseAB_ReadMask(void) 
{
    return WheelPhaseAB_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
