/*******************************************************************************
* File Name: Wheel_InA.h  
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

#if !defined(CY_PINS_Wheel_InA_ALIASES_H) /* Pins Wheel_InA_ALIASES_H */
#define CY_PINS_Wheel_InA_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Wheel_InA_0			(Wheel_InA__0__PC)
#define Wheel_InA_0_PS		(Wheel_InA__0__PS)
#define Wheel_InA_0_PC		(Wheel_InA__0__PC)
#define Wheel_InA_0_DR		(Wheel_InA__0__DR)
#define Wheel_InA_0_SHIFT	(Wheel_InA__0__SHIFT)
#define Wheel_InA_0_INTR	((uint16)((uint16)0x0003u << (Wheel_InA__0__SHIFT*2u)))

#define Wheel_InA_INTR_ALL	 ((uint16)(Wheel_InA_0_INTR))


#endif /* End Pins Wheel_InA_ALIASES_H */


/* [] END OF FILE */
