/* ========================================
 *
 * Copyright CLS LED, 2015
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF CLS LED.
 *
 * ========================================
*/
/**
 * @file main.c
 * @brief Entry point and RTOS resources
 * @author Ingmar Delsink
 */

/*  INCLUDES  */
/* Project and default includes */
#include <project.h>
#include <protocol.h>
#include <variables.h>
#include <keyboard.h>
#include <ibutton.h>
#include <RFPoll.h>
#include <LCD.h>
#include <I2C.h>
#include <Printing.h>

/* RTOS includes. */
#include <FreeRTOS.h>
#include "os_mutex.h"
#include "os_resource.h"

/* Task includes */
#include "RFtask.h"
#include "PumpTask.h"

xSemaphoreHandle g_pUARTSemaphore;
      
//CY_ISR(animacion2);
//CY_ISR(animacion);

void GlobalInitializer(){
    EEPROM_1_Start();
    Pump_Start(); 
    Display1_Start();
    Display2_Start();    
    Printer_Start();   
    I2C_Bus_Start();
    RF_Connection_Start();
//    isr_3_StartEx(animacion);
//    isr_4_StartEx(animacion2);    
}

/* 
 * Almacena la configuracion de puntos decimales y posiciones detectadas 
*/
void StoreConfiguration(){
    WriteEeprom(0,UnitTypeA);
    WriteEeprom(1,ConversionFactorA);
    WriteEeprom(2,MoneyDecA);
    WriteEeprom(3,VolDecA);
    WriteEeprom(4,PPUDecA);    
    WriteEeprom(5,DDModeA);
}

/*
 * Carga las configuraciones desde la memoria EEPROM
*/
void loadConfiguration(){      
    uint8 x;
    LeerEeprom(0,1);
    UnitType = buffer_i2c[0];
    LeerEeprom(1,1);
    ConversionFactor = buffer_i2c[0];
    LeerEeprom(2,1);
    MoneyDec = buffer_i2c[0];
    LeerEeprom(3,1);
    VolDec = buffer_i2c[0];
    LeerEeprom(4,1);
    PPUDec = buffer_i2c[0];
    LeerEeprom(5,1);
    DDMode = buffer_i2c[0];
    LeerEeprom(6,1);
    lockTurn[0] = buffer_i2c[0];
    LeerEeprom(7,1);
    printPortA[0] = buffer_i2c[0];  //Puertos de impresion
    LeerEeprom(8,1);
    printPortB[0] = buffer_i2c[0];  //Puertos de impresion
    LeerEeprom(9,2);
    IDCast[0]  = buffer_i2c[0]; //ID Estacion1
    IDCast[1]  = buffer_i2c[1]; //ID Estacion2    
    ////// CONFIGURACION GENERAL /////////////
    LeerEeprom(11,30);
    for(x = 0; x <30; x++)
    {
        Encabezado1[x] = buffer_i2c[x];
    } 
    LeerEeprom(41,30);
    for(x = 0; x <30; x++)
    {
        Encabezado2[x] = buffer_i2c[x];
    }
    LeerEeprom(71,30);
    for(x = 0; x <30; x++)
    {
        Encabezado3[x] = buffer_i2c[x];
    }
    LeerEeprom(101,30);
    for(x = 0; x <30; x++)
    {
        Encabezado4[x] = buffer_i2c[x];
    }
    LeerEeprom(131,30);
    for(x = 0; x <30; x++)
    {
        Encabezado5[x] = buffer_i2c[x];
    }
    LeerEeprom(161,30);
    for(x = 0; x <30; x++)
    {
        Pie1[x] = buffer_i2c[x];
    }
    LeerEeprom(191,30);
    for(x = 0; x <30; x++)
    {
        Pie2[x] = buffer_i2c[x];
    }
    LeerEeprom(221,30);
    for(x = 0; x <30; x++)
    {
        Pie3[x] = buffer_i2c[x];
    }
    LeerEeprom(251,16);
    for(x = 0; x <16; x++)
    {
        Product1[x] = buffer_i2c[x];
    }
    LeerEeprom(267,16);
    for(x = 0; x <16; x++)
    {
        Product2[x] = buffer_i2c[x];
    }
    LeerEeprom(283,16);
    for(x = 0; x <16; x++)
    {
        Product3[x] = buffer_i2c[x];
    }
    LeerEeprom(299,16);
    for(x = 0; x <16; x++)
    {
        Product4[x] = buffer_i2c[x];
    }
    configAccess[0] = 0x04; 
    PrinterType = 1; // Tipo de impresora
}
void console(void)
{
    Pump_PutChar(0x10 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x30 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x10 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x30 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x10 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x30 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x10 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x30 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x10 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x30 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x10 | 0x03);
    CyDelay(100);
    Pump_PutChar(0x30 | 0x03);
    CyDelay(100);
}
/*
*********************************************************************************************************
*                                         void InitPump(void)
*
* Description : Pregunta configuraciones al dispensador 
*               
*
*********************************************************************************************************
*/
void InitPump(){   
    
    //Detecta las posiciones del surtidor
    NumPositions = get_position();
    //console();
    if(NumPositions > 0)
    {
        
        // Pregunta si tiene un estado valido
        if(!PumpIsInValidState(get_state(side.a.dir)))
        {
            SetPicture(1,DISPLAY_ERROR);
            SetPicture(2,DISPLAY_ERROR);
            NumPositions = 0;
        }
        if(!PumpIsInValidState(get_state(side.b.dir)))
        {
            SetPicture(1,DISPLAY_ERROR);
            SetPicture(2,DISPLAY_ERROR);
            NumPositions = 0;
        }
        
        if(PumpIsInValidState(get_state(side.a.dir)) && PumpIsInValidState(get_state(side.b.dir)))
        {      
            NumPositions = get_position();
            
            if(get_state(side.a.dir) == 0x06 && get_state(side.b.dir) == 0x06)
            {
                PumpCompleteConfiguration(side.a.dir);
                StoreConfiguration();
            }else
            {
                SetPicture(1,DISPLAY_BAJE_MANIJA);
                SetPicture(2,DISPLAY_BAJE_MANIJA);           
                InitPump();                
            }
        }        
    }
}

/*----------------------------------------------------------------------------
   Main: Initialize and start Kernel
*---------------------------------------------------------------------------*/
int main()
{	  

    /* Drivers                                                                          */
    CySysTickStart();
    
    /* ISR                                                                              */
    CyGlobalIntEnable;                          /* Init the interrupts                  */
    GlobalInitializer();
    
    /* Init Pump                                                                        */       
    InitPump(); 
    loadConfiguration();
    
    /* OS Init                                                                          */
    osInit();                               /* Initialize all thread related tasks      */
    prvHardwareSetup();                     /* FreeRTOS setup                           */
	vTaskStartScheduler();                  /* Start the scheduler                      */
    
    g_pUARTSemaphore = xSemaphoreCreateMutex();  /* Initialize the UARTg Mutex          */
        
	return 1;
}

int osInit(void)
{
    /*  tasks  */
    xTaskCreate(RF_Task, "RFTask", (configMINIMAL_STACK_SIZE), NULL, tskIDLE_PRIORITY + 2, NULL);            /* RF TASK         */
    xTaskCreate(Pump_Task, "PumpTask", (configMINIMAL_STACK_SIZE), NULL, tskIDLE_PRIORITY + 2, NULL);        /* PUMP TASK       */
    xTaskCreate(Display_Task, "DisplayTask", (configMINIMAL_STACK_SIZE), NULL, tskIDLE_PRIORITY + 2, NULL);  /* DISPLAY TASK    */
    
    return 1;// All went well
}

int osDeInit(void)
{
    return 1;
}

/* [] END OF FILE */
