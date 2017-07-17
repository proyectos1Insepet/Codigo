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
      
CY_ISR(animacion2);
CY_ISR(animacion);

void GlobalInitializer(){
    EEPROM_1_Start();
    Pump_Start(); 
    Display1_Start();
    Display2_Start();    
    Printer_Start();   
    I2C_Bus_Start();
    RF_Connection_Start();
    isr_3_StartEx(animacion);
    isr_4_StartEx(animacion2);    
}

/* 
 * Almacena la configuracion de puntos decimales y posiciones detectadas 
*/
void StoreConfiguration(){
    EEPROM_1_WriteByte(UnitType,0);
    EEPROM_1_WriteByte(ConversionFactor,1);
    EEPROM_1_WriteByte(MoneyDec,2);
    EEPROM_1_WriteByte(VolDec,3);
    EEPROM_1_WriteByte(PPUDec,4);
    EEPROM_1_WriteByte(DDMode,5);
    EEPROM_1_WriteByte(digits,6); 
    EEPROM_1_WriteByte(side.a.dir,12);
    EEPROM_1_WriteByte(side.b.dir,13);
    EEPROM_1_WriteByte(side.c.dir,14);
    EEPROM_1_WriteByte(side.d.dir,15);
}

/*
 * Carga las configuraciones desde la memoria EEPROM
*/
void loadConfiguration(){
  
    MoneyDec   = EEPROM_1_ReadByte(2);  //Punto decimal dinero
    VolDec     = EEPROM_1_ReadByte(3);  //Punto decimal volumen
    PPUDec     = EEPROM_1_ReadByte(4);  //Punto decimal PPU
    DDMode     = EEPROM_1_ReadByte(5);  //Punto decimal 
    digits     = EEPROM_1_ReadByte(6);  //Digitos
    lockTurn   = EEPROM_1_ReadByte(7);  //Fijo turno abierto para pruebas
    printPortA = EEPROM_1_ReadByte(8);  //Puertos de impresion
    printPortB = EEPROM_1_ReadByte(9);  //Puertos de impresion
    IDCast[0]  = EEPROM_1_ReadByte(10); //ID Estacion1
    IDCast[1]  = EEPROM_1_ReadByte(11); //ID Estacion2    
    
    configAccess[0] = 0x04; 
    side.a.dir = EEPROM_1_ReadByte(12); //Primera posicion
    side.b.dir = EEPROM_1_ReadByte(13); //Segunda posicion
    side.c.dir = EEPROM_1_ReadByte(14); //Tercera posicion
    side.d.dir = EEPROM_1_ReadByte(15); //Cuarta posicion

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
/*
*********************************************************************************************************
*                                         CY_ISR(animacion)
*
* Description : Interrupcion que temporiza las imagenes informativas que aparecen en la pantalla 1
*               
*
*********************************************************************************************************
*/
CY_ISR(animacion){
    Timer_Animacion_ReadStatusRegister();    					
    count_protector++; 							//Incrementa el contador 
}

/*
*********************************************************************************************************
*                                         CY_ISR(animacion2)
*
* Description : Interrupcion que temporiza las imagenes informativas que aparecen en la pantalla 2
*               
*
*********************************************************************************************************
*/
CY_ISR(animacion2){
    Timer_Animacion2_ReadStatusRegister();
    count_protector2++;
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
