/*******************************************************************************
* File Name: Mouse_SenseClk.h
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

#if !defined(CY_CLOCK_Mouse_SenseClk_H)
#define CY_CLOCK_Mouse_SenseClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Mouse_SenseClk_StartEx(uint32 alignClkDiv);
#define Mouse_SenseClk_Start() \
    Mouse_SenseClk_StartEx(Mouse_SenseClk__PA_DIV_ID)

#else

void Mouse_SenseClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Mouse_SenseClk_Stop(void);

void Mouse_SenseClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Mouse_SenseClk_GetDividerRegister(void);
uint8  Mouse_SenseClk_GetFractionalDividerRegister(void);

#define Mouse_SenseClk_Enable()                         Mouse_SenseClk_Start()
#define Mouse_SenseClk_Disable()                        Mouse_SenseClk_Stop()
#define Mouse_SenseClk_SetDividerRegister(clkDivider, reset)  \
    Mouse_SenseClk_SetFractionalDividerRegister((clkDivider), 0u)
#define Mouse_SenseClk_SetDivider(clkDivider)           Mouse_SenseClk_SetDividerRegister((clkDivider), 1u)
#define Mouse_SenseClk_SetDividerValue(clkDivider)      Mouse_SenseClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Mouse_SenseClk_DIV_ID     Mouse_SenseClk__DIV_ID

#define Mouse_SenseClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Mouse_SenseClk_CTRL_REG   (*(reg32 *)Mouse_SenseClk__CTRL_REGISTER)
#define Mouse_SenseClk_DIV_REG    (*(reg32 *)Mouse_SenseClk__DIV_REGISTER)

#define Mouse_SenseClk_CMD_DIV_SHIFT          (0u)
#define Mouse_SenseClk_CMD_PA_DIV_SHIFT       (8u)
#define Mouse_SenseClk_CMD_DISABLE_SHIFT      (30u)
#define Mouse_SenseClk_CMD_ENABLE_SHIFT       (31u)

#define Mouse_SenseClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Mouse_SenseClk_CMD_DISABLE_SHIFT))
#define Mouse_SenseClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Mouse_SenseClk_CMD_ENABLE_SHIFT))

#define Mouse_SenseClk_DIV_FRAC_MASK  (0x000000F8u)
#define Mouse_SenseClk_DIV_FRAC_SHIFT (3u)
#define Mouse_SenseClk_DIV_INT_MASK   (0xFFFFFF00u)
#define Mouse_SenseClk_DIV_INT_SHIFT  (8u)

#else 

#define Mouse_SenseClk_DIV_REG        (*(reg32 *)Mouse_SenseClk__REGISTER)
#define Mouse_SenseClk_ENABLE_REG     Mouse_SenseClk_DIV_REG
#define Mouse_SenseClk_DIV_FRAC_MASK  Mouse_SenseClk__FRAC_MASK
#define Mouse_SenseClk_DIV_FRAC_SHIFT (16u)
#define Mouse_SenseClk_DIV_INT_MASK   Mouse_SenseClk__DIVIDER_MASK
#define Mouse_SenseClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Mouse_SenseClk_H) */

/* [] END OF FILE */
