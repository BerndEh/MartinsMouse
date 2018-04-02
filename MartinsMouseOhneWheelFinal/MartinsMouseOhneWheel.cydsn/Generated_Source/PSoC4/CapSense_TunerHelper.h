/*******************************************************************************
* File Name: Mouse_TunerHelper.h
* Version 2.60
*
* Description:
*  This file provides constants and structure declarations for the tuner helper
*  APIs for the Mouse CSD component.
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_Mouse_CSD_TUNERHELPER_Mouse_H)
#define CY_Mouse_CSD_TUNERHELPER_Mouse_H

#include "Mouse.h"

#if(0u != Mouse_CSHL_API_GENERATE)
    #include "Mouse_CSHL.h"
#endif /* (0u != Mouse_CSHL_API_GENERATE) */

#if (Mouse_TUNER_API_GENERATE)
    #include "Mouse_MBX.h"
#endif /* (Mouse_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define Mouse_DEFAULT_MAILBOXES_NUMBER       (1u)

#define Mouse_CENTROID_POSITION_BUFFER_SIZE  (2u)
#define Mouse_MB_POSITION_BUFFER_SIZE        (2u)

#define Mouse_DEFAULT_TRACKPAD_INDEX         (0u)

#define Mouse_FIRST_FINGER_INDEX             (0u)
#define Mouse_SECOND_FINGER_INDEX            (1u)

#define Mouse_FIRST_FINGER_X_INDEX           (0u)
#define Mouse_FIRST_FINGER_Y_INDEX           (1u)

#define Mouse_SECOND_FINGER_X_INDEX          (2u)
#define Mouse_SECOND_FINGER_Y_INDEX          (3u)

#define Mouse_GESTURE_TYPE_MASK              (0xF0u)
#define Mouse_INERTIAL_SCROLL                (0xB0u)
#define Mouse_NON_INERTIAL_SCROLL            (0xC0u)


/***************************************
*        Function Prototypes
***************************************/

void Mouse_TunerStart(void);
void Mouse_TunerComm(void);

#if (Mouse_TUNER_API_GENERATE)
   Mouse_NO_STRICT_VOLATILE void Mouse_ProcessAllWidgets(volatile Mouse_OUTBOX *outbox);
#endif /* (Mouse_TUNER_API_GENERATE) */


/***************************************
*     Vars with External Linkage
***************************************/
#if (Mouse_TUNER_API_GENERATE)
    extern volatile Mouse_MAILBOXES Mouse_mailboxesComm;
#endif /* (Mouse_TUNER_API_GENERATE) */

#endif  /* (CY_Mouse_CSD_TUNERHELPER_Mouse_H)*/


/* [] END OF FILE */
