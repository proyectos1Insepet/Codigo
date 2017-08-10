/*
*********************************************************************************************************
*                                         MUX ADVANCE CODE
*
*                             (c) Copyright 2016; Sistemas Insepet LTDA
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            MUX ADVANCE CODE
*
*                                             CYPRESS PSoC5LP
*                                                with the
*                                            CY8C5969AXI-LP035
*
* Filename      : keyboard.c
* Version       : V1.00
* Programmer(s) : 
                  
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <device.h>
#include "variables.h"
#include <LCD.h>
#include "LCD.h"

/* RTOS includes. */
#include <FreeRTOS.h>
#include <semphr.h>
#include "os_resource.h"

/*
*********************************************************************************************************
*               uint8 alphanumeric_keyboard(uint8 lcd, uint8 maxNumberKeys, uint8 character)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   :  
*
* Note(s)     : none.
*********************************************************************************************************
*/

uint8 alphanumeric_keyboard( uint8 maxNumberKeys, uint8 character){
    char8 PressedKey = 0;    
    if(Display1_GetRxBufferSize() == 8)
    {
        if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
        {                
            if(numberKeys1 <= (maxNumberKeys - 1))
            {
                PressedKey = UnmapCode(Display1_rxBuffer[3]);
                if(PressedKey >= 0x20 && PressedKey <= 0x2B) //Comandos ascii sin incluir coma (,)
                { 
                    numberKeys1++;  
                    bufferDisplay1.valueKeys[numberKeys1] = PressedKey;                        
                    if(character == 0)
                    {
                        WriteLCD(1,bufferDisplay1.valueKeys[numberKeys1],3,numberKeys1+2,1,0x0000,'N');
                    }else{
                        WriteLCD(1,character,3,numberKeys1+2,1,0x0000,'N');
                    }
                }else if(PressedKey >= 0x2D && PressedKey <= 0x7E) //Comandos ascii sin incluir coma (,)
                { 
                    numberKeys1++;  
                    bufferDisplay1.valueKeys[numberKeys1] = PressedKey;
                    if(character == 0)
                    {
                        WriteLCD(1,bufferDisplay1.valueKeys[numberKeys1],3,numberKeys1+2,1,0x0000,'N');
                    }else{
                        WriteLCD(1,character,3,numberKeys1+2,1,0x0000,'N');
                    }
                }else if(PressedKey == ',' && numberKeys1 >= 1 && flagPoint1 == 0) //Coma (,)
                {  
                    numberKeys1++;  
                    flagPoint1 = 1;
                    bufferDisplay1.valueKeys[numberKeys1] = PressedKey;
                    if(character == 0)
                    {
                        WriteLCD(1,bufferDisplay1.valueKeys[numberKeys1],3,numberKeys1+2,1,0x0000,'N');
                    }else{
                        WriteLCD(1,character,3,numberKeys1+2,1,0x0000,'N');
                    }
                }
            }  
            if(Display1_rxBuffer[3] == 0x0B) //Borrar
            { 
                if(numberKeys1 == 0) //Si no ha presionado nada regresa al menu anterior
                {		
                    vTaskDelay( 20 / portTICK_PERIOD_MS );        
                    Display1_ClearRxBuffer();
                    return 0;
                }
                else{
                    WriteLCD(1,' ',3,numberKeys1+2,1,0x0000,'N');	//Si ya presiono borra el dato	
                    if(bufferDisplay1.valueKeys[numberKeys1] == 0x2C)
                    {
                        flagPoint1=0;
                    }
                    numberKeys1--;
                }
            }
            if(PressedKey == 0x0A) //Enter
            { 
                if(numberKeys1 > 0) 
                {
                    bufferDisplay1.valueKeys[0] = numberKeys1;
                    //CyDelay(20);
                    vTaskDelay( 20 / portTICK_PERIOD_MS );
                    Display1_ClearRxBuffer();
                    return 1;
                }
            }
            if(Display1_rxBuffer[3] == 0x94) //Atras
            { 
                //CyDelay(20);
                vTaskDelay( 20 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
                return 0;
            }
        } 
        //CyDelay(20);
        vTaskDelay( 20 / portTICK_PERIOD_MS );
        Display1_ClearRxBuffer();
    }    
    return 2;
}

uint8 alphanumeric_keyboard2( uint8 maxNumberKeys, uint8 character){
    char8 PressedKey = 0;    
    if(Display2_GetRxBufferSize() == 8)
    {
        if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
        {                
            if(numberKeys2 <= (maxNumberKeys - 1))
            {
                PressedKey = UnmapCode(Display2_rxBuffer[3]);
                if(PressedKey >= 0x20 && PressedKey <= 0x2B)  //Comandos ascii sin incluir coma (,)
                { 
                    numberKeys2++;  
                    bufferDisplay2.valueKeys[numberKeys2] = PressedKey;                        
                    if(character == 0)
                    {
                        WriteLCD(2,bufferDisplay2.valueKeys[numberKeys2],3,numberKeys2+2,1,0x0000,'N');
                    }else{
                        WriteLCD(2,character,3,numberKeys2+2,1,0x0000,'N');
                    }
                }else if(PressedKey >= 0x2D && PressedKey <= 0x7E) //Comandos ascii sin incluir coma (,)
                { 
                    numberKeys2++;  
                    bufferDisplay2.valueKeys[numberKeys2] = PressedKey;
                    if(character == 0)
                    {
                        WriteLCD(2,bufferDisplay2.valueKeys[numberKeys2],3,numberKeys2+2,1,0x0000,'N');
                    }else{
                        WriteLCD(2,character,3,numberKeys2+2,1,0x0000,'N');
                    }
                }else if(PressedKey == ',' && numberKeys2 >= 1 && flagPoint2 == 0)  //Coma (,)
                { 
                    numberKeys2++;  
                    flagPoint2 = 1;
                    bufferDisplay2.valueKeys[numberKeys2] = PressedKey;
                    if(character == 0)
                    {
                        WriteLCD(2,bufferDisplay2.valueKeys[numberKeys2],3,numberKeys2+2,1,0x0000,'N');
                    }else{
                        WriteLCD(2,character,3,numberKeys2+2,1,0x0000,'N');
                    }
                }
            }  
            if(Display2_rxBuffer[3] == 0x0B) //Borrar
            { 
                if(numberKeys2 == 0)           //Si no ha presionado nada regresa al menu anterior
                {		    
                    //CyDelay(20);  
                    vTaskDelay( 20 / portTICK_PERIOD_MS );
                    Display2_ClearRxBuffer();
                    return 0;
                }
                else{
                    WriteLCD(2,' ',3,numberKeys2+2,1,0x0000,'N');	//Si ya presiono borra el dato	
                    if(bufferDisplay2.valueKeys[numberKeys2] == 0x2C)
                    {
                        flagPoint2=0;
                    }
                    numberKeys2--;
                }
            }
            if(PressedKey == 0x0A)
            { //Enter
                if(numberKeys2 > 0)
                {
                    bufferDisplay2.valueKeys[0] = numberKeys2;
                    //CyDelay(20);
                    vTaskDelay( 20 / portTICK_PERIOD_MS );
                    Display2_ClearRxBuffer();
                    return 1;
                }
            }
            if(Display2_rxBuffer[3] == 0x94)
            { //Atras
                //CyDelay(20);
                vTaskDelay( 20 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
                return 0;
            }
        } 
        //CyDelay(20);
        vTaskDelay( 20 / portTICK_PERIOD_MS );
        Display2_ClearRxBuffer();
    }    
    return 2;
}

uint8 alphanumeric_keyboard3( uint8 maxNumberKeys, uint8 character){
    char8 PressedKey = 0;    
    if(Display1_GetRxBufferSize() == 8)
    {
        if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
        {                
            if(numberKeys3 <= (maxNumberKeys - 1))
            {
                PressedKey = UnmapCode(Display1_rxBuffer[3]);
                if(PressedKey >= 0x20 && PressedKey <= 0x2B) //Comandos ascii sin incluir coma (,)
                { 
                    numberKeys3++;  
                    bufferDisplay3.valueKeys[numberKeys1] = PressedKey;                        
                    if(character == 0)
                    {
                        WriteLCD(1,bufferDisplay3.valueKeys[numberKeys3],3,numberKeys3+2,1,0x0000,'N');
                    }else{
                        WriteLCD(1,character,3,numberKeys3+2,1,0x0000,'N');
                    }
                }else if(PressedKey >= 0x2D && PressedKey <= 0x7E) //Comandos ascii sin incluir coma (,)
                { 
                    numberKeys3++;  
                    bufferDisplay3.valueKeys[numberKeys3] = PressedKey;
                    if(character == 0)
                    {
                        WriteLCD(1,bufferDisplay3.valueKeys[numberKeys3],3,numberKeys3+2,1,0x0000,'N');
                    }else{
                        WriteLCD(1,character,3,numberKeys3+2,1,0x0000,'N');
                    }
                }else if(PressedKey == ',' && numberKeys3 >= 1 && flagPoint3 == 0) //Coma (,)
                {  
                    numberKeys3++;  
                    flagPoint3 = 1;
                    bufferDisplay3.valueKeys[numberKeys3] = PressedKey;
                    if(character == 0)
                    {
                        WriteLCD(1,bufferDisplay3.valueKeys[numberKeys3],3,numberKeys3+2,1,0x0000,'N');
                    }else{
                        WriteLCD(1,character,3,numberKeys3+2,1,0x0000,'N');
                    }
                }
            }  
            if(Display1_rxBuffer[3] == 0x0B) //Borrar
            { 
                if(numberKeys3 == 0) //Si no ha presionado nada regresa al menu anterior
                {		
                    vTaskDelay( 20 / portTICK_PERIOD_MS );
                    Display1_ClearRxBuffer();
                    return 0;
                }
                else{
                    WriteLCD(1,' ',3,numberKeys3+2,1,0x0000,'N');	//Si ya presiono borra el dato	
                    if(bufferDisplay3.valueKeys[numberKeys3] == 0x2C)
                    {
                        flagPoint3=0;
                    }
                    numberKeys3--;
                }
            }
            if(PressedKey == 0x0A) //Enter
            { 
                if(numberKeys3 > 0) 
                {
                    bufferDisplay3.valueKeys[0] = numberKeys3;
                    //CyDelay(20);
                    vTaskDelay( 20 / portTICK_PERIOD_MS );
                    Display1_ClearRxBuffer();
                    return 1;
                }
            }
            if(Display1_rxBuffer[3] == 0x94) //Atras
            { 
                //CyDelay(20);
                vTaskDelay( 20 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
                return 0;
            }
        } 
        //CyDelay(20);
        vTaskDelay( 20 / portTICK_PERIOD_MS );
        Display1_ClearRxBuffer();
    }    
    return 2;
}

uint8 alphanumeric_keyboard4( uint8 maxNumberKeys, uint8 character){
    char8 PressedKey = 0;    
    if(Display2_GetRxBufferSize() == 8)
    {
        if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
        {                
            if(numberKeys4 <= (maxNumberKeys - 1))
            {
                PressedKey = UnmapCode(Display2_rxBuffer[3]);
                if(PressedKey >= 0x20 && PressedKey <= 0x2B)  //Comandos ascii sin incluir coma (,)
                { 
                    numberKeys4++;  
                    bufferDisplay4.valueKeys[numberKeys4] = PressedKey;                        
                    if(character == 0)
                    {
                        WriteLCD(2,bufferDisplay4.valueKeys[numberKeys4],3,numberKeys4+2,1,0x0000,'N');
                    }else{
                        WriteLCD(2,character,3,numberKeys4+2,1,0x0000,'N');
                    }
                }else if(PressedKey >= 0x2D && PressedKey <= 0x7E) //Comandos ascii sin incluir coma (,)
                { 
                    numberKeys4++;  
                    bufferDisplay4.valueKeys[numberKeys4] = PressedKey;
                    if(character == 0)
                    {
                        WriteLCD(2,bufferDisplay4.valueKeys[numberKeys4],3,numberKeys4+2,1,0x0000,'N');
                    }else{
                        WriteLCD(2,character,3,numberKeys4+2,1,0x0000,'N');
                    }
                }else if(PressedKey == ',' && numberKeys4 >= 1 && flagPoint4 == 0)  //Coma (,)
                { 
                    numberKeys4++;  
                    flagPoint4 = 1;
                    bufferDisplay4.valueKeys[numberKeys4] = PressedKey;
                    if(character == 0)
                    {
                        WriteLCD(2,bufferDisplay4.valueKeys[numberKeys4],3,numberKeys4+2,1,0x0000,'N');
                    }else{
                        WriteLCD(2,character,3,numberKeys4+2,1,0x0000,'N');
                    }
                }
            }  
            if(Display2_rxBuffer[3] == 0x0B) //Borrar
            { 
                if(numberKeys4 == 0)           //Si no ha presionado nada regresa al menu anterior
                {		    
                    //CyDelay(20);  
                    vTaskDelay( 20 / portTICK_PERIOD_MS );
                    Display2_ClearRxBuffer();
                    return 0;
                }
                else{
                    WriteLCD(2,' ',3,numberKeys4+2,1,0x0000,'N');	//Si ya presiono borra el dato	
                    if(bufferDisplay4.valueKeys[numberKeys4] == 0x2C)
                    {
                        flagPoint4=0;
                    }
                    numberKeys4--;
                }
            }
            if(PressedKey == 0x0A)
            { //Enter
                if(numberKeys4 > 0)
                {
                    bufferDisplay4.valueKeys[0] = numberKeys4;
                    //CyDelay(20);
                    vTaskDelay( 20 / portTICK_PERIOD_MS );
                    Display2_ClearRxBuffer();
                    return 1;
                }
            }
            if(Display2_rxBuffer[3] == 0x94)
            { //Atras
                //CyDelay(20);
                vTaskDelay( 20 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
                return 0;
            }
        } 
        //CyDelay(20);
        vTaskDelay( 20 / portTICK_PERIOD_MS );
        Display2_ClearRxBuffer();
    }    
    return 2;
}