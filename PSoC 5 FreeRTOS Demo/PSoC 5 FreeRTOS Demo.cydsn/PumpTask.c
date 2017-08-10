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

char mensaje[] = {"OPERACION"};
char mensaje2[] = {"RECHAZADA"};
char mensaje3[] = {"MANGUERA"};
char mensaje4[] = {"INCORRECTA"};
uint8 producto2[13] = "CORRIENTE    ";
uint8 producto1[13] = "ACPM         ";
uint8 producto3[13] = "CORRIENTE    ";
uint8 producto4[13] = "CORRIENTE    ";
uint8 precios = 0;

/* Drivers */
#include "PumpTask.h"

/* Configuration Password */
uint8 passwordPump[5] = "00204";

/*
*********************************************************************************************************
*                                         void InitDisplay1(void)
*
* Description : Pregunta configuraciones al dispensador 
*               
*
*********************************************************************************************************
*/
void InitDisplay1(){ 
    uint8 x;
    uint8_t iniText[10]        = "Iniciando"; 
    uint8_t Unit[10]           = "S/D/Dupla ";
    uint8_t Unit2[10]          = "Quadrupla";
    uint8_t Unit3[10]          = "Sextupla";
    uint8_t Unit4[10]          = "Octupla";
    uint8_t DisplayMode0[10]   = "5-5-4     ";
    uint8_t DisplayMode[10]    = "6-6-4     ";
    uint8_t DisplayMode2[10]   = "7-7-5     ";
    uint8_t DisplayMode3[10]   = "8-7-5     ";
    
    
    for(x = 0; x < 13; x++)
    {
        producto[0][x] = producto1[x];
    }
    for(x = 0; x < 13; x++)
    {
        producto[1][x] = producto2[x];
    }
    for(x = 0; x < 13;x++)
    {
        producto[2][x] = producto3[x];
    }
    for(x = 0; x < 13;x++)
    {
        producto[3][x] = producto4[x];
    }
    
    if(NumPositions == 2)
    {
        SetPicture(1, DISPLAY_INICIO0);         
        ShowMessage(1,iniText,0);    
        vTaskDelay( 300 / portTICK_PERIOD_MS );              //Freertos delay
        if(UnitType == 0)
        {
            side.a.hoseNumber = UnitType + 1;
            side.b.hoseNumber = UnitType + 1;
            ShowMessage(1,Unit,0);                          //Mostrar tipo de dispensador      
        }
        if(UnitType == 1)
        {
            side.a.hoseNumber = UnitType + 1;
            side.b.hoseNumber = UnitType + 1;
            ShowMessage(1,Unit2,0);
        }
        if(UnitType == 2)
        {
            side.a.hoseNumber = UnitType + 1;
            side.b.hoseNumber = UnitType + 1;
            ShowMessage(1,Unit3,0);
        }
        if(UnitType == 3)
        {
            side.a.hoseNumber = UnitType + 1;
            side.b.hoseNumber = UnitType + 1;
            ShowMessage(1,Unit4,0);
        } 
        if(DDMode == 0)
        {
            ShowMessage(1,DisplayMode0,22);                 //Mostrar modo de display
            digits = 5;
        }
        if(DDMode == 1)
        {
            ShowMessage(1,DisplayMode,22);                  //Mostrar modo de display
            digits = 6;
        }
        if(DDMode == 2)
        {
           ShowMessage(1,DisplayMode2,22);
            digits = 7;
        }
        if(DDMode == 3)
        {
            ShowMessage(1,DisplayMode3,22);
            digits = 8;
        }
//        if (precios == 0)
//            ReadPPUFromEEpromInit();
    }
    if(NumPositions == 4)
    {
        SetPicture(1,DISPLAY_SELECCIONE_POSICION);        
        ShowMessage(1, iniText, 0);    
        vTaskDelay( 300 / portTICK_PERIOD_MS );              //Freertos delay
        if(UnitType == 0)
        {
            side.a.hoseNumber = UnitType + 1;
            side.b.hoseNumber = UnitType + 1;
            side.c.hoseNumber = UnitType + 1;
            side.d.hoseNumber = UnitType + 1;
            ShowMessage(1,Unit,0);                          //Mostrar tipo de dispensador      
        }
        if(UnitType == 1)
        {
            side.a.hoseNumber = UnitType ;
            side.b.hoseNumber = UnitType ;
            side.c.hoseNumber = UnitType ;
            side.d.hoseNumber = UnitType ;
            ShowMessage(1,Unit2,0);
        }
        if(UnitType == 2)
        {
            side.a.hoseNumber = UnitType;
            side.b.hoseNumber = UnitType;
            side.c.hoseNumber = UnitType - 1;
            side.d.hoseNumber = UnitType - 1;
            ShowMessage(1,Unit3,0);
        }
        if(UnitType == 3)
        {
            side.a.hoseNumber = UnitType - 1;
            side.b.hoseNumber = UnitType - 1;
            side.c.hoseNumber = UnitType - 1;
            side.d.hoseNumber = UnitType - 1;
            ShowMessage(1,Unit4,0);
        }    
        if(DDMode == 1)
        {
            ShowMessage(1,DisplayMode,22);                  //Mostrar modo de display
            digits = 6;
        }
        if(DDMode == 2)
        {
            ShowMessage(1,DisplayMode2,22);
            digits = 7;
        } 
    }        
    if(NumPositions < 2)
    {
        SetPicture(1,DISPLAY_ERROR);        
    }      
   
}

/*
*********************************************************************************************************
*                                         void InitDisplay2(void)
*
* Description : Pregunta configuraciones al dispensador 
*               
*
*********************************************************************************************************
*/
void InitDisplay2(){ 
    uint8 MuxVersion [10] = "MUX V. 1.0";
    if(NumPositions == 2){        
        SetPicture(2,DISPLAY_INICIO0);  
        flowDisplay2 = 0;
        Display2_ClearRxBuffer();
    }
    if(NumPositions == 4){
        flowDisplay2 = 0;
        SetPicture(2,DISPLAY_SELECCIONE_POSICION);         
    }        
    if(NumPositions < 2){        
        SetPicture(2,DISPLAY_ERROR);
    }     
    ShowMessage(2,MuxVersion,0);
}

/*
*********************************************************************************************************
*                                         void PrintTest(void)
*
* Description : Pregunta configuraciones al dispensador 
*               
*
*********************************************************************************************************
*/
void PrintTest(){
    uint8 datoA[26] = "Test de Impresion Puerto 1";
    uint8 datoB[26] = "Test de Impresion Puerto 2";
    uint8 x;
    for(x = 0; x < 26;x++)
    {
        write_psoc1(1,datoA[x]);
    }
    write_psoc1(1,10);
    write_psoc1(1,10);
    write_psoc1(1,10);
    write_psoc1(1,10);
    write_psoc1(1,10);
    write_psoc1(1,10);
    for(x = 0; x < 26; x++)
    {
        write_psoc1(2,datoB[x]);
    }
    write_psoc1(2,10);
    write_psoc1(2,10);
    write_psoc1(2,10);
    write_psoc1(2,10);
    write_psoc1(2,10);
    write_psoc1(2,10);
}

/*
*********************************************************************************************************
*                                         void PollingDisplay1(void)
*
* Description : Pregunta estado al surtidor 
*               
*
*********************************************************************************************************
*/

void PollingDisplay1(void){    
    uint8 x, y;
    
    switch(flowDisplay1){
        case 0:            
            InitDisplay1();                              
            if(NumPositions == 2){
                SetPicture(1, DISPLAY_INICIO0);
            }else{
                SetPicture(1, DISPLAY_SELECCIONE_POSICION);
            }            
            // Impresion de tiquete 
            bufferDisplay1.flagActiveSale = false;
            flowDisplay1 = 1;    
            vTaskDelay( 10 / portTICK_PERIOD_MS );            
            Display1_ClearRxBuffer();             
        break;
        case 1: //Menu              
            if(NumPositions > 2){
                if(bufferDisplay1.flagActiveSale){
                    flowDisplay1 = flowPos;            
                }
                if(bufferDisplay3.flagActiveSale){
                    flowDisplay3 = flowPosC;
                    selectPos = 2;
                }
            }
            
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {     
                    if(NumPositions == 2){
                        flowDisplay1 = 2;                               //Pantalla forma de pago                      
                        SetPicture(1, DISPLAY_FORMA_PAGO_DESEADA);                                                
                    }else{
                        switch(Display1_rxBuffer[3]){
				        case 0x56:						        //POS A                                                                                                                       
                                if(bufferDisplay1.flagActiveSale){
                                    flowDisplay1 = flowPos;      //Si está vendiendo en esta posición regresa a su flujo
                                }else{
                                    SetPicture(1, DISPLAY_FORMA_PAGO_DESEADA); //Pantalla forma de pago
                                    selectPos     = 1;
                                    flowDisplay1  = 2;
                                }
                                if (bufferDisplay3.flagActiveSale){
                                    flowDisplay3 = flowPosC;    //Si la venta está activa en POS B regresa a su flujo
                                }
                                else{
                                    flowDisplay3 = 0; 
                                    flowPosC     = 0;
                                }
					    break;
                            		       
					    case 0x57:  				            //POS B                                                                 
                                if(bufferDisplay1.flagActiveSale){
                                    flowDisplay1 = flowPos;      //Si está vendiendo en esta posición regresa a su flujo
                                }
                                else{
                                    flowDisplay1 = 0;
                                    flowPos      = 0;
                                } 
                                if (bufferDisplay3.flagActiveSale){                                    
                                    flowDisplay3 = flowPosC;    //Si la venta está activa en POS B regresa a su flujo
                                }else{
                                    SetPicture(1, DISPLAY_FORMA_PAGO_DESEADA); //Pantalla forma de pago
							        selectPos     = 2;
                                    flowDisplay3  = 2;                            
                                }                           
					    break;                                                   
				    }                      
                    }                    
                } 
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }         
        break;
        case 2:  //Pantalla forma de pago
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C)){
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x0D:  //Pantalla efectivo   
                            if(lockTurn == 1)
                            {
                                flowDisplay1 = 3; 
                                bufferDisplay1.saleType = 1;
                                SetPicture(1, DISPLAY_FORMA_PROGRAMACION);  
                                AuthType = 2;
                            }else
                            {
                                SetPicture(1, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay1 = 0;                                                                
                            }
                        break;
                        case 0x0E:  //Pantalla credito 
                            for(x = 0; x < 10; x++)
                            {                                
                                bufferDisplay1.CreditpresetValue[0][x] = 0x00;
                                bufferDisplay1.CreditpresetValue[1][x] = 0x00;
                                
                            }
                            if(lockTurn == 1)
                            {
                                flowDisplay1 = 10;
                                bufferDisplay1.saleType = 2;                                
                                SetPicture(1, DISPLAY_ID_DIGITAL);
                                AuthType = 1;
                                Display1_ClearRxBuffer();
                            }else
                            {
                                SetPicture(1, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay1 = 0;                                                                
                            }
                        break;
                            
                        case 0x45:  //Pantalla otras opciones 
                            flowDisplay1 = 12;                            
                            SetPicture(1,  DISPLAY_OPERACIONES);
                            AuthType = 0;
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1, DISPLAY_INICIO0);   
                            AuthType = 0;
                            vTaskDelay( 10 / portTICK_PERIOD_MS );
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 0x7E:  //Pantalla Inicial 
                             bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1, DISPLAY_INICIO0);
                            AuthType = 0;
                            vTaskDelay( 10 / portTICK_PERIOD_MS );
                            Display1_ClearRxBuffer();
                        break;
                    }
                }                  
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
            
        case 3: //Pantalla ingreso de valores
            for(x = 0; x <10; x++)
            {
                bufferDisplay1.presetValue[0][x] = 0x00;
                bufferDisplay1.presetValue[1][x] = 0x00;
            }
            Credit_Auth_OK = 0;
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x0F:  //Preset dinero                
                            flowDisplay1 = 4;                              
                            bufferDisplay1.presetType[0] = 2;
                            bufferDisplay1.presetType[1] = '$';
                            numberKeys1 = 0;
                            SetPicture(1, DISPLAY_INTRODUZCA_VALOR);   
                            WriteLCD(1, '$', 3, 2, 1, 0x0000, 'N');                            
                            if(MoneyDec > 0)
                            {
                                flagPoint1 = 0;
                            }else
                            {
                                flagPoint1 = 1;
                            }
                            Display1_ClearRxBuffer();
                        break; 
                        case 0x10:  //Preset volumen                 
                            flowDisplay1 = 4;                            
                            bufferDisplay1.presetType[0] = 1;
                            bufferDisplay1.presetType[1] = 'G';
                            numberKeys1 = 0;                            
                            SetPicture(1, DISPLAY_INTRODUZCA_VOLUMEN);                            
                            WriteLCD(1, 'G', 3, 2, 1, 0x0000, 'N');                            
                            if(VolDec > 0)
                            {
                                flagPoint1 = 0;
                            }else
                            {
                                flagPoint1 = 1;
                            }
                            Display1_ClearRxBuffer();
                        break;
                        case 0x43:     //Preset full 
                            bufferDisplay1.presetType[0] = 3;
                            bufferDisplay1.presetType[1] = 'F';
                            
                            for(x = 1; x <(digits - 1); x++)
                            {
                                bufferDisplay1.presetValue[0][x] = '9';
                            }
                            
                            bufferDisplay1.presetValue[0][x] = '0';
                            bufferDisplay1.presetValue[0][x + 1] = '0';
                            bufferDisplay1.presetValue[0][0] = digits;
                            
                            for(x = 0; x <= bufferDisplay1.presetValue[0][0]; x++)
                            {
                                bufferDisplay1.presetValue[1][x] = bufferDisplay1.presetValue[0][x];
                            }
                            
                            flowDisplay1 = 5;
                            SetPicture(1,DISPLAY_SELECCIONE_PRODUCTO4);
                            Display1_ClearRxBuffer();
                        break;
                        case 0x94:  //Retroceso
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                        case 0x3B:  //Pantalla Inicial  
                            AuthType = 0;                        
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            AuthType = 0;
                            Display1_ClearRxBuffer();
                        break;
                        case 0x7E:  //Pantalla Inicial  
                            AuthType = 0;
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0); 
                            AuthType = 0;
                            Display1_ClearRxBuffer();
                        break;
                    }
                }  
                //CyDelay(10);
                //vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
        
        case 4:            
            switch (alphanumeric_keyboard(digits + 1, 0))
            {
                case 0:  //Pantalla Inicial    
                    flowDisplay1 = 0;
                    SetPicture(1,DISPLAY_INICIO0); 
                    Display1_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    for(x = 0; x <= bufferDisplay1.valueKeys[0]; x++)
                    {
                        bufferDisplay1.presetValue[0][x] = bufferDisplay1.valueKeys[x];
                        bufferDisplay1.presetValue[1][x] = bufferDisplay1.valueKeys[x];
                    }  
                    flowDisplay1 = 5;   //caso para seleccion de producto
                    SetPicture(1, DISPLAY_SELECCIONE_PRODUCTO4);
                    Display1_ClearRxBuffer();
                break;
            }
        break;
            
        case 5: //Seleccion de producto            
            if(1)//(side.a.GradesHose[0] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                   WriteMessage(1, producto1[x],11,7+x,2,0x0000,'Y'); //WriteMessage(1, producto[side.a.GradesHose[0]-1][x],11,7+x,2,0xFFFF,'Y');
                } 
            }
            if(1)//(side.a.GradesHose[1] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto2[x],17,8+x,2,0x0000,'Y');//WriteMessage(1, producto[side.a.GradesHose[1]-1][x],17,8+x,2,0x0000,'Y');
                } 
            }
            if(side.a.GradesHose[2] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.a.GradesHose[2]-1][x],23,9+x,2,0x0000,'Y');
                } 
            }
            if(side.a.GradesHose[3] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.a.GradesHose[3]-1][x],29,10+x,2,0x0000,'Y');
                } 
            }
            
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x80:  //Grado 1 
                            numberKeys1=0; 
                            flowDisplay1 = 9;
                            side.a.hose = 1;                         
                            //Credit
                            if(AuthType == 2)
                            {
                                bufferDisplay1.flagKeyboard = 1; 
                                SetPicture(1, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType == 1)
                            {                    
                                bufferDisplay1.flagKeyboard = 2;
                                SetPicture(1, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display1_ClearRxBuffer();
                        break;                            
                        case 0x81:  //Grado 2 
                            numberKeys1=0; 
                            flowDisplay1 = 9;
                            side.a.hose = 2;
                            //Credit
                            if(AuthType == 2)
                            {
                                bufferDisplay1.flagKeyboard = 1; 
                                SetPicture(1, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType == 1)
                            {                    
                                bufferDisplay1.flagKeyboard = 2;
                                SetPicture(1, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display1_ClearRxBuffer(); 
                        break;
                            
                        case 0x7F:  //Grado 3
                            numberKeys1=0; 
                            flowDisplay1 = 9;
                            side.a.hose = 3;
                           //Credit
                            if(AuthType == 2)
                            {
                                bufferDisplay1.flagKeyboard = 1; 
                                SetPicture(1, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType == 1)
                            {                    
                                bufferDisplay1.flagKeyboard = 2;
                                SetPicture(1, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display1_ClearRxBuffer();
                        break;   
                        
                        case 0xB8:  //Grado 4 
                            numberKeys1=0; 
                            flowDisplay1 = 9;
                            side.a.hose = 4;
                            //Credit
                            if(AuthType == 2)
                            {
                                bufferDisplay1.flagKeyboard = 1; 
                                SetPicture(1, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType == 1)
                            {                    
                                bufferDisplay1.flagKeyboard = 2;
                                SetPicture(1, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            } 
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Retroceso
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            AuthType = 0;
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                }
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
                    
        case 6: // Print Screen
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x39:  //Print 
                            flowDisplay1 = 7;
                            numberKeys1 = 0;
                            bufferDisplay1.flagPrint =  1;                                                
                            SetPicture(1, DISPLAY_SUBA_MANIJA);  
                            Display1_ClearRxBuffer();
                        break; 
                        case 0x38:  //No Print 
//                            if(bufferDisplay1.flagEndSale)
//                            {
//                                flowDisplay1 = 0;
//                                bufferDisplay1.flagPrint = 0;
//                                SetPicture(1, DISPLAY_INICIO0);
//                            }else
//                            {
                                flowDisplay1 = 7; // Wait of handle up
                                bufferDisplay1.flagPrint =  0;                                                               
                                SetPicture(1, DISPLAY_SUBA_MANIJA);
                                Display1_ClearRxBuffer();
//                            } 
                        break;
                        case 0x7E:  //Return to initial screen                                                                                    
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            bufferDisplay1.flagPrint =  0;
                            AuthType = 0;
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                }
               
                //vTaskDelay( 10 / portTICK_PERIOD_MS );              // 10mS delay
                Display1_ClearRxBuffer();
            }
        break;
                                        
            // PRESET flag ON and wait to handle up
        case 7:                       
            PresetFlag = 1;
            //Touch for return to init display
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            PresetFlag = 0;
                            iButtonFlag = 0;
                            AuthType = 0;     
                            bufferDisplay1.zeroSend = 1;                            
                            vTaskDelay( 50 / portTICK_PERIOD_MS );                      
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            PresetFlag = 0;
                            iButtonFlag = 0;
                            AuthType = 0;
                            bufferDisplay1.zeroSend = 1;
                            vTaskDelay( 50 / portTICK_PERIOD_MS );
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                }
                
               Display1_ClearRxBuffer();
            }
           
        break;
            
          
        case 8:           
           //Pump Busy...
           vTaskDelay( 10 / portTICK_PERIOD_MS ); 
           if(NumPositions > 2)
            flowDisplay1 = 1;
        break;
            
        case 9: //Keyboard           
            switch (alphanumeric_keyboard(11,0))
            {
                case 0: //Cancel
                    switch(bufferDisplay1.flagKeyboard)
                    {
                        case 1://License plate
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay1.licenceSale[x] = 0;
                            }
                            flowDisplay1 = 0; 
                            SetPicture(1, DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 2://Mileage
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay1.mileageSale[x] = 0;
                            }        
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay1.identySale[x] = 0;
                            }
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 4://ID
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay1.shiftId[x] = 0;
                            }
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                    Display1_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay1.flagKeyboard)
                    {
                        case 1://License plate
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay1.licenceSale[x] = 0x00;
                            }
                            
                            for(x = 0; x <= bufferDisplay1.valueKeys[0]; x++)
                            {
                                bufferDisplay1.licenceSale[x] = bufferDisplay1.valueKeys[x];
                            }
//                            if(bufferDisplay1.flagEndSale)
//                            {
//                                bufferDisplay1.flagPrint = 1;
//                                flowDisplay1 = 0;
//                                SetPicture(1, DISPLAY_INICIO0); 
//                            }else{
                                flowDisplay1 = 6;
                                SetPicture(1, DISPLAY_DESEA_IMPRIMIR_RECIBO);                                 
//                            }
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 2:// mileage
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay1.mileageSale[x] = 0;
                            }                         
                            for(x = 0; x <= bufferDisplay1.valueKeys[0]; x++)
                            {
                                bufferDisplay1.mileageSale[x] = bufferDisplay1.valueKeys[x];
                            }
                            
                            //Credit Sale
                            if(AuthType == 1)
                            {
                                flowDisplay1 = 7;                               
                                SetPicture(1, DISPLAY_SUBA_MANIJA);
                            }
                            else
                            {                                
                                flowDisplay1 = 6;       
                                SetPicture(1, DISPLAY_DESEA_IMPRIMIR_RECIBO);
                            }                                               
                            Display1_ClearRxBuffer();                           
                        break;
                        
                        case 3:     //CC/NIT                            
                            for(x = 0; x <= bufferDisplay1.valueKeys[0]; x++)
                            {
                                bufferDisplay1.identySale[x] = bufferDisplay1.valueKeys[x];
                            }
                            Display1_ClearRxBuffer();
                        break;
                        case 4:     //ID                            
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay1.shiftId[x] = bufferDisplay1.valueKeys[x];
                            }                            
                            flowDisplay1 = 14;          //password request                    
                            numberKeys1 = 0;
                            hiddenKeys = 10;
                            controlChar = '*';
                            bufferDisplay1.flagKeyboard = 3;
                            SetPicture(1,DISPLAY_INGRESE_PASSWORD);
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                    //Display1_ClearRxBuffer();
                break;
            }  
            Display1_ClearRxBuffer();
        break;    
    
    ////////////////// CASOS PARA CRÉDITO  /////////////////////
        case 10:
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0xB6:  //ibutton Request   
                            flowDisplay1 = 11;
                            numberKeys1 = 0;                            
                            bufferDisplay1.flagPrint =  1;
                            SetPicture(1, DISPLAY_ESPERANDO_ID);                            
                        break; 
                        case 0xB7:  //ID Number
                            bufferDisplay1.flagKeyboard = 0;
                            flowDisplay1 = 7;
                            SetPicture(1,DISPLAY_SUBA_MANIJA);                            
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1, DISPLAY_INICIO0);                            
                        break;
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            AuthType = 0;
                        break;
                    }                    
                }             
                //vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
        
        case 11:
            // iButton read
            for(x = 0; x < 30; x++)
            {
                temporal[x] = 0x00;
            }
            for(x = 0; x < 25; x++)
            {
                bufferDisplay1.idSerial[x] = 0x00;
            }
            if(touch_present(1) == 1)
            {
				if(touch_write(1,0x33))
                {
					for(x = 1; x <= 8; x++)
                    {
						temporal[x] = touch_read_byte(1);  // Id
					}
					y = 0;
					for(x = 1; x < 8; x++)
                    {
                        y = crc_check(y,temporal[x]);      // Checksum
                    }
					
                    if(y == temporal[8])
                    {
						bufferDisplay1.idSerial[0] = 16;
						y = 16;
						
                        for(x = 1; x <= 8; x++)
                        {
							if((temporal[x] & 0x0F) >= 10)
                            {
								bufferDisplay1.idSerial[y] = (temporal[x] & 0x0F) + 55;
							}else{
								bufferDisplay1.idSerial[y] = (temporal[x] & 0x0F) + 48;				
							}
                            y--;
							if(((temporal[x] >> 4) & 0x0F) >= 10)
                            {
								bufferDisplay1.idSerial[y] = ((temporal[x] >> 4) & 0x0F) + 55;
							}else{
								bufferDisplay1.idSerial[y] = ((temporal[x] >> 4) & 0x0F) + 48;				
							}
                            y--;
						}
                        
                        // Authorization request
                        SetPicture(1,DISPLAY_ID_RECONOCIDO);                                                                 
                        vTaskDelay( 500 / portTICK_PERIOD_MS );                       
                        iButtonFlag = 1;
                        SetPicture(1, DISPLAY_FORMA_PROGRAMACION);
                        flowDisplay1 = 3;                                                                        
					}
				}else
                    {   // iButton Error
                        SetPicture(1,DISPLAY_ID_NO_RECONOCIDO);                                           
                        Display1_ClearRxBuffer();
                        vTaskDelay( 700 / portTICK_PERIOD_MS );    
                        SetPicture(1, DISPLAY_INICIO0);
                        flowDisplay1 = 0;
                        bufferDisplay1.flagPrint =  0;
                        PresetFlag = 0;
                        iButtonFlag = 0;
                        AuthType = 0;                        
                    }
			}
                     
            //Touch for return to init display
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            PresetFlag = 0;
                            iButtonFlag = 0;
                            AuthType = 0;
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            PresetFlag = 0;
                            iButtonFlag = 0;
                            AuthType = 0;
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                }
                
                Display1_ClearRxBuffer();
            }
          
                                    
        break;    
    ///////////////FIN CASOS PARA CRÉDITO  /////////////////////  
    
    //////////// CASOS PARA CONFIGURACIONES  ///////////////////
        case 12:
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x46:  //Turnos              
                            flowDisplay1 = 13; 
                            if(lockTurn == 1)
                            {
                                SetPicture(1,DISPLAY_CERRAR_TURNO);
                            }else
                            {
                                SetPicture(1,DISPLAY_ABRIR_TURNO);
                            }                            
                        break;
                        case 0x55:  //Configurar módulo 
                            flowDisplay1 = 14;
                            bufferDisplay1.flagKeyboard = 6;
                            numberKeys1 = 0;
                            hiddenKeys = 5;
                            controlChar ='*';
                            SetPicture(1,DISPLAY_INGRESE_PASSWORD);  
                            Display1_ClearRxBuffer();
                        break;
                        case 0xB5:  //Copia de recibo 
                            if(lockTurn == 1)
                            {
                                side.a.RFstateReport = 1;
                                side.a.rfStateCopy = RF_COPY_RECEIPT;
                                SetPicture(1,DISPLAY_IMPRIMIENDO_RECIBO); 
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay1  = 0; 
                                Display1_ClearRxBuffer();
                            }else
                            {
                                SetPicture(1, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay1 = 0;                                                                
                            }
                        break;
                        
                        case 0x3B:  //Pantalla Inicial    
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0); 
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                    }
                }                  
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }
        break;

        case 13:
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x47:  //Open Shift  
                            flowDisplay1 = 9;
                            numberKeys1 = 0;    
                            bufferDisplay1.flagKeyboard = 4;
                            SetPicture(1,DISPLAY_INTRODUZCA_CEDULA);                            
                        break;
                        case 0x48:  //Close shift 
                            flowDisplay1 = 9;
                            numberKeys1 = 0;   
                            bufferDisplay1.flagKeyboard = 4;
                            SetPicture(1,DISPLAY_INTRODUZCA_CEDULA);                           
                        break;                         
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break; 
            
        case 14: //Teclado general      
            switch (alphanumeric_keyboard(hiddenKeys,controlChar))
            {
                case 0: //Cancelar
                    switch(bufferDisplay1.flagKeyboard)
                    {
                        case 1://ID Estacion
                            for(x = 0; x <= 4; x++)
                            {
                                idStation[x] = 0;
                            }
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                        case 2://Umbral
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                pumpGap[x] = 0;
                            }
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                        case 3://Pass turno
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay1.shiftPassword[x] = 0;
                            }
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                        case 5://Pass
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay1.shiftPassword[x] = 0;
                            }
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                            
                        case 6://Pass
                            for(x = 1; x <= configAccess[0]; x++)
                            {
                                configAccess[x] = 0;
                            }
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                    }                    
                    Display1_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay1.flagKeyboard)
                    {   
                        case 1://ID Estacion
                            for(x = 0; x <= 4; x++)
                            {
                                idStation[x] = bufferDisplay1.valueKeys[x+1];                                
                            }
                            idStation[4] = 0x00;
                            intIDStation = atoi(idStation);
                            IDCast[0] = intIDStation;
                            IDCast[1] = intIDStation>>8;
                            EEPROM_1_WriteByte(IDCast[0],10);
                            EEPROM_1_WriteByte(IDCast[1],11);
                            flowDisplay1 = 15;
                            SetPicture(1,DISPLAY_CONFIGURACIONES);
                        break;
                        case 2://Pass
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                pumpGap[x] = bufferDisplay1.valueKeys[x];
                            }
                            flowDisplay1 = 15;
                            SetPicture(1,DISPLAY_CONFIGURACIONES);
                        break;
                        case 3://Pass turno
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay1.shiftPassword[x] = bufferDisplay1.valueKeys[x];
                            }
                            flowDisplay1 = 19;
                            SetPicture(1,DISPLAY_POR_FAVOR_ESPERE);
                            side.a.RFstateReport = 1;
                            ShiftState = 1;
                        break;
                        case 5://Pass
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay1.shiftPassword[x] = bufferDisplay1.valueKeys[x];
                            }
                            flowDisplay1 = 15;
                            SetPicture(1,DISPLAY_CONFIGURACIONES);
                        break; 
                        case 6://Pass
                            for(x = 1; x <= configAccess[0]; x++)
                            {
                                configAccess[x] = bufferDisplay1.valueKeys[x];
                            }
                            if(configAccess[1] == passwordPump[1] && configAccess[2] == passwordPump[2] && configAccess[3] == passwordPump[3] && configAccess[4] == passwordPump[4])
                            {                                
                                SetPicture(1,DISPLAY_PASSWORD_VALIDO);
                                //CyDelay(700);
                                vTaskDelay( 700 / portTICK_PERIOD_MS );
                                flowDisplay1 = 15;
                                SetPicture(1,DISPLAY_CONFIGURACIONES);
                            }else{
                                SetPicture(1,DISPLAY_PASSWORD_INVALIDO);
                                //CyDelay(700);
                                vTaskDelay( 700 / portTICK_PERIOD_MS );
                                flowDisplay1 = 0;
                                SetPicture(1,DISPLAY_INICIO0);
                            }
                        break;
                    }                    
                    Display1_ClearRxBuffer();
                break;
            }            
        break;
            
        case 15: //Menu de configuraciones
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x82:  //ID Estacion  
                            flowDisplay1 = 14;
                            numberKeys1 = 0; 
                            controlChar = 0;
                            hiddenKeys  = 5;
                            bufferDisplay1.flagKeyboard = 1;
                            SetPicture(1,DISPLAY_INTRODUZCA_VALOR);                            
                        break;
                        case 0x65:  //Test Impresoras 
                            flowDisplay1 = 16; 
                            PrintTest();
                            SetPicture(1,DISPLAY_AMBAS_IMPRESORAS_FUNCIONANDO);                           
                        break;        
                        case 0x5A:  //Umbral 
                            flowDisplay1 = 14;
                            numberKeys1 = 0;
                            controlChar = 0;
                            hiddenKeys  = 3;
                            pumpGap[0]  = 2;
                            bufferDisplay1.flagKeyboard = 2;
                            SetPicture(1,DISPLAY_INTRODUZCA_VALOR);                            
                        break;
                        case 0x58:  //Hora y Fecha
                            flowDisplay1 = 18;
                            numberKeys1 = 0;   
                            bufferDisplay1.flagKeyboard = 4;
                            SetPicture(1,DISPLAY_CONFIGURAR_FECHA_HORA);                           
                        break;
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                    }                    
                }
                     
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break; 
            
        case 16: //Menu de configuraciones de impresoras 1
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x38:  //No funcionan  
                            flowDisplay1 = 17;
                            SetPicture(1,DISPLAY_IMPRESORA_NO_FUNCIONANDO);                            
                        break;
                        case 0x39:  //Si funcionan
                            flowDisplay1 = 15; 
                            printPortA = 1;
                            printPortB = 2;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(1,DISPLAY_CONFIGURACIONES);
                        break;                                             
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
            
        case 17: //Menu de configuraciones de impresoras 2
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x8C:  //1 no funciona  
                            flowDisplay1 = 15; 
                            printPortA = 2;
                            printPortB = 2;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(1,DISPLAY_CONFIGURACIONES);                            
                        break;
                        case 0x8D:  //2 no funciona
                            flowDisplay1 = 15; 
                            printPortA = 1;
                            printPortB = 1;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(1,DISPLAY_CONFIGURACIONES); 
                        break;
                            
                        case 0x8E:  //ninguna funciona
                            flowDisplay1 = 15; 
                            printPortA = 0;
                            printPortB = 0;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(1,DISPLAY_CONFIGURACIONES); 
                        break;
                                               
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                    }                    
                }

                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
            
        case 18: //Menu configuracion manual de fecha y hora
            if(leer_hora() == 1)
            {
                WriteLCD(1,(((timeDownHandle[1] & 0x10) >> 4) + 48),13,8,1,0x0000,'N');
                WriteLCD(1,((timeDownHandle[1] & 0x0F) + 48),13,9,1,0x0000,'N');
                WriteLCD(1,':',13,10,1,0x0000,'N');
                WriteLCD(1,(((timeDownHandle[0] & 0xF0)>>4)+48),13,11,1,0x0000,'N');
                WriteLCD(1,((timeDownHandle[0] & 0x0F)+48),13,12,1,0x0000,'N');               
            }
            if(leer_fecha() == 1)
            {                
                WriteLCD(1,(((dateDownHandle[0] & 0x30) >> 4) + 48),21,8,1,0x0000,'N');
                WriteLCD(1,((dateDownHandle[0] & 0x0F) + 48),21,9,1,0x0000,'N');
                WriteLCD(1,'/',21,10,1,0x0000,'N');
                WriteLCD(1,(((dateDownHandle[1] & 0x10) >> 4) + 48),21,11,1,0x0000,'N');
                WriteLCD(1,((dateDownHandle[1] & 0x0F) + 48),21,12,1,0x0000,'N');
                WriteLCD(1,'/',21,13,1,0x0000,'N');
                WriteLCD(1,(((dateDownHandle[2] & 0xF0) >> 4) + 48),21,14,1,0x0000,'N');
                WriteLCD(1,((dateDownHandle[2] & 0x0F) + 48),21,15,1,0x0000,'N');
            }
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                                                                       
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);                            
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);                            
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
            
        case 19:             
           
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            PresetFlag = 0;
                            iButtonFlag = 0;
                            ShiftState = 0;
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            PresetFlag = 0;
                            iButtonFlag = 0;
                            ShiftState = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
            }
            Display1_ClearRxBuffer();
            
        break;
            
        case 20: //pantalla de espera de autorizacion
            SetPicture(1, DISPLAY_ESPERANDO_AUTORIZACION);
            //Touch for return to init display
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            bufferDisplay1.flagPrint =  0;
                            flowPos      = 0;
                            flowDisplay1 = 0;                            
                            PresetFlag   = 0;
                            iButtonFlag  = 0;
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            bufferDisplay1.flagPrint =  0;
                            flowPos      = 0;
                            flowDisplay1 = 0;                            
                            PresetFlag   = 0;
                            iButtonFlag  = 0;
                            ShiftState   = 0;
                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
            }
            Display1_ClearRxBuffer();            
        break;
            
            
        case 21: //Pantalla de credito no autorizado            
            SetPicture(1, DISPLAY_AUTORIZACION_RECHAZADA);
            bufferDisplay1.flagPrint = 0;
            flowPos     = 0;
            PresetFlag  = 0;
            iButtonFlag = 0;
            for(x = 0; x < 9; x++)
            {
                WriteMessage(1, mensaje[x],17,1 + x,4,0x0000,'Y');
                WriteMessage(1, mensaje2[x],21,1 + x,4,0x0000,'Y');                    
            }
            side.a.rfState = RF_IDLE;
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
            flowDisplay1 = 0;
            SetPicture(1, DISPLAY_INICIO0);
        break;
            
        // Pantalla de Manguera equivocada
        case 22:
            
            SetPicture(1, DISPLAY_AUTORIZACION_RECHAZADA);
            bufferDisplay1.flagPrint = 0;

            for(x = 0; x < 8; x++)
            {                
                WriteMessage(1, mensaje3[x], 17, 1 + x, 4, 0x0000, 'Y');                    
            }
            for(x = 0; x < 10; x++)
            {
                
                WriteMessage(1, mensaje4[x], 21, 1 + x, 4, 0x0000, 'Y');
                    
            }
             
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
            flowDisplay1 = 0;
            SetPicture(1, DISPLAY_INICIO0);
        break;            
    }    
}

/*
*********************************************************************************************************
*                                         void PollingDisplay2(void)
*
* Description : Pregunta estado al surtidor 
*               
*
*********************************************************************************************************
*/
void PollingDisplay2(void){    
    uint8 x,y;  
    
    switch(flowDisplay2){
        case 0:
			//InitDisplay2();            
            if(NumPositions == 2){
                SetPicture(2, DISPLAY_INICIO0);
            }else{
                SetPicture(2, DISPLAY_SELECCIONE_POSICION);
            } 
            bufferDisplay1.flagActiveSale = false;
            flowDisplay2 = 1;
            vTaskDelay( 10 / portTICK_PERIOD_MS );
            Display2_ClearRxBuffer();            
        break;
        case 1: //Menu             
            if(NumPositions > 2){
                if(bufferDisplay2.flagActiveSale){
                    flowDisplay2 = flowPosB;            
                }
                if(bufferDisplay4.flagActiveSale){
                    flowDisplay4 = flowPosD;
                    selectPosB = 2;
                }
            }
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {                                             
                    if(NumPositions == 2){
                        flowDisplay2 = 2;                               //Pantalla forma de pago                      
                        SetPicture(2, DISPLAY_FORMA_PAGO_DESEADA);                                                
                    }else{
                        switch(Display2_rxBuffer[3]){
				        case 0x57:						        //POS A                                                                                                                       
                                if(bufferDisplay2.flagActiveSale){
                                    flowDisplay2 = flowPos;      //Si está vendiendo en esta posición regresa a su flujo
                                }else{
                                    SetPicture(2, DISPLAY_FORMA_PAGO_DESEADA); //Pantalla forma de pago
                                    selectPosB    = 1;
                                    flowDisplay2  = 2;
                                }
                                if (bufferDisplay4.flagActiveSale){
                                    flowDisplay4 = flowPosD;    //Si la venta está activa en POS B regresa a su flujo
                                }
                                else{
                                    flowDisplay4 = 0; 
                                    flowPosD     = 0;
                                }
					    break;
                            		       
					    case 0x56:  				            //POS B                                                                 
                                if(bufferDisplay2.flagActiveSale){
                                    flowDisplay2 = flowPosB;      //Si está vendiendo en esta posición regresa a su flujo
                                }
                                else{
                                    flowDisplay2 = 0;
                                    flowPosB     = 0;
                                } 
                                if (bufferDisplay4.flagActiveSale){                                    
                                    flowDisplay4 = flowPosD;    //Si la venta está activa en POS B regresa a su flujo
                                }else{
                                    SetPicture(2, DISPLAY_FORMA_PAGO_DESEADA); //Pantalla forma de pago
							        selectPosB    = 2;
                                    flowDisplay4  = 2;                            
                                }                           
					    break;                                                   
				    }                      
                    }
                }  
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();               
            }
            
        break;
        case 2: //Menu de metodo de pago
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x0D:  //Pantalla efectivo   
                            if(lockTurn == 1)
                            {
                                flowDisplay2 = 3; 
                                bufferDisplay2.saleType = 1;
                                SetPicture(2,DISPLAY_FORMA_PROGRAMACION);
                                AuthType2 = 2;
                                Display2_ClearRxBuffer();
                            }else{
                                SetPicture(2, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay2 = 0; 
                                Display2_ClearRxBuffer();                                                               
                            }
                        break;
                        case 0x0E:  //Pantalla credito 
                            for(x = 0; x < 10; x++)
                            {                                
                                bufferDisplay2.CreditpresetValue[0][x] = 0x00;
                                bufferDisplay2.CreditpresetValue[1][x] = 0x00;                                
                            }                            
                            if(lockTurn == 1)
                            {
                                flowDisplay2 = 10;
                                bufferDisplay2.saleType = 2;                                
                                SetPicture(2, DISPLAY_ID_DIGITAL);
                                AuthType2 = 1;
                                Display2_ClearRxBuffer();
                            }else
                            {
                                SetPicture(2, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay2 = 0;  
                                Display2_ClearRxBuffer();                                                             
                            }
                            
                        break;
                        case 0x45:  //Pantalla otras opciones 
                            flowDisplay2 = 12;                            
                            SetPicture(2,DISPLAY_OPERACIONES); 
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }
                } 
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();                
            }
            
        break;
            
        case 3:  //Menu de tipo de preset
            for(x = 0; x <10; x++)
            {
                bufferDisplay2.presetValue[0][x] = 0x00;
                bufferDisplay2.presetValue[1][x] = 0x00;
            }
            Credit_Auth_OK2 = 0;
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x0F:  //Preset dinero                
                            flowDisplay2 = 4;                              
                            bufferDisplay2.presetType[0] = 2;
                            bufferDisplay2.presetType[1] = '$';
                            numberKeys2 = 0;
                            SetPicture(2,DISPLAY_INTRODUZCA_VALOR);   
                            WriteLCD(2,'$',3,2,1,0x0000,'N');							
                            if(MoneyDec > 0)
                            {
                                flagPoint2 = 0;
                            }else
                            {
                                flagPoint2 = 1;
                            }
                            Display2_ClearRxBuffer();
                        break; 
                        case 0x10:  //Preset volumen                 
                            flowDisplay2 = 4;                            
                            bufferDisplay2.presetType[0] = 1;
                            bufferDisplay2.presetType[1] = 'G';
                            numberKeys2 = 0;                            
                            SetPicture(2,DISPLAY_INTRODUZCA_VOLUMEN);                            
                            WriteLCD(2,'G',3,2,1,0x0000,'N');                            							
                            if(VolDec > 0)
                            {
                                flagPoint2 = 0;
                            }else
                            {
                                flagPoint2 = 1;
                            }
                            Display2_ClearRxBuffer();
                        break;
                        case 0x43:  //Preset full 
                            bufferDisplay2.presetType[0] = 3;
                            bufferDisplay2.presetType[1] = 'F';                            
                            for(x = 1; x <(digits - 1); x++)
                            {
                                bufferDisplay2.presetValue[0][x] = '9';
                            }                            
                            bufferDisplay2.presetValue[0][x] = '0';
                            bufferDisplay2.presetValue[0][x + 1] = '0';
                            bufferDisplay2.presetValue[0][0] = digits;
                            for(x = 0; x <= bufferDisplay2.presetValue[0][0]; x++)
                            {
                                bufferDisplay2.presetValue[1][x] = bufferDisplay2.presetValue[0][x];
                            }
                            flowDisplay2 = 5;
                            SetPicture(2,DISPLAY_SELECCIONE_PRODUCTO4); 
                            Display2_ClearRxBuffer();                         
                        break;
                        case 0x94:  //Retroceso 
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0); 
                            Display2_ClearRxBuffer();                                                       
                        break;
                        case 0x3B:  //Pantalla Inicial    
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }
                }                  
                //vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }
           
        break;
        
        case 4:    // Teclado general      
            switch (alphanumeric_keyboard2(digits + 1,0))
            {
                case 0:  //Pantalla Inicial    
                    flowDisplay2 = 0;
                    SetPicture(2,DISPLAY_INICIO0);
                    Display2_ClearRxBuffer();                            
                break;
                    
                case 1: //Enter
                    for(x = 0; x <= bufferDisplay2.valueKeys[0]; x++)
                    {
                        bufferDisplay2.presetValue[0][x] = bufferDisplay2.valueKeys[x];
                        bufferDisplay2.presetValue[1][x] = bufferDisplay2.valueKeys[x];
                    }  
                    flowDisplay2 = 5;       //caso para seleccion de producto
                    SetPicture(2,DISPLAY_SELECCIONE_PRODUCTO4);
                    Display2_ClearRxBuffer();                    
                break;
            }

        break;
            
        case 5: //Menu de seleccion de producto
            if(1)//(side.b.GradesHose[0] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto1[x],11,7+x,2,0x0000,'Y');//WriteMessage(2, producto[side.b.GradesHose[0]-1][x],11,7+x,2,0x0000,'Y');
                } 
            }
            if(1)//(side.b.GradesHose[1] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto2[x],17,8+x,2,0x0000,'Y');//WriteMessage(2, producto[side.b.GradesHose[1]-1][x],17,8+x,2,0x0000,'Y');
                } 
            }
            if(side.b.GradesHose[2] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.b.GradesHose[2]-1][x],23,9+x,2,0x0000,'Y');
                } 
            }
            if(side.b.GradesHose[3] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.b.GradesHose[3]-1][x],29,10+x,2,0x0000,'Y');
                } 
            }
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x80:  //Grado 1 
                            numberKeys2 = 0; 
                            flowDisplay2 = 9;
                            side.b.hose = 1;                            
                            //Credit
                            if(AuthType2 == 2)
                            {
                                bufferDisplay2.flagKeyboard = 1; 
                                SetPicture(2, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType2 == 1)
                            {                    
                                bufferDisplay2.flagKeyboard = 2;
                                SetPicture(2, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display2_ClearRxBuffer();
                        break;        
                        case 0x81:  //Grado 2 
                            numberKeys2 = 0;
                            flowDisplay2 = 9;
                            side.b.hose = 2;                          
                            //Credit
                            if(AuthType2 == 2)
                            {
                                bufferDisplay2.flagKeyboard = 1; 
                                SetPicture(2, DISPLAY_DIGITE_PLACA);
                            }
                            
                            //Cash
                            if(AuthType2 == 1)
                            {                    
                                bufferDisplay2.flagKeyboard = 2;
                                SetPicture(2, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0x7F:  //Grado 3
                            numberKeys2 = 0;
                            flowDisplay2 = 9;
                            side.b.hose = 3;                            
                            //Credit
                            if(AuthType2 == 2)
                            {
                                bufferDisplay2.flagKeyboard = 1; 
                                SetPicture(2, DISPLAY_DIGITE_PLACA);
                            }
                            
                            //Cash
                            if(AuthType2 == 1)
                            {                    
                                bufferDisplay2.flagKeyboard = 2;
                                SetPicture(2, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display2_ClearRxBuffer();
                        break;   
                        
                        case 0xB8:  //Grado 4 
                            numberKeys2 = 0;
                            flowDisplay2 = 9;
                            side.b.hose = 4;                           
                            //Credit
                            if(AuthType2 == 2)
                            {
                                bufferDisplay2.flagKeyboard = 1; 
                                SetPicture(2, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType2 == 1)
                            {                    
                                bufferDisplay2.flagKeyboard = 2;
                                SetPicture(2, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Retroceso
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);    
                            Display2_ClearRxBuffer();                                                    
                        break;
                            
                        case 0x7E:  //Pantalla Inicial    
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                }
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();               
            }
            
        break;
                    
        case 6: //Pantalla de impresion SI/NO
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x39:  //Si imprimir  
                            flowDisplay2 = 7;
                            numberKeys2 = 0;
                            bufferDisplay2.flagPrint =  1;                            
                            SetPicture(2,DISPLAY_SUBA_MANIJA); 
                            Display2_ClearRxBuffer();                           
                        break; 
                        case 0x38:  //No imprimir 
                            flowDisplay2 = 7;//Esperando estado del dispensador 
                            bufferDisplay2.flagPrint =  0;                                                                
                            SetPicture(2,DISPLAY_SUBA_MANIJA);
                            Display2_ClearRxBuffer();
                        break;
                        case 0x7E:  //Pantalla Inicial                                                                                    
                            flowDisplay2 = 0;
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            bufferDisplay2.flagPrint = 0;
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                } 
                //vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();             
            }
                        
        break;                                             
        case 7: //Preset ON y espera descuelgue de manguera
            PresetFlag2 = 1;                  
            //Touch for return to init display
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            bufferDisplay2.flagPrint =  0;
                            PresetFlag2 = 0;
                            iButtonFlag2 = 0;
                            AuthType2 = 0;
                            bufferDisplay2.zeroSend = 1;
                            vTaskDelay( 50 / portTICK_PERIOD_MS );
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            bufferDisplay2.flagPrint =  0;
                            PresetFlag2 = 0;
                            iButtonFlag2 = 0;
                            AuthType2 = 0;
                            bufferDisplay2.zeroSend = 1;
                            vTaskDelay( 50 / portTICK_PERIOD_MS );
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                }                
                Display2_ClearRxBuffer();                
            }                      
        break;
            
        case 8:       
            //Pump Busy...
            vTaskDelay( 10 / portTICK_PERIOD_MS ); 
            if(NumPositions > 2)
                flowDisplay2 = 1;
        break;
            
        case 9: // Keyboard           
            switch (alphanumeric_keyboard2(11,0))
            {
                case 0: //Cancel
                    switch(bufferDisplay2.flagKeyboard)
                    {
                        case 1://License plate
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay2.licenceSale[x] = 0;
                            }
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 2://Mileage
                            for(x = 0; x < 10; x++)
                            {
                                bufferDisplay2.mileageSale[x] = 0;
                            }
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay2.identySale[x] = 0;
                            }
                            Display2_ClearRxBuffer();
                        break;
                        case 4://ID
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay2.shiftId[x] = 0;
                            }
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                    Display2_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay2.flagKeyboard)
                    {
                        case 1://Licence plate
                            for(x = 0; x < 10; x++)
                            {
                                bufferDisplay2.licenceSale[x] = 0x00;
                            }
                            
                            for(x = 0; x <= bufferDisplay2.valueKeys[0]; x++)
                            {
                                bufferDisplay2.licenceSale[x] = bufferDisplay2.valueKeys[x];
                            }
//                            if(bufferDisplay2.flagEndSale)
//                            {                                
//                                bufferDisplay2.flagPrint = 1;
//                                flowDisplay2 = 0;
//                                SetPicture(2,DISPLAY_INICIO0); 
//                            }else
//                            {
                                flowDisplay2 = 6;   
                                SetPicture(2, DISPLAY_DESEA_IMPRIMIR_RECIBO);                              
//                            }
                            
                        break;
                        
                        case 2:  //Mileage
                            for(x = 0; x < 10; x++)
                            {
                                bufferDisplay2.mileageSale[x] = 0x00;
                            }
                            for(x = 0; x <= bufferDisplay2.valueKeys[0]; x++)
                            {
                                bufferDisplay2.mileageSale[x] = bufferDisplay2.valueKeys[x];
                            }
                            
                            if(AuthType2 == 1)
                            {
                                flowDisplay2 = 7;                                
                                SetPicture(2,DISPLAY_SUBA_MANIJA);
                            }
                            else
                            {
                                flowDisplay2 = 6;      
                                SetPicture(2,DISPLAY_DESEA_IMPRIMIR_RECIBO);
                            }
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x <= bufferDisplay2.valueKeys[0]; x++)
                            {
                                bufferDisplay2.identySale[x] = bufferDisplay2.valueKeys[x];
                            }
                            Display2_ClearRxBuffer();
                        case 4://ID
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay2.shiftId[x] = bufferDisplay2.valueKeys[x];
                            }
                            flowDisplay2 = 14;      //Pide clave                            
                            numberKeys2 = 0;
                            hiddenKeys  = 10;
                            controlChar ='*';
                            bufferDisplay2.flagKeyboard = 3;
                            SetPicture(2,DISPLAY_INGRESE_PASSWORD);
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                //Display2_ClearRxBuffer();    
                break;

            }  
            Display2_ClearRxBuffer();           
        break;    
    ////////////////// CASOS PARA CRÉDITO  /////////////////////
        case 10:
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0xB6:  //Solicitud ibutton  
                            flowDisplay2 = 11;
                            numberKeys2  = 0;                            
                            bufferDisplay2.flagPrint =  1;
                            SetPicture(2,DISPLAY_ESPERANDO_ID);                            
                        break; 
                        case 0xB7:  //ID por número
                            bufferDisplay2.flagKeyboard = 0;
                            flowDisplay2 = 7;//Esperando estado del dispensador                                                          
                            SetPicture(2,DISPLAY_SUBA_MANIJA);                            
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                        break;
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            AuthType2 = 0;
                        break;
                    }                    
                }
                
                //vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            } 
                                  
        break;
        
        case 11: //Lectura del iButton  
            for(x = 0; x < 30; x++)
            {
                temporal[x] = 0x00;
            }
            for(x = 0; x < 25; x++)
            {
                bufferDisplay2.idSerial[x] = 0x00;
            }
            if(touch_present(2) == 1)
            {
				if(touch_write(2,0x33))
                {
					for(x = 1; x <= 8; x++)
                    {
						temporal[x] = touch_read_byte(2);
					}
					y = 0;
					for(x = 1; x < 8; x++)
                    {
                        y = crc_check(y,temporal[x]);
                    }
					if(y == temporal[8])
                    {
						bufferDisplay2.idSerial[0] = 16;
						y = 16;
						for(x = 1; x <= 8; x++)
                        {
							if((temporal[x] & 0x0F) >= 10)
                            {
								bufferDisplay2.idSerial[y] = (temporal[x] & 0x0F) + 55;
							}else{
								bufferDisplay2.idSerial[y] = (temporal[x] & 0x0F) + 48;				
							}
                            y--;
							if(((temporal[x] >> 4) & 0x0F) >= 10)
                            {
								bufferDisplay2.idSerial[y] = ((temporal[x] >> 4) & 0x0F) + 55;
							}else{
								bufferDisplay2.idSerial[y] = ((temporal[x] >> 4) & 0x0F) + 48;				
							}
                            y--;
						}                        
                        SetPicture(2,DISPLAY_ID_RECONOCIDO);                                           
                        vTaskDelay( 500 / portTICK_PERIOD_MS ); 
                        iButtonFlag2 = 1;
                        SetPicture(2, DISPLAY_FORMA_PROGRAMACION);
                        flowDisplay2 = 3;
					}                                     
				}
                else //iButton Error
                {
                    SetPicture(2,DISPLAY_ID_NO_RECONOCIDO);                                           
                    Display2_ClearRxBuffer();
                    vTaskDelay( 200 / portTICK_PERIOD_MS );    
                    SetPicture(2, DISPLAY_INICIO0);
                    flowDisplay2 = 0;
                    bufferDisplay2.flagPrint =  0;
                    PresetFlag2 = 0;
                    iButtonFlag2 = 0;
                    AuthType2 = 0;                    
                }
			}
           //Touch for return to init display
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            bufferDisplay2.flagPrint =  0;
                            PresetFlag2 = 0;
                            iButtonFlag2 = 0;
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            bufferDisplay2.flagPrint =  0;
                            PresetFlag2 = 0;
                            iButtonFlag2 = 0;
                            AuthType2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                }
                
                //vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display2_ClearRxBuffer();
            }
           
                                 
        break;    
    ///////////////FIN CASOS PARA CRÉDITO  /////////////////////  
    
    //////////// CASOS PARA CONFIGURACIONES  ///////////////////
        case 12:
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x46:  //Turnos              
                            flowDisplay2 = 13; 
                            if(lockTurn == 1)
                            {
                                SetPicture(2,DISPLAY_CERRAR_TURNO);
                            }else
                            {
                                SetPicture(2,DISPLAY_ABRIR_TURNO);
                            }                             
                        break;
                        case 0x55:  //Configurar módulo 
                            flowDisplay2 = 14;
                            bufferDisplay2.flagKeyboard = 6;
                            numberKeys2 = 0;
                            hiddenKeys = 5;
                            controlChar ='*';
                            SetPicture(2,DISPLAY_INGRESE_PASSWORD); 
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0xB5:  //Copia de recibo 
                            if(lockTurn == 1)
                            {
                                side.b.RFstateReport = 1;
                                side.b.rfStateCopy = RF_COPY_RECEIPT;
                                SetPicture(2,DISPLAY_IMPRIMIENDO_RECIBO); 
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay2  = 0;
                                Display2_ClearRxBuffer();
                            }else
                            {
                                SetPicture(2, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay2 = 0; 
                                Display2_ClearRxBuffer();
                            }
                        break;
                        
                        case 0x3B:  //Pantalla Inicial    
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0); 
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 0x7E:  //Pantalla Inicial
                             bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            Display2_ClearRxBuffer();
                        break;
                    }
                }                  
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }
        break;

        case 13:  //Pantalla de apertura/Cierre de turno
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x47:  //Cambio de turno: Abrir  
                            flowDisplay2 = 9;
                            numberKeys2 = 0;    
                            bufferDisplay2.flagKeyboard = 4;
                            SetPicture(2,DISPLAY_INTRODUZCA_CEDULA); 
                            Display2_ClearRxBuffer();
                        break;
                        case 0x48:  //Cambio de turno: Cerrar 
                            flowDisplay2 = 9;
                            numberKeys2 = 0;   
                            bufferDisplay2.flagKeyboard = 4;
                            SetPicture(2,DISPLAY_INTRODUZCA_CEDULA);  
                            Display2_ClearRxBuffer();
                        break;                         
                        case 0x7E:  //Pantalla Inicial 
                             bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }                        
        break; 
            
        case 14: //Teclado general      
            switch (alphanumeric_keyboard2(hiddenKeys,controlChar))
            {
                case 0: //Cancelar
                    switch(bufferDisplay2.flagKeyboard)
                    {
                        case 1://ID Estacion
                            
                            for(x = 0; x <= 4; x++)
                            {
                                idStation[x] = 0;
                            }                            
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                        case 2://Umbral
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                pumpGap[x] = 0;
                            }                            
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                        case 3://Pass turno
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay2.shiftPassword[x] = 0;
                            }
                            
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                        case 5://Pass
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay2.shiftPassword[x] = 0;
                            }
                            
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                            
                        case 6://Pass
                            
                            for(x = 1; x <= configAccess[0]; x++)
                            {
                                configAccess[x] = 0;
                            }
                            
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                    }                    
                    Display2_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay2.flagKeyboard)
                    {   
                        case 1://ID Estacion
                            
                            for(x = 0; x <= 4; x++)
                            {
                                idStation[x] = bufferDisplay2.valueKeys[x + 1];                                
                            }
                            
                            idStation[4] = 0x00;
                            intIDStation = atoi(idStation);
                            IDCast[0] = intIDStation;
                            IDCast[1] = intIDStation >> 8;
                            EEPROM_1_WriteByte(IDCast[0],10);
                            EEPROM_1_WriteByte(IDCast[1],11);
                            flowDisplay2 = 15;
                            SetPicture(2,DISPLAY_CONFIGURACIONES);
                        break;
                        case 2://Pass
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                pumpGap[x] = bufferDisplay2.valueKeys[x];
                            }
                            
                            flowDisplay2 = 15;
                            SetPicture(2,DISPLAY_CONFIGURACIONES);
                        break;
                        case 3://Pass turno
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay2.shiftPassword[x] = bufferDisplay2.valueKeys[x];
                            }
                            
                            flowDisplay2 = 19;
                            ShiftState = 1;
                            side.b.RFstateReport = 1;
                            SetPicture(2,DISPLAY_POR_FAVOR_ESPERE);
                        break;
                        case 5://Pass
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay2.shiftPassword[x] = bufferDisplay2.valueKeys[x];
                            }
                            
                            flowDisplay2 = 15;
                            SetPicture(2,DISPLAY_CONFIGURACIONES);
                        break; 
                        case 6://Pass
                            
                            for(x = 1; x <= configAccess[0]; x++)
                            {
                                configAccess[x] = bufferDisplay2.valueKeys[x];
                            }
                            
                            if(configAccess[1] == passwordPump[1] && configAccess[2] == passwordPump[2] && configAccess[3] == passwordPump[3] && configAccess[4] == passwordPump[4])
                            {                                
                                SetPicture(2,DISPLAY_PASSWORD_VALIDO);
                               
                                vTaskDelay( 700 / portTICK_PERIOD_MS );
                                flowDisplay2 = 15;
                                SetPicture(2,DISPLAY_CONFIGURACIONES);
                            }else
                            {
                                SetPicture(2,DISPLAY_PASSWORD_INVALIDO);
                               
                                vTaskDelay( 700 / portTICK_PERIOD_MS );
                                flowDisplay2 = 0;
                                SetPicture(2,DISPLAY_INICIO0);
                            }
                        break;
                    }                    
                    Display2_ClearRxBuffer();
                break;
            }            
        break;
            
        case 15: // Menu de configuraciones
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x82:  //ID Estacion  
                            flowDisplay2 = 14;
                            numberKeys2 = 0; 
                            controlChar = 0;
                            hiddenKeys  = 5;
                            bufferDisplay2.flagKeyboard = 1;
                            SetPicture(2,DISPLAY_INTRODUZCA_VALOR);                            
                        break;
                        case 0x65:  //Test Impresoras 
                            flowDisplay2 = 16; 
                            PrintTest();
                            SetPicture(2,DISPLAY_AMBAS_IMPRESORAS_FUNCIONANDO);                           
                        break;                        
                        case 0x5A:  //Umbral 
                            flowDisplay2 = 14;
                            numberKeys2 = 0;
                            controlChar = 0;
                            hiddenKeys  = 3;
                            pumpGap[0]  = 2;
                            bufferDisplay2.flagKeyboard = 2;
                            SetPicture(2,DISPLAY_INTRODUZCA_VALOR);                            
                        break;
                        case 0x58:  //Hora y Fecha
                            flowDisplay2 = 18;
                            numberKeys2 = 0;   
                            bufferDisplay2.flagKeyboard = 4;
                            SetPicture(2,DISPLAY_CONFIGURAR_FECHA_HORA);                           
                        break;
                        case 0x7E:  //Pantalla Inicial  
                             bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial   
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;

                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }                        
        break; 
            
        case 16:  //Menu de impresoras 1
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x38:  //No funcionan  
                            flowDisplay2 = 17;
                            SetPicture(2,DISPLAY_IMPRESORA_NO_FUNCIONANDO);                            
                        break;
                        case 0x39:  //Si funcionan
                            flowDisplay2 = 15; 
                            printPortA = 1;
                            printPortB = 2;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(2,DISPLAY_CONFIGURACIONES);
                        break;
                                               
                        case 0x7E:  //Pantalla Inicial  
                             bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 17:  //Menu de impresoras 2
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x8C:  //1 no funciona  
                            flowDisplay2 = 15; 
                            printPortA = 2;
                            printPortB = 2;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(2,DISPLAY_CONFIGURACIONES);                            
                        break;
                        case 0x8D:  //2 no funciona
                            flowDisplay2 = 15; 
                            printPortA = 1;
                            printPortB = 1;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(2,DISPLAY_CONFIGURACIONES); 
                        break;
                            
                        case 0x8E:  //ninguna funciona
                            flowDisplay2 = 15; 
                            printPortA = 0;
                            printPortB = 0;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(2,DISPLAY_CONFIGURACIONES); 
                        break;
                                               
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;

                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;

                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 18:  //Pantalla de ocnfiguracion de fecha y hora
            if(leer_hora() == 1)
            {
                WriteLCD(2,(((timeDownHandle[1] & 0x10) >> 4) + 48), 13, 8, 1, 0x0000, 'N');
                WriteLCD(2,((timeDownHandle[1] & 0x0F) + 48), 13, 9, 1, 0x0000, 'N');
                WriteLCD(2, ':', 13, 10, 1, 0x0000, 'N');
                WriteLCD(2,(((timeDownHandle[0] & 0xF0) >> 4) + 48), 13, 11, 1, 0x0000, 'N');
                WriteLCD(2,((timeDownHandle[0] & 0x0F) + 48), 13, 12, 1, 0x0000, 'N');               
            }
            if(leer_fecha() == 1)
            {                
                WriteLCD(2,(((dateDownHandle[0] & 0x30) >> 4) + 48), 21, 8, 1, 0x0000, 'N');
                WriteLCD(2,((dateDownHandle[0] & 0x0F) + 48), 21, 9, 1, 0x0000, 'N');
                WriteLCD(2, '/', 21, 10, 1, 0x0000, 'N');
                WriteLCD(2,(((dateDownHandle[1] & 0x10) >> 4) + 48), 21, 11, 1, 0x0000, 'N');
                WriteLCD(2,((dateDownHandle[1] & 0x0F) + 48), 21, 12, 1, 0x0000, 'N');
                WriteLCD(2, '/', 21, 13, 1, 0x0000, 'N');
                WriteLCD(2,(((dateDownHandle[2] & 0xF0) >> 4) + 48), 21, 14, 1, 0x0000, 'N');
                WriteLCD(2,((dateDownHandle[2] & 0x0F) + 48), 21, 15, 1, 0x0000, 'N');
            }
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                                                                       

                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                            
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                            
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 19:            
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;                            
                            flowPosB     = 0;
                            PresetFlag2  = 0;                            
                            iButtonFlag2 = 0;
                            ShiftState   = 0;
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;             
                            flowPosB     = 0;
                            PresetFlag2  = 0;
                            iButtonFlag2 = 0;
                            ShiftState   = 0;
                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
            }
            Display2_ClearRxBuffer();            
        break;
            
         // Credit NOT authorized
        case 21:            
            SetPicture(2, DISPLAY_AUTORIZACION_RECHAZADA);
            bufferDisplay2.flagPrint = 0;
            flowPosB     = 0;
            PresetFlag2  = 0;
            iButtonFlag2 = 0;
            for(x = 0; x < 9; x++)
            {
                WriteMessage(2, mensaje[x], 17, 1 + x, 3, 0x0000, 'Y');
                WriteMessage(2, mensaje2[x], 21, 1 + x, 3, 0x0000, 'Y');
                    
            }
            side.b.rfState = RF_IDLE;
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
            flowDisplay2 = 0;
            SetPicture(2, DISPLAY_INICIO0);
        break;
            // Wrong Grade handle
        case 22:
            
            SetPicture(2, DISPLAY_AUTORIZACION_RECHAZADA);
            bufferDisplay2.flagPrint = 0;
            
            for(x = 0; x < 8; x++)
            {                
                WriteMessage(2, mensaje3[x], 17, 1 + x, 4, 0x0000, 'Y');                    
            }
            for(x = 0; x < 10; x++)
            {                
                WriteMessage(2, mensaje4[x], 21, 1 + x, 4, 0x0000, 'Y');       
            }
             
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
            flowDisplay2 = 0;
            SetPicture(2, DISPLAY_INICIO0);
        break;
         
    }    
}

/*
*********************************************************************************************************
*                                         void PollingDisplay3(void)
*
* Description : Pregunta estado al surtidor 
*               
*
*********************************************************************************************************
*/

void PollingDisplay3(void){    
    uint8 x, y;
    
    switch(flowDisplay3){
        case 0:                                                
            SetPicture(1, DISPLAY_SELECCIONE_POSICION);
            flowDisplay3 = 1; 
            vTaskDelay( 10 / portTICK_PERIOD_MS );
            Display1_ClearRxBuffer();            
        break;
            
        case 1: //Menu 
             // Impresion de tiquete            
            if(bufferDisplay3.flagActiveSale){
                flowDisplay3 = flowPosC;            
            }
            if(bufferDisplay1.flagActiveSale){
                flowDisplay1 = flowPos;
                selectPos = 1;
            }
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {                                             
                    switch(Display1_rxBuffer[3]){
				        case 0x56:						        //POS A                                                                                                                       
                                if(bufferDisplay1.flagActiveSale){
                                    flowDisplay1 = flowPos;      //Si está vendiendo en esta posición regresa a su flujo
                                }else{
                                    SetPicture(1, DISPLAY_FORMA_PAGO_DESEADA); //Pantalla forma de pago
                                    selectPos     = 1;
                                    flowDisplay1  = 2;
                                }
                                if (bufferDisplay3.flagActiveSale){
                                    flowDisplay3 = flowPosC;    //Si la venta está activa en POS B regresa a su flujo
                                }
                                else{
                                    flowDisplay3 = 0; 
                                    flowPosC     = 0;
                                }
					    break;
                            		       
					    case 0x57:  				            //POS B                                                                 
                                if(bufferDisplay1.flagActiveSale){
                                    flowDisplay1 = flowPos;      //Si está vendiendo en esta posición regresa a su flujo
                                }
                                else{
                                    flowDisplay1 = 0;
                                    flowPos      = 0;
                                } 
                                if (bufferDisplay3.flagActiveSale){                                    
                                    flowDisplay3 = flowPosC;    //Si la venta está activa en POS B regresa a su flujo
                                }else{
                                    SetPicture(1, DISPLAY_FORMA_PAGO_DESEADA); //Pantalla forma de pago
							        selectPos     = 2;
                                    flowDisplay3  = 2;                            
                                }                           
					    break;                                                   
				    } 
                }  
            }         
        break;
        case 2:  //Pantalla forma de pago
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C)){
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x0D:  //Pantalla efectivo   
                            if(lockTurn == 1)
                            {
                                flowDisplay3 = 3; 
                                bufferDisplay3.saleType = 1;
                                SetPicture(1, DISPLAY_FORMA_PROGRAMACION);  
                                AuthType3 = 2;
                                Display1_ClearRxBuffer();
                            }else
                            {
                                SetPicture(1, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay3 = 0;
                                Display1_ClearRxBuffer();
                            }
                        break;
                        case 0x0E:  //Pantalla credito 
                            for(x = 0; x < 10; x++)
                            {                                
                                bufferDisplay3.CreditpresetValue[0][x] = 0x00;
                                bufferDisplay3.CreditpresetValue[1][x] = 0x00;
                                
                            }
                            if(lockTurn == 1)
                            {
                                flowDisplay3 = 10;
                                bufferDisplay3.saleType = 2;                                
                                SetPicture(1, DISPLAY_ID_DIGITAL);
                                AuthType3 = 1;
                                Display1_ClearRxBuffer();
                            }else
                            {
                                SetPicture(1, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay3 = 0;                                                                
                            }
                        break;
                            
                        case 0x45:  //Pantalla otras opciones 
                            flowDisplay3 = 12;                            
                            SetPicture(1,  DISPLAY_OPERACIONES);
                            AuthType3 = 0;
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1, DISPLAY_INICIO0);   
                            AuthType3 = 0;
                            vTaskDelay( 10 / portTICK_PERIOD_MS );
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1, DISPLAY_INICIO0);
                            AuthType3 = 0;
                            vTaskDelay( 10 / portTICK_PERIOD_MS );
                            Display1_ClearRxBuffer();
                        break;
                    }
                }                  
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
            
        case 3: //Pantalla ingreso de valores
            for(x = 0; x <10; x++)
            {
                bufferDisplay3.presetValue[0][x] = 0x00;
                bufferDisplay3.presetValue[1][x] = 0x00;
            }
            Credit_Auth_OK3 = 0;
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x0F:  //Preset dinero                
                            flowDisplay3 = 4;                              
                            bufferDisplay3.presetType[0] = 2;
                            bufferDisplay3.presetType[1] = '$';
                            numberKeys3 = 0;
                            SetPicture(1, DISPLAY_INTRODUZCA_VALOR);   
                            WriteLCD(1, '$', 3, 2, 1, 0x0000, 'N');                            
                            if(MoneyDec > 0)
                            {
                                flagPoint3 = 0;
                            }else
                            {
                                flagPoint3 = 1;
                            }
                            Display1_ClearRxBuffer();
                        break; 
                        case 0x10:  //Preset volumen                 
                            flowDisplay3 = 4;                            
                            bufferDisplay3.presetType[0] = 1;
                            bufferDisplay3.presetType[1] = 'G';
                            numberKeys3 = 0;                            
                            SetPicture(1, DISPLAY_INTRODUZCA_VOLUMEN);                            
                            WriteLCD(1, 'G', 3, 2, 1, 0x0000, 'N');                            
                            if(VolDec > 0)
                            {
                                flagPoint3 = 0;
                            }else
                            {
                                flagPoint3 = 1;
                            }
                            Display1_ClearRxBuffer();
                        break;
                        case 0x43:     //Preset full 
                            bufferDisplay3.presetType[0] = 3;
                            bufferDisplay3.presetType[1] = 'F';                            
                            for(x = 1; x <(digits - 1); x++)
                            {
                                bufferDisplay3.presetValue[0][x] = '9';
                            }                            
                            bufferDisplay3.presetValue[0][x] = '0';
                            bufferDisplay3.presetValue[0][x + 1] = '0';
                            bufferDisplay3.presetValue[0][0] = digits;
                            
                            for(x = 0; x <= bufferDisplay3.presetValue[0][0]; x++)
                            {
                                bufferDisplay3.presetValue[1][x] = bufferDisplay3.presetValue[0][x];
                            }                            
                            flowDisplay3 = 5;
                            SetPicture(1,DISPLAY_SELECCIONE_PRODUCTO4);
                            Display1_ClearRxBuffer();
                        break;
                        case 0x94:  //Retroceso
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                        case 0x3B:  //Pantalla Inicial  
                            AuthType3 = 0;                        
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);                            
                            Display1_ClearRxBuffer();
                        break;
                        case 0x7E:  //Pantalla Inicial  
                            AuthType3 = 0;
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);                             
                            Display1_ClearRxBuffer();
                        break;
                    }
                }  
                //CyDelay(10);
                //vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
        
        case 4:            
            switch (alphanumeric_keyboard3(digits + 1, 0))
            {
                case 0:  //Pantalla Inicial    
                    flowDisplay3 = 0;
                    SetPicture(1,DISPLAY_INICIO0); 
                    Display1_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    for(x = 0; x <= bufferDisplay3.valueKeys[0]; x++)
                    {
                        bufferDisplay3.presetValue[0][x] = bufferDisplay3.valueKeys[x];
                        bufferDisplay3.presetValue[1][x] = bufferDisplay3.valueKeys[x];
                    }  
                    flowDisplay3 = 5;   //caso para seleccion de producto
                    SetPicture(1, DISPLAY_SELECCIONE_PRODUCTO4);
                    Display1_ClearRxBuffer();
                break;
            }
        break;
            
        case 5: //Seleccion de producto
            
            if(side.c.GradesHose[0] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                   WriteMessage(1, producto1[x],11,7+x,2,0x0000,'Y'); //WriteMessage(1, producto[side.a.GradesHose[0]-1][x],11,7+x,2,0xFFFF,'Y');
                } 
            }
            if(side.c.GradesHose[1] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.c.GradesHose[1]-1][x],17,8+x,2,0x0000,'Y');
                } 
            }
            if(side.c.GradesHose[2] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.c.GradesHose[2]-1][x],23,9+x,2,0x0000,'Y');
                } 
            }
            if(side.c.GradesHose[3] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.c.GradesHose[3]-1][x],29,10+x,2,0x0000,'Y');
                } 
            }
            
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x80:  //Grado 1 
                            numberKeys3=0; 
                            flowDisplay3 = 9;
                            side.c.hose = 1;                         
                            //Credit
                            if(AuthType3 == 2)
                            {
                                bufferDisplay3.flagKeyboard = 1; 
                                SetPicture(1, DISPLAY_DIGITE_PLACA);
                            }
                            
                            //Cash
                            if(AuthType3 == 1)
                            {                    
                                bufferDisplay3.flagKeyboard = 2;
                                SetPicture(1, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x81:  //Grado 2 
                            numberKeys3=0; 
                            flowDisplay3 = 9;
                            side.c.hose = 2;
                            //Credit
                            if(AuthType3 == 2)
                            {
                                bufferDisplay3.flagKeyboard = 1; 
                                SetPicture(1, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType3 == 1)
                            {                    
                                bufferDisplay3.flagKeyboard = 2;
                                SetPicture(1, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display1_ClearRxBuffer(); 
                        break;
                            
                        case 0x7F:  //Grado 3
                            numberKeys3=0; 
                            flowDisplay3 = 9;
                            side.c.hose = 3;
                           //Credit
                            if(AuthType3 == 2)
                            {
                                bufferDisplay3.flagKeyboard = 1; 
                                SetPicture(1, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType3 == 1)
                            {                    
                                bufferDisplay3.flagKeyboard = 2;
                                SetPicture(1, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display1_ClearRxBuffer();
                        break;   
                        
                        case 0xB8:  //Grado 4 
                            numberKeys3=0; 
                            flowDisplay3 = 9;
                            side.c.hose = 4;
                            //Credit
                            if(AuthType3 == 2)
                            {
                                bufferDisplay3.flagKeyboard = 1; 
                                SetPicture(1, DISPLAY_DIGITE_PLACA);
                            }                            
                            //Cash
                            if(AuthType3 == 1)
                            {                    
                                bufferDisplay3.flagKeyboard = 2;
                                SetPicture(1, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            } 
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Retroceso
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                            AuthType3 = 0;
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                }
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
                    
        case 6: // Print Screen

            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x39:  //Print 
                            flowDisplay3 = 7;
                            numberKeys3 = 0;
                            bufferDisplay3.flagPrint =  1;                                                       
                            SetPicture(1, DISPLAY_SUBA_MANIJA);  
                            Display1_ClearRxBuffer();
                        break; 
                        case 0x38:  //No Print 
//                            if(bufferDisplay3.flagEndSale)
//                            {
//                                flowDisplay3 = 0;
//                                bufferDisplay3.flagPrint = 0;
//                                SetPicture(1, DISPLAY_INICIO0);
//                            }else
//                            {
                            flowDisplay3 = 7; // Wait of handle up
                            bufferDisplay3.flagPrint =  0;                                                               
                            SetPicture(1, DISPLAY_SUBA_MANIJA);
                            Display1_ClearRxBuffer();
//                            } 
                        break;
                        case 0x7E:  //Return to initial screen                                                                                    
                            flowDisplay3 = 0;
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            bufferDisplay3.flagPrint =  0;
                            AuthType3 = 0;
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                }               
                //vTaskDelay( 10 / portTICK_PERIOD_MS );              // 10mS delay
                Display1_ClearRxBuffer();
            }
        break;
                                        
            // PRESET flag ON and wait to handle up
        case 7:                       
            PresetFlag3 = 1;
            //Touch for return to init display
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                            bufferDisplay3.flagPrint =  0;
                            PresetFlag3 = 0;
                            iButtonFlag3 = 0;
                            AuthType3 = 0;     
                            bufferDisplay3.zeroSend = 1;
                            vTaskDelay( 50 / portTICK_PERIOD_MS );                      
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                            bufferDisplay3.flagPrint =  0;
                            PresetFlag3 = 0;
                            iButtonFlag3 = 0;
                            AuthType3 = 0;
                            bufferDisplay4.zeroSend = 1;
                            vTaskDelay( 50 / portTICK_PERIOD_MS );
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                }
                
               Display1_ClearRxBuffer();
            }
           
        break;
            
          
        case 8:                          
           //Pump Busy...
            vTaskDelay( 10 / portTICK_PERIOD_MS ); 
            if(NumPositions > 2)
                flowDisplay3 = 1;
        break;
            
        case 9: //Keyboard           
            switch (alphanumeric_keyboard3(11,0))
            {
                case 0: //Cancel
                    switch(bufferDisplay3.flagKeyboard)
                    {
                        case 1://License plate
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay3.licenceSale[x] = 0;
                            }
                            flowDisplay3 = 0; 
                            SetPicture(1, DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 2://Mileage
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay3.mileageSale[x] = 0;
                            }        
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay3.identySale[x] = 0;
                            }
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 4://ID
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay3.shiftId[x] = 0;
                            }
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                    Display1_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay3.flagKeyboard)
                    {
                        case 1://License plate
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay3.licenceSale[x] = 0x00;
                            }                            
                            for(x = 0; x <= bufferDisplay3.valueKeys[0]; x++)
                            {
                                bufferDisplay3.licenceSale[x] = bufferDisplay3.valueKeys[x];
                            }
//                            if(bufferDisplay3.flagEndSale)
//                            {
//                                bufferDisplay3.flagPrint = 1;
//                                flowDisplay3 = 0;
//                                SetPicture(1, DISPLAY_INICIO0); 
//                            }else{
                            flowDisplay3 = 6;
                            SetPicture(1, DISPLAY_DESEA_IMPRIMIR_RECIBO);                                 
//                            }
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 2:// mileage
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay3.mileageSale[x] = 0;
                            }                         
                            for(x = 0; x <= bufferDisplay3.valueKeys[0]; x++)
                            {
                                bufferDisplay3.mileageSale[x] = bufferDisplay3.valueKeys[x];
                            }
                            
                            //Credit Sale
                            if(AuthType3 == 1)
                            {
                                flowDisplay3 = 7;                               
                                SetPicture(1, DISPLAY_SUBA_MANIJA);
                            }
                            else
                            {                                
                                flowDisplay3 = 6;       
                                SetPicture(1, DISPLAY_DESEA_IMPRIMIR_RECIBO);
                            }                                               
                            Display1_ClearRxBuffer();                           
                        break;
                        
                        case 3:     //CC/NIT                            
                            for(x = 0; x <= bufferDisplay3.valueKeys[0]; x++)
                            {
                                bufferDisplay3.identySale[x] = bufferDisplay3.valueKeys[x];
                            }
                            Display1_ClearRxBuffer();
                        break;
                        case 4:     //ID                            
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay3.shiftId[x] = bufferDisplay3.valueKeys[x];
                            }                            
                            flowDisplay3 = 14;          //password request                    
                            numberKeys3 = 0;
                            hiddenKeys = 10;
                            controlChar = '*';
                            bufferDisplay3.flagKeyboard = 3;
                            SetPicture(1,DISPLAY_INGRESE_PASSWORD);
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                    //Display1_ClearRxBuffer();
                break;
            }  
            Display1_ClearRxBuffer();
        break;    
    
    ////////////////// CASOS PARA CRÉDITO  /////////////////////
        case 10:
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0xB6:  //ibutton Request   
                            flowDisplay3 = 11;
                            numberKeys3 = 0;                            
                            bufferDisplay3.flagPrint =  1;
                            SetPicture(1, DISPLAY_ESPERANDO_ID);                            
                        break; 
                        case 0xB7:  //ID Number
                            bufferDisplay3.flagKeyboard = 0;
                            flowDisplay3 = 7;                                                        
                            SetPicture(1,DISPLAY_SUBA_MANIJA);                            
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1, DISPLAY_INICIO0);                            
                        break;
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                        break;
                    }                    
                }             
                //vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
        
        case 11:
            // iButton read
            for(x = 0; x < 30; x++)
            {
                temporal[x] = 0x00;
            }
            for(x = 0; x < 25; x++)
            {
                bufferDisplay3.idSerial[x] = 0x00;
            }
            if(touch_present(1) == 1)
            {
				if(touch_write(1,0x33))
                {
					for(x = 1; x <= 8; x++)
                    {
						temporal[x] = touch_read_byte(1);  // Id
					}
					y = 0;
					for(x = 1; x < 8; x++)
                    {
                        y = crc_check(y,temporal[x]);      // Checksum
                    }
					
                    if(y == temporal[8])
                    {
						bufferDisplay3.idSerial[0] = 16;
						y = 16;						
                        for(x = 1; x <= 8; x++)
                        {
							if((temporal[x] & 0x0F) >= 10)
                            {
								bufferDisplay3.idSerial[y] = (temporal[x] & 0x0F) + 55;
							}else{
								bufferDisplay3.idSerial[y] = (temporal[x] & 0x0F) + 48;				
							}
                            y--;
							if(((temporal[x] >> 4) & 0x0F) >= 10)
                            {
								bufferDisplay3.idSerial[y] = ((temporal[x] >> 4) & 0x0F) + 55;
							}else{
								bufferDisplay3.idSerial[y] = ((temporal[x] >> 4) & 0x0F) + 48;				
							}
                            y--;
						}                        
                        // Authorization request
                        SetPicture(1,DISPLAY_ID_RECONOCIDO);                                                                 
                        vTaskDelay( 500 / portTICK_PERIOD_MS );                       
                        iButtonFlag3 = 1;
                        SetPicture(1, DISPLAY_FORMA_PROGRAMACION);
                        flowDisplay3 = 3;                                                                        
					}
				}else
                    {   // iButton Error
                        SetPicture(1,DISPLAY_ID_NO_RECONOCIDO);                                           
                        Display1_ClearRxBuffer();
                        vTaskDelay( 500 / portTICK_PERIOD_MS );    
                        SetPicture(1, DISPLAY_INICIO0);
                        flowDisplay3 = 0;
                        bufferDisplay3.flagPrint =  0;
                        PresetFlag3 = 0;
                        iButtonFlag3 = 0;
                        AuthType3 = 0;                        
                    }
			}
                     
            //Touch for return to init display
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                            bufferDisplay3.flagPrint =  0;
                            PresetFlag3 = 0;
                            iButtonFlag3 = 0;
                            AuthType3 = 0;
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                            bufferDisplay3.flagPrint =  0;
                            PresetFlag3 = 0;
                            iButtonFlag3 = 0;
                            AuthType3 = 0;
                            Display1_ClearRxBuffer();
                        break;
                    }                    
                }                
                Display1_ClearRxBuffer();
            }
          
                                    
        break;    
    ///////////////FIN CASOS PARA CRÉDITO  /////////////////////  
    
    //////////// CASOS PARA CONFIGURACIONES  ///////////////////
        case 12:
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x46:  //Turnos              
                            flowDisplay3 = 13; 
                            if(lockTurn == 1)
                            {
                                SetPicture(1,DISPLAY_CERRAR_TURNO);
                            }else
                            {
                                SetPicture(1,DISPLAY_ABRIR_TURNO);
                            }                            
                        break;
                        case 0x55:  //Configurar módulo 
                            flowDisplay3 = 14;
                            bufferDisplay3.flagKeyboard = 6;
                            numberKeys3 = 0;
                            hiddenKeys = 5;
                            controlChar ='*';
                            SetPicture(1,DISPLAY_INGRESE_PASSWORD);                           
                        break;
                        case 0xB5:  //Copia de recibo 
                            if(lockTurn == 1)
                            {
                                side.c.RFstateReport = 1;
                                side.c.rfStateCopy = RF_COPY_RECEIPT;
                                SetPicture(1,DISPLAY_IMPRIMIENDO_RECIBO); 
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay3  = 0;                                                                                                 
                            }else
                            {
                                SetPicture(1, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay3 = 0;                                                                
                            }
                        break;
                        
                        case 0x3B:  //Pantalla Inicial    
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0); 
                            Display1_ClearRxBuffer();
                        break;
                        
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            Display1_ClearRxBuffer();
                        break;
                    }
                }                  
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }
        break;

        case 13:
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x47:  //Open Shift  
                            flowDisplay3 = 9;
                            numberKeys3 = 0;    
                            bufferDisplay3.flagKeyboard = 4;
                            SetPicture(1,DISPLAY_INTRODUZCA_CEDULA);                            
                        break;
                        case 0x48:  //Close shift 
                            flowDisplay3 = 9;
                            numberKeys3 = 0;   
                            bufferDisplay3.flagKeyboard = 4;
                            SetPicture(1,DISPLAY_INTRODUZCA_CEDULA);                           
                        break;                         
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break; 
            
        case 14: //Teclado general      
            switch (alphanumeric_keyboard(hiddenKeys,controlChar))
            {
                case 0: //Cancelar
                    switch(bufferDisplay3.flagKeyboard)
                    {
                        case 1://ID Estacion
                            for(x = 0; x <= 4; x++)
                            {
                                idStation[x] = 0;
                            }
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                        case 2://Umbral
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                pumpGap[x] = 0;
                            }
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                        case 3://Pass turno
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay3.shiftPassword[x] = 0;
                            }
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                        case 5://Pass
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay3.shiftPassword[x] = 0;
                            }
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                            
                        case 6://Pass
                            for(x = 1; x <= configAccess[0]; x++)
                            {
                                configAccess[x] = 0;
                            }
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                    }                    
                    Display1_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay3.flagKeyboard)
                    {   
                        case 1://ID Estacion
                            for(x = 0; x <= 4; x++)
                            {
                                idStation[x] = bufferDisplay3.valueKeys[x+1];                                
                            }
                            idStation[4] = 0x00;
                            intIDStation = atoi(idStation);
                            IDCast[0] = intIDStation;
                            IDCast[1] = intIDStation>>8;
                            EEPROM_1_WriteByte(IDCast[0],10);
                            EEPROM_1_WriteByte(IDCast[1],11);
                            flowDisplay3 = 15;
                            SetPicture(1,DISPLAY_CONFIGURACIONES);
                        break;
                        case 2://Pass
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                pumpGap[x] = bufferDisplay1.valueKeys[x];
                            }
                            flowDisplay3 = 15;
                            SetPicture(1,DISPLAY_CONFIGURACIONES);
                        break;
                        case 3://Pass turno
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay3.shiftPassword[x] = bufferDisplay3.valueKeys[x];
                            }
                            flowDisplay3 = 19;
                            SetPicture(1,DISPLAY_POR_FAVOR_ESPERE);
                            side.c.RFstateReport = 1;
                            ShiftState = 1;
                        break;
                        case 5://Pass
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay3.shiftPassword[x] = bufferDisplay3.valueKeys[x];
                            }
                            flowDisplay3 = 15;
                            SetPicture(1,DISPLAY_CONFIGURACIONES);
                        break; 
                        case 6://Pass
                            for(x = 1; x <= configAccess[0]; x++)
                            {
                                configAccess[x] = bufferDisplay3.valueKeys[x];
                            }
                            if(configAccess[1] == passwordPump[1] && configAccess[2] == passwordPump[2] && configAccess[3] == passwordPump[3] && configAccess[4] == passwordPump[4])
                            {                                
                                SetPicture(1,DISPLAY_PASSWORD_VALIDO);
                                //CyDelay(700);
                                vTaskDelay( 700 / portTICK_PERIOD_MS );
                                flowDisplay3 = 15;
                                SetPicture(1,DISPLAY_CONFIGURACIONES);
                            }else{
                                SetPicture(1,DISPLAY_PASSWORD_INVALIDO);
                                //CyDelay(700);
                                vTaskDelay( 700 / portTICK_PERIOD_MS );
                                flowDisplay3 = 0;
                                SetPicture(1,DISPLAY_INICIO0);
                            }
                        break;
                    }                    
                    Display1_ClearRxBuffer();
                break;
            }            
        break;
            
        case 15: //Menu de configuraciones
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x82:  //ID Estacion  
                            flowDisplay3 = 14;
                            numberKeys3 = 0; 
                            controlChar = 0;
                            hiddenKeys  = 5;
                            bufferDisplay3.flagKeyboard = 1;
                            SetPicture(1,DISPLAY_INTRODUZCA_VALOR);                            
                        break;
                        case 0x65:  //Test Impresoras 
                            flowDisplay3 = 16; 
                            PrintTest();
                            SetPicture(1,DISPLAY_AMBAS_IMPRESORAS_FUNCIONANDO);                           
                        break;        
                        case 0x5A:  //Umbral 
                            flowDisplay3 = 14;
                            numberKeys3 = 0;
                            controlChar = 0;
                            hiddenKeys  = 3;
                            pumpGap[0]  = 2;
                            bufferDisplay3.flagKeyboard = 2;
                            SetPicture(1,DISPLAY_INTRODUZCA_VALOR);                            
                        break;
                        case 0x58:  //Hora y Fecha
                            flowDisplay3 = 18;
                            numberKeys3 = 0;   
                            bufferDisplay3.flagKeyboard = 4;
                            SetPicture(1,DISPLAY_CONFIGURAR_FECHA_HORA);                           
                        break;
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                        break;
                    }                    
                }
                     
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break; 
            
        case 16: //Menu de configuraciones de impresoras 1
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x38:  //No funcionan  
                            flowDisplay3 = 17;
                            SetPicture(1,DISPLAY_IMPRESORA_NO_FUNCIONANDO);                            
                        break;
                        case 0x39:  //Si funcionan
                            flowDisplay3 = 15; 
                            printPortA = 1;
                            printPortB = 2;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(1,DISPLAY_CONFIGURACIONES);
                        break;                                             
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
            
        case 17: //Menu de configuraciones de impresoras 2
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x8C:  //1 no funciona  
                            flowDisplay3 = 15; 
                            printPortA = 2;
                            printPortB = 2;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(1,DISPLAY_CONFIGURACIONES);                            
                        break;
                        case 0x8D:  //2 no funciona
                            flowDisplay3 = 15; 
                            printPortA = 1;
                            printPortB = 1;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(1,DISPLAY_CONFIGURACIONES); 
                        break;
                            
                        case 0x8E:  //ninguna funciona
                            flowDisplay3 = 15; 
                            printPortA = 0;
                            printPortB = 0;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(1,DISPLAY_CONFIGURACIONES); 
                        break;
                                               
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay3.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                        break;
                    }                    
                }

                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
            
        case 18: //Menu configuracion manual de fecha y hora
            if(leer_hora() == 1)
            {
                WriteLCD(1,(((timeDownHandle[1] & 0x10) >> 4) + 48),13,8,1,0x0000,'N');
                WriteLCD(1,((timeDownHandle[1] & 0x0F) + 48),13,9,1,0x0000,'N');
                WriteLCD(1,':',13,10,1,0x0000,'N');
                WriteLCD(1,(((timeDownHandle[0] & 0xF0)>>4)+48),13,11,1,0x0000,'N');
                WriteLCD(1,((timeDownHandle[0] & 0x0F)+48),13,12,1,0x0000,'N');               
            }
            if(leer_fecha() == 1)
            {                
                WriteLCD(1,(((dateDownHandle[0] & 0x30) >> 4) + 48),21,8,1,0x0000,'N');
                WriteLCD(1,((dateDownHandle[0] & 0x0F) + 48),21,9,1,0x0000,'N');
                WriteLCD(1,'/',21,10,1,0x0000,'N');
                WriteLCD(1,(((dateDownHandle[1] & 0x10) >> 4) + 48),21,11,1,0x0000,'N');
                WriteLCD(1,((dateDownHandle[1] & 0x0F) + 48),21,12,1,0x0000,'N');
                WriteLCD(1,'/',21,13,1,0x0000,'N');
                WriteLCD(1,(((dateDownHandle[2] & 0xF0) >> 4) + 48),21,14,1,0x0000,'N');
                WriteLCD(1,((dateDownHandle[2] & 0x0F) + 48),21,15,1,0x0000,'N');
            }
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                                                                       
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);                            
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            SetPicture(1,DISPLAY_INICIO0);                            
                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
            
        case 19:             
           
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                            bufferDisplay3.flagPrint =  0;
                            PresetFlag3 = 0;
                            iButtonFlag3 = 0;
                            ShiftState = 0;
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay3 = 0;
                            bufferDisplay3.flagPrint =  0;
                            PresetFlag3 = 0;
                            iButtonFlag3 = 0;
                            ShiftState = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
            }
            Display1_ClearRxBuffer();
            
        break;
            
        case 20: //pantalla de espera de autorizacion
            SetPicture(1, DISPLAY_ESPERANDO_AUTORIZACION);
            //Touch for return to init display
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            bufferDisplay3.flagPrint =  0;
                            flowPosC     = 0;
                            flowDisplay3 = 0;                            
                            PresetFlag3  = 0;
                            iButtonFlag3 = 0;
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            bufferDisplay3.flagPrint =  0;
                            flowDisplay3 = 0;
                            flowPosC     = 0;
                            PresetFlag3  = 0;
                            iButtonFlag3 = 0;
                            ShiftState   = 0;
                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
            }
            Display1_ClearRxBuffer();
            
        break;
            
            
        case 21: //Pantalla de credito no autorizado            
            SetPicture(1, DISPLAY_AUTORIZACION_RECHAZADA);
            bufferDisplay3.flagPrint = 0;
            flowPosC     = 0;
            PresetFlag3  = 0;
            iButtonFlag3 = 0;
            for(x = 0; x < 9; x++)
            {
                WriteMessage(1, mensaje[x],17,1 + x,4,0x0000,'Y');
                WriteMessage(1, mensaje2[x],21,1 + x,4,0x0000,'Y');
                    
            }
            side.c.rfState = RF_IDLE;
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
            flowDisplay3 = 0;
            SetPicture(1, DISPLAY_INICIO0);
        break;
            
        // Pantalla de Manguera equivocada
        case 22:
            
            SetPicture(1, DISPLAY_AUTORIZACION_RECHAZADA);
            bufferDisplay3.flagPrint = 0;

            for(x = 0; x < 8; x++)
            {                
                WriteMessage(1, mensaje3[x], 17, 1 + x, 4, 0x0000, 'Y');                    
            }
            for(x = 0; x < 10; x++)
            {                
                WriteMessage(1, mensaje4[x], 21, 1 + x, 4, 0x0000, 'Y');                    
            }
             
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
            flowDisplay3 = 0;
            SetPicture(1, DISPLAY_INICIO0);
        break;            
    }    
}
/*
*********************************************************************************************************
*                                         void PollingDisplay4(void)
*
* Description : Pregunta estado al surtidor 
*               
*
*********************************************************************************************************
*/
void PollingDisplay4(void){    
    uint8 x,y;  
    
    switch(flowDisplay4){
        case 0:			                      
            SetPicture(2, DISPLAY_SELECCIONE_POSICION);
            // Impresion de tiquete  
            flowDisplay4 = 1; 
            vTaskDelay( 10 / portTICK_PERIOD_MS );
            Display2_ClearRxBuffer();                
        break;
        case 1: //Menu               
            if(bufferDisplay4.flagActiveSale){
                flowDisplay4 = flowPosD;            
            }
            if(bufferDisplay2.flagActiveSale){
                flowDisplay2 = flowPosB;
                selectPosB   = 1;
            }
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {                                             
                    switch(Display2_rxBuffer[3]){
				        case 0x57:						        //POS A                                                                                                                       
                                if(bufferDisplay2.flagActiveSale){
                                    flowDisplay2 = flowPosB;      //Si está vendiendo en esta posición regresa a su flujo
                                }else{
                                    SetPicture(2, DISPLAY_FORMA_PAGO_DESEADA); //Pantalla forma de pago
                                    selectPosB     = 1;
                                    flowDisplay2   = 2;
                                }
                                if (bufferDisplay4.flagActiveSale){
                                    flowDisplay4 = flowPosD;    //Si la venta está activa en POS B regresa a su flujo
                                }
                                else{
                                    flowDisplay4 = 0;  
                                    flowPosD     = 0;
                                }
					    break;
                            		       
					    case 0x56:  				            //POS B                                                                 
                                if(bufferDisplay2.flagActiveSale){
                                    flowDisplay2 = flowPosB;      //Si está vendiendo en esta posición regresa a su flujo
                                }
                                else{
                                    flowDisplay2 = 0;
                                    flowPosB     = 0;
                                } 
                                if (bufferDisplay4.flagActiveSale){                                    
                                    flowDisplay4 = flowPosD;    //Si la venta está activa en POS B regresa a su flujo
                                }else{
                                    SetPicture(2, DISPLAY_FORMA_PAGO_DESEADA); //Pantalla forma de pago
							        selectPosB    = 2;
                                    flowDisplay4  = 2;                            
                                }                           
					    break;                                                   
				    }
                }                 
            }
            
        break;
        case 2: //Menu de metodo de pago
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x0D:  //Pantalla efectivo   
                            if(lockTurn == 1)
                            {
                                flowDisplay4 = 3; 
                                bufferDisplay4.saleType = 1;
                                SetPicture(2,DISPLAY_FORMA_PROGRAMACION);
                                AuthType4 = 2;
                                Display2_ClearRxBuffer();
                            }else{
                                SetPicture(2, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay4 = 0; 
                                Display2_ClearRxBuffer();                                                               
                            }
                        break;
                        case 0x0E:  //Pantalla credito 
                            for(x = 0; x < 10; x++)
                            {                                
                                bufferDisplay4.CreditpresetValue[0][x] = 0x00;
                                bufferDisplay4.CreditpresetValue[1][x] = 0x00;
                                
                            }                            
                            if(lockTurn == 1)
                            {
                                flowDisplay4 = 10;
                                bufferDisplay4.saleType = 2;                                
                                SetPicture(2, DISPLAY_ID_DIGITAL);
                                AuthType4 = 1;
                                Display2_ClearRxBuffer();
                            }else
                            {
                                SetPicture(2, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay4 = 0;  
                                Display2_ClearRxBuffer();                                                             
                            }
                            
                        break;
                        case 0x45:  //Pantalla otras opciones 
                            flowDisplay4 = 12;                            
                            SetPicture(2,DISPLAY_OPERACIONES); 
                            AuthType4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType4 = 0;
                            vTaskDelay( 10 / portTICK_PERIOD_MS );
                            Display2_ClearRxBuffer();
                        break;
                    }
                } 
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();                
            }
            
        break;
            
        case 3:  //Menu de tipo de preset
            for(x = 0; x <10; x++)
            {
                bufferDisplay4.presetValue[0][x] = 0x00;
                bufferDisplay4.presetValue[1][x] = 0x00;
            }
            Credit_Auth_OK4 = 0;
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x0F:  //Preset dinero                
                            flowDisplay4 = 4;                              
                            bufferDisplay4.presetType[0] = 2;
                            bufferDisplay4.presetType[1] = '$';
                            numberKeys4 = 0;
                            SetPicture(2,DISPLAY_INTRODUZCA_VALOR);   
                            WriteLCD(2,'$',3,2,1,0x0000,'N');							
                            if(MoneyDec > 0)
                            {
                                flagPoint4 = 0;
                            }else
                            {
                                flagPoint4 = 1;
                            }
                            Display2_ClearRxBuffer();
                        break; 
                        case 0x10:  //Preset volumen                 
                            flowDisplay4 = 4;                            
                            bufferDisplay4.presetType[0] = 1;
                            bufferDisplay4.presetType[1] = 'G';
                            numberKeys4 = 0;                            
                            SetPicture(2,DISPLAY_INTRODUZCA_VOLUMEN);                            
                            WriteLCD(2,'G',3,2,1,0x0000,'N');                            							
                            if(VolDec > 0)
                            {
                                flagPoint4 = 0;
                            }else
                            {
                                flagPoint4 = 1;
                            }
                            Display2_ClearRxBuffer();
                        break;
                        case 0x43:  //Preset full 
                            bufferDisplay4.presetType[0] = 3;
                            bufferDisplay4.presetType[1] = 'F';
                            
                            for(x = 1; x <(digits - 1); x++)
                            {
                                bufferDisplay4.presetValue[0][x] = '9';
                            }                            
                            bufferDisplay4.presetValue[0][x] = '0';
                            bufferDisplay4.presetValue[0][x + 1] = '0';
                            bufferDisplay4.presetValue[0][0] = digits;
                            for(x = 0; x <= bufferDisplay4.presetValue[0][0]; x++)
                            {
                                bufferDisplay4.presetValue[1][x] = bufferDisplay4.presetValue[0][x];
                            }
                            flowDisplay4 = 5;
                            SetPicture(2,DISPLAY_SELECCIONE_PRODUCTO4); 
                            Display2_ClearRxBuffer();                         
                        break;
                        case 0x94:  //Retroceso 
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0); 
                            Display2_ClearRxBuffer();                                                       
                        break;
                        case 0x3B:  //Pantalla Inicial  
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }
                }  
                
                //vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }
           
        break;
        
        case 4:    // Teclado general      
            switch (alphanumeric_keyboard4(digits + 1,0))
            {
                case 0:  //Pantalla Inicial    
                    flowDisplay4 = 0;
                    bufferDisplay4.flagPrint =  0;
                    SetPicture(2,DISPLAY_INICIO0);
                    Display2_ClearRxBuffer();                            
                break;
                    
                case 1: //Enter
                    for(x = 0; x <= bufferDisplay4.valueKeys[0]; x++)
                    {
                        bufferDisplay4.presetValue[0][x] = bufferDisplay4.valueKeys[x];
                        bufferDisplay4.presetValue[1][x] = bufferDisplay4.valueKeys[x];
                    }  
                    flowDisplay4 = 5;       //caso para seleccion de producto
                    SetPicture(2,DISPLAY_SELECCIONE_PRODUCTO4);
                    Display2_ClearRxBuffer();
                    vTaskDelay( 10 / portTICK_PERIOD_MS );
                break;
            }

        break;
            
        case 5: //Menu de seleccion de producto
            if(side.d.GradesHose[0] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.d.GradesHose[0]-1][x],11,7+x,2,0x0000,'Y');
                } 
            }
            if(side.d.GradesHose[1] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.d.GradesHose[1]-1][x],17,8+x,2,0x0000,'Y');
                } 
            }
            if(side.d.GradesHose[2] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.d.GradesHose[2]-1][x],23,9+x,2,0x0000,'Y');
                } 
            }
            if(side.d.GradesHose[3] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.d.GradesHose[3]-1][x],29,10+x,2,0x0000,'Y');
                } 
            }
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x80:  //Grado 1 
                            numberKeys4 = 0; 
                            flowDisplay4 = 9;
                            side.d.hose = 1;                            
                            //Credit
                            if(AuthType4 == 2)
                            {
                                bufferDisplay4.flagKeyboard = 1; 
                                SetPicture(2, DISPLAY_DIGITE_PLACA);
                            }
                            
                            //Cash
                            if(AuthType4 == 1)
                            {                    
                                bufferDisplay4.flagKeyboard = 2;
                                SetPicture(2, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display2_ClearRxBuffer();
                        break;        
                        case 0x81:  //Grado 2 
                            numberKeys4 = 0;
                            flowDisplay4 = 9;
                            side.d.hose = 2;                          
                            //Credit
                            if(AuthType4 == 2)
                            {
                                bufferDisplay4.flagKeyboard = 1; 
                                SetPicture(2, DISPLAY_DIGITE_PLACA);
                            }
                            
                            //Cash
                            if(AuthType4 == 1)
                            {                    
                                bufferDisplay4.flagKeyboard = 2;
                                SetPicture(2, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0x7F:  //Grado 3
                            numberKeys4 = 0;
                            flowDisplay4 = 9;
                            side.d.hose = 3;                            
                            //Credit
                            if(AuthType4 == 2)
                            {
                                bufferDisplay4.flagKeyboard = 1; 
                                SetPicture(2, DISPLAY_DIGITE_PLACA);
                            }
                            
                            //Cash
                            if(AuthType4 == 1)
                            {                    
                                bufferDisplay4.flagKeyboard = 2;
                                SetPicture(2, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display2_ClearRxBuffer();
                        break;   
                        
                        case 0xB8:  //Grado 4 
                            numberKeys4 = 0;
                            flowDisplay4 = 9;
                            side.d.hose = 4;                           
                            //Credit
                            if(AuthType4 == 2)
                            {
                                bufferDisplay4.flagKeyboard = 1; 
                                SetPicture(2, DISPLAY_DIGITE_PLACA);
                            }
                            
                            //Cash
                            if(AuthType4 == 1)
                            {                    
                                bufferDisplay4.flagKeyboard = 2;
                                SetPicture(2, DISPLAY_INTRODUZCA_KILOMETRAJE);                            
                            }
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Retroceso
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);    
                            Display2_ClearRxBuffer();                                                    
                        break;
                            
                        case 0x7E:  //Pantalla Inicial    
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                            AuthType4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                }
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();               
            }            
        break;
                    
        case 6: //Pantalla de impresion SI/NO

            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x39:  //Si imprimir  
                            flowDisplay4 = 7;
                            numberKeys4 = 0;
                            bufferDisplay4.flagPrint =  1;
                            SetPicture(2,DISPLAY_SUBA_MANIJA); 
                            Display2_ClearRxBuffer();                           
                        break; 
                        case 0x38:  //No imprimir 
//                            if(bufferDisplay4.flagEndSale)
//                            {
//                                bufferDisplay4.flagPrint = 0;
//                                flowDisplay4 = 0;
//                                SetPicture(2,DISPLAY_INICIO0);
//                                Display2_ClearRxBuffer();
//                            }else
//                            {
                            flowDisplay4 = 7;//Esperando estado del dispensador 
                            bufferDisplay4.flagPrint =  0;                                                                
                            SetPicture(2,DISPLAY_SUBA_MANIJA);
                            Display2_ClearRxBuffer();
//                            }
                        break;
                        case 0x7E:  //Pantalla Inicial                                                                                    
                            flowDisplay4 = 0;
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            bufferDisplay4.flagPrint = 0;
                            AuthType4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                } 
                //vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();             
            }            
        break;                                             
        case 7: //Preset ON y espera descuelgue de manguera
            PresetFlag4 = 1;                 
            //Touch for return to init display
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                            bufferDisplay4.flagPrint =  0;
                            PresetFlag4 = 0;
                            iButtonFlag4 = 0;
                            AuthType4 = 0;
                            bufferDisplay4.zeroSend = 1;
                            vTaskDelay( 50 / portTICK_PERIOD_MS );
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                            bufferDisplay4.flagPrint =  0;
                            PresetFlag4 = 0;
                            iButtonFlag4 = 0;
                            AuthType4 = 0;
                            bufferDisplay4.zeroSend = 1;
                            vTaskDelay( 50 / portTICK_PERIOD_MS );
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                }                
                Display2_ClearRxBuffer();                
            }                      
        break;
            
        case 8:           
            //Pump Busy...
            vTaskDelay( 10 / portTICK_PERIOD_MS ); 
            if(NumPositions > 2)
                flowDisplay4 = 1;
        break;
            
        case 9: // Keyboard           
            switch (alphanumeric_keyboard4(11,0))
            {
                case 0: //Cancel
                    switch(bufferDisplay4.flagKeyboard)
                    {
                        case 1://License plate
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay4.licenceSale[x] = 0;
                            }
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 2://Mileage
                            for(x = 0; x < 10; x++)
                            {
                                bufferDisplay4.mileageSale[x] = 0;
                            }
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay4.identySale[x] = 0;
                            }
                            Display2_ClearRxBuffer();
                        break;
                        case 4://ID
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay4.shiftId[x] = 0;
                            }
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                    Display2_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay4.flagKeyboard)
                    {
                        case 1://Licence plate
                            for(x = 0; x < 10; x++)
                            {
                                bufferDisplay4.licenceSale[x] = 0x00;
                            }                            
                            for(x = 0; x <= bufferDisplay4.valueKeys[0]; x++)
                            {
                                bufferDisplay4.licenceSale[x] = bufferDisplay4.valueKeys[x];
                            }
//                            if(bufferDisplay4.flagEndSale)
//                            {                                
//                                bufferDisplay4.flagPrint = 1;
//                                flowDisplay4 = 0;
//                                SetPicture(2,DISPLAY_INICIO0); 
//                            }else
//                            {
                            flowDisplay4 = 6;   
                            SetPicture(2, DISPLAY_DESEA_IMPRIMIR_RECIBO);                              
//                            }   
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 2:  //Mileage
                            for(x = 0; x < 10; x++)
                            {
                                bufferDisplay4.mileageSale[x] = 0x00;
                            }
                            for(x = 0; x <= bufferDisplay4.valueKeys[0]; x++)
                            {
                                bufferDisplay4.mileageSale[x] = bufferDisplay4.valueKeys[x];
                            }
                            
                            if(AuthType4 == 1)
                            {
                                flowDisplay4 = 7;                                
                                SetPicture(2,DISPLAY_SUBA_MANIJA);
                            }
                            else
                            {
                                flowDisplay4 = 6;      
                                SetPicture(2,DISPLAY_DESEA_IMPRIMIR_RECIBO);
                            }
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x <= bufferDisplay4.valueKeys[0]; x++)
                            {
                                bufferDisplay4.identySale[x] = bufferDisplay4.valueKeys[x];
                            }
                            Display2_ClearRxBuffer();
                        case 4://ID
                            for(x = 0; x <= 10; x++)
                            {
                                bufferDisplay4.shiftId[x] = bufferDisplay4.valueKeys[x];
                            }
                            flowDisplay4 = 14;      //Pide clave                            
                            numberKeys4 = 0;
                            hiddenKeys  = 10;
                            controlChar ='*';
                            bufferDisplay4.flagKeyboard = 3;
                            SetPicture(2,DISPLAY_INGRESE_PASSWORD);
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                //Display2_ClearRxBuffer();    
                break;

            }  
            Display2_ClearRxBuffer();           
        break;    
    ////////////////// CASOS PARA CRÉDITO  /////////////////////
        case 10:
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0xB6:  //Solicitud ibutton  
                            flowDisplay4 = 11;
                            numberKeys4  = 0;                            
                            bufferDisplay4.flagPrint =  1;
                            SetPicture(2,DISPLAY_ESPERANDO_ID);                            
                        break; 
                        case 0xB7:  //ID por número
                            bufferDisplay4.flagKeyboard = 0;
                            flowDisplay4 = 7;//Esperando estado del dispensador                                                          
                            SetPicture(2,DISPLAY_SUBA_MANIJA);                            
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType4 = 0;
                        break;
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                            AuthType4 = 0;
                        break;
                    }                    
                }
                
                //vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            } 
                                  
        break;
        
        case 11: //Lectura del iButton 
            for(x = 0; x < 30; x++)
            {
                temporal[x] = 0x00;
            }
            for(x = 0; x < 25; x++)
            {
                bufferDisplay4.idSerial[x] = 0x00;
            }
            if(touch_present(2) == 1)
            {
				if(touch_write(2,0x33))
                {
					for(x = 1; x <= 8; x++)
                    {
						temporal[x] = touch_read_byte(2);
					}
					y = 0;
					for(x = 1; x < 8; x++)
                    {
                        y = crc_check(y,temporal[x]);
                    }
					if(y == temporal[8])
                    {
						bufferDisplay4.idSerial[0] = 16;
						y = 16;
						for(x = 1; x <= 8; x++)
                        {
							if((temporal[x] & 0x0F) >= 10)
                            {
								bufferDisplay4.idSerial[y] = (temporal[x] & 0x0F) + 55;
							}else{
								bufferDisplay4.idSerial[y] = (temporal[x] & 0x0F) + 48;				
							}
                            y--;
							if(((temporal[x] >> 4) & 0x0F) >= 10)
                            {
								bufferDisplay4.idSerial[y] = ((temporal[x] >> 4) & 0x0F) + 55;
							}else{
								bufferDisplay4.idSerial[y] = ((temporal[x] >> 4) & 0x0F) + 48;				
							}
                            y--;
						}                        
                        SetPicture(2,DISPLAY_ID_RECONOCIDO);                                           
                        vTaskDelay( 500 / portTICK_PERIOD_MS ); 
                        iButtonFlag4 = 1;
                        SetPicture(2, DISPLAY_FORMA_PROGRAMACION);
                        flowDisplay4 = 3;
					}                                     
				}
                else //iButton Error
                {
                    SetPicture(2,DISPLAY_ID_NO_RECONOCIDO);                                           
                    Display2_ClearRxBuffer();
                    vTaskDelay( 500 / portTICK_PERIOD_MS );    
                    SetPicture(2, DISPLAY_INICIO0);
                    flowDisplay4 = 0;
                    bufferDisplay4.flagPrint =  0;
                    PresetFlag4 = 0;
                    iButtonFlag4 = 0;
                    AuthType4 = 0;
                    Display2_ClearRxBuffer();
                }
			}
           //Touch for return to init display
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                            bufferDisplay4.flagPrint =  0;
                            PresetFlag4 = 0;
                            iButtonFlag4 = 0;
                            AuthType4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                            bufferDisplay4.flagPrint =  0;
                            PresetFlag4 = 0;
                            iButtonFlag4 = 0;
                            AuthType4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                }
                
                //vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display2_ClearRxBuffer();
            }
           
                                 
        break;    
    ///////////////FIN CASOS PARA CRÉDITO  /////////////////////  
    
    //////////// CASOS PARA CONFIGURACIONES  ///////////////////
        case 12:
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x46:  //Turnos              
                            flowDisplay4 = 13; 
                            if(lockTurn == 1)
                            {
                                SetPicture(2,DISPLAY_CERRAR_TURNO);
                            }else
                            {
                                SetPicture(2,DISPLAY_ABRIR_TURNO);
                            } 
                            Display2_ClearRxBuffer();
                        break;
                        case 0x55:  //Configurar módulo 
                            flowDisplay4 = 14;
                            bufferDisplay4.flagKeyboard = 6;
                            numberKeys4 = 0;
                            hiddenKeys = 5;
                            controlChar ='*';
                            SetPicture(2,DISPLAY_INGRESE_PASSWORD); 
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0xB5:  //Copia de recibo 
                            if(lockTurn == 1)
                            {
                                side.d.RFstateReport = 1;
                                side.d.rfStateCopy = RF_COPY_RECEIPT;
                                SetPicture(2,DISPLAY_IMPRIMIENDO_RECIBO); 
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay4  = 0;
                                Display2_ClearRxBuffer();
                            }else
                            {
                                SetPicture(2, DISPLAY_CANCELADO_X_PC);
                                vTaskDelay( 900 / portTICK_PERIOD_MS );
                                flowDisplay4 = 0; 
                                Display2_ClearRxBuffer();
                            }
                        break;
                        
                        case 0x3B:  //Pantalla Inicial    
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0); 
                            Display2_ClearRxBuffer();
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            Display2_ClearRxBuffer();
                        break;
                        
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            Display2_ClearRxBuffer();
                        break;
                    }
                }  
                
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }
        break;

        case 13:  //Pantalla de apertura/Cierre de turno
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x47:  //Cambio de turno: Abrir  
                            flowDisplay4 = 9;
                            numberKeys4 = 0;    
                            bufferDisplay4.flagKeyboard = 4;
                            SetPicture(2,DISPLAY_INTRODUZCA_CEDULA); 
                            Display2_ClearRxBuffer();
                        break;
                        case 0x48:  //Cambio de turno: Cerrar 
                            flowDisplay4 = 9;
                            numberKeys4 = 0;   
                            bufferDisplay4.flagKeyboard = 4;
                            SetPicture(2,DISPLAY_INTRODUZCA_CEDULA);  
                            Display2_ClearRxBuffer();
                        break;                         
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                            Display2_ClearRxBuffer();
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }                        
        break; 
            
        case 14: //Teclado general      
            switch (alphanumeric_keyboard4(hiddenKeys,controlChar))
            {
                case 0: //Cancelar
                    switch(bufferDisplay4.flagKeyboard)
                    {
                        case 1://ID Estacion
                            
                            for(x = 0; x <= 4; x++)
                            {
                                idStation[x] = 0;
                            }                            
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                        case 2://Umbral                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                pumpGap[x] = 0;
                            }                            
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                        case 3://Pass turno                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay4.shiftPassword[x] = 0;
                            }
                            
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                        case 5://Pass
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay4.shiftPassword[x] = 0;
                            }
                            
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                            
                        case 6://Pass
                            
                            for(x = 1; x <= configAccess[0]; x++)
                            {
                                configAccess[x] = 0;
                            }
                            
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                    }                    
                    Display2_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay4.flagKeyboard)
                    {   
                        case 1://ID Estacion
                            
                            for(x = 0; x <= 4; x++)
                            {
                                idStation[x] = bufferDisplay4.valueKeys[x + 1];                                
                            }
                            
                            idStation[4] = 0x00;
                            intIDStation = atoi(idStation);
                            IDCast[0] = intIDStation;
                            IDCast[1] = intIDStation >> 8;
                            EEPROM_1_WriteByte(IDCast[0],10);
                            EEPROM_1_WriteByte(IDCast[1],11);
                            flowDisplay4 = 15;
                            SetPicture(2,DISPLAY_CONFIGURACIONES);
                        break;
                        case 2://Pass
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                pumpGap[x] = bufferDisplay4.valueKeys[x];
                            }
                            
                            flowDisplay4 = 15;
                            SetPicture(2,DISPLAY_CONFIGURACIONES);
                        break;
                        case 3://Pass turno
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay4.shiftPassword[x] = bufferDisplay4.valueKeys[x];
                            }
                            
                            flowDisplay4 = 19;
                            ShiftState = 1;
                            side.d.RFstateReport = 1;
                            SetPicture(2,DISPLAY_POR_FAVOR_ESPERE);
                        break;
                        case 5://Pass
                            
                            for(x = 0; x < hiddenKeys; x++)
                            {
                                bufferDisplay4.shiftPassword[x] = bufferDisplay4.valueKeys[x];
                            }
                            
                            flowDisplay4 = 15;
                            SetPicture(2,DISPLAY_CONFIGURACIONES);
                        break; 
                        case 6://Pass                            
                            for(x = 1; x <= configAccess[0]; x++)
                            {
                                configAccess[x] = bufferDisplay4.valueKeys[x];
                            }                            
                            if(configAccess[1] == passwordPump[1] && configAccess[2] == passwordPump[2] && configAccess[3] == passwordPump[3] && configAccess[4] == passwordPump[4])
                            {                                
                                SetPicture(2,DISPLAY_PASSWORD_VALIDO);                               
                                vTaskDelay( 700 / portTICK_PERIOD_MS );
                                flowDisplay4 = 15;
                                SetPicture(2,DISPLAY_CONFIGURACIONES);
                            }else
                            {
                                SetPicture(2,DISPLAY_PASSWORD_INVALIDO);                               
                                vTaskDelay( 700 / portTICK_PERIOD_MS );
                                flowDisplay4 = 0;
                                SetPicture(2,DISPLAY_INICIO0);
                            }
                        break;
                    }                    
                    Display2_ClearRxBuffer();
                break;
            }            
        break;
            
        case 15: // Menu de configuraciones
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x82:  //ID Estacion  
                            flowDisplay4 = 14;
                            numberKeys4 = 0; 
                            controlChar = 0;
                            hiddenKeys  = 5;
                            bufferDisplay4.flagKeyboard = 1;
                            SetPicture(2,DISPLAY_INTRODUZCA_VALOR);                            
                        break;
                        case 0x65:  //Test Impresoras 
                            flowDisplay4 = 16; 
                            PrintTest();
                            SetPicture(2,DISPLAY_AMBAS_IMPRESORAS_FUNCIONANDO);                           
                        break;                        
                        case 0x5A:  //Umbral 
                            flowDisplay4 = 14;
                            numberKeys4 = 0;
                            controlChar = 0;
                            hiddenKeys  = 3;
                            pumpGap[0]  = 2;
                            bufferDisplay4.flagKeyboard = 2;
                            SetPicture(2,DISPLAY_INTRODUZCA_VALOR);                            
                        break;
                        case 0x58:  //Hora y Fecha
                            flowDisplay4 = 18;
                            numberKeys4 = 0;   
                            bufferDisplay4.flagKeyboard = 4;
                            SetPicture(2,DISPLAY_CONFIGURAR_FECHA_HORA);                           
                        break;
                        case 0x7E:  //Pantalla Inicial  
                             bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial   
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }                        
        break; 
            
        case 16:  //Menu de impresoras 1
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x38:  //No funcionan  
                            flowDisplay4 = 17;
                            SetPicture(2,DISPLAY_IMPRESORA_NO_FUNCIONANDO);                            
                        break;
                        case 0x39:  //Si funcionan
                            flowDisplay4 = 15; 
                            printPortA = 1;
                            printPortB = 2;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(2,DISPLAY_CONFIGURACIONES);
                        break;
                                               
                        case 0x7E:  //Pantalla Inicial  
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 17:  //Menu de impresoras 2
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x8C:  //1 no funciona  
                            flowDisplay4 = 15; 
                            printPortA = 2;
                            printPortB = 2;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(2,DISPLAY_CONFIGURACIONES);                            
                        break;
                        case 0x8D:  //2 no funciona
                            flowDisplay4 = 15; 
                            printPortA = 1;
                            printPortB = 1;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(2,DISPLAY_CONFIGURACIONES); 
                        break;
                            
                        case 0x8E:  //ninguna funciona
                            flowDisplay4 = 15; 
                            printPortA = 0;
                            printPortB = 0;
                            EEPROM_1_WriteByte(printPortA,8);
                            EEPROM_1_WriteByte(printPortB,9);
                            SetPicture(2,DISPLAY_CONFIGURACIONES); 
                        break;
                                               
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                        break;
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay4.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay4 = 0;
                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 18:  //Pantalla de ocnfiguracion de fecha y hora
            if(leer_hora() == 1)
            {
                WriteLCD(2,(((timeDownHandle[1] & 0x10) >> 4) + 48), 13, 8, 1, 0x0000, 'N');
                WriteLCD(2,((timeDownHandle[1] & 0x0F) + 48), 13, 9, 1, 0x0000, 'N');
                WriteLCD(2, ':', 13, 10, 1, 0x0000, 'N');
                WriteLCD(2,(((timeDownHandle[0] & 0xF0) >> 4) + 48), 13, 11, 1, 0x0000, 'N');
                WriteLCD(2,((timeDownHandle[0] & 0x0F) + 48), 13, 12, 1, 0x0000, 'N');               
            }
            if(leer_fecha() == 1)
            {                
                WriteLCD(2,(((dateDownHandle[0] & 0x30) >> 4) + 48), 21, 8, 1, 0x0000, 'N');
                WriteLCD(2,((dateDownHandle[0] & 0x0F) + 48), 21, 9, 1, 0x0000, 'N');
                WriteLCD(2, '/', 21, 10, 1, 0x0000, 'N');
                WriteLCD(2,(((dateDownHandle[1] & 0x10) >> 4) + 48), 21, 11, 1, 0x0000, 'N');
                WriteLCD(2,((dateDownHandle[1] & 0x0F) + 48), 21, 12, 1, 0x0000, 'N');
                WriteLCD(2, '/', 21, 13, 1, 0x0000, 'N');
                WriteLCD(2,(((dateDownHandle[2] & 0xF0) >> 4) + 48), 21, 14, 1, 0x0000, 'N');
                WriteLCD(2,((dateDownHandle[2] & 0x0F) + 48), 21, 15, 1, 0x0000, 'N');
            }
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                                                                       

                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                            
                        break;
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay4.flagPrint =  0;
                            flowDisplay4 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                            
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 19:            
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                        
                        case 0x7E:  //Init Screen                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            bufferDisplay4.flagPrint =  0;
                            flowPosD     = 0;
                            flowDisplay4 = 0;                            
                            PresetFlag4  = 0;
                            iButtonFlag4 = 0;
                            ShiftState   = 0;
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            bufferDisplay4.flagPrint =  0;
                            flowPosD     = 0;
                            flowDisplay4 = 0;                            
                            PresetFlag4  = 0;
                            iButtonFlag4 = 0;
                            ShiftState   = 0;
                        break;
                    }                    
                }                
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
            }
            Display2_ClearRxBuffer();
            
        break;
            
         // Credit NOT authorized
        case 21:            
            SetPicture(2, DISPLAY_AUTORIZACION_RECHAZADA);
            bufferDisplay4.flagPrint = 0;
            flowPosD     = 0;
            PresetFlag4  = 0;
            iButtonFlag4 = 0;
            for(x = 0; x < 9; x++)
            {
                WriteMessage(2, mensaje[x], 17, 1 + x, 3, 0x0000, 'Y');
                WriteMessage(2, mensaje2[x], 21, 1 + x, 3, 0x0000, 'Y');                    
            }
            side.d.rfState = RF_IDLE;
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
            flowDisplay4 = 0;
            SetPicture(2, DISPLAY_INICIO0);
        break;
            
            // Wrong Grade handle
        case 22:            
            SetPicture(2, DISPLAY_AUTORIZACION_RECHAZADA);
            bufferDisplay4.flagPrint = 0;            
            for(x = 0; x < 8; x++)
            {                
                WriteMessage(2, mensaje3[x], 17, 1 + x, 4, 0x0000, 'Y');                    
            }
            for(x = 0; x < 10; x++)
            {                
                WriteMessage(2, mensaje4[x], 21, 1 + x, 4, 0x0000, 'Y');       
            }
             
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
            flowDisplay4 = 0;
            SetPicture(2, DISPLAY_INICIO0);
        break;
         
    }    
}


/*
 * Actualiza los precios con los valores almacenados en la memoria
 */
void ReadPPUFromEEprom(void)
{
    uint8 i, x;
                               
    //PPU recovery from EEprom
        for(i = 0; i <  side.a.hoseNumber; i++)
        {   
            if(i == 0)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.a.ppuAuthorized[0][x] =  EEPROM_1_ReadByte(20 + x); //PPU to EEprom
                }
            }
            
            if(i == 1)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.a.ppuAuthorized[1][x] =  EEPROM_1_ReadByte(25 + x); //PPU to EEprom
                }
            }
            if(i == 2)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.a.ppuAuthorized[2][x] =  EEPROM_1_ReadByte(30 + x); //PPU to EEprom
                }
            }
            if(i == 3)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.a.ppuAuthorized[3][x] =  EEPROM_1_ReadByte(35 + x); //PPU to EEprom
                }
            }
                    
        }
        
        for(i = 0; i <  side.b.hoseNumber; i++)
        {   
            if(i == 0)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.b.ppuAuthorized[0][x] =  EEPROM_1_ReadByte(40 + x); //PPU to EEprom
                }
            }
            
            if(i == 1)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.b.ppuAuthorized[1][x] =  EEPROM_1_ReadByte(45 + x); //PPU to EEprom
                }
            }
            if(i == 2)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.b.ppuAuthorized[2][x] =  EEPROM_1_ReadByte(50 + x); //PPU to EEprom
                }
            }
            if(i == 3)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.b.ppuAuthorized[3][x] =  EEPROM_1_ReadByte(55 + x); //PPU to EEprom
                }
            }
                    
        }
        for(i = 0; i <  side.c.hoseNumber; i++)
        {   
            if(i == 0)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.c.ppuAuthorized[0][x] =  EEPROM_1_ReadByte(60 + x); //PPU to EEprom
                }
            }
            
            if(i == 1)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.c.ppuAuthorized[1][x] =  EEPROM_1_ReadByte(65 + x); //PPU to EEprom
                }
            }
            if(i == 2)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.c.ppuAuthorized[2][x] =  EEPROM_1_ReadByte(70 + x); //PPU to EEprom
                }
            }
            if(i == 3)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.c.ppuAuthorized[3][x] =  EEPROM_1_ReadByte(75 + x); //PPU to EEprom
                }
            }
                    
        }
        for(i = 0; i <  side.d.hoseNumber; i++)
        {   
            if(i == 0)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.d.ppuAuthorized[0][x] =  EEPROM_1_ReadByte(80 + x); //PPU to EEprom
                }
            }
            
            if(i == 1)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.d.ppuAuthorized[1][x] =  EEPROM_1_ReadByte(85 + x); //PPU to EEprom
                }
            }
            if(i == 2)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.d.ppuAuthorized[2][x] =  EEPROM_1_ReadByte(90 + x); //PPU to EEprom
                }
            }
            if(i == 3)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    side.d.ppuAuthorized[3][x] =  EEPROM_1_ReadByte(95 + x); //PPU to EEprom
                }
            }
                    
        }
}

void ReadPPUFromEEpromInit(void)
{
    uint8 i, x;
    uint8 TempPrice[5]  = "00060";
    uint8 TempPrice2[5] = "00070";
    uint8 TempPrice3[5] = "00080";
    uint8 TempPrice4[5] = "00090";
    //PPU recovery from EEprom
    //Comentar los precios temporales cuando se trabaje con sistema
        for(i = 0; i <  side.a.hoseNumber; i++)
        {   
            if(i == 0)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.a.ppuAuthorized[0][x] =  EEPROM_1_ReadByte(20 + x); //PPU to EEprom
                    side.a.ppuAuthorized[0][x] =  TempPrice[x];
                }
            }
            
            if(i == 1)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.a.ppuAuthorized[1][x] =  EEPROM_1_ReadByte(25 + x); //PPU to EEprom
                    side.a.ppuAuthorized[1][x] =  TempPrice2[x];
                }
            }
            if(i == 2)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.a.ppuAuthorized[2][x] =  EEPROM_1_ReadByte(30 + x); //PPU to EEprom
                    side.a.ppuAuthorized[2][x] =  TempPrice3[x];
                }
            }
            if(i == 3)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.a.ppuAuthorized[3][x] =  EEPROM_1_ReadByte(35 + x); //PPU to EEprom
                    side.a.ppuAuthorized[3][x] =  TempPrice4[x];
                }
            }
                    
        }
        
        for(i = 0; i <  side.b.hoseNumber; i++)
        {   
            if(i == 0)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.b.ppuAuthorized[0][x] =  EEPROM_1_ReadByte(40 + x); //PPU to EEprom
                    side.b.ppuAuthorized[0][x] =  TempPrice[x];
                }
            }
            
            if(i == 1)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.b.ppuAuthorized[1][x] =  EEPROM_1_ReadByte(45 + x); //PPU to EEprom
                    side.b.ppuAuthorized[1][x] =  TempPrice2[x];
                }
            }
            if(i == 2)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.b.ppuAuthorized[2][x] =  EEPROM_1_ReadByte(50 + x); //PPU to EEprom
                    side.b.ppuAuthorized[2][x] =  TempPrice3[x];
                }
            }
            if(i == 3)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.b.ppuAuthorized[3][x] =  EEPROM_1_ReadByte(55 + x); //PPU to EEprom
                    side.b.ppuAuthorized[3][x] =  TempPrice4[x];
                }
            }                    
        }
        for(i = 0; i <  side.c.hoseNumber; i++)
        {   
            if(i == 0)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.c.ppuAuthorized[0][x] =  EEPROM_1_ReadByte(40 + x); //PPU to EEprom
                    side.c.ppuAuthorized[0][x] =  TempPrice[x];
                }
            }
            
            if(i == 1)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.c.ppuAuthorized[1][x] =  EEPROM_1_ReadByte(45 + x); //PPU to EEprom
                    side.c.ppuAuthorized[1][x] =  TempPrice2[x];
                }
            }
            if(i == 2)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.c.ppuAuthorized[2][x] =  EEPROM_1_ReadByte(50 + x); //PPU to EEprom
                    side.c.ppuAuthorized[2][x] =  TempPrice3[x];
                }
            }
            if(i == 3)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.c.ppuAuthorized[3][x] =  EEPROM_1_ReadByte(55 + x); //PPU to EEprom
                    side.c.ppuAuthorized[3][x] =  TempPrice4[x];
                }
            }
                    
        }
        for(i = 0; i <  side.d.hoseNumber; i++)
        {   
            if(i == 0)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.d.ppuAuthorized[0][x] =  EEPROM_1_ReadByte(40 + x); //PPU to EEprom
                    side.d.ppuAuthorized[0][x] =  TempPrice[x];
                }
            }
            
            if(i == 1)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.d.ppuAuthorized[1][x] =  EEPROM_1_ReadByte(45 + x); //PPU to EEprom
                    side.d.ppuAuthorized[1][x] =  TempPrice2[x];
                }
            }
            if(i == 2)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.d.ppuAuthorized[2][x] =  EEPROM_1_ReadByte(50 + x); //PPU to EEprom
                    side.d.ppuAuthorized[2][x] =  TempPrice3[x];
                }
            }
            if(i == 3)
            {
                for(x = 0; x < 5 ; x++ )
                {
                    //side.d.ppuAuthorized[3][x] =  EEPROM_1_ReadByte(55 + x); //PPU to EEprom
                    side.d.ppuAuthorized[3][x] =  TempPrice4[x];
                }
            }
                    
        }
}

/*
 * Realiza el preset tanto en credito como en efectivo
 */
void PresetAuthorize(uint8 Position)
{

    if(Position == side.a.dir)
    {
    if(PresetFlag == 1)
    {
        //Detecta la manija subida 
        side.a.activeHose = PumpHoseActiveState(side.a.dir);
                
        //Habilita consulta de credito con el servidor
        if(iButtonFlag == 1 && side.a.activeHose == side.a.hose) 
        {
            CreditAuth = RF_CREDITSALEAUTH;
            side.a.RFstateReport = 1;
            iButtonFlag = 0;
            side.a.rfState = RF_CREDITSALEAUTH;
            
        }
        
        //iButton Autorizado
        if(Credit_Auth_OK == 1 && AuthType == 1)
        {
            if (side.a.activeHose == side.a.hose)
            {   
                //Cambia precio segun el nivel
                priceChange(side.a.dir, side.a.grade, ppuiButtonA);
                
                // PRESET
                if(PresetData(side.a.dir, side.a.activeHose, bufferDisplay1.CreditpresetValue[0], bufferDisplay1.CreditpresetType[0] & 0x03) == 1)
                {                                     
                    get_state(side.a.dir);
              
                    //Authoriza
                    Authorization(side.a.dir);                                                         
                    side.a.RFstateReport = 1;                                     
                    bufferDisplay1.flagActiveSale = true;
                    if(NumPositions == 4){
                        SetPicture(1, DISPLAY_DESPACHANDO);
                        vTaskDelay( 200 / portTICK_PERIOD_MS );
                        SetPicture(1, DISPLAY_SELECCIONE_POSICION);
                    }else{
                        SetPicture(1, DISPLAY_DESPACHANDO);
                        ShowMessage(1,(bufferDisplay1.CreditpresetValue[0]),18);
                    } 
                    flowDisplay1 = 8;
                    flowPos = flowDisplay1;
                    PresetFlag = 0;
                    AuthType = 0;
                    Credit_Auth_OK = 0;                     
                    return;
                }else
                {
                    flowDisplay1 = 0;
                    SetPicture(1, DISPLAY_ERROR);
                    vTaskDelay( 200 / portTICK_PERIOD_MS );
                    SetPicture(1, DISPLAY_INICIO0);
                    PresetFlag = 0;
                    AuthType = 0;
                    Credit_Auth_OK = 0;
                    
                }
            }
        }
    
       //Venta en efectivo
        if(AuthType == 2)
        {       
            //Grade selected  =  Grade pump handle
            if (side.a.activeHose == side.a.hose)
            {                                 
                //Actualiza precios por el almacenado en la memoria
                
                ReadPPUFromEEprom();
                priceChange(side.a.dir, side.a.activeHose - 1, side.a.ppuAuthorized[side.a.activeHose - 1]);
                
                // PRESET
                if(PresetData(side.a.dir, side.a.hose, bufferDisplay1.presetValue[0], bufferDisplay1.presetType[0] & 0x03) == 1)
                {                                           

                    get_state(side.a.dir);

                    //Authorize
                    Authorization(side.a.dir);                                                         
                    side.a.RFstateReport = 1;
                    count_protector = 0;                    
                    bufferDisplay1.flagActiveSale = true;
                    if(NumPositions == 4){
                        SetPicture(1, DISPLAY_DESPACHANDO);
                        vTaskDelay( 200 / portTICK_PERIOD_MS );
                        SetPicture(1, DISPLAY_SELECCIONE_POSICION);
                    }else{
                        SetPicture(1, DISPLAY_DESPACHANDO);
                        ShowMessage(1,(bufferDisplay1.presetValue[1]),18);
                    }                    
                    flowDisplay1 = 8;
                    flowPos = 8;                    
                    PresetFlag = 0;                    
                    return;                      
                    
                }else
                {
                    flowDisplay1 = 0;
                    SetPicture(1, DISPLAY_ERROR);
                    vTaskDelay( 200 / portTICK_PERIOD_MS );
                    SetPicture(1, DISPLAY_INICIO0);
                    PresetFlag = 0;  
                    flowPos = 0;
                    return;                  
                    
                }
            }
        }
    }
    }
    
    if(Position == side.b.dir)
    {
    if(PresetFlag2 == 1)
    {
        side.b.activeHose = PumpHoseActiveState(side.b.dir);  
        
        if(iButtonFlag2 == 1 && side.b.activeHose == side.b.hose)
        {
            CreditAuth2 = RF_CREDITSALEAUTH;
            side.b.RFstateReport = 1;
            iButtonFlag2 = 0;
            side.b.rfState = RF_CREDITSALEAUTH;
        }
        
        //iButton Authorized Credit
        if(Credit_Auth_OK2 == 1 && AuthType2 == 1)
        {                          
            if (side.b.activeHose == side.b.hose)
            {   
                priceChange(side.b.dir, side.b.grade, ppuiButtonB);
                
                if(PresetData(side.b.dir, side.b.activeHose, bufferDisplay2.CreditpresetValue[0], bufferDisplay2.CreditpresetType[0] & 0x03) == 1)
                {                    
                    get_state(side.b.dir);

                    // Authorize
                    Authorization(side.b.dir);
                    side.b.RFstateReport = 1;                                        
    				bufferDisplay2.flagActiveSale = true;
                    if(NumPositions == 4){
                        SetPicture(2, DISPLAY_DESPACHANDO);
                        vTaskDelay( 200 / portTICK_PERIOD_MS );
                        SetPicture(2, DISPLAY_SELECCIONE_POSICION);
                    }else{
                        SetPicture(2, DISPLAY_DESPACHANDO);
                        ShowMessage(2,(bufferDisplay2.CreditpresetValue[0]),18);
                    } 
                    flowDisplay2 = 8;
                    flowPosB = flowDisplay2;
                    PresetFlag2 = 0;
                    AuthType2 = 0;
                    return;
                    
                }else
                {
                    flowDisplay2 = 0;
                    SetPicture(2, DISPLAY_ERROR);
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                    SetPicture(2, DISPLAY_INICIO0);
                    PresetFlag2 = 0;
                    AuthType2 = 0;
                    Credit_Auth_OK2 = 0;
                    return;
                    
                }
            }
        }
        
         // Cash Sale
        if(AuthType2 == 2)
        {   
            //Grade selected  =  Grade pump handle                   
            if (side.b.activeHose == side.b.hose)
            {
                ReadPPUFromEEprom();             
                priceChange(side.b.dir, side.b.activeHose - 1, side.b.ppuAuthorized[side.b.activeHose - 1]);
                
                if(PresetData(side.b.dir, side.b.hose, bufferDisplay2.presetValue[0], bufferDisplay2.presetType[0] & 0x03) == 1)
                {                    
                    get_state(side.b.dir);
                    //Authorize
                    Authorization(side.b.dir);
                    side.b.RFstateReport = 1;                       
    				bufferDisplay2.flagActiveSale = true;					
                    if(NumPositions == 4){
                        SetPicture(2, DISPLAY_DESPACHANDO);
                        vTaskDelay( 200 / portTICK_PERIOD_MS );
                        SetPicture(2, DISPLAY_SELECCIONE_POSICION);
                    }else{
                        SetPicture(2, DISPLAY_DESPACHANDO);
                        ShowMessage(2,(bufferDisplay2.presetValue[1]),18);
                    }
                    flowDisplay2 = 8;
                    flowPosB = flowDisplay2;
                    PresetFlag2 = 0;  
                    return;                
                    
                }else
                {
                    flowDisplay2 = 0;
                    SetPicture(2, DISPLAY_ERROR);
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                    SetPicture(2, DISPLAY_INICIO0);
                    PresetFlag2 = 0;
                    return;                 
                    
                }
            }
        }
    }
    }
    
    if(Position == side.c.dir)
    {
        if(PresetFlag3 == 1)
        {
            //Detecta la manija subida 
            side.c.activeHose = PumpHoseActiveState(side.c.dir);
                    
            //Habilita consulta de credito con el servidor
            if(iButtonFlag3 == 1 && side.c.activeHose == side.c.hose) 
            {
                CreditAuth3 = RF_CREDITSALEAUTH;
                side.c.RFstateReport = 1;
                iButtonFlag3 = 0;
                side.c.rfState = RF_CREDITSALEAUTH;
                
            }
            
            //iButton Autorizado
            if(Credit_Auth_OK3 == 1 && AuthType3 == 1)
            {
                if (side.c.activeHose == side.c.hose)
                {   
                    //Cambia precio segun el nivel
                    priceChange(side.c.dir, side.c.grade, ppuiButtonA);
                    
                    // PRESET
                    if(PresetData(side.c.dir, side.c.activeHose, bufferDisplay3.CreditpresetValue[0], bufferDisplay3.CreditpresetType[0] & 0x03) == 1)
                    {                                     
                        get_state(side.c.dir);              
                        //Authoriza
                        Authorization(side.c.dir);                                                         
                        side.c.RFstateReport = 1;                    
                        bufferDisplay3.flagActiveSale = true;
                        if(NumPositions == 4){
                            SetPicture(1, DISPLAY_DESPACHANDO);
                            vTaskDelay( 200 / portTICK_PERIOD_MS );
                            SetPicture(1, DISPLAY_SELECCIONE_POSICION);
                        }else{
                            SetPicture(1, DISPLAY_DESPACHANDO);
                            ShowMessage(1,(bufferDisplay3.CreditpresetValue[1]),18);
                        }
                        flowDisplay3 = 8;
                        flowPosC = flowDisplay3;
                        PresetFlag3 = 0;
                        AuthType3 = 0;
                        Credit_Auth_OK3 = 0;                                        
                    }else
                    {
                        flowDisplay3 = 0;
                        SetPicture(1, DISPLAY_ERROR);
                        vTaskDelay( 200 / portTICK_PERIOD_MS );
                        //SetPicture(1, DISPLAY_INICIO0);
                        PresetFlag3 = 0;
                        AuthType3 = 0;
                        Credit_Auth_OK3 = 0;                    
                    }
                }
            }
        
           //Venta en efectivo
            if(AuthType3 == 2)
            {       
                //Grade selected  =  Grade pump handle
                if (side.c.activeHose == side.c.hose)
                {                    
                        //Actualiza precios por el almacenado en la memoria
                    ReadPPUFromEEprom();
                    priceChange(side.c.dir, side.c.activeHose - 1, side.c.ppuAuthorized[side.c.activeHose - 1]);
                    
                    // PRESET
                    if(PresetData(side.c.dir, side.c.hose, bufferDisplay3.presetValue[0], bufferDisplay3.presetType[0] & 0x03) == 1)
                    {                                           
                        get_state(side.c.dir);        
                        //Authorize
                        Authorization(side.c.dir);                                                         
                        side.c.RFstateReport = 1;                            
                        bufferDisplay3.flagActiveSale = true;
                        if(NumPositions == 4){
                            SetPicture(1, DISPLAY_DESPACHANDO);
                            vTaskDelay( 200 / portTICK_PERIOD_MS );
                            SetPicture(1, DISPLAY_SELECCIONE_POSICION);
                        }else{
                            SetPicture(1, DISPLAY_DESPACHANDO);
                            ShowMessage(1,(bufferDisplay3.presetValue[1]),18);
                        }
                        flowDisplay3 = 8;
                        flowPosC = flowDisplay3;                             
                        PresetFlag3 = 0;
                        
                        return;                                                  
                    }else
                    {                                                        
                        SetPicture(1, DISPLAY_ERROR);
                        vTaskDelay( 200 / portTICK_PERIOD_MS );
                        //SetPicture(1, DISPLAY_INICIO0);
                        PresetFlag3 = 0;  
                        flowPosC = 0; 
                        flowDisplay3 = 0;
                        return;                  
                        
                    }
                }
            }
        }
    }
    
    if(Position == side.d.dir)
    {
        if(PresetFlag4 == 1)
        {
            side.d.activeHose = PumpHoseActiveState(side.d.dir);  
            
            if(iButtonFlag4 == 1 && side.d.activeHose == side.d.hose)
            {
                CreditAuth4 = RF_CREDITSALEAUTH;
                side.d.RFstateReport = 1;
                iButtonFlag4 = 0;
                side.d.rfState = RF_CREDITSALEAUTH;
            }        
            //iButton Authorized Credit
            if(Credit_Auth_OK4 == 1 && AuthType4 == 1)
            {                          
                if (side.d.activeHose == side.d.hose)
                {   
                    priceChange(side.d.dir, side.d.grade, ppuiButtonB);                
                    if(PresetData(side.d.dir, side.d.activeHose, bufferDisplay4.CreditpresetValue[0], bufferDisplay4.CreditpresetType[0] & 0x03) == 1)
                    {                    
                        get_state(side.d.dir);
                        // Authorize
                        Authorization(side.d.dir);
                        side.d.RFstateReport = 1;                                        
        				bufferDisplay4.flagActiveSale = true;					
                        if(NumPositions == 4){
                            SetPicture(2, DISPLAY_DESPACHANDO);
                            vTaskDelay( 200 / portTICK_PERIOD_MS );
                            SetPicture(2, DISPLAY_SELECCIONE_POSICION);
                        }else{
                            SetPicture(2, DISPLAY_DESPACHANDO);
                            ShowMessage(2,(bufferDisplay4.CreditpresetValue[0]),18);
                        }
                        PresetFlag4 = 0;
                        flowDisplay4 = 8;
                        flowPosD = flowDisplay4;
                        AuthType4 = 0;
                        return;
                        
                    }else
                    {
                        flowDisplay4 = 0;
                        SetPicture(2, DISPLAY_ERROR);
                        vTaskDelay(200 / portTICK_PERIOD_MS);
                        SetPicture(2, DISPLAY_INICIO0);
                        PresetFlag4 = 0;
                        AuthType4 = 0;
                        Credit_Auth_OK4 = 0;
                        return;
                        
                    }
                }
            }        
             // Cash Sale
            if(AuthType4 == 2)
            {   
                //Grade selected  =  Grade pump handle                   
                if (side.d.activeHose == side.d.hose)
                {
                    if(!RFOnline)
                        side.d.grade = side.d.activeHose - 1;
                    ReadPPUFromEEprom();             
                    priceChange(side.d.dir, side.d.activeHose - 1, side.d.ppuAuthorized[side.d.activeHose - 1]);
                    
                    if(PresetData(side.d.dir, side.d.hose, bufferDisplay4.presetValue[0], bufferDisplay4.presetType[0] & 0x03) == 1)
                    {                    
                        get_state(side.d.dir);
                        //Authorize
                        Authorization(side.d.dir);
                        side.d.RFstateReport = 1;                                        
        				bufferDisplay4.flagActiveSale = true;					
                        if(NumPositions == 4){
                            SetPicture(2, DISPLAY_DESPACHANDO);
                            vTaskDelay( 200 / portTICK_PERIOD_MS );
                            SetPicture(2, DISPLAY_SELECCIONE_POSICION);
                        }else{
                            SetPicture(2, DISPLAY_DESPACHANDO);
                            ShowMessage(2,(bufferDisplay4.presetValue[1]),18);
                        } 
                        flowDisplay4 = 8;
                        flowPosD = flowDisplay4;                        
                        PresetFlag4 = 0;
                       
                        return;                
                        
                    }else
                    {
                        flowDisplay4 = 0;
                        SetPicture(2, DISPLAY_ERROR);
                        vTaskDelay(200 / portTICK_PERIOD_MS);
                        SetPicture(2, DISPLAY_INICIO0);
                        PresetFlag4 = 0;
                        return;                                     
                    }
                }
            }
        }
    }
}  


/* Display Task */
void Display_Task(void *arg)
{
   TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;
    
    xLastWakeTime = xTaskGetTickCount();
    selectPos   = 1;
    selectPosB  = 1;
    OSonline    = 1;
    InitDisplay1();
    while(1) 
    {
        if(NumPositions <=2){
            flowDisplay3 = 0;
            flowDisplay4 = 0;
            PollingDisplay1();
            PollingDisplay2();   
        }
        if(NumPositions > 2)
        {               
            switch(selectPos){
                case 1:
                    PollingDisplay1();
                break;
                    
                case 2:
                    PollingDisplay3();
                break;                            
            }
            switch(selectPosB){
                case 1:
                    PollingDisplay2();
                break;
                    
                case 2:
                    PollingDisplay4();
                break;                            
            }
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void PumpAction(uint8 PositionPump, uint8 State)
{   
    uint8 x;
      
    // Current Loop recovery
    if(ActualState[PositionPump] == PUMP_FAIL && State != PUMP_FAIL)
    {
        flowDisplay1 = 0;
        flowDisplay2 = 0;
        flowDisplay3 = 0;
        flowDisplay4 = 0;
        //InitPump();
    }
    
    // Zero Sale detect
    if((ActualState[PositionPump] == PUMP_BUSY && State == PUMP_IDLE) || (ActualState[PositionPump] == PUMP_AUTHORIZED && State == PUMP_IDLE) || (ActualState[PositionPump] == PUMP_BUSY && State == PUMP_CALLING) )
    {

            if(PositionPump == side.a.dir)
            {
                bufferDisplay1.flagPrint = 0;
                bufferDisplay1.zeroSend  = 1;
                flowDisplay1 = 0;                
                side.a.rfState = RF_ZERO_SALE;
                bufferDisplay1.flagActiveSale = false;
                //SetPicture(1, DISPLAY_INICIO0);
                ActualState[PositionPump] = State;
                flowPos = 0;
                return;
            }
            if(PositionPump == side.b.dir)
            {
                bufferDisplay2.flagPrint = 0;
                bufferDisplay2.zeroSend  = 1;
                flowDisplay2 = 0;
                side.b.rfState = RF_ZERO_SALE;
                bufferDisplay2.flagActiveSale = false;
                //SetPicture(2, DISPLAY_INICIO0);
                ActualState[PositionPump] = State;
                flowPosB = 0;
                return;
            }
            if(PositionPump == side.c.dir)
            {
                bufferDisplay3.flagPrint = 0;
                bufferDisplay3.zeroSend  = 1;
                flowDisplay3 = 0;
                side.c.rfState = RF_ZERO_SALE;
                bufferDisplay3.flagActiveSale = false;
                ActualState[PositionPump] = State;
                flowPosC = 0;
                return;
            }
            if(PositionPump == side.d.dir)
            {
                bufferDisplay4.flagPrint = 0;
                bufferDisplay4.zeroSend  = 1;
                flowDisplay4 = 0;
                side.d.rfState = RF_ZERO_SALE;
                bufferDisplay4.flagActiveSale = false;                
                ActualState[PositionPump] = State;
                flowPosD = 0;
                return;
            }
                         
    }

    //Actualize state
    ActualState[PositionPump] = State;
       
    switch(State)
    {
        case PUMP_FAIL:          
            
            SetPicture(1, DISPLAY_ERROR);
            SetPicture(2, DISPLAY_ERROR);
            side.a.rfState = RF_ERROR;
            side.b.rfState = RF_ERROR;
            side.c.rfState = RF_ERROR;
            side.d.rfState = RF_ERROR;
            InitDisplay1();
            InitDisplay2();
                         
        break;
        case PUMP_IDLE:

            if(PositionPump == side.a.dir)
            {   
                bufferDisplay1.flagActiveSale = false;
                flowPos = 0;
                side.a.rfState = RF_IDLE;                 
            }
                    
            if(PositionPump == side.b.dir)
            {              			                
                bufferDisplay2.flagActiveSale = false;
                flowPosB = 0;
                side.b.rfState = RF_IDLE;                 
            }
            if(PositionPump == side.c.dir)
            {                               			                
                bufferDisplay3.flagActiveSale = false;
                flowPosC = 0;
                side.c.rfState = RF_IDLE;                 
            }
            if(PositionPump == side.d.dir)
            {                               			                
                bufferDisplay4.flagActiveSale = false;
                flowPosD = 0;
                side.d.rfState = RF_IDLE;                 
            }
                         
        break;
            
        case PUMP_CALLING:                            
            PresetAuthorize(PositionPump);                                                 
        break;
            
        case PUMP_AUTHORIZED:

        break;
            
        case PUMP_BUSY:          
                                 
            if(PositionPump == side.a.dir)
            {
                bufferDisplay1.flagActiveSale = true;
                bufferDisplay1.flagEndSale = false;
                if(side.a.BusyChange == 1)
                    side.a.rfState = RF_DELIVERING;              
            }
            if(PositionPump == side.b.dir)
            {
                bufferDisplay2.flagActiveSale = true;
                bufferDisplay2.flagEndSale = false;
                if(side.b.BusyChange == 1)
                    side.b.rfState = RF_DELIVERING;               
            }
            if(PositionPump == side.c.dir)
            {
                bufferDisplay3.flagActiveSale = true;
                bufferDisplay3.flagEndSale = false;
                if(side.c.BusyChange == 1)
                    side.c.rfState = RF_DELIVERING;               
            }
            if(PositionPump == side.d.dir)
            {
                bufferDisplay4.flagActiveSale = true;
                bufferDisplay4.flagEndSale = false;
                if(side.d.BusyChange == 1)
                    side.d.rfState = RF_DELIVERING;                
            }      
        break;
        case PUMP_PEOT:            
            if(getSale(PositionPump))
            {
                if(PositionPump == side.a.dir)
                {                                     
                    bufferDisplay1.flagEndSale = true;
                    side.a.RFstateReport = 1; 
                    if(!RFOnline){
                        side.a.grade = side.a.activeHose - 1;
                        if(bufferDisplay1.flagPrint == 1)
                        {
                            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
                            {   										    							
                		        side.a.saleNumber[x] = 0x00;
                	        }
                            bufferDisplay1.PrintFlagEOT     = 1;                            
                        }else
                        {
                            bufferDisplay1.PrintFlagEOT     = 0;
                            bufferDisplay1.flagActiveSale = false;
                            flowDisplay1 = 0;
                            //SetPicture(1, DISPLAY_INICIO0);
                        }
                    }
                    priceChange(side.a.dir, side.a.grade, side.a.ppuAuthorized[side.a.grade]);
                    iButtonFlag = 0;
                    flowPos = 0;
                    PresetFlag = 0;
               }               
               if(PositionPump == side.b.dir)
               {                                                           
                    bufferDisplay2.flagEndSale = true;
                    side.b.RFstateReport = 1;
                    if(!RFOnline){
                        side.b.grade = side.b.activeHose - 1;
                        if(bufferDisplay2.flagPrint == 1)
                        {
                            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
                            {   										    							
                		        side.b.saleNumber[x] = 0x00;
                	        }
                            bufferDisplay2.PrintFlagEOT     = 1;                            
                        }else
                        {
                            bufferDisplay2.PrintFlagEOT     = 0;
                            bufferDisplay2.flagActiveSale = false;
                            flowDisplay2 = 0;
                            //SetPicture(2,DISPLAY_INICIO0);
                        }
                    }
                    priceChange(side.b.dir, side.b.grade, side.b.ppuAuthorized[side.b.grade]);
                    iButtonFlag2 = 0;
                    flowPosB = 0;
                    PresetFlag2 =0;
                } 
            
                if(PositionPump == side.c.dir)
                {                                       
                    //SetPicture(1,DISPLAY_INICIO0);                    
                    bufferDisplay3.flagEndSale = true;
                    side.c.RFstateReport = 1;
                    if(!RFOnline){
                        side.c.grade = side.c.activeHose - 1;
                        if(bufferDisplay3.flagPrint == 1)
                        {
                            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
                            {   										    							
                		        side.c.saleNumber[x] = 0x00;
                	        }
                            bufferDisplay3.PrintFlagEOT     = 1;                            
                        }else
                        {
                            bufferDisplay3.PrintFlagEOT     = 0;
                            bufferDisplay3.flagActiveSale = false;
                            flowDisplay3 = 0;
                        }
                    }
                    priceChange(side.c.dir, side.c.grade, side.c.ppuAuthorized[side.c.grade]);
                    iButtonFlag3 = 0;
                    flowPosC = 0;
                    PresetFlag3 = 0;
                } 
                
                if(PositionPump == side.d.dir)
                {                                       
                    //SetPicture(2,DISPLAY_INICIO0);                    
                    bufferDisplay4.flagEndSale = true;
                    if(!RFOnline){
                        side.d.grade = side.d.activeHose - 1;
                        if(bufferDisplay4.flagPrint == 1)
                        {
                            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
                            {   										    							
                		        side.d.saleNumber[x] = 0x00;
                	        }
                            bufferDisplay4.PrintFlagEOT     = 1;                            
                        }else
                        {
                            bufferDisplay4.PrintFlagEOT     = 0;
                            bufferDisplay4.flagActiveSale = false;
                            flowDisplay4 = 0;
                        }
                    }
                    priceChange(side.d.dir, side.d.grade, side.d.ppuAuthorized[side.d.grade]);
                    iButtonFlag4 = 0;
                    flowPosD = 0;
                    PresetFlag4 = 0;                    
                    side.d.RFstateReport = 1;
                }                 
            }                       
        break;
            
        case PUMP_FEOT:            
            if(getSale(PositionPump))
            {
                if(PositionPump == side.a.dir)
                {                                                         
                    bufferDisplay1.flagEndSale = true;
                    side.a.RFstateReport = 1;   
                    if(!RFOnline){
                        side.a.grade = side.a.activeHose - 1;
                        if(bufferDisplay1.flagPrint == 1)
                        {
                            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
                            {   										    							
                		        side.a.saleNumber[x] = 0x00;
                	        }
                            bufferDisplay1.PrintFlagEOT     = 1;                            
                        }else
                        {
                            bufferDisplay1.PrintFlagEOT     = 0;
                            bufferDisplay1.flagActiveSale = false;
                            flowDisplay1 = 0;
                            //SetPicture(1, DISPLAY_INICIO0);
                        }
                    }
                    priceChange(side.a.dir, side.a.grade, side.a.ppuAuthorized[side.a.grade]);
                    iButtonFlag = 0;
                    flowPos = 0;
                    PresetFlag = 0;
               }
               
               if(PositionPump == side.b.dir)
               {                                                           
                    bufferDisplay2.flagEndSale = true;
                    side.b.RFstateReport = 1;
                    if(!RFOnline){
                        side.b.grade = side.b.activeHose - 1;
                        if(bufferDisplay2.flagPrint == 1)
                        {
                            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
                            {   										    							
                		        side.b.saleNumber[x] = 0x00;
                	        }
                            bufferDisplay2.PrintFlagEOT     = 1;                            
                        }else
                        {
                            bufferDisplay2.PrintFlagEOT     = 0;
                            bufferDisplay2.flagActiveSale = false;
                            flowDisplay2 = 0;
                            //SetPicture(2,DISPLAY_INICIO0);
                        }
                    }
                    priceChange(side.b.dir, side.b.grade, side.b.ppuAuthorized[side.b.grade]);
                    iButtonFlag2 = 0;
                    flowPosB = 0;
                    PresetFlag2 = 0;
                } 
            
                if(PositionPump == side.c.dir)
                {                                                           
                    bufferDisplay3.flagEndSale = true;
                    side.c.RFstateReport = 1;
                    if(!RFOnline){
                        side.c.grade = side.c.activeHose - 1;
                        if(bufferDisplay3.flagPrint == 1)
                        {
                            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
                            {   										    							
                		        side.c.saleNumber[x] = 0x00;
                	        }
                            bufferDisplay3.PrintFlagEOT     = 1;                            
                        }else
                        {
                            bufferDisplay3.PrintFlagEOT     = 0;
                            bufferDisplay3.flagActiveSale = false;
                            flowDisplay3 = 0;
                            //SetPicture(1,DISPLAY_INICIO0);
                        }
                    }
                    priceChange(side.c.dir, side.c.grade, side.c.ppuAuthorized[side.c.grade]);
                    iButtonFlag3 = 0;
                    flowPosC = 0;
                    PresetFlag3 = 0;
                } 
                
                if(PositionPump == side.d.dir)
                {                                                           
                    bufferDisplay4.flagEndSale = true;
                    side.d.RFstateReport = 1;
                    if(!RFOnline){
                        side.d.grade = side.d.activeHose - 1;
                        if(bufferDisplay4.flagPrint == 1)
                        {
                            for(x = 0; x < 10; x++)                                         //NUMERO DE VENTA
                            {   										    							
                		        side.d.saleNumber[x] = 0x00;
                	        }
                            bufferDisplay4.PrintFlagEOT     = 1;                            
                        }else
                        {
                            bufferDisplay4.PrintFlagEOT     = 0;
                            bufferDisplay4.flagActiveSale = false;
                            flowDisplay4 = 0;
                            //SetPicture(2,DISPLAY_INICIO0);
                        }
                    }
                    priceChange(side.d.dir, side.d.grade, side.d.ppuAuthorized[side.d.grade]);
                    iButtonFlag4 = 0;
                    flowPosD = 0;
                    PresetFlag4 = 0;
                }                 
            }         
        break;
            
        case PUMP_STOPPED:

        break;
            
        default:
        break;
    }   
    
}

bool LoopOpen(void)
{
    switch (NumPositions)
    {
        case 1:
            if(StatePosition[0] != 0)
            {
               return true;
            }
            else
            {
                return false;
            }
        break;
        case 2:
            if(StatePosition[0] != 0 && StatePosition[1] != 0)
            {
               
                return true;
            }
            else
            {
                return false;
            }      
        break;
        case 3: 
            if(StatePosition[0] != 0 && StatePosition[1] != 0 && StatePosition[2] != 0)
            {
               return true;
            }
            else
            {
                return false;
            }      
        break;
        case 4: 
            if(StatePosition[0] != 0 && StatePosition[1] != 0 && StatePosition[2] != 0 && StatePosition[3] != 0)
            {
               return true; 
            }
            else
            {
                return false;
            }      
        break;
        default:
            return false;
        break;
    }
}

void CheckInitState(void)
{
    uint8 i;
    
    for(i = 0; i < NumPositions; i++)
    {
        if(InitState[i] == PUMP_PEOT || InitState[i] == PUMP_FEOT)
        {
            flowDisplay1 = 0;
        }        
    }
}

void SetPPU(void)
{
    uint8 i;
  
    // PPU Change
        if(side.a.changePPU)
        {
            for(i = 0; i <  side.a.hoseNumber; i++)
            {   
                if(i == 0)
                {
                    if(priceChange(side.a.dir, i, side.a.ppuAuthorized[i]))
                    {
                        //side.a.changePPU = false;
                    }
                }
                if(i == 1)
                {
                    if(priceChange(side.a.dir, i, side.a.ppuAuthorized[i]))
                    {
                        //side.a.changePPU = false;
                    }
                }
                if(i == 2)
                {
                    if(priceChange(side.a.dir, i, side.a.ppuAuthorized[i]))
                    {
                        //side.a.changePPU = false;
                    }
                }
                if(i == 3)
                {
                    if(priceChange(side.a.dir, i, side.a.ppuAuthorized[i]))
                    {
                        //side.a.changePPU = false;
                    }
                }
            }  
        }
        
        if(side.b.changePPU)
        {
            for(i = 0; i <  side.b.hoseNumber; i++)
            {   
                if(i == 0)
                {
                    if(priceChange(side.b.dir, i, side.b.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 1)
                {
                    if(priceChange(side.b.dir, i, side.b.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 2)
                {
                    if(priceChange(side.b.dir, i, side.b.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 3)
                {
                    if(priceChange(side.b.dir, i, side.b.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
            }
        }
        
        if(side.c.changePPU)
        {
            for(i = 0; i <  side.c.hoseNumber; i++)
            {   
                if(i == 0)
                {
                    if(priceChange(side.c.dir, i, side.c.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 1)
                {
                    if(priceChange(side.c.dir, i, side.c.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 2)
                {
                    if(priceChange(side.c.dir, i, side.c.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 3)
                {
                    if(priceChange(side.c.dir, i, side.c.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
            }
        }
        
        if(side.d.changePPU)
        {
            for(i = 0; i <  side.d.hoseNumber; i++)
            {   
                if(i == 0)
                {
                    if(priceChange(side.d.dir, i, side.d.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 1)
                {
                    if(priceChange(side.d.dir, i, side.d.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 2)
                {
                    if(priceChange(side.d.dir, i, side.d.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
                if(i == 3)
                {
                    if(priceChange(side.d.dir, i, side.d.ppuAuthorized[i]))
                    {
                        //side.b.changePPU = false;
                    }
                }
            }
        }
   
    side.a.changePPU = false;
    side.b.changePPU = false;
    side.c.changePPU = false;
    side.d.changePPU = false;
}




/* Pump Task */
void Pump_Task(void *arg)
{
    //TickType_t xLastWakeTime;
    //const TickType_t xFrequency = 1;
    uint8 i;
    
    //xLastWakeTime = xTaskGetTickCount();
    
    // Vector State Init
    for(i = 0; i < 8; i++)
    {
        StatePosition[i] = 0x00;
    }
    
    CheckInitState();   
    ReadPPUFromEEprom();                       
    side.a.changePPU = true;
    side.b.changePPU = true;
    if(NumPositions == 4){
        side.c.changePPU = true;
        side.d.changePPU = true;
    }
    SetPPU();
       
    while(1) 
    {    
        
        // Polling 
        for(i = 0; i < NumPositions; i++)
        {               
            if(i == 0)
            {      
                StatePosition[0] = get_state(side.a.dir);
                side.a.pumpState = StatePosition[0];
            }
            if(i == 1)
            {
                StatePosition[1] = get_state(side.b.dir);
                side.b.pumpState = StatePosition[1];
            }
            if(i == 2)
            {
                StatePosition[2] = get_state(side.c.dir);
                side.c.pumpState = StatePosition[2];
            }
            if(i == 3)
            {
                StatePosition[3] = get_state(side.d.dir);
                side.d.pumpState = StatePosition[3];
            }       
        }
        
        // Detecting loop open
        if(LoopOpen())
        {
            for(i = 0; i < NumPositions; i++)
            {               
                if(i == 0)
                {                  
                    PumpAction(side.a.dir, StatePosition[i]);
                }
                if(i == 1)
                {              
                    PumpAction(side.b.dir, StatePosition[i]);
                }
                if(i == 2)
                {                
                    PumpAction(side.c.dir, StatePosition[i]);
                }
                if(i == 3)
                {                  
                    PumpAction(side.d.dir, StatePosition[i]);
                }       
            }      
        }else
        {
            PumpAction(side.a.dir, 0);           
        }
       
     
        // Totals
        if(pollTotals == 1)
        {   
            if(getTotals(side.a.dir) != 0)
            {
                side.a.RFstateReport = 1;
                side.b.RFstateReport = 0;
                FlagTotal = 1;
             }     
        }
        
        if(pollTotals == 2)
        {
            if(getTotals(side.b.dir) != 0)
            {
                side.b.RFstateReport = 1;
                side.a.RFstateReport = 0;
                FlagTotalB = 1;
            }
        }
        
        if(pollTotals == 3)
        {
            if(getTotals(side.c.dir) != 0)
            {
                side.c.RFstateReport = 1;
                side.d.RFstateReport = 0;
                FlagTotalC = 1;
            }
        }
        
        if(pollTotals == 4)
        {
            if(getTotals(side.d.dir) != 0)
            {
                side.c.RFstateReport = 0;
                side.d.RFstateReport = 1;
                FlagTotalD = 1;
            }
        }
        
        if(side.a.changePPU || side.b.changePPU || side.c.changePPU || side.d.changePPU)
        {
            SetPPU();
        }
        //vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


/* [] END OF FILE */
