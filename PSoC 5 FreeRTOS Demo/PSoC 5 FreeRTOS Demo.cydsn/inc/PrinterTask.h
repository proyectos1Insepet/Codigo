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
#ifndef __PRINTERTASK_H
#define __PRINTERTASK_H
/*  INCLUDES  */
/* Project and default includes */
#include <project.h>
#include <stdio.h>
    
/* RTOS includes. */
#include <FreeRTOS.h>
#include <semphr.h>
#include "os_resource.h"
    
void PrinterTask(void *arg);
    
#endif

/* [] END OF FILE */
