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
    uint8 producto1[13] = "CORRIENTE    ";
    uint8 producto2[13] = "ACPM         ";
    uint8 producto3[13] = "EXTRA        ";
    uint8 producto4[13] = "             ";
    for(x=0; x<13;x++)
    {
        producto[0][x] = producto1[x];
    }
    for(x=0; x<13;x++)
    {
        producto[1][x] = producto2[x];
    }
    for(x=0; x<13;x++)
    {
        producto[2][x] = producto3[x];
    }
    for(x=0; x<13;x++)
    {
        producto[3][x] = producto4[x];
    }
    
    if(NumPositions == 2)
    {
        SetPicture(1, DISPLAY_INICIO0);         
        ShowMessage(1,iniText,0);    
        //CyDelay(300);
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
    }
    if(NumPositions == 4)
    {
        SetPicture(1,DISPLAY_SELECCIONE_POSICION);        
        ShowMessage(1, iniText, 0);    
        //CyDelay(300);
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
            side.a.hoseNumber = UnitType + 1;
            side.b.hoseNumber = UnitType + 1;
            side.c.hoseNumber = UnitType + 1;
            side.d.hoseNumber = UnitType + 1;
            ShowMessage(1,Unit2,0);
        }
        if(UnitType == 2)
        {
            side.a.hoseNumber = UnitType + 1;
            side.b.hoseNumber = UnitType + 1;
            side.c.hoseNumber = UnitType + 1;
            side.d.hoseNumber = UnitType + 1;
           ShowMessage(1,Unit3,0);
        }
        if(UnitType == 3)
        {
            side.a.hoseNumber = UnitType + 1;
            side.b.hoseNumber = UnitType + 1;
            side.c.hoseNumber = UnitType + 1;
            side.d.hoseNumber = UnitType + 1;
            ShowMessage(1,Unit4,0);
        }    
        if(DDMode == 1)
        {
            ShowMessage(1,DisplayMode,22);                  //Mostrar modo de display
        }
        if(DDMode == 2)
        {
           ShowMessage(1,DisplayMode2,22);
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
    }
    if(NumPositions == 4){
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
*                                         void polling_Pump(void)
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
            flowDisplay1 = 1;
            count_protector = 0;            
            bufferDisplay1.flagEndSale = false;  
            side.a.RFstateReport = 0;
            
            if(bufferDisplay1.flagPrint == 1)
            {    
                vTaskDelay( 200 / portTICK_PERIOD_MS );
                imprimir(printPortA, side.a.dir);
                bufferDisplay1.flagPrint = 0;
                bufferDisplay1.saleType = 0;
            }
        break;
        case 1: //Menu
            
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {                                             
                    flowDisplay1 = 2;                               //Pantalla forma de pago                      
                    SetPicture(1, DISPLAY_FORMA_PAGO_DESEADA);                                                                                                  
                }  
       
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
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
                            flowDisplay1 = 3; 
                            bufferDisplay1.saleType = 1;
                            SetPicture(1, DISPLAY_FORMA_PROGRAMACION);  
                            AuthType = 0;
                        break;
                        case 0x0E:  //Pantalla credito  
                            flowDisplay1 = 3;
                            bufferDisplay1.saleType = 2;
                            count_protector = 0;
                            SetPicture(1, DISPLAY_FORMA_PROGRAMACION);
                            //SetPicture(1, DISPLAY_ID_DIGITAL);
                            AuthType = 1;
                        break;
                        case 0x45:  //Pantalla otras opciones 
                            flowDisplay1 = 12;                            
                            SetPicture(1,  DISPLAY_OPERACIONES);
                            AuthType = 0;
                        break;
                            
                        case 0x94:  //Pantalla Inicial 
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1, DISPLAY_INICIO0);   
                            AuthType = 0;
                        break;
                        
                        case 0x7E:  //Pantalla Inicial 
                             bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1, DISPLAY_INICIO0);
                            AuthType = 0;
                        break;
                    }
                }  
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
            
        case 3: //Pantalla ingreso de valores
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x0F:  //Preset dinero                
                            flowDisplay1 = 4;                              
                            bufferDisplay1.presetType[0] = 2;
                            bufferDisplay1.presetType[1] = 'D';
                            numberKeys1 = 0;
                            SetPicture(1,DISPLAY_INTRODUZCA_VALOR);   
                            WriteLCD(1,'$',3,2,1,0x0000,'N');
                            
                            if(MoneyDec > 0)
                            {
                                flagPoint1 = 0;
                            }else
                            {
                                flagPoint1 = 1;
                            }                           
                        break; 
                        case 0x10:  //Preset volumen                 
                            flowDisplay1 = 4;                            
                            bufferDisplay1.presetType[0] = 1;
                            bufferDisplay1.presetType[1] = 'V';
                            numberKeys1 = 0;                            
                            SetPicture(1, DISPLAY_INTRODUZCA_VOLUMEN);                            
                            WriteLCD(1,'G',3,2,1,0x0000,'N');
                            
                            if(VolDec > 0)
                            {
                                flagPoint1 = 0;
                            }else
                            {
                                flagPoint1 = 1;
                            }                                                       
                        break;
                        case 0x43:     //Preset full 
                            bufferDisplay1.presetType[0] = 2;
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
                          
                        break;
                        case 0x94:  //Retroceso
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                        case 0x3B:  //Pantalla Inicial  
                            AuthType = 0;                        
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            AuthType = 0;
                        break;
                        case 0x7E:  //Pantalla Inicial  
                            AuthType = 0;
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0); 
                            AuthType = 0;
                        break;
                    }
                }  
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
        
        case 4:            
            switch (alphanumeric_keyboard(digits + 1, 0))
            {
                case 0:  //Pantalla Inicial    
                    flowDisplay1 = 0;
                    SetPicture(1,DISPLAY_INICIO0);                            
                break;
                    
                case 1: //Enter
                    for(x = 0; x <= bufferDisplay1.valueKeys[0]; x++)
                    {
                        bufferDisplay1.presetValue[0][x] = bufferDisplay1.valueKeys[x];
                        bufferDisplay1.presetValue[1][x] = bufferDisplay1.valueKeys[x];
                    }  
                    flowDisplay1 = 5;//caso para seleccion de producto
                    SetPicture(1, DISPLAY_SELECCIONE_PRODUCTO4);
                break;
            }
        break;
            
        case 5:
            
            if(side.a.GradesHose[0] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.a.GradesHose[0]-1][x],11,7+x,2,0xFFFF,'Y');
                } 
            }
            if(side.a.GradesHose[1] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.a.GradesHose[1]-1][x],17,8+x,2,0xFFFF,'Y');
                } 
            }
            if(side.a.GradesHose[2] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.a.GradesHose[2]-1][x],23,9+x,2,0xFFFF,'Y');
                } 
            }
            if(side.a.GradesHose[3] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(1, producto[side.a.GradesHose[3]-1][x],29,10+x,2,0xFFFF,'Y');
                } 
            }
            
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x80:  //Grado 1 
                            
                            flowDisplay1 = 7;
                            side.a.hose = 1;
                            //SetPicture(1, DISPLAY_SUBA_MANIJA);
                        break;
                            
                        case 0x81:  //Grado 2 
                            flowDisplay1 = 7;
                            side.a.hose = 2;
                            //SetPicture(1, DISPLAY_SUBA_MANIJA);
                        break;
                            
                        case 0x7F:  //Grado 3
                            flowDisplay1 = 7;
                            side.a.hose = 3;
                            //SetPicture(1, DISPLAY_SUBA_MANIJA);
                        break;   
                        
                        case 0xB8:  //Grado 4 
                            flowDisplay1 = 7;
                            side.a.hose = 4;
                            //SetPicture(1, DISPLAY_SUBA_MANIJA);
                        break;
                            
                        case 0x94:  //Retroceso
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0); 
                        break;
                            
                        case 0x7E:  //Pantalla Inicial
                             bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            AuthType = 0;
                        break;
                    }                    
                }
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
        break;
                    
        case 6: // Pantalla impresion

            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0x39:  //Si imprimir  
                            flowDisplay1 = 9;
                            numberKeys1 = 0;
                            bufferDisplay1.flagKeyboard = 1;
                            bufferDisplay1.flagPrint =  1;
                            PrintON = 1;
                            SetPicture(1, DISPLAY_DIGITE_PLACA);                            
                        break; 
                        case 0x38:  //No imprimir 
                            if(bufferDisplay1.flagEndSale)
                            {
                                flowDisplay1 = 0;
                                bufferDisplay1.flagPrint = 0;
                                SetPicture(1, DISPLAY_INICIO0);
                            }else
                            {
                                flowDisplay1 = 7; //Esperando estado del dispensador 
                                bufferDisplay1.flagPrint =  0;
                                count_protector = 0;
                                SetPicture(1, DISPLAY_SUBA_MANIJA);
                            }
                        break;
                        case 0x7E:  //Pantalla Inicial                                                                                    
                            flowDisplay1 = 0;
                             bufferDisplay1.flagPrint =  0;
                            SetPicture(1,DISPLAY_INICIO0);
                            bufferDisplay1.flagPrint =  0;
                        break;
                    }                    
                }
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
                Display1_ClearRxBuffer();
            }
            if(count_protector >= 30)
            {
                flowDisplay1 = 0;	
    			count_protector = 0;
                SetPicture(1, DISPLAY_INICIO0);
    		}
        break;
                                        
            // Realiza el preset
        case 7:
            
            PresetFlag = 1;
            
            if(AuthType == 1)
            {
                bufferDisplay1.flagKeyboard = 2;
                flowDisplay1 = 9;
                SetPicture(1,DISPLAY_INTRODUZCA_KILOMETRAJE);
            }
            else
            {
                SetPicture(1, DISPLAY_SUBA_MANIJA);
                flowDisplay1 = 8;
            }
            
        break;
            
        // Wait to end of transaction   
        case 8:
                          
            //Touch for return to init display
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {                        
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay1.flagPrint =  0;
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            PresetFlag = 0;
                            AuthType = 0;
                        break;
                    }                    
                }
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );              
            }
            Display1_ClearRxBuffer();
                        
        break;
            
        case 9: //Teclado           
            switch (alphanumeric_keyboard(11,0))
            {
                case 0: //Cancelar
                    switch(bufferDisplay1.flagKeyboard)
                    {
                        case 1://Placa
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay1.licenceSale[x] = 0;
                            }
                            flowDisplay1 = 0; 
                            SetPicture(1,DISPLAY_INICIO0);
                        break;
                        
                        case 2://Kilometraje
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay1.mileageSale[x] = 0;
                            }
                                                      
                                                                                    
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay1.identySale[x] = 0;
                            }
                        break;
                        case 4://ID
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay1.shiftId[x] = 0;
                            }
                        break;
                    }                    
                    Display1_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay1.flagKeyboard)
                    {
                        case 1://Placa
                            for(x = 0; x <= bufferDisplay1.valueKeys[0]; x++)
                            {
                                bufferDisplay1.licenceSale[x] = bufferDisplay1.valueKeys[x];
                            }
                            if(bufferDisplay1.flagEndSale)
                            {
                                bufferDisplay1.flagPrint = 1;
                                flowDisplay1 = 0;
                                SetPicture(1, DISPLAY_INICIO0); 
                            }else{
                                flowDisplay1 = 7;   //handle Up
                                SetPicture(1, DISPLAY_SUBA_MANIJA);
                            }
                        break;
                        
                        case 2://Kilometraje
                            for(x = 0; x <= bufferDisplay1.valueKeys[0]; x++)
                            {
                                bufferDisplay1.mileageSale[x] = bufferDisplay1.valueKeys[x];
                            }
                            
                            //Credit Sale
                            if(AuthType == 1)
                            {
                                flowDisplay1 = 10;
                                SetPicture(1,DISPLAY_ID_DIGITAL);
                            }
                            else
                            {                                
                                flowDisplay1 = 7;       //Handle Up
                                SetPicture(1,DISPLAY_SUBA_MANIJA);
                            }                                               
                                                       
                        break;
                        
                        case 3:     //CC/NIT
                            for(x = 0; x <= bufferDisplay1.valueKeys[0]; x++)
                            {
                                bufferDisplay1.identySale[x] = bufferDisplay1.valueKeys[x];
                            }
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
                        break;
                    }                    
                    Display1_ClearRxBuffer();
                break;
            }            
        break;    
    
    ////////////////// CASOS PARA CRÉDITO  /////////////////////
        case 10:
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3])
                    {
                        case 0xB6:  //Solicitud ibutton  
                            flowDisplay1 = 11;
                            numberKeys1 = 0;                            
                            bufferDisplay1.flagPrint =  1;
                            SetPicture(1, DISPLAY_ESPERANDO_ID);                            
                        break; 
                        case 0xB7:  //ID por número
                            bufferDisplay1.flagKeyboard = 0;
                            flowDisplay1 = 7;//Esperando estado del dispensador  
                            count_protector = 0;
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
                        break;
                    }                    
                }
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
        
        case 11:
            // iButton read
            if(touch_present(1) == 1)
            {
				if(touch_write(1,0x33))
                {
					for(x = 1; x <= 8; x++)
                    {
						temporal[x] = touch_read_byte(1);   // Id
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
                        Display1_ClearRxBuffer();
                        vTaskDelay( 700 / portTICK_PERIOD_MS );                       
                        iButtonFlag = 1;
                        SetPicture(1, DISPLAY_SUBA_MANIJA);
                        flowDisplay1 = 8;
                        
                        
                                                
					}
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
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(1, DISPLAY_INICIO0);
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            PresetFlag = 0;
                            iButtonFlag = 0;
                            AuthType = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
            }
            Display1_ClearRxBuffer();
                                    
        break;    
    ///////////////FIN CASOS PARA CRÉDITO  /////////////////////  
    
    //////////// CASOS PARA CONFIGURACIONES  ///////////////////
        case 12:
            if(Display1_GetRxBufferSize() == 8)
            {
                if((Display1_rxBuffer[0] == 0xAA) && (Display1_rxBuffer[6] == 0xC3) && (Display1_rxBuffer[7] == 0x3C))
                {
                    switch(Display1_rxBuffer[3]){
                        case 0x46:  //Turnos              
                            flowDisplay1 = 13; 
                            if(lockTurn == 1){
                                SetPicture(1,DISPLAY_CERRAR_TURNO);
                            }else{
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
                        break;
                        case 0xB5:  //Copia de recibo 
                            flowDisplay1  = 0;
                            side.a.RFstateReport = 1;
                            side.a.rfState = RF_COPY_RECEIPT;
                            SetPicture(1,DISPLAY_INICIO0);                            
                        break;
                        
                        case 0x3B:  //Pantalla Inicial    
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);                            
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);                            
                        break;
                        
                        case 0x7E:  //Pantalla Inicial
                            bufferDisplay1.flagPrint =  0;
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            
                        break;
                    }
                }  
                //CyDelay(10);
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
                        case 0x47:  //Cambio de turno: Abrir  
                            flowDisplay1 = 9;
                            numberKeys1 = 0;    
                            bufferDisplay1.flagKeyboard = 4;
                            SetPicture(1,DISPLAY_INTRODUZCA_CEDULA);                            
                        break;
                        case 0x48:  //Cambio de turno: Cerrar 
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
                //CyDelay(10);
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
            
        case 15:
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
                //CyDelay(10);     
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break; 
            
        case 16:
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
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
            
        case 17:
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
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display1_ClearRxBuffer();
            }                        
        break;
            
        case 18:
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
                //CyDelay(10);
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
            
        case 20:
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
                            flowDisplay1 = 0;
                            bufferDisplay1.flagPrint =  0;
                            PresetFlag = 0;
                            iButtonFlag = 0;
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
    }    
}

/*
*********************************************************************************************************
*                                         void polling_Pump(void)
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
			InitDisplay2();
            flowDisplay2 = 1;
            count_protector2 = 0;
            bufferDisplay2.flagEndSale = false;  
			side.b.RFstateReport = 0;
            
            if(bufferDisplay2.flagPrint == 1)
            {    
                vTaskDelay( 200 / portTICK_PERIOD_MS );
                imprimir(printPortB, side.b.dir);
                bufferDisplay2.flagPrint = 0;
                bufferDisplay2.saleType = 0;               
            }
        break;
        case 1: //Menu
            
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {                                             
                    flowDisplay2 = 2;                          //Pantalla forma de pago                      
                    SetPicture(2,DISPLAY_FORMA_PAGO_DESEADA);                                                                                                  

                }  

                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }
        break;
        case 2:
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x0D:  //Pantalla efectivo              
                            flowDisplay2 = 3; 
                            bufferDisplay2.saleType = 1;
                            SetPicture(2,DISPLAY_FORMA_PROGRAMACION);
                            AuthType2 = 0;
                        break;
                        case 0x0E:  //Pantalla credito  
                            flowDisplay2 = 3;
                            bufferDisplay2.saleType = 2;
                            count_protector2 = 0;
                            SetPicture(2,DISPLAY_FORMA_PROGRAMACION); 
                            AuthType2 = 1;
                        break;
                        case 0x45:  //Pantalla otras opciones 
                            flowDisplay2 = 12;                            
                            SetPicture(2,DISPLAY_OPERACIONES); 
                            AuthType2 = 0;
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                        break;
                        
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                        break;
                    }
                }  
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }
        break;
            
        case 3: 
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x0F:  //Preset dinero                
                            flowDisplay2 = 4;                              
                            bufferDisplay2.presetType[0] = 2;
                            bufferDisplay2.presetType[1] = 'D';
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
                        break; 
                        case 0x10:  //Preset volumen                 
                            flowDisplay2 = 4;                            
                            bufferDisplay2.presetType[0] = 1;
                            bufferDisplay2.presetType[1] = 'V';
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
                        break;
                        case 0x43:  //Preset full 
                            bufferDisplay2.presetType[0] = 2;
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
                        break;
                        case 0x94:  //Retroceso 
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                                                        
                        break;
                        case 0x3B:  //Pantalla Inicial    
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                        break;
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);
                            AuthType2 = 0;
                        break;
                    }
                }  
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }
        break;
        
        case 4:            
            switch (alphanumeric_keyboard2(digits + 1,0))
            {
                case 0:  //Pantalla Inicial    
                    flowDisplay2 = 0;
                    SetPicture(2,DISPLAY_INICIO0);                            
                break;
                    
                case 1: //Enter
                    for(x = 0; x <= bufferDisplay2.valueKeys[0]; x++)
                    {
                        bufferDisplay2.presetValue[0][x] = bufferDisplay2.valueKeys[x];
                        bufferDisplay2.presetValue[1][x] = bufferDisplay2.valueKeys[x];
                    }  
                    flowDisplay2 = 5;       //caso para seleccion de producto
                    SetPicture(2,DISPLAY_SELECCIONE_PRODUCTO4);
                break;
            }
        break;
            
        case 5:
            if(side.b.GradesHose[0] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.b.GradesHose[0]-1][x],11,7+x,2,0xFFFF,'Y');
                } 
            }
            if(side.b.GradesHose[1] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.b.GradesHose[1]-1][x],17,8+x,2,0xFFFF,'Y');
                } 
            }
            if(side.b.GradesHose[2] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.b.GradesHose[2]-1][x],23,9+x,2,0xFFFF,'Y');
                } 
            }
            if(side.b.GradesHose[3] !=0)
            {
                for(x = 0; x < 10; x++)
                {
                    WriteMessage(2, producto[side.b.GradesHose[3]-1][x],29,10+x,2,0xFFFF,'Y');
                } 
            }
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x80:  //Grado 1 
                            flowDisplay2 = 7;
                            side.b.hose = 1;                            
                            //SetPicture(2,DISPLAY_SUBA_MANIJA);
                        break;        
                        case 0x81:  //Grado 2 
                            flowDisplay2 = 7;
                            side.b.hose = 2;                          
                            //SetPicture(2,DISPLAY_SUBA_MANIJA);
                        break;
                            
                        case 0x7F:  //Grado 3
                            flowDisplay2 = 7;
                            side.b.hose = 3;                            
                            //SetPicture(2,DISPLAY_SUBA_MANIJA);
                        break;   
                        
                        case 0xB8:  //Grado 4 
                            flowDisplay2 = 7;
                            side.b.hose = 4;                           
                            //SetPicture(2,DISPLAY_SUBA_MANIJA);
                        break;
                            
                        case 0x94:  //Retroceso
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                                                        
                        break;
                            
                        case 0x7E:  //Pantalla Inicial    
                             bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            AuthType2 = 0;
                        break;
                    }                    
                }
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }
        break;
                    
        case 6:

            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0] == 0xAA) && (Display2_rxBuffer[6] == 0xC3) && (Display2_rxBuffer[7] == 0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {
                        case 0x39:  //Si imprimir  
                            flowDisplay2 = 9;
                            numberKeys2 = 0;
                            bufferDisplay2.flagKeyboard = 1;
                            bufferDisplay2.flagPrint =  1;
                            SetPicture(2,DISPLAY_DIGITE_PLACA);                            
                        break; 
                        case 0x38:  //No imprimir 
                            if(bufferDisplay2.flagEndSale)
                            {
                                bufferDisplay2.flagPrint = 0;
                                flowDisplay2 = 0;
                                SetPicture(2,DISPLAY_INICIO0);
                            }else
                            {
                                flowDisplay2 = 7;//Esperando estado del dispensador 
                                bufferDisplay2.flagPrint =  0;
                                count_protector = 0;
                                SetPicture(2,DISPLAY_SUBA_MANIJA);
                            }
                        break;
                        case 0x7E:  //Pantalla Inicial                                                                                    
                            flowDisplay2 = 0;
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            bufferDisplay2.flagPrint = 0;
                            AuthType2 = 0;
                        break;
                    }                    
                }
         
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }
            if(count_protector >= 30)
            {
                flowDisplay2 = 0;	
    			count_protector=0;
                SetPicture(2,DISPLAY_INICIO0);
    		}
        break;                                             
        case 7:

            PresetFlag2 = 1;
            
            if(AuthType2 == 1)
            {
                bufferDisplay2.flagKeyboard = 2;
                flowDisplay2 = 9;  
                SetPicture(2,DISPLAY_INTRODUZCA_KILOMETRAJE);
            }
            else
            {
                SetPicture(2, DISPLAY_SUBA_MANIJA);
                flowDisplay2 = 8;
            }
                       
        break;
            
        case 8:            
            if(Display2_GetRxBufferSize() == 8)
            {
                if((Display2_rxBuffer[0]==0xAA) && (Display2_rxBuffer[6]==0xC3) && (Display2_rxBuffer[7]==0x3C))
                {
                    switch(Display2_rxBuffer[3])
                    {                        
                        case 0x7E:  //Pantalla Inicial 
                            bufferDisplay2.flagPrint =  0;
                            SetPicture(2,DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            PresetFlag2 = 0;
                            AuthType2 = 0;
                        break;
                    }                    
                }
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
            }
            Display2_ClearRxBuffer();    
            
        break;
            
        case 9: //Teclado           
            switch (alphanumeric_keyboard2(11,0))
            {
                case 0: //Cancelar
                    switch(bufferDisplay2.flagKeyboard)
                    {
                        case 1://Placa
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay2.licenceSale[x] = 0;
                            }
                            flowDisplay2 = 0;//Inicio
                            SetPicture(2,DISPLAY_INICIO0);
                        break;
                        
                        case 2://Kilometraje
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay2.mileageSale[x] = 0;
                            }
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay2.identySale[x] = 0;
                            }
                        break;
                        case 4://ID
                            for(x = 0; x < 11; x++)
                            {
                                bufferDisplay2.shiftId[x] = 0;
                            }
                        break;
                    }                    
                    Display2_ClearRxBuffer();
                break;
                    
                case 1: //Enter
                    switch(bufferDisplay2.flagKeyboard)
                    {
                        case 1://Placa
                            for(x = 0; x <= bufferDisplay2.valueKeys[0]; x++)
                            {
                                bufferDisplay2.licenceSale[x] = bufferDisplay2.valueKeys[x];
                            }
                            if(bufferDisplay2.flagEndSale)
                            {
                                
                                bufferDisplay2.flagPrint = 1;
                                flowDisplay2 = 0;
                                SetPicture(2,DISPLAY_INICIO0); 
                            }else{
                                flowDisplay2 = 7;   //Suba manija

                                SetPicture(2,DISPLAY_SUBA_MANIJA);
                            }
                        break;
                        
                        case 2:                     //Kilometraje
                            for(x = 0; x <= bufferDisplay2.valueKeys[0]; x++)
                            {
                                bufferDisplay2.mileageSale[x] = bufferDisplay2.valueKeys[x];
                            }
                            
                            if(AuthType2 == 1)
                            {
                                flowDisplay2 = 10;
                                SetPicture(2,DISPLAY_ID_DIGITAL);
                            }
                            else
                            {
                                flowDisplay2 = 7;       //Suba manija
                                SetPicture(2,DISPLAY_SUBA_MANIJA);
                            }
                            
                        break;
                        
                        case 3://CC/NIT
                            for(x = 0; x <= bufferDisplay2.valueKeys[0]; x++)
                            {
                                bufferDisplay2.identySale[x] = bufferDisplay2.valueKeys[x];
                            }
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
                        break;
                    }                    
                    Display2_ClearRxBuffer();
                break;
            }            
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
                            count_protector = 0;
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
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }                        
        break;
        
        case 11:
            
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
                        Display2_ClearRxBuffer();
                        vTaskDelay( 700 / portTICK_PERIOD_MS ); 
                        iButtonFlag2 = 1;
                        SetPicture(2, DISPLAY_SUBA_MANIJA);
                        flowDisplay2 = 8;
                        
					}
                    else
                    {
//                        SetPicture(2,DISPLAY_ID_NO_RECONOCIDO);                                           
//                        Display2_ClearRxBuffer();
//                        vTaskDelay( 700 / portTICK_PERIOD_MS );    
//                        SetPicture(2, DISPLAY_INICIO0);
//                        flowDisplay2 = 0;
//                        bufferDisplay2.flagPrint =  0;
//                        PresetFlag2 = 0;
//                        iButtonFlag2 = 0;
//                        AuthType2 = 0;
                    }
                    
                    
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
                        break;
                        
                        case 0x94:  //Cancel Button                                                        
                            SetPicture(2, DISPLAY_INICIO0);
                            flowDisplay2 = 0;
                            bufferDisplay2.flagPrint =  0;
                            PresetFlag2 = 0;
                            iButtonFlag2 = 0;
                            AuthType2 = 0;
                        break;
                    }                    
                }
                
                vTaskDelay( 10 / portTICK_PERIOD_MS );              //Freertos delay
            }
            Display2_ClearRxBuffer();
                                    
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
                        break;
                            
                        case 0xB5:  //Copia de recibo 
                            flowDisplay2  = 0;
                            side.b.RFstateReport = 1;
                            side.b.rfState = RF_COPY_RECEIPT;
                            SetPicture(2,DISPLAY_INICIO0);                            
                        break;
                        
                        case 0x3B:  //Pantalla Inicial    
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                            
                        break;
                            
                        case 0x94:  //Pantalla Inicial
                            bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                            
                        break;
                        
                        case 0x7E:  //Pantalla Inicial
                             bufferDisplay2.flagPrint =  0;
                            flowDisplay2 = 0;
                            SetPicture(2,DISPLAY_INICIO0);                            
                        break;
                    }
                }  
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }
        break;

        case 13:
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
                        break;
                        case 0x48:  //Cambio de turno: Cerrar 
                            flowDisplay2 = 9;
                            numberKeys2 = 0;   
                            bufferDisplay2.flagKeyboard = 4;
                            SetPicture(2,DISPLAY_INTRODUZCA_CEDULA);                           
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
                //CyDelay(10);
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
            
        case 15:
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
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS ); 
                Display2_ClearRxBuffer();
            }                        
        break; 
            
        case 16:
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
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 17:
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
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 18:
            if(leer_hora() == 1)
            {
                WriteLCD(2,(((timeDownHandle[1]&0x10)>>4)+48),13,8,1,0x0000,'N');
                WriteLCD(2,((timeDownHandle[1]&0x0F)+48),13,9,1,0x0000,'N');
                WriteLCD(2,':',13,10,1,0x0000,'N');
                WriteLCD(2,(((timeDownHandle[0]&0xF0)>>4)+48),13,11,1,0x0000,'N');
                WriteLCD(2,((timeDownHandle[0]&0x0F)+48),13,12,1,0x0000,'N');               
            }
            if(leer_fecha() == 1)
            {                
                WriteLCD(2,(((dateDownHandle[0]&0x30)>>4)+48),21,8,1,0x0000,'N');
                WriteLCD(2,((dateDownHandle[0]&0x0F)+48),21,9,1,0x0000,'N');
                WriteLCD(2,'/',21,10,1,0x0000,'N');
                WriteLCD(2,(((dateDownHandle[1]&0x10)>>4)+48),21,11,1,0x0000,'N');
                WriteLCD(2,((dateDownHandle[1]&0x0F)+48),21,12,1,0x0000,'N');
                WriteLCD(2,'/',21,13,1,0x0000,'N');
                WriteLCD(2,(((dateDownHandle[2]&0xF0)>>4)+48),21,14,1,0x0000,'N');
                WriteLCD(2,((dateDownHandle[2]&0x0F)+48),21,15,1,0x0000,'N');
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
                //CyDelay(10);
                vTaskDelay( 10 / portTICK_PERIOD_MS );
                Display2_ClearRxBuffer();
            }                        
        break;
            
        case 19:            
            side.b.RFstateReport = 1;
            ShiftState =1;
        break;
         
    }    
}

void PresetAuthorize(void)
{
    if(PresetFlag == 1)
    {
        // pump handle detect 
        side.a.activeHose = PumpHoseActiveState(side.a.dir);
                
        if(iButtonFlag == 1) 
        {
            CreditAuth = RF_CREDITSALEAUTH;
            side.a.RFstateReport = 1;
            iButtonFlag = 0;
        }
        
            //iButton Authorized
            if(Credit_Auth_OK == 1 && AuthType == 1)
            {
                if (side.a.activeHose == side.a.hose)
                {   
                    priceChange(side.a.dir, side.a.grade, ppuiButtonA[side.a.grade]);
                    
                    // PRESET
                    if(PresetData(side.a.dir, side.a.activeHose, bufferDisplay1.presetValue[0], bufferDisplay1.presetType[0] & 0x03) == 1)
                    {                    
                        
                        get_state(side.a.dir);
                  
                        //Authorize
                        Authorization(side.a.dir);                                                         
                        side.a.RFstateReport = 1;
                        count_protector = 0;                    
                        bufferDisplay1.flagActiveSale = true;
                        SetPicture(1, DISPLAY_DESPACHANDO);   
                        ShowMessage(1, (bufferDisplay1.presetValue[1]), 18);
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
                        return;
                    }
                }else
                {
                    flowDisplay1 = 7;
                    return;
                }
                
            }
            if(AuthType == 0)
            {       
                //Grade selected  =  Grade pump handle
                if (side.a.activeHose == side.a.hose)
                {   
                        priceChange(side.a.dir, side.a.grade, side.a.ppuAuthorized[side.a.grade]);
                        
                        // PRESET
                        if(PresetData(side.a.dir, side.a.activeHose, bufferDisplay1.presetValue[0], bufferDisplay1.presetType[0] & 0x03) == 1)
                        {                    
                            
                            get_state(side.a.dir);
                      
                            //Authorize
                            Authorization(side.a.dir);                                                         
                            side.a.RFstateReport = 1;
                            count_protector = 0;                    
                            bufferDisplay1.flagActiveSale = true;
                            SetPicture(1, DISPLAY_DESPACHANDO);   
                            ShowMessage(1,(bufferDisplay1.presetValue[1]),18);
                            PresetFlag = 0;
                            AuthType = 0;
                            return;
                        }else
                        {
                            flowDisplay1 = 0;
                            SetPicture(1, DISPLAY_ERROR);
                            vTaskDelay( 200 / portTICK_PERIOD_MS );
                            SetPicture(1, DISPLAY_INICIO0);
                            PresetFlag = 0;
                            AuthType = 0;
                            return;
                        }
                }else
                {
                    flowDisplay1 = 7;
                    return;
                }
         PresetFlag = 0;
         AuthType = 0;
        }
    }
        
    if(PresetFlag2 == 1)
    {
        side.b.activeHose = PumpHoseActiveState(side.b.dir);  
        
        if(iButtonFlag2 == 1)
        {
            CreditAuth2 = RF_CREDITSALEAUTH;
            side.b.RFstateReport = 1;
            iButtonFlag2 = 0;
        }
        
        //iButton Authorized Credit
            if(Credit_Auth_OK2 == 1 && AuthType2 == 1)
            {
                              
                if (side.b.activeHose == side.b.hose)
                {   
                    priceChange(side.b.dir, side.b.grade, ppuiButtonB[side.b.grade]);
                    
                    if(PresetData(side.b.dir, side.b.activeHose, bufferDisplay2.presetValue[0], bufferDisplay2.presetType[0] & 0x03) == 1)
                    {                    
                        get_state(side.b.dir);

                        //Authorize
                        Authorization(side.b.dir);
                        side.b.RFstateReport = 1;                    
                        count_protector2 = 0;   
        				bufferDisplay2.flagActiveSale = true;					
                        SetPicture(2, DISPLAY_DESPACHANDO);   
                        ShowMessage(2,(bufferDisplay2.presetValue[0]),18);
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
                }else
                {
                    flowDisplay2 = 7;
                    return;
                }
            }else
                {
                    flowDisplay2 = 7;
                    return;
                }
    }
                // Cash Sale
    if(AuthType2 == 0)
    {
                     
        if (side.b.activeHose == side.b.hose)
        {                
            priceChange(side.b.dir, side.b.grade, side.b.ppuAuthorized[side.b.grade]);
            
            if(PresetData(side.b.dir, side.b.activeHose, bufferDisplay2.presetValue[0], bufferDisplay2.presetType[0] & 0x03) == 1)
            {                    
                get_state(side.b.dir);

                //Authorize
                Authorization(side.b.dir);
                side.b.RFstateReport = 1;                    
                count_protector2 = 0;   
				bufferDisplay2.flagActiveSale = true;					
                SetPicture(2, DISPLAY_DESPACHANDO);   
                ShowMessage(2,(bufferDisplay2.presetValue[0]),18);
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
                return;
            }
        }else
        {
            flowDisplay2 = 7;
            return;
        }
    }
            
        PresetFlag2 = 0;
        AuthType2 = 0;
    }

    


/* Total Task */
void Display_Task(void *arg)
{
   TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;
    
    xLastWakeTime = xTaskGetTickCount();

    while(1) 
    {
        PollingDisplay1();
        PollingDisplay2();   
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void PumpAction(uint8 PositionPump, uint8 State)
{
      
    // Current Loop recovery
    if(ActualState[PositionPump] == PUMP_FAIL && State != PUMP_FAIL)
    {
        flowDisplay1 = 0;
        flowDisplay2 = 0;
        //InitPump();
    }
    
    // Zero Sale detect
    if(ActualState[PositionPump] == PUMP_BUSY && State == PUMP_IDLE)
    {

            if(PositionPump == side.a.dir)
            {
                flowDisplay1 = 0;
                side.a.rfState = RF_ZERO_SALE;
                bufferDisplay1.flagActiveSale = false;
                SetPicture(1,DISPLAY_INICIO0);
                ActualState[PositionPump] = State;
                return;
            }
            if(PositionPump == side.b.dir)
            {
                flowDisplay2 = 0;
                side.b.rfState = RF_ZERO_SALE;
                bufferDisplay2.flagActiveSale = false;
                SetPicture(2,DISPLAY_INICIO0);
                ActualState[PositionPump] = State;
                return;
            }
            
                
    }

    //Actualize state
    ActualState[PositionPump] = State;
       
    switch(State)
    {
        case PUMP_FAIL:          
            SetPicture(1,DISPLAY_ERROR);
            SetPicture(2,DISPLAY_ERROR);
            side.a.rfState = RF_ERROR;
            side.b.rfState = RF_ERROR;
                         
        break;
        case PUMP_IDLE:

            if(PositionPump == side.a.dir)
            {    
                if(flowDisplay1 == 1)
                {
                    ShowRectangle(1,31);
                }
                side.a.rfState = RF_IDLE;             
            }
                    
            if(PositionPump == side.b.dir)
            {        
                if(flowDisplay2 == 1)
                {
                    ShowRectangle(2,31);
                }       			
                side.b.rfState = RF_IDLE;
            }
                         
        break;
        case PUMP_CALLING:
                                     
                PresetAuthorize();                                                 

        break;
        case PUMP_AUTHORIZED:

        break;
        case PUMP_BUSY:
            
                                 
            if(PositionPump == side.a.dir)
            {
                bufferDisplay1.flagActiveSale = true;
                bufferDisplay1.flagEndSale = false;
                side.a.rfState = RF_DELIVERING;
                ShowMessage(PositionPump,(bufferDisplay1.presetValue[1]),18);
            }
            if(PositionPump == side.b.dir)
            {
                bufferDisplay2.flagActiveSale = true;
                bufferDisplay2.flagEndSale = false;
                side.b.rfState = RF_DELIVERING;
                ShowMessage(PositionPump,(bufferDisplay2.presetValue[1]),18);
            }
                    
        break;
        case PUMP_PEOT:
            
            if(getSale(PositionPump))
            {
                if(PositionPump == side.a.dir)
                {
                    if(bufferDisplay1.flagPrint == 1)
                    {
                        bufferDisplay1.flagEndSale = true;
                        flowDisplay1 = 0;
                        SetPicture(1, DISPLAY_INICIO0); 
                    }else
                    {
                        flowDisplay1 = 6;
                        bufferDisplay1.flagEndSale = true;
                        SetPicture(1, DISPLAY_DESEA_IMPRIMIR_RECIBO); 
                    }
                    bufferDisplay1.flagActiveSale = false;
                    side.a.RFstateReport = 1;  
                    priceChange(side.a.dir, side.a.grade, side.a.ppuAuthorized[side.a.grade]);
               }
               if(PositionPump == side.b.dir)
               {
                    if(bufferDisplay2.flagPrint == 1)
                    {
                        bufferDisplay2.flagEndSale = true;
                        flowDisplay2 = 0;
                        SetPicture(2,DISPLAY_INICIO0); 
                    }else
                    {
                        flowDisplay2 = 6;
                        bufferDisplay2.flagEndSale = true;
                        SetPicture(2,DISPLAY_DESEA_IMPRIMIR_RECIBO); 
                    }
                    bufferDisplay2.flagActiveSale = false;
                    side.b.RFstateReport = 1;
                    priceChange(side.b.dir, side.b.grade, side.b.ppuAuthorized[side.b.grade]);
                }
                              
            }
            iButtonFlag = 0;
            
        break;
        case PUMP_FEOT:
            
            if(getSale(PositionPump))
            {
                if(PositionPump == side.a.dir)
                {
                    if(bufferDisplay1.flagPrint == 1)
                    {
                        bufferDisplay1.flagEndSale = true;
                        flowDisplay1 = 0;
                        SetPicture(1, DISPLAY_INICIO0); 
                    }else
                    {
                        flowDisplay1 = 6;
                        bufferDisplay1.flagEndSale = true;
                        SetPicture(1, DISPLAY_DESEA_IMPRIMIR_RECIBO); 
                    }
                    bufferDisplay1.flagActiveSale = false;
                    side.a.RFstateReport = 1;   
                    priceChange(side.a.dir, side.a.grade, side.a.ppuAuthorized[side.a.grade]);
               }
               if(PositionPump == side.b.dir)
               {
                    if(bufferDisplay2.flagPrint == 1)
                    {
                        bufferDisplay2.flagEndSale = true;
                        flowDisplay2 = 0;
                        SetPicture(2,DISPLAY_INICIO0); 
                    }else
                    {
                        flowDisplay2 = 6;
                        bufferDisplay2.flagEndSale = true;
                        SetPicture(2,DISPLAY_DESEA_IMPRIMIR_RECIBO); 
                    }
                    bufferDisplay2.flagActiveSale = false;
                    side.b.RFstateReport = 1;
                    priceChange(side.b.dir, side.b.grade, side.b.ppuAuthorized[side.b.grade]);
                }
                              
            }
            iButtonFlag = 0;
         
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
               StatePosition[0];
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
        
   
    side.a.changePPU = false;
    side.b.changePPU = false;
}

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

}


/* Pump Task */
void Pump_Task(void *arg)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 1;
    uint8 i;
    
    xLastWakeTime = xTaskGetTickCount();
    
    // Vector State Init
    for(i = 0; i < 8; i++)
    {
        StatePosition[i] = 0x00;
    }
    
    CheckInitState();   
    ReadPPUFromEEprom();                       
    side.a.changePPU = true;
    side.b.changePPU = true;
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
//                if(i == 2)
//                {                
//                    PumpAction(side.c.dir, StatePosition[i]);
//                }
//                if(i == 3)
//                {                  
//                    PumpAction(side.d.dir, StatePosition[i]);
//                }       
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
        
        if(side.a.changePPU || side.b.changePPU)
        {
            SetPPU();
        }
        //vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


/* [] END OF FILE */
