/******************************************************************************
* Project Name		: PSoC_4_BLE_Mouse_Slider_LED
* File Name			: main.c
* Version 			: 1.0
* Device Used		: CY8C4247LQI-BL483
* Software Used		: PSoC Creator 3.3 SP1
* Compiler    		: ARM GCC 4.9.3, ARM MDK Generic
* Related Hardware	: CY8CKIT-042-BLE Bluetooth Low Energy Pioneer Kit 
* Owner             : ROIT
*
********************************************************************************
* Copyright (2015-16), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/

/******************************************************************************
*                           THEORY OF OPERATION
*******************************************************************************
* This project will showcase the capability of PSoC 4 BLE to communicate 
* bi-directionally with a BLE Central device over custom services. The Mouse 
* custom service allows notifications to be sent to central device when 
* notifications are enabled. On the other hand, the RGB LED custom service allows 
* read and write of attributes under the RGB characteristics.
* This project utilizes Mouse component to check finger position on slider 
* and report this to central device over BLE. On the other hand, the control values 
* sent to PSoC 4 BLE is converted to respective color and intensity on the onboard  
* RGB LED. The BLE central device can be any BLE central device, including CySmart 
* mobile app or CySmart PC tool. 
* This project also inludes low power mode operation, idle for battery operated 
* devices. The project utlizes Deep Sleep feature of both BLESS and CPU to remain 
* in low power mode as much as possible, while maintaining the BLE connection and  
* data transfer. This allows the device to run on coin cell battery for long time.
*
* Note:
* The programming pins have been configured as GPIO, and not SWD. This is because 
* when programming pins are configured for SWD, then the silicon consumes extra
* power through the pins. To prevent the leakage of power, the pins have been set 
* to GPIO. With this setting, the kit can still be acquired by PSoC Creator or
* PSoC Programmer software tools for programming, but the project cannot be 
* externally debugged. To re-enable debugging, go to PSoC_4_BLE_Mouse_Slider_LED.cydwr 
* from Workspace Explorer, go to Systems tab, and set the Debug Select option to 'SWD'.
* Build and program this project to enable external Debugging.
*
* Refer to BLE Pioneer Kit user guide for details.
*******************************************************************************
* Hardware connection required for testing -
* Slider pins 	- P2[1]-P2[5] (hard-wired on the BLE Pioneer kit)
* Cmod pin		- P4[0] (hard-wired on the PSoC 4 BLE module)
* R-G-B LED 	- P2[6], P3[6] and P3[7] (hard-wired on the BLE Pioneer kit)
* User Switch	- P2[7] (hard-wired on the BLE Pioneer kit)
******************************************************************************/
#include <main.h>

// CYBLE 214015-01:     Taster bei 3.5
// CY8C4247LQI-BL483    Taster bei 2.7

/* This flag is used by application to know whether a Central 
* device has been connected. This is updated in BLE event callback 
* function*/
extern uint8 deviceConnected;

/*This flag is set when the Central device writes to CCCD of the 
* Mouse slider Characteristic to enable notifications */
extern uint8 sendMouseNotifications;

/* 'restartAdvertisement' flag is used to restart advertisement */
extern uint8 restartAdvertisement;

extern volatile uint8 gbCurrentKeys;


void ToogleGREEN(void)
{
    static uint8 bState=0;
    bState++;
    if (bState&1)
        GREEN_ON;
    else
        GREEN_OFF;
}

/*******************************************************************************
* Function Name: MyISR
********************************************************************************
* Summary:
*        ISR routine for isr_button. System enters here after CPU wakeup.
* Clears pending interrupt.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
CY_ISR(MyMainButtonsISR)
{
	/* Clear Pending interrupts */
	isr_buttons_ClearPending();
	Buttons_ClearInterrupt();
//    GREEN_ON;
}

CY_ISR(MyMidButtonISR)
{
    isr_mid_button_ClearPending();
    Mid_Button_ClearInterrupt();
}

CY_ISR(MyRightButtonISR)
{
//    isr_right_button_ClearPending();
//    Right_Button_ClearInterrupt();
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*        System entrance point. This calls the initializing function and
* continuously process BLE and Mouse events.
*
* Parameters:
*  void
*
* Return:
*  int
*

*******************************************************************************/
int main()
{
    static bool bIsKeyWTD_Started=false;
    
	/* This function will initialize the system resources such as BLE and Mouse */
    InitializeSystem();
	
    while(1)
    {
        /*Process event callback to handle BLE events. The events generated and 
		* used for this application are inside the 'CustomEventHandler' routine*/
        CyBle_ProcessEvents();
		
		/* Updated LED for status during BLE active states */
		HandleStatusLED();
		
        //ToogleGREEN();
        
		if(TRUE == deviceConnected)
		{
			/* After the connection, send new connection parameter to the Client device 
			* to run the BLE communication on desired interval. This affects the data rate 
			* and power consumption. High connection interval will have lower data rate but 
			* lower power consumption. Low connection interval will have higher data rate at
			* expense of higher power. This function is called only once per connection. */
			UpdateConnectionParam();
			
			/* Send Mouse Slider data when respective notification is enabled */
			if(sendMouseNotifications == CCCD_NTF_BIT_MASK)
			{
                if (!bIsKeyWTD_Started)
                {
                    /* Unlock the WDT registers for modification */
            		CySysWdtUnlock();
            		
            		/* Disable Counter 0 to allow modifications */
            		CySysWdtEnable(CY_SYS_WDT_COUNTER1_MASK);
                    		/* Lock Watchdog to prevent any further change */
            	    CySysWdtLock();
                    bIsKeyWTD_Started = true;
        		}
                // Bewusst nicht gechecked, denn slave-latency ist 100
                // so würden also 100*40ms bis etwas gesendet wird
                // das ist so natürlich nicht erwünscht!!
				// if(CYBLE_BLESS_STATE_ECO_STABLE ==CyBle_GetBleSsState())
			    HandleMouse();      // hier die Mouse-Daten übertragen
			}
		}
        else if (bIsKeyWTD_Started)
        {
            /* Unlock the WDT registers for modification */
    		CySysWdtUnlock();
    		
    		/* Disable Counter 0 to allow modifications */
    		CySysWdtDisable(CY_SYS_WDT_COUNTER1_MASK);
            		/* Lock Watchdog to prevent any further change */
    	    CySysWdtLock();
            bIsKeyWTD_Started = false;
		}
        
		#ifdef ENABLE_LOW_POWER_MODE
			/* Put system to Deep sleep, including BLESS, and wakeup on interrupt. 
			* The source of the interrupt can be either BLESS Link Layer in case of 
			* BLE advertisement and connection or by User Button press during BLE 
			* disconnection */
			HandleLowPowerMode();
		#endif
		
		if(restartAdvertisement)
		{
			/* Reset 'restartAdvertisement' flag*/
			restartAdvertisement = FALSE;
			
			/* Start Advertisement and enter Discoverable mode*/
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);	
		}
    }	/* End of for(;;) */
}

/*******************************************************************************
* Function Name: InitializeSystem
********************************************************************************
* Summary:
*        Start the components and initialize system 
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void InitializeSystem(void)
{
    /* Unfreeze IO-Cells */
    CySysPmUnfreezeIo();
    
	/* Enable global interrupt mask */
	CyGlobalIntEnable; 

    RED_Write(0);               // LED gegen VDD (+3V) geschaltet
    GREEN_Write(0);
    BLUE_Write(0);
    
    RED_ON;
    
    // Damit das Ein/Auschalten über den Drive-Mode sicher funktioniert
    //PWM_Start();
    //CyDelay(100);
    //MouseWheel_Stop();
    
	/* Set the Watchdog Interrupt vector to the address of Interrupt routine 
	* WDT_INT_Handler. This routine counts the 3 seconds for LED ON state during
	* connection. */
	CyIntSetVector(WATCHDOG_INT_VEC_NUM, &WDT_INT_Handler);
    
    /* Start the Button ISR to allow wakeup from sleep */
	isr_buttons_StartEx(MyMainButtonsISR);
    isr_mid_button_StartEx(MyMidButtonISR);
    //isr_right_button_StartEx(MyRightButtonISR);

    RED_ON;
    
    /* Retrieve and print last reset reason */
    uint32 reason = CySysPmGetResetReason();
    switch (reason)    
    {
        case CY_PM_RESET_REASON_WAKEUP_STOP:
            CyDelay(3000);
            RED_OFF;
            break;
        case CY_PM_RESET_REASON_WAKEUP_HIB:
            /* Start BLE component and register the CustomEventHandler function. This 
        	* function exposes the events from BLE component for application use */
            CyBle_Start(CustomEventHandler);
            while (restartAdvertisement==false)
                CyBle_ProcessEvents();
            RED_OFF;

            GREEN_OFF;
            
            /* Reset 'restartAdvertisement' flag*/
			restartAdvertisement = FALSE;
			
			/* Start Advertisement and enter Discoverable mode*/
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);	
            break;
            
        default:
        case CY_PM_RESET_REASON_XRES:
            CyDelay(500);
            RED_OFF;
            GREEN_ON;
            
            /* Freeze IO-Cells */
            CySysPmFreezeIo();
            CySysPmHibernate();
            // hier kommen wir nicht mehr hin
    }
    RED_OFF;
}



/*******************************************************************************
* Function Name: HandleMouse
********************************************************************************
* Summary:
* This function scans for pushed Buttons and MouseWeehlPosition  
* If position is different, triggers separate routine for BLE notification
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void HandleMouse(void)
{
    static uint8 bLastButton;
    static uint8 bLastMouseWheel = 0;

    // Hier die Mouse-Buttons abfragen
    uint8 bCurButton     = gbCurrentKeys;
    uint8 bCurMouseWeehl = 0; //MouseWheel_GetPosition();
    
    if (CyBle_GattGetBusyStatus() == CYBLE_STACK_STATE_FREE)
    {
        //if (bLastButton != bCurButton || bLastMouseWheel != bCurMouseWeehl)
        if (bLastButton != bCurButton)
        {
            //ToogleGREEN();

            // hier nun die static-Variablen updaten
            bLastButton     = bCurButton;
            bLastMouseWheel = bCurMouseWeehl;

            // Mouse-Data has 2 Bytes: Byte[0]=Button Byte[1]=Position
            static uint8 bMouseData[2];
            bMouseData[0] = bLastButton;
            bMouseData[1] = bLastMouseWheel;          // hier war das Mouse-Wheel vorgesehen
            SendDataOverMouseNotification(bMouseData);
        }
    }
}

/* [] END OF FILE */
