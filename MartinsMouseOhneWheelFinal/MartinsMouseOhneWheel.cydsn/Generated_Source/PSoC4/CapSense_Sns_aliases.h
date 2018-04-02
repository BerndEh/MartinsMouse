/*******************************************************************************
* File Name: Mouse_Sns.h  
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

#if !defined(CY_PINS_Mouse_Sns_ALIASES_H) /* Pins Mouse_Sns_ALIASES_H */
#define CY_PINS_Mouse_Sns_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Mouse_Sns_0			(Mouse_Sns__0__PC)
#define Mouse_Sns_0_PS		(Mouse_Sns__0__PS)
#define Mouse_Sns_0_PC		(Mouse_Sns__0__PC)
#define Mouse_Sns_0_DR		(Mouse_Sns__0__DR)
#define Mouse_Sns_0_SHIFT	(Mouse_Sns__0__SHIFT)
#define Mouse_Sns_0_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__0__SHIFT*2u)))

#define Mouse_Sns_1			(Mouse_Sns__1__PC)
#define Mouse_Sns_1_PS		(Mouse_Sns__1__PS)
#define Mouse_Sns_1_PC		(Mouse_Sns__1__PC)
#define Mouse_Sns_1_DR		(Mouse_Sns__1__DR)
#define Mouse_Sns_1_SHIFT	(Mouse_Sns__1__SHIFT)
#define Mouse_Sns_1_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__1__SHIFT*2u)))

#define Mouse_Sns_2			(Mouse_Sns__2__PC)
#define Mouse_Sns_2_PS		(Mouse_Sns__2__PS)
#define Mouse_Sns_2_PC		(Mouse_Sns__2__PC)
#define Mouse_Sns_2_DR		(Mouse_Sns__2__DR)
#define Mouse_Sns_2_SHIFT	(Mouse_Sns__2__SHIFT)
#define Mouse_Sns_2_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__2__SHIFT*2u)))

#define Mouse_Sns_3			(Mouse_Sns__3__PC)
#define Mouse_Sns_3_PS		(Mouse_Sns__3__PS)
#define Mouse_Sns_3_PC		(Mouse_Sns__3__PC)
#define Mouse_Sns_3_DR		(Mouse_Sns__3__DR)
#define Mouse_Sns_3_SHIFT	(Mouse_Sns__3__SHIFT)
#define Mouse_Sns_3_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__3__SHIFT*2u)))

#define Mouse_Sns_4			(Mouse_Sns__4__PC)
#define Mouse_Sns_4_PS		(Mouse_Sns__4__PS)
#define Mouse_Sns_4_PC		(Mouse_Sns__4__PC)
#define Mouse_Sns_4_DR		(Mouse_Sns__4__DR)
#define Mouse_Sns_4_SHIFT	(Mouse_Sns__4__SHIFT)
#define Mouse_Sns_4_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__4__SHIFT*2u)))

#define Mouse_Sns_INTR_ALL	 ((uint16)(Mouse_Sns_0_INTR| Mouse_Sns_1_INTR| Mouse_Sns_2_INTR| Mouse_Sns_3_INTR| Mouse_Sns_4_INTR))
#define Mouse_Sns_LinearSlider0_e0__LS			(Mouse_Sns__LinearSlider0_e0__LS__PC)
#define Mouse_Sns_LinearSlider0_e0__LS_PS		(Mouse_Sns__LinearSlider0_e0__LS__PS)
#define Mouse_Sns_LinearSlider0_e0__LS_PC		(Mouse_Sns__LinearSlider0_e0__LS__PC)
#define Mouse_Sns_LinearSlider0_e0__LS_DR		(Mouse_Sns__LinearSlider0_e0__LS__DR)
#define Mouse_Sns_LinearSlider0_e0__LS_SHIFT	(Mouse_Sns__LinearSlider0_e0__LS__SHIFT)
#define Mouse_Sns_LinearSlider0_e0__LS_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__0__SHIFT*2u)))

#define Mouse_Sns_LinearSlider0_e1__LS			(Mouse_Sns__LinearSlider0_e1__LS__PC)
#define Mouse_Sns_LinearSlider0_e1__LS_PS		(Mouse_Sns__LinearSlider0_e1__LS__PS)
#define Mouse_Sns_LinearSlider0_e1__LS_PC		(Mouse_Sns__LinearSlider0_e1__LS__PC)
#define Mouse_Sns_LinearSlider0_e1__LS_DR		(Mouse_Sns__LinearSlider0_e1__LS__DR)
#define Mouse_Sns_LinearSlider0_e1__LS_SHIFT	(Mouse_Sns__LinearSlider0_e1__LS__SHIFT)
#define Mouse_Sns_LinearSlider0_e1__LS_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__1__SHIFT*2u)))

#define Mouse_Sns_LinearSlider0_e2__LS			(Mouse_Sns__LinearSlider0_e2__LS__PC)
#define Mouse_Sns_LinearSlider0_e2__LS_PS		(Mouse_Sns__LinearSlider0_e2__LS__PS)
#define Mouse_Sns_LinearSlider0_e2__LS_PC		(Mouse_Sns__LinearSlider0_e2__LS__PC)
#define Mouse_Sns_LinearSlider0_e2__LS_DR		(Mouse_Sns__LinearSlider0_e2__LS__DR)
#define Mouse_Sns_LinearSlider0_e2__LS_SHIFT	(Mouse_Sns__LinearSlider0_e2__LS__SHIFT)
#define Mouse_Sns_LinearSlider0_e2__LS_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__2__SHIFT*2u)))

#define Mouse_Sns_LinearSlider0_e3__LS			(Mouse_Sns__LinearSlider0_e3__LS__PC)
#define Mouse_Sns_LinearSlider0_e3__LS_PS		(Mouse_Sns__LinearSlider0_e3__LS__PS)
#define Mouse_Sns_LinearSlider0_e3__LS_PC		(Mouse_Sns__LinearSlider0_e3__LS__PC)
#define Mouse_Sns_LinearSlider0_e3__LS_DR		(Mouse_Sns__LinearSlider0_e3__LS__DR)
#define Mouse_Sns_LinearSlider0_e3__LS_SHIFT	(Mouse_Sns__LinearSlider0_e3__LS__SHIFT)
#define Mouse_Sns_LinearSlider0_e3__LS_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__3__SHIFT*2u)))

#define Mouse_Sns_LinearSlider0_e4__LS			(Mouse_Sns__LinearSlider0_e4__LS__PC)
#define Mouse_Sns_LinearSlider0_e4__LS_PS		(Mouse_Sns__LinearSlider0_e4__LS__PS)
#define Mouse_Sns_LinearSlider0_e4__LS_PC		(Mouse_Sns__LinearSlider0_e4__LS__PC)
#define Mouse_Sns_LinearSlider0_e4__LS_DR		(Mouse_Sns__LinearSlider0_e4__LS__DR)
#define Mouse_Sns_LinearSlider0_e4__LS_SHIFT	(Mouse_Sns__LinearSlider0_e4__LS__SHIFT)
#define Mouse_Sns_LinearSlider0_e4__LS_INTR	((uint16)((uint16)0x0003u << (Mouse_Sns__4__SHIFT*2u)))


#endif /* End Pins Mouse_Sns_ALIASES_H */


/* [] END OF FILE */
