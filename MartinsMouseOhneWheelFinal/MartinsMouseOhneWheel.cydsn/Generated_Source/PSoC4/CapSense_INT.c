/*******************************************************************************
* File Name: Mouse_INT.c
* Version 2.60
*
* Description:
*  This file provides the source code of the  Interrupt Service Routine (ISR)
*  for the Mouse CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Mouse.h"
#include "Mouse_PVT.h"



/*******************************************************************************
*  Place your includes, defines and code here
********************************************************************************/
/* `#START Mouse_ISR_INTC` */

/* `#END` */


/*******************************************************************************
* Function Name: Mouse_ISR
********************************************************************************
*
* Summary:
*  This ISR is executed when the measure window is closed. The measure window
*  depends on the sensor scan resolution parameter.
*  The ISR has two modes:
*   1. Scans a single sensor and stores the measure result.
*   2. Stores the result of the current measure and starts the next scan till all
*      the enabled sensors are scanned.
*  This interrupt handler is only used in one channel designs.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Mouse_csv - used to provide the status and mode of the scanning process.
*   Mode - a single scan or scan of all the enabled sensors.
*   Status - scan is in progress or ready for new scan.
*  Mouse_sensorIndex - used to store a sensor scanning sensor number.
*
* Reentrant:
*  No
*
*******************************************************************************/
CY_ISR(Mouse_ISR)
{
    static uint8 Mouse_snsIndexTmp;

    #ifdef Mouse_ISR_ENTRY_CALLBACK
        Mouse_ISR_EntryCallback();
    #endif /* Mouse_ISR_ENTRY_CALLBACK */

    /*  Place your Interrupt code here. */
    /* `#START Mouse_ISR_ENTER` */

    /* `#END` */

    CyIntDisable(Mouse_ISR_NUMBER);

    Mouse_CSD_INTR_REG = 1u;

    Mouse_PostScan((uint32)Mouse_sensorIndex);

    if ((Mouse_csdStatusVar & Mouse_SW_CTRL_SINGLE_SCAN) != 0u)
    {
        Mouse_csdStatusVar &= (uint8)~Mouse_SW_STS_BUSY;
    }
    else
    {
        /* Proceed scanning */
        Mouse_sensorIndex = Mouse_FindNextSensor(Mouse_sensorIndex);

        /* Check end of scan */
        if(Mouse_sensorIndex < Mouse_TOTAL_SENSOR_COUNT)
        {
            Mouse_PreScan((uint32)Mouse_sensorIndex);
        }
        else
        {
            Mouse_csdStatusVar &= (uint8)~Mouse_SW_STS_BUSY;

             /* Check if widget has been scanned */
            if((Mouse_csdStatusVar & Mouse_SW_CTRL_WIDGET_SCAN) != 0u)
            {
                /* Restore sensorEnableMask array */
                for(Mouse_snsIndexTmp = 0u;
                    Mouse_snsIndexTmp < Mouse_TOTAL_SENSOR_MASK;
                    Mouse_snsIndexTmp++)
                {
                    /* Backup sensorEnableMask array */
                    Mouse_sensorEnableMask[Mouse_snsIndexTmp] = Mouse_sensorEnableMaskBackup[Mouse_snsIndexTmp];
                }
            }
        }
    }
    CyIntEnable(Mouse_ISR_NUMBER);

    /*  Place your Interrupt code here. */
    /* `#START Mouse_ISR_EXIT` */

    /* `#END` */

    #ifdef Mouse_ISR_EXIT_CALLBACK
        Mouse_ISR_ExitCallback();
    #endif /* Mouse_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
