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
    uint16 i,x,y,contEscape;  
    uint8 buffer_rfTMP;
    uint8 GeneralConfig;
    
    //uint8 EEpromGradeAddress;
    
    //setup();
    
    while(1) 
    {       
        while(RF_Connection_GetRxBufferSize() > 0)  //&& (RF_Connection_RX_STS_FIFO_NOTEMPTY)
        {
            
            buffer_rfTMP  = RF_Connection_ReadRxData();
            GeneralConfig = 0;
            if (buffer_rfTMP == 0xBC)
            {
                i = 0;
            }
            
            // Status
            if( buffer_rf[6] == 0xA1)
            {
                LongEsperada = 8;              
            }
            else if ( buffer_rf[6] == 0xA3)
            {
                LongEsperada = 25;
            }
            
            // Totals request
            else if ( buffer_rf[6] == 0xA5)
            {
                LongEsperada = 8;
            }
            
            // PPU Setting
            else if ( buffer_rf[6] == 0xA6)
            {
                LongEsperada = 15;
            }
            
            // Impresion general   
            else if ( buffer_rf[6] == 0xA7)
            {
                if(buffer_rf[8] != 0x00)
                {
                    if(buffer_rf[8] + 0x09 < 234)
                    {
                        LongEsperada = buffer_rf[8] + 0x09;
                        vTaskDelay( 1 / portTICK_PERIOD_MS );
                    }
                    else
                    {
                        LongEsperada = 234;
                    }
                }
            } 
            
            else if ( buffer_rf[6] == 0xA9)
            {
                LongEsperada = 19;
            }
            
            // big config
            else if ( buffer_rf[6] == 0xE1)
            {
                LongEsperada   = 337;
                GeneralConfig  = 1;
            }
            
            else if ( buffer_rf[6] == 0xE2)
            {
                LongEsperada = 14;
            }
   
            // Turno
            else if ( buffer_rf[6] == 0xE4)
            {
                LongEsperada = 9;
            }
            else if ( buffer_rf[6] == 0xA8)
            {
                LongEsperada = 117;
            }
                                
            buffer_rf[i] = buffer_rfTMP;         
            
            if (i == LongEsperada - 1)
            {                
                RF_Connection_ClearRxBuffer();
                
                if(buffer_rf[5] == side.a.dir || buffer_rf[5] == side.b.dir || buffer_rf[5] == side.c.dir|| buffer_rf[5] == side.d.dir || GeneralConfig == 1)
                {
                   pollingRF_Rx(buffer_rf);
                   RFOnline = 1; 
                }
                else
                {
                   RFOnline = 0;
                }  
                
                buffer_rf[6] = 0xFF;
                RF_Connection_ClearRxBuffer();
                          
                
                break;
            }
             
            i++;                       
        }               
         
        if(RFOnline == 1)
        {
            pollingRFA_Tx();           
            pollingRFB_Tx();
            if(NumPositions > 2)
            {
                pollingRFC_Tx();
                pollingRFD_Tx();
            }
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);    
        
    }
        
       

}







/* [] END OF FILE */
