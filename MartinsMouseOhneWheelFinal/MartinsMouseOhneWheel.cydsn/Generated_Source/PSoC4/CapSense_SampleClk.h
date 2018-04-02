/*******************************************************************************
* File Name: Mouse_SampleClk.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Mouse_SampleClk_H)
#define CY_CLOCK_Mouse_SampleClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Mouse_SampleClk_StartEx(uint32 alignClkDiv);
#define Mouse_SampleClk_Start() \
    Mouse_SampleClk_StartEx(Mouse_SampleClk__PA_DIV_ID)

#else

void Mouse_SampleClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Mouse_SampleClk_Stop(void);

void Mouse_SampleClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Mouse_SampleClk_GetDividerRegister(void);
uint8  Mouse_SampleClk_GetFractionalDividerRegister(void);

#define Mouse_SampleClk_Enable()                         Mouse_SampleClk_Start()
#define Mouse_SampleClk_Disable()                        Mouse_SampleClk_Stop()
#define Mouse_SampleClk_SetDividerRegister(clkDivider, reset)  \
    Mouse_SampleClk_SetFractionalDividerRegister((clkDivider), 0u)
#define Mouse_SampleClk_SetDivider(clkDivider)           Mouse_SampleClk_SetDividerRegister((clkDivider), 1u)
#define Mouse_SampleClk_SetDividerValue(clkDivider)      Mouse_SampleClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Mouse_SampleClk_DIV_ID     Mouse_SampleClk__DIV_ID

#define Mouse_SampleClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Mouse_SampleClk_CTRL_REG   (*(reg32 *)Mouse_SampleClk__CTRL_REGISTER)
#define Mouse_SampleClk_DIV_REG    (*(reg32 *)Mouse_SampleClk__DIV_REGISTER)

#define Mouse_SampleClk_CMD_DIV_SHIFT          (0u)
#define Mouse_SampleClk_CMD_PA_DIV_SHIFT       (8u)
#define Mouse_SampleClk_CMD_DISABLE_SHIFT      (30u)
#define Mouse_SampleClk_CMD_ENABLE_SHIFT       (31u)

#define Mouse_SampleClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Mouse_SampleClk_CMD_DISABLE_SHIFT))
#define Mouse_SampleClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Mouse_SampleClk_CMD_ENABLE_SHIFT))

#define Mouse_SampleClk_DIV_FRAC_MASK  (0x000000F8u)
#define Mouse_SampleClk_DIV_FRAC_SHIFT (3u)
#define Mouse_SampleClk_DIV_INT_MASK   (0xFFFFFF00u)
#define Mouse_SampleClk_DIV_INT_SHIFT  (8u)

#else 

#define Mouse_SampleClk_DIV_REG        (*(reg32 *)Mouse_SampleClk__REGISTER)
#define Mouse_SampleClk_ENABLE_REG     Mouse_SampleClk_DIV_REG
#define Mouse_SampleClk_DIV_FRAC_MASK  Mouse_SampleClk__FRAC_MASK
#define Mouse_SampleClk_DIV_FRAC_SHIFT (16u)
#define Mouse_SampleClk_DIV_INT_MASK   Mouse_SampleClk__DIVIDER_MASK
#define Mouse_SampleClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Mouse_SampleClk_H) */

/* [] END OF FILE */
