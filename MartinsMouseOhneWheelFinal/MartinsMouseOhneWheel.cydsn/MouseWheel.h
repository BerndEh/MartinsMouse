/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef __WHEEL
#define __WHEEL
 
#include <project.h>
#include <main.h>
    
CY_ISR_PROTO(MouseWheel_ISR);
void  MouseWheel_Start(void);
void  MouseWheel_Stop(void);
uint8 MouseWheel_GetPosition(void);
uint8 MouseWheel_GetKeys(void);

#endif
/* [] END OF FILE */
