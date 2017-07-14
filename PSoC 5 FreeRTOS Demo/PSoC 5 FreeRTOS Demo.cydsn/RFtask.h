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

#ifndef __RF_TASK_H
#define __RF_TASK_H

/*  INCLUDES  */
/* Project and default includes */
#include <project.h>
#include <stdio.h>
    
/* RTOS includes. */
#include <FreeRTOS.h>
#include <semphr.h>
#include "os_resource.h"
/* Drivers */


/** @brief RF thread */
void RF_Task(void *arg);


#endif

/* [] END OF FILE */
