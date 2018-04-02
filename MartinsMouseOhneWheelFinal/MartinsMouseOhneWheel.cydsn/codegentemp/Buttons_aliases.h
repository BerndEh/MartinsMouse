/*******************************************************************************
* File Name: Buttons.h  
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

#if !defined(CY_PINS_Buttons_ALIASES_H) /* Pins Buttons_ALIASES_H */
#define CY_PINS_Buttons_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Buttons_0			(Buttons__0__PC)
#define Buttons_0_PS		(Buttons__0__PS)
#define Buttons_0_PC		(Buttons__0__PC)
#define Buttons_0_DR		(Buttons__0__DR)
#define Buttons_0_SHIFT	(Buttons__0__SHIFT)
#define Buttons_0_INTR	((uint16)((uint16)0x0003u << (Buttons__0__SHIFT*2u)))

#define Buttons_1			(Buttons__1__PC)
#define Buttons_1_PS		(Buttons__1__PS)
#define Buttons_1_PC		(Buttons__1__PC)
#define Buttons_1_DR		(Buttons__1__DR)
#define Buttons_1_SHIFT	(Buttons__1__SHIFT)
#define Buttons_1_INTR	((uint16)((uint16)0x0003u << (Buttons__1__SHIFT*2u)))

#define Buttons_INTR_ALL	 ((uint16)(Buttons_0_INTR| Buttons_1_INTR))


#endif /* End Pins Buttons_ALIASES_H */


/* [] END OF FILE */
