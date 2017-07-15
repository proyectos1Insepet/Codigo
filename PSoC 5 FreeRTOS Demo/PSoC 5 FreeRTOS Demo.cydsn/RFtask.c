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

/*  INCLUDES  */
/* Project and default includes */
#include <project.h>
#include <stdio.h>
/* RTOS includes. */
#include <FreeRTOS.h>
#include <semphr.h>
#include <protocol.h>
#include <variables.h>
#include <keyboard.h>
#include <ibutton.h>
#include <RFPoll.h>
#include <LCD.h>
#include <I2C.h>
#include <Printing.h>
#include "os_resource.h"
/* Drivers */
#include "RFtask.h"

/** @brief RF Task */
void RF_Task(void *arg)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;
    xLastWakeTime = xTaskGetTickCount();
    
    
    //setup();
    
    while(1) 
    {        
        pollingRF_Rx();
        pollingRFA_Tx();
        pollingRFB_Tx();
        //vTaskDelayUntil(&xLastWakeTime, xFrequency);    
	}
}







/* [] END OF FILE */
