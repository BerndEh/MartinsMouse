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

#include <MouseWheel.h>

volatile uint8 mbMouseWheelPosition = 0;
volatile uint8 mbCurrentKeys        = 0;

//========================================================================================
// in diesem Interrupt auch die Tasten abfragen, da die Grundschleife zeitlich zu ungenau
//========================================================================================
//                                           LFCLK     PWM-Teiler (8-Bit)
// Dieser IRQ wird mit ca. 128Hz aufgerufen (32768Hz / 256)
// Um die Tasten auch zu entprellen nur jedes 4 mal diese Prüfen -> 32ms abfragen
//========================================================================================
CY_ISR(MouseWheel_ISR)
{
    // Clear Interrupt Flags
    isr_Wheel_ClearPending();
    PWM_ClearInterrupt(PWM_INTR_MASK_TC);
    //PWM_ReadStatusRegister();
    
    static const int8 bPhaseToDelta[16] = 
    {       // Index Alt - New     Delta
         0,  //  0     00  - 00   ->  0
         1,  //  1     00  - 01   -> +1
        -1,  //  2     00  - 10   -> -1
         0,  //  3     00  - 11   ->  0  (ilegalter schritt!!!)
        
        -1,  //  4     01  - 00   -> -1
         0,  //  5     01  - 01   ->  0
         0,  //  6     01  - 10   ->  0  (ilegalter schritt!!!)
         1,  //  7     01  - 11   ->  1  
        
         1,  //  8     10  - 00   -> +1
         0,  //  9     10  - 01   ->  0  (ilegalter schritt!!!)
         0,  // 10     10  - 10   ->  0 
        -1,  // 11     10  - 11   -> -1
        
         0,  // 12     11  - 00   ->  0  (ilegalter schritt!!!)
        -1,  // 13     11  - 01   -> -1
        +1,  // 14     11  - 10   -> +1 
         0,  // 15     11  - 11   ->  0
    };

    static uint8 bLastPhase;
    
    uint8 bCurPhase = WheelPhaseAB_Read();
    
    if(bLastPhase != bCurPhase)
    {
        // Phasen haben sich geändert -> Counter entsprechend anpassen
        mbMouseWheelPosition += bPhaseToDelta[(bLastPhase<<2) | bCurPhase];
        bLastPhase = bCurPhase;
    }
    
    static uint8 uiCounter=8;
    uiCounter--;
    
    if (uiCounter==0)
    {
        uiCounter = 8;
        mbCurrentKeys = 0;
        if (Left_Button_Read() == 0)
            mbCurrentKeys = 1;
        if (Mid_Button_Read() == 0)
            mbCurrentKeys |= 2;
        if (Right_Button_Read() == 0)
            mbCurrentKeys |= 4;    
    }
}

void  MouseWheel_Start(void)
{
    WHEEL_LED_ON;
    //PWM_RestoreConfig();
    //PWM_Enable();
    Clock_1_Start();
    PWM_Start();
    isr_Wheel_ClearPending();
    isr_Wheel_StartEx(MouseWheel_ISR);  
    GREEN_ON;
}

void  MouseWheel_Stop(void)
{
    WHEEL_LED_OFF;
    PWM_Stop();
    GREEN_OFF;
}

uint8 MouseWheel_GetPosition(void)
{
    return mbMouseWheelPosition;
}

uint8 MouseWheel_GetKeys(void)
{
    return mbCurrentKeys;
}


/* [] END OF FILE */
