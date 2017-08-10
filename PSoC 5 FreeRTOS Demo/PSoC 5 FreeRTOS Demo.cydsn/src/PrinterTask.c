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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "os_resource.h"

#include "PrinterTask.h"



void PrinterTask(void *arg)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;
    xLastWakeTime = xTaskGetTickCount();
    uint8 x; 
 
    
    while(1) 
    {                                               
        if(bufferDisplay1.flagPrint == 1 && bufferDisplay1.PrintFlagEOT == 1)
        {    
            //Retardo para espera de datos del tiquete desde el mux
            vTaskDelay( 5000 / portTICK_PERIOD_MS );                
            //Impresion
            imprimir(printPortA, side.a.dir);
            bufferDisplay1.flagPrint = 0;            
            bufferDisplay1.PrintFlagEOT = 0;
            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
            {   										    							
		        side.a.saleNumber[x] = 0x00;
	        } 
            bufferDisplay1.flagActiveSale = false;
            flowDisplay1 = 0;
            //SetPicture(1, DISPLAY_INICIO0);
        }
        if(bufferDisplay2.flagPrint == 1 && bufferDisplay2.PrintFlagEOT == 1)
        {    
            //Retardo para espera de datos del tiquete desde el mux
            vTaskDelay( 5000 / portTICK_PERIOD_MS );
            //Impresion
            imprimir(printPortB, side.b.dir);
            bufferDisplay2.flagPrint = 0;             
            bufferDisplay2.PrintFlagEOT = 0;            
            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
            {   										    							
		        side.b.saleNumber[x] = 0x00;
	        }
            bufferDisplay2.flagActiveSale = false;
            flowDisplay2 = 0;
            //SetPicture(2,DISPLAY_INICIO0);
        }
        if(bufferDisplay3.flagPrint == 1 && bufferDisplay3.PrintFlagEOT == 1)
        {    
            //Retardo para espera de datos del tiquete desde el mux
            vTaskDelay( 5000 / portTICK_PERIOD_MS );                
            //Impresion
            imprimir(printPortA, side.c.dir);
            bufferDisplay3.flagPrint = 0;            
            bufferDisplay3.PrintFlagEOT = 0;
            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
            {   										    							
		        side.c.saleNumber[x] = 0x00;
	        }               
            bufferDisplay3.flagActiveSale = false;
            flowDisplay3 = 0;
        }
        if(bufferDisplay4.flagPrint == 1 && bufferDisplay4.PrintFlagEOT == 1 )
        {    
            //Retardo para espera de datos del tiquete desde el mux
            vTaskDelay( 5000 / portTICK_PERIOD_MS );                
            //Impresion
            imprimir(printPortB, side.d.dir);
            bufferDisplay4.flagPrint = 0;
            bufferDisplay4.PrintFlagEOT = 0;                            
            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
            {   										    							
		        side.d.saleNumber[x] = 0x00;
	        }
            bufferDisplay4.flagActiveSale = false;
            flowDisplay4 = 0;
        }                   
        vTaskDelayUntil(&xLastWakeTime, xFrequency);            
    }
        
}

/* [] END OF FILE */
