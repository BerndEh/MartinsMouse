/*******************************************************************************
* File Name: Left_Button.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Left_Button_ALIASES_H) /* Pins Left_Button_ALIASES_H */
#define CY_PINS_Left_Button_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Left_Button_0			(Left_Button__0__PC)
#define Left_Button_0_PS		(Left_Button__0__PS)
#define Left_Button_0_PC		(Left_Button__0__PC)
#define Left_Button_0_DR		(Left_Button__0__DR)
#define Left_Button_0_SHIFT	(Left_Button__0__SHIFT)
#define Left_Button_0_INTR	((uint16)((uint16)0x0003u << (Left_Button__0__SHIFT*2u)))

#define Left_Button_INTR_ALL	 ((uint16)(Left_Button_0_INTR))


#endif /* End Pins Left_Button_ALIASES_H */


/* [] END OF FILE */
