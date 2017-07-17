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
    uint8 EEpromGradeAddress;
    
    //setup();
    
    while(1) 
    {       
        while(RF_Connection_GetRxBufferSize() > 0)  //&& (RF_Connection_RX_STS_FIFO_NOTEMPTY)
        {
            
            buffer_rfTMP = RF_Connection_ReadRxData();
        
            if (buffer_rfTMP == 0xBC)
            {
                i = 0;
            }
            // Status
            if( buffer_rf[6] == 0xA1)
            {
                LongEsperada = 8;              
            }
            // PPU Setting
            else if ( buffer_rf[6] == 0xA6)
            {
                LongEsperada = 15;
            }
            // Totals request
            else if ( buffer_rf[6] == 0xA5)
            {
                LongEsperada = 8;
            }
            
            else if ( buffer_rf[6] == 0xA3)
            {
                LongEsperada = 25;
            }
            else if ( buffer_rf[6] == 0xA9)
            {
                LongEsperada = 19;
            }
            // big config
            else if ( buffer_rf[6] == 0xE1)
            {
                LongEsperada = 140;
            }
            
//            else if ( buffer_rf[6] == 0xE2)
//            {
//                LongEsperada = 14;
//            }
            //Short Config 
//            else if ( buffer_rf[6] == 0xAD)
//            {
//                LongEsperada = 15;
//            }
            // Turno
            else if ( buffer_rf[6] == 0xE4)
            {
                LongEsperada = 9;
            }
            else
            {
                LongEsperada = 400;
            }
                                
            buffer_rf[i] = buffer_rfTMP;
            //buffer_rf[1] = 0xCB;
            
            if (i == LongEsperada - 1)
            {
                //RF_Connection_PutString(buffer_rf);
                RF_Connection_ClearRxBuffer();
                pollingRF_Rx(buffer_rf);
                buffer_rf[6] = 0xFF;
            }
            
            i++;                       
        }               
            pollingRFA_Tx();
            pollingRFB_Tx();
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);    

}







/* [] END OF FILE */
