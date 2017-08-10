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

#ifndef __PUMP_TASK_H
#define __PUMP_TASK_H

/*  INCLUDES  */
/* Project and default includes */
#include <project.h>
#include <stdio.h>
    
/* RTOS includes. */
#include <FreeRTOS.h>
#include <semphr.h>
#include "os_resource.h"
    
/* Drivers */

  

/** @brief Red LED blink demo thread */
void Pump_Task(void *arg);
void Display_Task(void *arg);
void Total_Task(void *arg);
void ReadPPUFromEEpromInit();


#endif

/* [] END OF FILE */
