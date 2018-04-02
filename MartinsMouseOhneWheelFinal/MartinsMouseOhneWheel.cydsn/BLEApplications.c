/******************************************************************************
* Project Name		: PSoC_4_BLE_Mouse_Slider_LED
* File Name			: BLEApplications.c
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
#include <main.h>

/**************************Variable Declarations*****************************/
/* 'connectionHandle' stores connection parameters */
CYBLE_CONN_HANDLE_T  connectionHandle;

// Hier die Variable, die über WDT geschrieben wird -> in main wird diese gelesen
volatile uint8 gbCurrentKeys;

/*This flag is set when the Central device writes to CCCD (Client Characteristic 
* Configuration Descriptor) of the Mouse slider Characteristic to enable 
* notifications */
uint8 sendMouseNotifications = FALSE;	

/* This flag is used by application to know whether a Central 
* device has been connected. This is updated in BLE event callback 
* function*/
uint8 deviceConnected = FALSE;

/* 'restartAdvertisement' flag provided the present state of power mode in firmware */
uint8 restartAdvertisement = FALSE;

/* This flag is used to let application send a L2CAP connection update request
* to Central device */
static uint8 isConnectionUpdateRequested = TRUE;

/* Connection Parameter update values. This values are used by the BLE component
* to update the connector parameter, including connection interval, to desired 
* value */
static CYBLE_GAP_CONN_UPDATE_PARAM_T ConnectionParam =
{
    CYBLE_GAPP_CONNECTION_INTERVAL_MIN,  		      
    CYBLE_GAPP_CONNECTION_INTERVAL_MAX,		       
    CYBLE_GAPP_CONNECTION_SLAVE_LATENCY,			    
    CYBLE_GAPP_CONNECTION_TIME_OUT 			         	
};

/* This flag is used to switch between CPU Deep Sleep and Sleep, depending on
* the status of RGB LED color values received. This method allows to conserve 
* more power while LED usage */
uint8 shut_down_led = TRUE;

/* Counter to keep the LED ON for a selected period before shuting the LEDs down */
uint8 led_timer = FALSE;

/* Counter to allow an initial 3 second Status LED ON for indicating connection */
uint8 timer_tick = FALSE;

/* Flag to switch of the LED after connection */
uint8 switch_off_status_led = FALSE;

/* Status flag for the Stack Busy state. This flag is used to notify the application 
* whether there is stack buffer free to push more data or not */
uint8 busyStatus = 0;

/* Local variable to store the current CCCD value */
uint8 MouseCCCDvalue[2];

/* Handle value to update the CCCD */
CYBLE_GATT_HANDLE_VALUE_PAIR_T MouseNotificationCCCDhandle;

/* Handle value to update the CCCD */
CYBLE_GATT_HANDLE_VALUE_PAIR_T RGBNotificationCCCDhandle;

/*******************************************************************************
void GoHypernate(void)
*******************************************************************************/
void GoHypernate(void)
{
    CyIntDisable(WATCHDOG_INT_VEC_NUM);

    /* If the present BLE state is disconnected, switch off LED
    * and set the drive mode of LED to Hi-Z (Analog)*/
    RED_OFF;
    GREEN_OFF;
    BLUE_ON;

    /* Freeze IO-Cells */
    CySysPmFreezeIo();
    CySysPmHibernate();
}

/*******************************************************************************
* Function Name: CustomEventHandler
********************************************************************************
* Summary:
*        Call back event function to handle various events from BLE stack
*
* Parameters:
*  event:		event returned
*  eventParam:	link to value of the events returned
*
* Return:
*  void
*
*******************************************************************************/
void CustomEventHandler(uint32 event, void * eventParam)
{
	/* Local variable to store the data received as part of the Write request 
	* events */
	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;

    switch(event)
    {
        case CYBLE_EVT_STACK_ON:
			/* This event is received when component is Started */
			
			/* Set restartAdvertisement flag to allow calling Advertisement 
			* API from main function */
			restartAdvertisement = TRUE;
			
			break;
			
		case CYBLE_EVT_TIMEOUT:
			/* Event Handling for Timeout  */
	
			break;
        
		/**********************************************************
        *                       GAP Events
        ***********************************************************/
		case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
			
			/* If the current BLE state is Disconnected, then the Advertisement
			* Start Stop event implies that advertisement has stopped */
			if(CyBle_GetState() == CYBLE_STATE_DISCONNECTED)
			{
				/* Set restartAdvertisement flag to allow calling Advertisement 
				* API from main function */
                GoHypernate();
				//restartAdvertisement = TRUE;
			}
			break;
			
		case CYBLE_EVT_GAP_DEVICE_CONNECTED: 					
			/* This event is received when device is connected over GAP layer */
			break;

			
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
			/* This event is received when device is disconnected */
			
			/* Set restartAdvertisement flag to allow calling Advertisement 
			* API from main function */
			restartAdvertisement = TRUE;
			
			/* Set flag to allow system to go to Deep Sleep */
			shut_down_led = TRUE;
			break;
        
		/**********************************************************
        *                       GATT Events
        ***********************************************************/
        case CYBLE_EVT_GATT_CONNECT_IND:
			/* This event is received when device is connected over GATT level */
			
			/* Update attribute handle on GATT Connection*/
            connectionHandle = *(CYBLE_CONN_HANDLE_T  *)eventParam;
			
			/* This flag is used in application to check connection status */
			deviceConnected = TRUE;
			break;
        
        case CYBLE_EVT_GATT_DISCONNECT_IND:
			/* This event is received when device is disconnected */
			
			/* Update deviceConnected flag*/
			deviceConnected = FALSE;
			
			/* Reset Mouse notification flag to prevent further notifications
			 * being sent to Central device after next connection. */
			sendMouseNotifications = FALSE;
			
			/* Reset the CCCD value to disable notifications */
    		/* Write the present Mouse notification status to the local variable */
    		MouseCCCDvalue[0] = sendMouseNotifications;
    		MouseCCCDvalue[1] = 0x00;
    		
    		/* Update CCCD handle with notification status data*/
    		MouseNotificationCCCDhandle.attrHandle = CYBLE_MOUSE_SERVICE_MOUSE_DATA_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE;
    		MouseNotificationCCCDhandle.value.val = MouseCCCDvalue;
    		MouseNotificationCCCDhandle.value.len = CCCD_DATA_LEN;
    		
    		/* Report data to BLE component for sending data when read by Central device */
    		CyBle_GattsWriteAttributeValue(&MouseNotificationCCCDhandle, ZERO, &connectionHandle, CYBLE_GATT_DB_PEER_INITIATED);
		
			/* Reset the isConnectionUpdateRequested flag to allow sending
			* connection parameter update request in next connection */
			isConnectionUpdateRequested = TRUE;
			
			/* Set the flag to allow system to go to Deep Sleep */
			shut_down_led = TRUE;
			
			break;
            
        case CYBLE_EVT_GATTS_WRITE_REQ:
			/* This event is received when Central device sends a Write command on an Attribute */
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
            
			/* When this event is triggered, the peripheral has received a write command on the custom characteristic */
			/* Check if command is for correct attribute and update the flag for sending Notifications */
            if(CYBLE_MOUSE_SERVICE_MOUSE_DATA_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE == wrReqParam->handleValPair.attrHandle)
			{
				/* Extract the Write value sent by the Client for Mouse Slider CCCD */
                if(wrReqParam->handleValPair.value.val[CYBLE_MOUSE_SERVICE_MOUSE_DATA_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX] == TRUE)
                {
                    sendMouseNotifications = TRUE;
                }
                else if(wrReqParam->handleValPair.value.val[CYBLE_MOUSE_SERVICE_MOUSE_DATA_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX] == FALSE)
                {
                    sendMouseNotifications = FALSE;
                }
				
        		/* Write the present Mouse notification status to the local variable */
        		MouseCCCDvalue[0] = sendMouseNotifications;
        		MouseCCCDvalue[1] = 0x00;
        		
        		/* Update CCCD handle with notification status data*/
        		MouseNotificationCCCDhandle.attrHandle = CYBLE_MOUSE_SERVICE_MOUSE_DATA_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE;
        		MouseNotificationCCCDhandle.value.val = MouseCCCDvalue;
        		MouseNotificationCCCDhandle.value.len = CCCD_DATA_LEN;
        		
        		/* Report data to BLE component for sending data when read by Central device */
        		CyBle_GattsWriteAttributeValue(&MouseNotificationCCCDhandle, ZERO, &connectionHandle, CYBLE_GATT_DB_PEER_INITIATED);
            }
						
			/* Send the response to the write request received. */
			CyBle_GattsWriteRsp(connectionHandle);
			
			break;
			
		case CYBLE_EVT_L2CAP_CONN_PARAM_UPDATE_RSP:
				/* If L2CAP connection parameter update response received, reset application flag */
            	isConnectionUpdateRequested = FALSE;
            break;
			
		case CYBLE_EVT_STACK_BUSY_STATUS:
			/* This event is generated when the internal stack buffer is full and no more
			* data can be accepted or the stack has buffer available and can accept data.
			* This event is used by application to prevent pushing lot of data to stack. */
			
			/* Extract the present stack status */
            busyStatus = * (uint8*)eventParam;
            break;
			
        default:

       	 	break;
    }   	/* switch(event) */
}

/*******************************************************************************
* Function Name: SendDataOverMouseNotification
********************************************************************************
* Summary:
*        Send Mouse Slider data as BLE Notifications. This function updates
* the notification handle with data and triggers the BLE component to send 
* notification
*
* Parameters:
*  MouseSliderData:	Mouse slider value	
*
* Return:
*  void
*
*******************************************************************************/
void SendDataOverMouseNotification(uint8 *pbMouseData)
{
	/* 'MousenotificationHandle' stores Mouse notification data parameters */
	CYBLE_GATTS_HANDLE_VALUE_NTF_T		MousenotificationHandle;	
	
	/* If stack is not busy, then send the notification */
	if(busyStatus == CYBLE_STACK_STATE_FREE)
	{
		/* Update notification handle with Mouse slider data*/
		MousenotificationHandle.attrHandle = CYBLE_MOUSE_SERVICE_MOUSE_DATA_CHAR_HANDLE;				
		MousenotificationHandle.value.val = pbMouseData;
		MousenotificationHandle.value.len = MOUSE_NTF_DATA_LEN;
		
		/* Send the updated handle as part of attribute for notifications */
		CyBle_GattsNotification(connectionHandle,&MousenotificationHandle);
	}
}


/*******************************************************************************
* Function Name: UpdateConnectionParam
********************************************************************************
* Summary:
*        Send the Connection Update Request to Client device after connection 
* and modify theconnection interval for low power operation.
*
* Parameters:
*	void
*
* Return:
*  void
*
*******************************************************************************/
void UpdateConnectionParam(void)
{
	/* If device is connected and Update connection parameter not updated yet,
	* then send the Connection Parameter Update request to Client. */
    if(deviceConnected && isConnectionUpdateRequested)
	{
		/* Reset the flag to indicate that connection Update request has been sent */
		isConnectionUpdateRequested = FALSE;
		
		/* Send Connection Update request with set Parameter */
		CyBle_L2capLeConnectionParamUpdateRequest(connectionHandle.bdHandle, &ConnectionParam);
	}
}

/*******************************************************************************
* Function Name: HandleStatusLED
********************************************************************************
* Summary:
*        Handle LED status, such as blinking, ON or OFF depending on BLE state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void HandleStatusLED(void)
{
	/* Local static counter to handle the periodic toggling of LED or keeping LED ON
	* for some time. */
	static uint32 led_counter = TRUE;
	
	/* Local static variable that stores the last BLE state in which firmware was */
	static uint8 state = 0xFF;
	
	/* Flag to indicate that the state of BLE has changed from the last known value */
	uint8 state_changed = FALSE;
	
	if(state != CyBle_GetState())
	{
		/* If the present BLE state is different from the new BLE state, set the 
		* state_changed flag and reset the local counter */
		state_changed = TRUE;
				
		if(CyBle_GetState() == CYBLE_STATE_ADVERTISING)
		{
			led_counter = TRUE;
		}
	}
	
	/* Store the new BLE state into the present state variable */
	state = CyBle_GetState();
		
	switch(state)
	{
		case CYBLE_STATE_CONNECTED:
			/* If the present BLE state is connected, keep the LED ON for
			* pre-determined time and then switch it OFF in WDT ISR */
			if(state_changed)
			{
				/* Reset the flag for state change */
				state_changed = FALSE;
				
				RED_ON;
				//MouseWheel_Start();
				/* Set flag and counter for Connection indication on LED */
				switch_off_status_led = TRUE;
				timer_tick = LED_CONN_ON_TIME;
				
				/* Initialize the Watchdog for 1 second timing events. The watcdog 
				* is initialized after connection and disabled after disconnection.
				* This is done to ensure that Watchdog provided periodic wakeup only
				* when the system is connected and RGB LED control is required. During 
				* other times, it should not wakeup the syste, causing increase in power
				* consumption */
				InitializeWatchdog();
			}
		break;
		
		case CYBLE_STATE_ADVERTISING:
			/* If the present BLE state is advertising, toggle the LED
			* at pre-determined period to indicate advertising. */
			if((--led_counter) == FALSE)
			{
				/* Toggle Status LED for indicating Advertisement */
                static uint8_t LED_State=0;
                LED_State ^= 0xFF;
				if(LED_State)
				{
					RED_ON;
					led_counter	= LED_ADV_BLINK_PERIOD_ON;
				}
				else
				{
					RED_OFF;
					led_counter	= LED_ADV_BLINK_PERIOD_OFF;
				}
			}
		break;
		
		case CYBLE_STATE_DISCONNECTED:
            GoHypernate();


		break;
		
		default:

		break;
	}
	
	/* Reset the state changed flag. */
	state_changed = FALSE;
}

/*******************************************************************************
* Function Name: WDT_INT_Handler
********************************************************************************
* Summary:
*        Watchdog interrupt routine for controlling LED status in connected state
*       It also checks the Key-States (CY_SYS_WDT_COUNTER1_INT)
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void WDT_INT_Handler(void)
{
	/* If the Interrupt source is Counter 0 match, then process */
    uint32 uiSource = CySysWdtGetInterruptSource();
	if( uiSource & CY_SYS_WDT_COUNTER0_INT )
	{
		/* Clear Watchdog Interrupt from Counter 0 */
		CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
		
		/* If switch_off_status_led is TRUE, then the Connection status LED 
		* is ON and it is required to shut it down after 3 seconds */
		if(switch_off_status_led)
		{
			/* If timer ticks for Connection ON LED period has expired, then switch
			* off LED */
			if(timer_tick == FALSE)
			{
				/* Switch of Status LED */
                RED_OFF;
				
				/* Reset the Flag */
				switch_off_status_led = FALSE;
				
			}
			else
			{
				/* Decrement timer_tick as counting method */
				timer_tick--;
			}
		}
		
//		/* Unlock the WDT registers for modification */
//		CySysWdtUnlock();
//		
//		/* Disable Counter 0 to allow modifications */
//		CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK);
//		
//		/* Reset Counter 0 and give ~3 LFCLK cycles to take effect */
//		CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
//		CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
//		
//		/* Write the Counter 0 match value for 1 second and give ~3 LFCLK
//		* cycles to take effect */
//		CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WATCHDOG_ONE_SEC_COUNT_VAL);
//		CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
//		
//		/* Enable Watchdog Counter 0 */
//		CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
//		
//		/* Lock Watchdog to prevent any further change */
//	    CySysWdtLock();
	}
    // Dieser Teil wird bei einer bestehenden BLE-Verbindung (Notify enabled
    // alle 20ms ausfgerufen.
    if ( uiSource & CY_SYS_WDT_COUNTER1_INT )
    {
        CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER1_INT);

        static uint8 bCounter=2;
        
        bCounter--;
        if (bCounter==1)
            RED_ON;
        else if(bCounter==0)
        {
            RED_OFF;
            bCounter=250;       // 250 * 20ms = 5 Sekunden
        }
        
        gbCurrentKeys = 0;
        if (Buttons_Read() & 1)
            gbCurrentKeys = 1;
        if (Mid_Button_Read() == 0)
            gbCurrentKeys |= 2;
        if (Buttons_Read() & 2)
            gbCurrentKeys |= 4;   
    }
}

/*******************************************************************************
* Function Name: InitializeWatchdog
********************************************************************************
* Summary:
*        Reset watchdog timer
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void InitializeWatchdog(void)
{
	/* Unlock the WDT registers for modification */
	CySysWdtUnlock(); 
	
	/* Write Mode for Counter 0 as Interrupt on Match */
    CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
	
	/* Set Clear on Match for Counter 0*/
	CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, TRUE);
    
	/* Set Watchdog interrupt to lower priority */
	CyIntSetPriority(WATCHDOG_INT_VEC_NUM, WATCHDOG_INT_VEC_PRIORITY);
	
	/* Enable Watchdog Interrupt using Interrupt number */
    CyIntEnable(WATCHDOG_INT_VEC_NUM);
	
	/* Write the match value equal to 1 second in Counter 0 */
	CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WATCHDOG_ONE_SEC_COUNT_VAL);
    
	/* Enable Counter 0 */
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
	
	/* Lock Watchdog to prevent further changes */
    CySysWdtLock();
}
/* [] END OF FILE */
