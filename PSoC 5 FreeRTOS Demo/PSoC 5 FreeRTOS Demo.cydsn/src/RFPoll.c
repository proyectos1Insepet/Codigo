#include <project.h>
#include <protocol.h>
#include <variables.h>
#include <RFPoll.h>
#include <LCD.h>
#include <I2C.h>
#include "Printing.h"
#include <device.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* RTOS includes. */
#include <FreeRTOS.h>
#include <semphr.h>
#include "os_resource.h"

extern xSemaphoreHandle g_pUARTSemaphore;
uint8_t lengthPrint;

void RFTotalA(void)
{
    uint16 x,y,z;

        pollTotals = 0;
        y = 0;
        buffer_txTotals[y] = 0xBC; y++;
        buffer_txTotals[y] = 0xCB; y++;
        buffer_txTotals[y] = 0xC8; y++;
        buffer_txTotals[y] = IDCast[0]; y++;
        buffer_txTotals[y] = IDCast[1]; y++;
        buffer_txTotals[5] = side.a.dir; y++;
        buffer_txTotals[y] = 0xE8; y++;
        buffer_txTotals[y] = side.a.rfState; y++;
        buffer_txTotals[y] = 0x00; y++;
        
        for(z = 1;z <= side.a.hoseNumber; z++)
        {
            buffer_txTotals[y] = z; y++;                    
            if (digits == 6 || digits == 5)
            {                            
                for(x = 8; x > 0; x--)
                {  
                    //Volumen
                    buffer_txTotals[y] = (side.a.totalsHandle[z - 1][0][x]) - 48; y++;
                }
                for(x =0; x < 4; x++)
                {
                    buffer_txTotals[y] = 0x00; y++;
                }
                for(x = 8; x > 0; x--)
                {  
                    //Dinero
                    buffer_txTotals[y] = (side.a.totalsHandle[z - 1][1][x]) - 48; y++;
                }
                for(x =0; x < 4; x++)
                {
                    buffer_txTotals[y] = 0x00; y++;
                }
            }else{
                for(x = 12; x > 0; x--)
                { 
                    //Volumen              
                    buffer_txTotals[y] = (side.a.totalsHandle[z - 1][0][x]) - 48; y++;
                }                        
                for(x = 12; x > 0; x--)
                { 
                    //Dinero
                    buffer_txTotals[y] = (side.a.totalsHandle[z - 1][1][x]) - 48; y++;
                }                        
            } 
        }
        buffer_txTotals[8] = y - 9;
        buffer_txTotals[y] = verificar_check(buffer_txTotals,y + 1);y++;                 
        
        for (x = 0; x < y; x++)
        {
            RF_Connection_PutChar(buffer_txTotals[x]);
        }        
       
    bufferAready = 0; 
    FlagTotal = 0;
    pollTotals = 0;
    return;
}

void RFTotalB(void)
{
    uint16 x,y,z;
    
        pollTotals = 0;
        y = 0;
        buffer_txTotals[y] = 0xBC; y++;
        buffer_txTotals[y] = 0xCB; y++;
        buffer_txTotals[y] = 0xC8; y++;
        buffer_txTotals[y] = IDCast[0]; y++;
        buffer_txTotals[y] = IDCast[1]; y++;
        buffer_txTotals[5] = side.b.dir; y++;
        buffer_txTotals[y] = 0xE8; y++;
        buffer_txTotals[y] = side.b.rfState; y++;
        buffer_txTotals[y] = 0x00; y++;
        for(z = 1; z <= side.b.hoseNumber; z++)
        {
            buffer_txTotals[y] = z; y++;                    
            if (digits == 6 || digits == 5)
            {   
                //Volumen
                for(x = 8; x > 0; x--)
                { 
                    buffer_txTotals[y] = (side.b.totalsHandle[z - 1][0][x]) - 48; y++;
                }
                for(x = 0; x < 4; x++)
                {
                    buffer_txTotals[y] = 0x00; y++;
                }
                for(x = 8; x > 0; x--)
                {//Dinero
                    buffer_txTotals[y] = (side.b.totalsHandle[z - 1][1][x]) - 48; y++;
                }
                for(x = 0; x < 4; x++)
                {
                    buffer_txTotals[y] = 0x00; y++;
                }
            }else
            {
                 //Volumen
                for(x = 12; x > 0; x--)
                {
                    buffer_txTotals[y] = (side.b.totalsHandle[z - 1][0][x]) - 48; y++;
                }     
                
                //Dinero
                for(x = 12; x > 0; x--)
                {
                    buffer_txTotals[y] = (side.b.totalsHandle[z - 1][1][x]) - 48; y++;
                }                        
            } 
        }
        buffer_txTotals[8] = y - 9;
        buffer_txTotals[y] = verificar_check(buffer_txTotals,y + 1);y++;                 
        
        for (x = 0; x < y; x++)
        {
            RF_Connection_PutChar(buffer_txTotals[x]);
        }        
        side.b.RFstateReport = 0;
        bufferAreadyB = 0; 
        FlagTotalB = 0;
        pollTotals = 0;
        return;
       
    }    
    

/*
*********************************************************************************************************
*                             uint8 verificar_check(uint8 *datos, uint16 size)
*
* Description : calcula el checksum
*               

*********************************************************************************************************
*/
uint8 verificar_check(uint8 *datos, uint16 size){
	uint8 checksum,index; 
	uint16 i;
    uint8 table[256] = { 
    0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
    35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
    190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
    70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
    219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
    17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
    50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
    87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};	
	checksum=0;
	for(i = 0; i < (size - 1);i++)
    {
		index = (uint8)(checksum ^ datos[i]);
		checksum = table[index];				
	}
	return checksum;
}


void pollingRF_Rx(uint8 PRF_rxBuffer[])
{
    uint16 i, x, y, j, contEscape;  
    uint8 buffer_rfTMP;
    uint8 EEpromGradeAddress;
   
    ActiveRF = 1;
    counterRF = 0;
    i = 0;
    y = 0;
    buffer_tx[y] = 0xBC; y++;
    buffer_tx[y] = 0xCB; y++;
    buffer_tx[y] = 0xC8; y++;
    buffer_tx[y] = IDCast[0]; y++;
    buffer_tx[y] = IDCast[1]; y++;
        
        // Query Command Action
        if(((PRF_rxBuffer[0] == 0xBC) && (PRF_rxBuffer[1] == 0xBB) && (PRF_rxBuffer[2] == 0xB8) && (PRF_rxBuffer[3] == IDCast[0]) && (PRF_rxBuffer[4] == IDCast[1])) )
        {                  
            switch(PRF_rxBuffer[6])
            {
               case 0xA1:                  // Status
                // Position 1
                if(PRF_rxBuffer[5] == side.a.dir)                  
                {                 
                    // Idle state
                    if(bufferAready == 0 && FlagTotal == 0)
                    {                            
                        buffer_tx[5] = side.a.dir; y++;
                        buffer_tx[y] = 0xA1; y++;
                        buffer_tx[y] = side.a.rfState; y++;
                        buffer_tx[y] = verificar_check(buffer_tx,y+1); y++;
                                                        
                        /*Send Data */
                        for (x = 0; x < y; x++)
                        {
                            RF_Connection_PutChar(buffer_tx[x]);
                        }  
                    }
                    
                    // Preset or End Sale
                    if(bufferAready == 1)
                    {                                           
                           
                        for (x = 1; x < buffer_A[0]; x++)
                        { 
                            RF_Connection_PutChar(buffer_A[x]);
                        }
                        

                        bufferAready = 0;
                    }
                    
                    // Authorization Request
                    if(bufferAready == 2)
                    {                                                             
                        for (x = 0; x < 37; x++)
                        { 
                            RF_Connection_PutChar(buffer_A[x]);
                        }
                        bufferAready = 0;
                    }
                    
                    // Authorization Request ACK
                    if(bufferAready == 3)
                    {                                                             
                        for (x = 0; x < 10; x++)
                        { 
                            RF_Connection_PutChar(buffer_A[x]);
                        }
                        bufferAready = 0;
                        AckFlag = 0;
                    }                   
                    
                    // Total
                    if(bufferAready == 0 && FlagTotal == 1)
                    {                                           
                        RFTotalA();
                        bufferAready = 0;
                        FlagTotal = 0;
                        pollTotals = 0;
                        
                    }      
                }
                
                // Position 2
                if(PRF_rxBuffer[5] == side.b.dir)
                {
                    if(bufferAreadyB == 0 && FlagTotalB == 0)
                    {
                        buffer_tx[5] = side.b.dir; y++;
                        buffer_tx[y] = 0xA1; y++;
                        buffer_tx[y] = side.b.rfState;y++;
                        buffer_tx[y] = verificar_check(buffer_tx,y+1);y++;
                        for (x = 0; x < y; x++)
                        {
                            RF_Connection_PutChar(buffer_tx[x]);
                        }
                    }
                    
                    // Si el estado es PRESET o Fin Venta
                    if(bufferAreadyB == 1)
                    {                                                                   
                        for (x = 0; x < buffer_B[100]; x++)
                        {
                            RF_Connection_PutChar(buffer_B[x]);         
                        }
                     
                        bufferAreadyB = 0;
                    }
                    
                    // Authorization Request
                    if(bufferAreadyB == 2)
                    {                                                             
                        for (x = 0; x < 37; x++)
                        { 
                            RF_Connection_PutChar(buffer_B[x]);
                        }
                        bufferAreadyB = 0;
                        
                    }
                                                                                              
                    // Totales
                    if(bufferAreadyB == 0 && FlagTotalB == 1)
                    {                                           
                        RFTotalB(); 
                        bufferAreadyB = 0;
                        FlagTotalB = 0;
                        pollTotals = 0;
                    }           
                }
                
             
                RF_Connection_ClearRxBuffer(); 
                break;
                
                case 0xA3:               //Credit Authorization Response
                    if(PRF_rxBuffer[5] == side.a.dir)
                    {
                        if(PRF_rxBuffer[8] == 0x01)
                        {                                                                 
                            // PPU                                                        
                            for(x = 19; x < 24 ; x++ )
                            {
                                ppuiButtonA[x - 19] = PRF_rxBuffer[x];                            
                            } 
                       
                            for(x = 0; x < 5 ; x++ )
                            {
                                if(ppuiButtonA[x] == 0x00)
                                { 
                                    ppuiButtonA[x] = '0';
                                }                                
                            }
                            
                            //Grade
                            side.a.grade = PRF_rxBuffer[18];
                            
                            buffer_B[0]  = 0xBC;
                            buffer_B[1]  = 0xCB;
                            buffer_B[2]  = 0xC8;
                            buffer_B[3]  = IDCast[0];
                            buffer_B[4]  = IDCast[1];
                            buffer_B[5]  = side.a.dir;
                            buffer_B[6]  = 0xA7;
                            buffer_B[7]  = RF_IDLE;
                            buffer_B[8]  = ACK;
                            buffer_B[9]  = buffer_B[36] = verificar_check(buffer_B, 9);
                            
                            for (x = 0; x < 10; x++)
                            { 
                                RF_Connection_PutChar(buffer_B[x]);
                            }
                            
                            Credit_Auth_OK = 1;
                            bufferAready = 0; 
                            PresetFlag = 1;
                            
                        }
                        else
                        {                           
                            PresetFlag = 0;
                            iButtonFlag = 0;                           
                            AckFlag = 1;
                            CreditAuth = 0;
                            Credit_Auth_OK = 0;
                            bufferAready = 0;
                            buffer_B[0]  = 0xBC;
                            buffer_B[1]  = 0xCB;
                            buffer_B[2]  = 0xC8;
                            buffer_B[3]  = IDCast[0];
                            buffer_B[4]  = IDCast[1];
                            buffer_B[5]  = side.a.dir;
                            buffer_B[6]  = 0xA7;
                            buffer_B[7]  = RF_IDLE;
                            buffer_B[8]  = ACK;
                            buffer_B[9]  = buffer_B[36] = verificar_check(buffer_B, 9);
                            
                            for (x = 0; x < 10; x++)
                            { 
                                RF_Connection_PutChar(buffer_B[x]);
                            }
                            flowDisplay1 = 21;
                                                                           
                        }
                    } 
                    
                    if(PRF_rxBuffer[5] == side.b.dir)
                    {
                        if(PRF_rxBuffer[8] == 0x01)
                        {                           
                            // PPU
                            for(x = 19; x < 24 ; x++ )
                            {
                                ppuiButtonB[x - 19] = PRF_rxBuffer[x];                            
                            }  
                            
                            for(x = 0; x < 5 ; x++ )
                            {
                                if(ppuiButtonB[x] == 0x00)
                                { 
                                    ppuiButtonB[x] = '0';
                                }
                                
                            }
                            //Grade
                            side.b.grade = PRF_rxBuffer[18];
                            
                            buffer_B[0]  = 0xBC;
                            buffer_B[1]  = 0xCB;
                            buffer_B[2]  = 0xC8;
                            buffer_B[3]  = IDCast[0];
                            buffer_B[4]  = IDCast[1];
                            buffer_B[5]  = side.b.dir;
                            buffer_B[6]  = 0xA7;
                            buffer_B[7]  = RF_IDLE;
                            buffer_B[8]  = ACK;
                            buffer_B[9]  = buffer_B[36] = verificar_check(buffer_B, 9);
                            
                            for (x = 0; x < 10; x++)
                            { 
                                RF_Connection_PutChar(buffer_B[x]);
                            }
                           
                            Credit_Auth_OK2 = 1;
                            bufferAreadyB = 0;
                            PresetFlag2 = 1;                     
                                                       
                        }
                        else
                        {
                           
                            PresetFlag2 = 0;
                            iButtonFlag2 = 0;                           
                            AckFlag2 = 1;
                            CreditAuth2 = 0;
                            Credit_Auth_OK2 = 0;
                            bufferAreadyB = 0;
                            buffer_B[0]  = 0xBC;
                            buffer_B[1]  = 0xCB;
                            buffer_B[2]  = 0xC8;
                            buffer_B[3]  = IDCast[0];
                            buffer_B[4]  = IDCast[1];
                            buffer_B[5]  = side.b.dir;
                            buffer_B[6]  = 0xA7;
                            buffer_B[7]  = RF_IDLE;
                            buffer_B[8]  = ACK;
                            buffer_B[9]  = buffer_B[36] = verificar_check(buffer_B, 9);
                            for (x = 0; x < 10; x++)
                            { 
                                RF_Connection_PutChar(buffer_B[x]);
                            }    
                            flowDisplay2 = 21;
                        }
                    }                           
                break;
                  
                             
                case 0xA5:               // Total Request
                    
                    if(PRF_rxBuffer[5] == side.a.dir)
                    {                      
                        pollTotals = 1;                     
                        return;
                    }                
                    
                    if(PRF_rxBuffer[5] == side.b.dir)
                    {                      
                        pollTotals = 2;
                        return;
                    }                 
                 
                break;
                
                case 0xA6:               //PPU configure
                    
                    if(PRF_rxBuffer[5] == side.a.dir)
                    {                        
                        for(x = 9; x < 14 ; x++ )
                        {
                            side.a.ppuAuthorized[PRF_rxBuffer[8]][x - 9] = PRF_rxBuffer[x];
                        }
                        
                        if(PRF_rxBuffer[8] == 0)
                        {
                            EEpromGradeAddress = 20; //Address 20 to 24
                        }
                        
                        if(PRF_rxBuffer[8] == 1)
                        {
                            EEpromGradeAddress = 25; //Address 25 to 29
                        }
                        
                        if(PRF_rxBuffer[8] == 2)
                        {
                            EEpromGradeAddress = 30; //Address 30 to 34
                        }
                        
                        if(PRF_rxBuffer[8] == 3)
                        {
                            EEpromGradeAddress = 35; //Address 35 to 39
                        }
                                                
                        for(x = 0; x < 5 ; x++ )
                        {
                           EEPROM_1_WriteByte(side.a.ppuAuthorized[PRF_rxBuffer[8]][x], EEpromGradeAddress + x); //PPU to EEprom
                        }
                        
                        side.a.changePPU = true;
                        side.a.grade = PRF_rxBuffer[8];
                        buffer_tx[5] = side.a.dir; y++;
                        buffer_tx[6] = 0xA6; y++;
                        buffer_tx[7] = 0x03; y++;
                 
                    }                   
                    if(PRF_rxBuffer[5] == side.b.dir)
                    {                        
                        for(x = 9; x < 14 ; x++ )
                        {
                            side.b.ppuAuthorized[PRF_rxBuffer[8]][x - 9] = PRF_rxBuffer[x];
                        }
                        
                         if(PRF_rxBuffer[8] == 0)
                        {
                            EEpromGradeAddress = 40; //Address 40 to 44
                        }
                        
                        if(PRF_rxBuffer[8] == 1)
                        {
                            EEpromGradeAddress = 45; //Address 45 to 49
                        }
                        
                        if(PRF_rxBuffer[8] == 2)
                        {
                            EEpromGradeAddress = 50; //Address 50 to 54
                        }
                        
                        if(PRF_rxBuffer[8] == 3)
                        {
                            EEpromGradeAddress = 55; //Address 55 to 59
                        }
                        
                        for(x = 0; x < 5 ; x++ )
                        {
                            EEPROM_1_WriteByte(side.b.ppuAuthorized[PRF_rxBuffer[8]][x], EEpromGradeAddress + x); //PPU to EEprom
                        }
                        
                        side.b.changePPU = true;
                        side.b.grade = PRF_rxBuffer[8];
                        buffer_tx[5] = side.b.dir; y++;
                        buffer_tx[6] = 0xA6; y++;
                        buffer_tx[7] = 0x03; y++;
                       
                    }                     
                    if(PRF_rxBuffer[5] == side.c.dir)
                    {                        
                        for(x = 9; x < 14 ; x++ )
                        {
                            side.c.ppuAuthorized[PRF_rxBuffer[8]][x - 9] = PRF_rxBuffer[x];
                        }
                        
                        side.c.changePPU = true;
                        side.c.grade = PRF_rxBuffer[8];
                        buffer_tx[5] = side.c.dir; y++;
                        buffer_tx[y] = 0xA6;y++;
                        buffer_tx[y] = 0x03;y++;
                    } 
                    if(PRF_rxBuffer[5] == side.d.dir)
                    {                        
                        for(x = 9; x < 14 ; x++ )
                        {
                            side.d.ppuAuthorized[PRF_rxBuffer[8]][x - 9] = PRF_rxBuffer[x];
                        }
                        side.d.changePPU = true;
                        side.d.grade = PRF_rxBuffer[8];
                        buffer_tx[5] = side.d.dir; y++;
                        buffer_tx[6] = 0xA6;y++;
                        buffer_tx[7] = 0x03;y++;
                    }
                    
                    buffer_tx[8] = verificar_check(buffer_tx, y + 1); y++;
                                      
                    for (x = 0; x < 9; x++)
                    {
                        PPUAux =   buffer_tx[x];                   
                        RF_Connection_PutChar(PPUAux);
                    }
                    return;
                                                       
                break;
                
                case 0xA7:               //Impresion general
                    //lengthPrint = hexadecimal_to_decimal(PRF_rxBuffer[8]);
                    if(PRF_rxBuffer[5] == side.a.dir)

                    {                                                
                        buffer_tx[5] = side.a.dir;
                        buffer_tx[6] = 0xA7;
                        buffer_tx[7] = side.a.rfState;
                        buffer_tx[8] = 0x03;
                    }  
                    
                    if(PRF_rxBuffer[5] == side.b.dir)
                    {
                        buffer_tx[5] = side.b.dir;
                        buffer_tx[6] = 0xA7;
                        buffer_tx[7] = side.b.rfState;
                        buffer_tx[8] = 0x03;
                    }
                    
                    if(PRF_rxBuffer[5] == side.c.dir)
                    {
                        buffer_tx[5] = side.c.dir;
                        buffer_tx[6] = 0xA7;
                        buffer_tx[7] = side.c.rfState;
                        buffer_tx[8] = 0x03;
                    }
                    
                    if(PRF_rxBuffer[5] == side.d.dir)
                    {
                        buffer_tx[5] = side.d.dir;
                        buffer_tx[6] = 0xA7;
                        buffer_tx[7] = side.d.rfState;
                        buffer_tx[8] = 0x03;
                    }
                    
                    buffer_tx[8] = verificar_check(buffer_tx,9);

                    if(PRF_rxBuffer[5] == side.a.dir || PRF_rxBuffer[5] == side.c.dir)
                    {                                        
                        for(x = 9; x <= PRF_rxBuffer[8] + 8;x++)
                        {                            
                            write_psoc1(printPortA[0], PRF_rxBuffer[x]);
                        }
                        write_psoc1(printPortA[0],0x0A);
                    }
                    if(PRF_rxBuffer[5] == side.b.dir || PRF_rxBuffer[5] == side.d.dir)
                    {                                        
                        for(x = 9; x <= PRF_rxBuffer[8] + 8;x++)
                        {                            
                            write_psoc1(printPortB[0], PRF_rxBuffer[x]);
                        }
                        write_psoc1(printPortB[0],0x0A);
                    }
                    for (x = 0; x < 9; x++)
                    {
                        RF_Connection_PutChar(buffer_tx[x]);
                    }         

                break;
                
                case 0xA8:          //Print credit sale
                    
                    if(PRF_rxBuffer[5] == side.a.dir)
                    {   
                        // Id transaction
                        for(x = 0; x < 10; x++)
                        {
                            side.a.saleNumber[x] = PRF_rxBuffer[x + 8];
                        }   

                        //Licese plate
                        for(x = 0; x < 8; x++)
                        {
                            LicensePlate[x] = PRF_rxBuffer[x + 18]; 
                        }                                                  

                        //Balance
                        for(x = 0; x < 10; x++)
                        {
                            BalanceA[x] = PRF_rxBuffer[x + 26]; 
                        }                
                            
                        //Company
                        for(x = 0; x < 20; x++)
                        {
                            Company[x] = PRF_rxBuffer[x + 36]; 
                        }   

                        //Count ID
                        for(x = 0; x < 20; x++)
                        {
                            CountID[x] = PRF_rxBuffer[x + 36]; 
                        }  

                        DayVisit = PRF_rxBuffer[76] + 0x30;
                        WeekVisit = PRF_rxBuffer[77] + 0x30;
                        MonthVisit = PRF_rxBuffer[78] + 0x30;

                        //Volume for day
                        for(x = 0; x < 7; x++)
                        {
                            VolumeDay[x] = PRF_rxBuffer[x + 79]; 
                        } 

                        //Volume for Week
                        for(x = 0; x < 7; x++)
                        {
                            VolumeWeek[x] = PRF_rxBuffer[x + 86]; 
                        }
                         
                         //Volume for Month
                        for(x = 0; x < 7; x++)
                        {
                            VolumeMonth[x] = PRF_rxBuffer[x + 93]; 
                        }
                       

                        //Count type
                        for(x = 0; x < 16; x++)
                        {
                            CountType[x] = PRF_rxBuffer[x + 100]; 
                        }
                        
                        ControlType = PRF_rxBuffer[116];

                        return;

                    }

                    if(PRF_rxBuffer[5] == side.b.dir)
                    {
                        // Id transaction
                        for(x = 0; x < 10; x++)
                        {
                            side.b.saleNumber[x] = PRF_rxBuffer[x + 8];
                        }   

                        //Licese plate
                        for(x = 0; x < 8; x++)
                        {
                            LicensePlate[x] = PRF_rxBuffer[x + 18]; 
                        }                                                  

                        //Balance
                        for(x = 0; x < 10; x++)
                        {
                            BalanceA[x] = PRF_rxBuffer[x + 26]; 
                        }                
                            
                        //Company
                        for(x = 0; x < 20; x++)
                        {
                            Company[x] = PRF_rxBuffer[x + 36]; 
                        }   

                        //Count ID
                        for(x = 0; x < 20; x++)
                        {
                            CountID[x] = PRF_rxBuffer[x + 36]; 
                        }  

                        DayVisit = PRF_rxBuffer[76] + 0x30;
                        WeekVisit = PRF_rxBuffer[77] + 0x30;
                        MonthVisit = PRF_rxBuffer[78] + 0x30;

                        //Volume for day
                        for(x = 0; x < 7; x++)
                        {
                            VolumeDay[x] = PRF_rxBuffer[x + 79]; 
                        } 

                        //Volume for Week
                        for(x = 0; x < 7; x++)
                        {
                            VolumeWeek[x] = PRF_rxBuffer[x + 86]; 
                        }
                         
                         //Volume for Month
                        for(x = 0; x < 7; x++)
                        {
                            VolumeMonth[x] = PRF_rxBuffer[x + 93]; 
                        }
                       

                        //Count type
                        for(x = 0; x < 16; x++)
                        {
                            CountType[x] = PRF_rxBuffer[x + 100]; 
                        }
                        
                        ControlType = PRF_rxBuffer[116];
                        
                                            
                        return;
                    }

                break;

                case 0xA9:              // ID Transaction
                    if(PRF_rxBuffer[5] == side.a.dir)
                    {
                        for(x = 0; x < 10; x++)
                        {
                            side.a.saleNumber[x] = PRF_rxBuffer[x+8];
                        }                      
                    }
                    if(PRF_rxBuffer[5] == side.b.dir)
                    {
                        for(x = 0; x < 10; x++)
                        {
                            side.b.saleNumber[x] = PRF_rxBuffer[x+8];
                        }
                    }
                    if(PRF_rxBuffer[5] == side.c.dir)
                    {
                        for(x = 0; x < 10; x++)
                        {
                            side.c.saleNumber[x] = PRF_rxBuffer[x + 8];
                        }
                    }
                    if(PRF_rxBuffer[5] == side.d.dir)
                    {
                        for(x = 0; x < 10; x++)
                        {
                            side.d.saleNumber[x] = PRF_rxBuffer[x + 8];
                        }
                    }
                break;
                
                             
                case 0xE1:               //Configuracion de la estacion
                    symbols[1] = PRF_rxBuffer[8];
                    date[0]    = PRF_rxBuffer[9];
                    date[1]    = PRF_rxBuffer[10];
                    date[2]    = PRF_rxBuffer[11];
                    time[1]    = PRF_rxBuffer[12];
                    time[0]    = PRF_rxBuffer[13];
                    
                    for(x = 17; x < 47; x++)
                    {
                        Encabezado1[x-17] = PRF_rxBuffer[x];
                    }
                    for(x = 47; x < 77; x++)
                    {
                        Encabezado2[x-47] = PRF_rxBuffer[x];
                    }
                    for(x = 77; x < 107; x++)
                    {
                        Encabezado3[x-77] = PRF_rxBuffer[x];
                    }
                    for(x = 107; x < 137; x++)
                    {
                        Encabezado4[x-107] = PRF_rxBuffer[x];
                    }
                    for(x = 137; x < 167; x++)
                    {
                        Encabezado5[x-137] = PRF_rxBuffer[x];
                    }
                    for(x = 167; x < 197; x++)
                    {
                        Pie1[x-167] = PRF_rxBuffer[x];
                    }
                    for(x = 197; x < 227; x++)
                    {
                        Pie2[x-227] = PRF_rxBuffer[x];
                    }
                    for(x = 227; x < 257; x++)
                    {
                        Pie3[x-227] = PRF_rxBuffer[x];
                    }
                    for(x = 257; x < 273; x++)
                    {
                        Product1[x-257] = PRF_rxBuffer[x];
                    }
                    for(x = 273; x < 289; x++)
                    {
                        Product2[x-273] = PRF_rxBuffer[x];
                    }
                    for(x = 289; x < 305; x++)
                    {
                        Product3[x-289] = PRF_rxBuffer[x];
                    }
                    for(x = 305; x < 321; x++)
                    {
                        Product4[x-305] = PRF_rxBuffer[x];
                    }
                    CopiasCredito = PRF_rxBuffer[16];
                    write_hora();
                    write_fecha(); 
                    WriteEeprom(11,Encabezado1);
                    WriteEeprom(41,Encabezado2);
                    WriteEeprom(71,Encabezado3);
                    WriteEeprom(101,Encabezado4);
                    WriteEeprom(131,Encabezado5);
                    WriteEeprom(161,Pie1);
                    WriteEeprom(191,Pie2);
                    WriteEeprom(221,Pie3);
                    WriteEeprom(251,Product1);
                    WriteEeprom(267,Product2);
                    WriteEeprom(283,Product3);
                    WriteEeprom(299,Product4);
                break;
               
                case 0xE2:               //Configuracion de la posicion                                                 
                    if(PRF_rxBuffer[5] == side.a.dir)
                    {
                        for(x = 8; x < 12; x++){
                            side.a.GradesHose[x-8] = PRF_rxBuffer[x];
                        }
                    }
                    if(PRF_rxBuffer[5] == side.b.dir)
                    {
                        for(x = 8; x < 12; x++){
                            side.b.GradesHose[x-8] = PRF_rxBuffer[x];
                        }
                    }
                    if(PRF_rxBuffer[5] == side.c.dir)
                    {
                        for(x = 8; x < 12; x++){
                            side.c.GradesHose[x-8] = PRF_rxBuffer[x];
                        }
                    }
                    if(PRF_rxBuffer[5] == side.d.dir)
                    {
                        for(x = 8; x < 12; x++){
                            side.c.GradesHose[x-8] = PRF_rxBuffer[x];
                        }
                    }
                    lockTurn[0] = PRF_rxBuffer[12];
                    WriteEeprom(6,lockTurn);

//                    y = 60;
//                    for(x = y; x< y+5; x++)
//                    {
//                        EEPROM_1_WriteByte(side.a.GradesHose[x-60],x);
//                        y++;
//                    }
//                    for(x = y; x< y+5; x++)
//                    {
//                        EEPROM_1_WriteByte(side.b.GradesHose[x-50],x);
//                        y++;
//                    }
//                    for(x = y; x< y+5; x++)
//                    {
//                        EEPROM_1_WriteByte(side.c.GradesHose[x-50],x);
//                        y++;
//                    }
//                    for(x = y; x< y+5; x++)
//                    {
//                        EEPROM_1_WriteByte(side.d.GradesHose[x-50],x);
//                        y++;
//                    }
                break;
                
                case 0xE4:               //Turno  
                    if(PRF_rxBuffer[5] == side.a.dir && ShiftDone == 0)
                    {
                        ShiftDone = 1;
                        if(PRF_rxBuffer[7] == 0x02)
                        {                        
                            SetPicture(1,DISPLAY_PASSWORD_INVALIDO);
                            vTaskDelay( 900 / portTICK_PERIOD_MS );
                            flowDisplay1 = 0;
                            ShiftState = 0;
                            side.a.rfState = RF_IDLE;
                            side.a.RFstateReport = 0;
                        }
                        else
                        {
                            SetPicture(1,DISPLAY_ID_RECONOCIDO);
                            vTaskDelay( 900 / portTICK_PERIOD_MS );
                            ShiftState = 0;
                            lockTurn[0] = PRF_rxBuffer[7];
                            WriteEeprom(6,lockTurn);
                            flowDisplay1 = 0;
                            SetPicture(1,DISPLAY_INICIO0);
                            side.a.RFstateReport = 0;
                            side.a.rfState = RF_IDLE; 
                            return;
                        }
                    }    
               break;                
           }                       
        } 

    RF_Connection_ClearRxBuffer(); 
}


void pollingRFA_Tx(){   
    uint16 x,y;
    
    /////////////// TICKET COPY //////////////////
    if(side.a.rfState == RF_COPY_RECEIPT && side.a.RFstateReport == 1){ 
        buffer_A[0]   = 11;
		buffer_A[1]   = 0xBC;                                 // Encabezado byte 1
        buffer_A[2]   = 0xCB;                                 // Encabezado byte 2
        buffer_A[3]   = 0xC8;                                 // Encabezado byte 3
        buffer_A[4]   = IDCast[0];                            // ID estacion byte 1
        buffer_A[5]   = IDCast[1];                            // ID estacion byte 2
        buffer_A[6]   = side.a.dir;                           // ID posicion 
        buffer_A[7]   = 0xE6;
        buffer_A[8]   = RF_COPY_RECEIPT;
        buffer_A[9]   = verificar_check(buffer_A,10);        
		side.a.RFstateReport = 0;
        side.a.rfState = RF_IDLE;
        bufferAready = 1;
        if(PrinterType == 1)
        {
            printLogoP(printPortA[0],11);    
        }       
        write_psoc1(printPortA[0],10);
    }
    ////////////// PRESET - BUSY ////////////////////////////////////
    if(side.a.pumpState == PUMP_BUSY && side.a.RFstateReport == 1)
    {
        buffer_A[0]   = 40;
		buffer_A[1]   = 0xBC;                                 // Encabezado byte 1
        buffer_A[2]   = 0xCB;                                 // Encabezado byte 2
        buffer_A[3]   = 0xC8;                                 // Encabezado byte 3
        buffer_A[4]   = IDCast[0];                            // ID estacion byte 1
        buffer_A[5]   = IDCast[1];                            // ID estacion byte 2
        buffer_A[6]   = side.a.dir;                           // ID posicion 
        buffer_A[7]   = 0xAA;                                 // Comando   
        buffer_A[8]   = RF_DELIVERING;                        // Estado de la posicion
        buffer_A[9]   = side.a.activeHose;                    
        buffer_A[10]  = bufferDisplay1.presetType[0] & 0x03; 
        
        for(x = bufferDisplay1.presetValue[0][0] ; x > 0; x--)
        {
            tempPreset[bufferDisplay1.presetValue[0][0]-x] = (bufferDisplay1.presetValue[0][x]); 
        }
        
        for(x = 11; x <= 18; x++)
        {
            buffer_A[x] = tempPreset[x - 11];
            
            if(buffer_A[x]== 0x00)
            {
                buffer_A[x] = 0x30;
            }
        }        
        buffer_A[19]  = 0x00;
        if(bufferDisplay1.flagPrint == 0)
        {
            for(x = 0; x < 8 ; x++)
            {
                buffer_A[20 + x] = 0x20;
                tempPreset[x] = 0x00;
            }
        }else
        {
            for(x = 0; x < 8 ; x++)
            {
                buffer_A[20 + x] = bufferDisplay1.licenceSale[x + 1];
                tempPreset[x] = 0x00;
            }
        }
        for(x = 28; x <= 38; x++)
        {
            buffer_A[x] = 0x30;
        }
        buffer_A[39] = verificar_check(buffer_A, 40);                                
        side.a.rfState = RF_DELIVERING;     
        side.a.RFstateReport = 0;
        bufferAready = 1;
        FlagTotal = 0;
    }            
    ////////////// END SALE ////////////////////////////////////
    if((side.a.pumpState == PUMP_PEOT || side.a.pumpState == PUMP_FEOT) && side.a.RFstateReport == 1)//bufferDisplay1.flagEndSale == true  
    {        
        buffer_A[0]  = 32;
		buffer_A[1]  = 0xBC;
        buffer_A[2]  = 0xCB;
        buffer_A[3]  = 0xC8;
        buffer_A[4]  = IDCast[0];
        buffer_A[5]  = IDCast[1];
        buffer_A[6]  = side.a.dir;
        buffer_A[7]  = 0xA4;
        buffer_A[8]  = RF_CASHSALEREPORT;
        buffer_A[9]  = side.a.activeHose - 1;
        
        for(x = side.a.ppuSale[0]; x > 0 ; x--)
        {						   							
    		tempPPU[side.a.ppuSale[0] - x] = side.a.ppuSale[x];
    	}
        
        for(x = 10; x <= 14; x++)
        {
            buffer_A[x] = tempPPU[x - 10]; 
            if(buffer_A[x] == 0x00)
                buffer_A[x] = 0x30;
        } 
        if(digits == 5 || digits == 6)
        {
            for(x = 1; x <= side.a.volumeSale[0]; x++)
            {						   							
                buffer_A[x + 14] = side.a.volumeSale[7 - x];                
        	}
        }else
        {
            for(x = 1; x <= side.a.volumeSale[0]; x++)
            {						   							
                buffer_A[x + 14] = side.a.volumeSale[9 - x];                
        	}
        }        
        if(buffer_A[21] == 0x00)
        {
            buffer_A[21] = 0x30;
            buffer_A[22] = 0x30;
        }
        if(digits == 5 || digits == 6)
        {
            for(x = 1; x <= side.a.moneySale[0]; x++)
            {						   							
        		buffer_A[x + 22] = side.a.moneySale[7 - x];                
        	} 
        }else
        {
            for(x = 1; x <= side.a.moneySale[0]; x++)
            {						   							
        		buffer_A[x + 22] = side.a.moneySale[9 - x];                
        	}            
        }
        if(buffer_A[29] == 0x00)
        {
            buffer_A[29] = 0x30;
            buffer_A[30] = 0x30;
        }
        buffer_A[31]                    = verificar_check(buffer_A,32);                          
        side.a.pumpState                = PUMP_IDLE;
        side.a.rfState                  = RF_IDLE;
        bufferDisplay1.flagActiveSale   = false;
        side.a.RFstateReport            = 0;
        pollTotals                      = 1;
        bufferAready                    = 1;
        FlagTotal                       = 0;
       
    }                                               
    ////////////// SHIFT ////////////////////////////////////
    if(ShiftState == 1  && side.a.RFstateReport == 1){   
        for(x = 0; x < 100; x++){
            buffer_A[x] = 0x00;
        }
        buffer_A[0]  = 35;
        buffer_A[1]  = 0xBC;
        buffer_A[2]  = 0xCB;
        buffer_A[3]  = 0xC8;
        buffer_A[4]  = IDCast[0];
        buffer_A[5]  = IDCast[1];
        buffer_A[6]  = side.a.dir;
        buffer_A[7]  = 0xE3;
        buffer_A[8]  = RF_WORKSHIFTREQ;
        
//        buffer_txDisplay[0]  = 0xBC;
//        buffer_txDisplay[1]  = 0xCB;
//        buffer_txDisplay[2]  = 0xC8;
//        buffer_txDisplay[3]  = IDCast[0];
//        buffer_txDisplay[4]  = IDCast[1];
//        buffer_txDisplay[5]  = side.a.dir;
//        buffer_txDisplay[6]  = 0xE3;
//        buffer_txDisplay[7]  = RF_WORKSHIFTREQ;
        
//        for(x = bufferDisplay1.shiftId[0]; x >= 1; x--)
//        {						   							
//    		buffer_txDisplay[x+7]= bufferDisplay1.shiftId[x];            
//    	}
        for(x = bufferDisplay1.shiftId[0]; x >= 1; x--)
        {						   							
    		buffer_A[x+8]= bufferDisplay1.shiftId[x];            
    	}
//        for(x=8; x<19; x++){
//            if(buffer_txDisplay[x] ==0x00)
//                buffer_txDisplay[x]=0x30;
//        }
        for(x = 1; x <= bufferDisplay1.shiftPassword[0] ;x++)
        {						   							
    		buffer_A[x + 19] = bufferDisplay1.shiftPassword[(bufferDisplay1.shiftPassword[0] + 1) - x];            
    	}
//        for(x = 1; x <= bufferDisplay1.shiftPassword[0] ;x++)
//        {						   							
//    		buffer_txDisplay[x + 18] = bufferDisplay1.shiftPassword[(bufferDisplay1.shiftPassword[0] + 1) - x];            
//    	}
//        for(x=19; x<31; x++){
//            if(buffer_txDisplay[x] ==0x00)
//                buffer_txDisplay[x]=0x30;
//        }
        buffer_A[33] = verificar_check(buffer_txDisplay,34);
        
//        for (x = 0; x < 33; x++)
//        {
//            RF_Connection_PutChar(buffer_txDisplay[x]);
//        }
        
        //side.a.rfState = RF_IDLE;
        side.a.RFstateReport = 0;
        ShiftState = 0;
        bufferAready = 1;
        ShiftDone = 0;
    }

    ////////////// AUTHORIZATION REQUEST ////////////////////////
    if(side.a.pumpState == PUMP_CALLING && side.a.RFstateReport == 1 && CreditAuth == RF_CREDITSALEAUTH)
    {   
		buffer_A[0]  = 0xBC;
        buffer_A[1]  = 0xCB;
        buffer_A[2]  = 0xC8;
        buffer_A[3]  = IDCast[0];
        buffer_A[4]  = IDCast[1];
        buffer_A[5]  = side.a.dir;
        buffer_A[6]  = 0xA2;
        buffer_A[7]  = RF_CREDITSALEAUTH;
        buffer_A[8]  = side.a.hose - 1;
                                
        //iButton id
        for(y = 1; y <= 16; y++)
        {           
            if(bufferDisplay1.idSerial[y] < 0x3A)
            {
                Temp[y] = bufferDisplay1.idSerial[y] - 48;
            }
            else
            {
                Temp[y] = bufferDisplay1.idSerial[y] - 55;
            }
                                   
        }
        x = 0;      
        for(y = 1; y < 17; y+=2)
        {
            buffer_A[9 + x] =  ((Temp[y] & 0x0F) << 4) | (Temp[y + 1] & 0x0F);
            x++;
        }
       
        
        //Km (10 bytes)
        for(y = 0; y < 10; y++)
        {
            buffer_A[17 + y] = 0x00;
        }
        
        x = 0;
        
        for(y = bufferDisplay1.mileageSale[0]; y >= 1; y--)
        {           
            buffer_A[17 + x] = bufferDisplay1.mileageSale[y]; 
            x++;
        }
        
        // Preset Type
        buffer_A[27] =  bufferDisplay1.presetType[0];         
       
        //Preset (8 bytes)    
        for(y = 0; y < 8; y++)
        {
            buffer_A[28 + y] = 0x00;
        }
        x = 0;
        
        for(y = 8; y >= 1; y--)
        {
            buffer_A[28 + x] = bufferDisplay1.presetValue[0][y];
            x++;
        }       
        
        // CRC
        buffer_A[36] = verificar_check(buffer_A,37);    
        side.a.RFstateReport = 0;
        bufferAready = 2;
        FlagTotal = 0;
        side.a.rfState = RF_IDLE;
        CreditAuth = 0;
        AckFlag = 0;
    }
     
}

void pollingRFB_Tx(){   
    uint16 x,y;      
    /////////////// COPIA DE RECIBO //////////////////
    if(side.b.rfState == RF_COPY_RECEIPT && side.b.RFstateReport == 1){ 
        buffer_B[100]   = 11;
		buffer_B[0]   = 0xBC;                                 // Encabezado byte 1
        buffer_B[1]   = 0xCB;                                 // Encabezado byte 2
        buffer_B[2]   = 0xC8;                                 // Encabezado byte 3
        buffer_B[3]   = IDCast[0];                            // ID estacion byte 1
        buffer_B[4]   = IDCast[1];                            // ID estacion byte 2
        buffer_B[5]   = side.b.dir;                           // ID posicion 
        buffer_B[6]   = 0xE6;
        buffer_B[7]   = RF_COPY_RECEIPT;
        buffer_B[8]   = verificar_check(buffer_B,9);        
		side.b.RFstateReport = 0;
        side.b.rfState = RF_IDLE;
        bufferAreadyB = 1;
        if(PrinterType == 1)
        {
            printLogoP(printPortB[0],11);    
        }       
        write_psoc1(printPortB[0],10);
    }    
    ////////////// PRESET - DISPENSANDO ////////////////////////////////////
    if(side.b.pumpState == PUMP_BUSY && side.b.RFstateReport == 1)
    {
            buffer_B[100]   = 40;
            buffer_B[0]     = 0xBC;
            buffer_B[1]     = 0xCB;
            buffer_B[2]     = 0xC8;
            buffer_B[3]     = IDCast[0];
            buffer_B[4]     = IDCast[1];
            buffer_B[5]     = side.b.dir;
            buffer_B[6]     = 0xAA;
            buffer_B[7]     = RF_DELIVERING;
            buffer_B[8]     = side.b.activeHose;
            buffer_B[9]     = bufferDisplay2.presetType[0] & 0x03;
            
            for(x = bufferDisplay2.presetValue[0][0] ; x > 0; x--)
            {
                tempPreset[bufferDisplay2.presetValue[0][0] - x] = (bufferDisplay2.presetValue[0][x]); 
            }
            for(x = 10; x <= 17; x++)
            {
                buffer_B[x] = tempPreset[x - 10]; 
                if(buffer_B[x]== 0x00)
                    buffer_B[x] = 0x30;
            }        
            buffer_B[18]  = 0x00;
            if(bufferDisplay2.flagPrint == 0)
            {
                for(x = 0; x <8 ; x++)
                {
                    buffer_B[19+x]=0x20;
                    tempPreset[x] = 0x00;
                }
            }else
            {
                for(x = 0; x <8 ; x++)
                {
                    buffer_B[19+x]=bufferDisplay2.licenceSale[x+1];
                    tempPreset[x] = 0x00;
                }
            }
            for(x=27; x<=37;x++)
            {
                buffer_B[x]= 0x30;
            }
            
            buffer_B[38] = verificar_check(buffer_B,39);            
            //for (x = 0; x < 40; x++){
            //    RF_Connection_PutChar(buffer_B[x]);
            //}            
            side.b.rfState       = RF_DELIVERING;
            side.b.RFstateReport = 0;
            bufferAreadyB        = 1;
            FlagTotalB           = 0;
            
    }      
    ////////////// END SALE ////////////////////////////////////
 
    if ((side.b.pumpState == PUMP_PEOT || side.b.pumpState == PUMP_FEOT) && side.b.RFstateReport == 1)    
    {
        buffer_B[100] = 31;
        buffer_B[0]  = 0xBC;
        buffer_B[1]  = 0xCB;
        buffer_B[2]  = 0xC8;
        buffer_B[3]  = IDCast[0];
        buffer_B[4]  = IDCast[1];
        buffer_B[5]  = side.b.dir;
        buffer_B[6]  = 0xA4;
        buffer_B[7]  = RF_CASHSALEREPORT;
        buffer_B[8]  = side.b.activeHose - 1;
        
        for(x = side.b.ppuSale[0]; x > 0 ; x--)
        {						   							
    		tempPPU[side.b.ppuSale[0] - x] = side.b.ppuSale[x];
    	}
        
        for(x = 9; x <= 13; x++)
        {
            buffer_B[x] = tempPPU[x - 9]; 
            if(buffer_B[x] == 0x00)
                buffer_B[x] = 0x30;
        } 
        
        if(digits == 5 || digits == 6)
        {
            for(x = 1; x <= side.b.volumeSale[0]; x++)
            {						   							
                buffer_B[x + 13] = side.b.volumeSale[7 - x];                
        	}
        }else
        {
            for(x = 1; x <= side.b.volumeSale[0]; x++)
            {						   							
                buffer_B[x + 13] = side.b.volumeSale[9 - x];                
        	}
        }  
        
        if(buffer_B[20] == 0x00)
        {
            buffer_B[20] = 0x30;
            buffer_B[21] = 0x30;
        }
        
        if(digits == 5 || digits == 6)
        {
            for(x = 1; x <= side.b.moneySale[0]; x++)
            {						   							
        		buffer_B[x + 21] = side.b.moneySale[7 - x];                
        	} 
        }else
        {
            for(x = 1; x <= side.b.moneySale[0]; x++)
            {						   							
        		buffer_B[x + 21] = side.b.moneySale[9 - x];                
        	}            
        }
        
        if(buffer_B[28] == 0x00)
        {
            buffer_B[28] = 0x30;
            buffer_B[29] = 0x30;
        }
        
        buffer_B[30]              = verificar_check(buffer_B, 31);        
        side.b.pumpState              = PUMP_IDLE;
        side.b.rfState                = RF_IDLE;
        bufferDisplay2.flagActiveSale = false;
        side.b.RFstateReport          = 0;
        pollTotals                    = 2;        
        bufferAreadyB                 = 2;
        FlagTotalB                    = 0;    
        
    }                                               
    ////////////// SHIFT ////////////////////////////////////
    if(ShiftState == 1  && side.b.RFstateReport == 1){                                    
        buffer_txDisplay[5]  = side.b.dir;
        buffer_txDisplay[6]  = 0xE3;
        buffer_txDisplay[7]  = RF_WORKSHIFTREQ;
        
        for(x = 1; x <= bufferDisplay2.shiftId[0] ;x++)
        {						   							
    		buffer_txDisplay[x+7] = bufferDisplay2.shiftId[(bufferDisplay2.shiftId[0] + 1) - x];            
    	}
        for(x = 8; x < 19; x++)
        {
            if(buffer_txDisplay[x] == 0x00)
            {
                buffer_txDisplay[x] = 0x30;
            }
        }
        for(x = 1; x <= bufferDisplay2.shiftPassword[0] ;x++)
        {						   							
    		buffer_txDisplay[x+18] = bufferDisplay2.shiftPassword[(bufferDisplay2.shiftPassword[0] + 1) - x];            
    	}
        for(x = 19; x < 31; x++)
        {
            if(buffer_txDisplay[x] == 0x00)
            {
                buffer_txDisplay[x] = 0x30;
            }
        }
        
        buffer_txDisplay[32] = verificar_check(buffer_txDisplay,33);
        
        for (x = 0; x < 33; x++)
        {
            RF_Connection_PutChar(buffer_txDisplay[x]);
        }
        
        side.b.rfState = RF_WORKSHIFTREQ;
        side.b.RFstateReport = 0;
        ShiftState = 0;
    }
    
    ////////////// AUTHORIZATION REQUEST ////////////////////////
    if(side.b.pumpState == PUMP_CALLING && side.b.RFstateReport == 1 && CreditAuth2 == RF_CREDITSALEAUTH)
    {   
        buffer_B[0]  = 0xBC;
        buffer_B[1]  = 0xCB;
        buffer_B[2]  = 0xC8;
        buffer_B[3]  = IDCast[0];
        buffer_B[4]  = IDCast[1];
        buffer_B[5]  = side.b.dir;
        buffer_B[6]  = 0xA2;
        buffer_B[7]  = RF_CREDITSALEAUTH;
        buffer_B[8]  = side.b.hose - 1;
                             
        //iButton id
        for(y = 1; y <= 16; y++)
        {           
            if(bufferDisplay2.idSerial[y] < 0x3A)
            {
                Temp[y] = bufferDisplay2.idSerial[y] - 48;
            }
            else
            {
                Temp[y] = bufferDisplay2.idSerial[y] - 55;
            }                                
        }
        
        x = 0;
        
        for(y = 1; y < 17; y+=2)
        {
            buffer_B[9 + x] =  ((Temp[y] & 0x0F) << 4) | (Temp[y + 1] & 0x0F);
            x++;
        }
        
        //Km
        for(y = 0; y < 10; y++)
        {
            buffer_B[17 + y] = 0x00;
        }
        
        x = 0;
        
        for(y = bufferDisplay2.mileageSale[0]; y >= 1; y--)
        {           
            buffer_B[17 + x] = bufferDisplay2.mileageSale[y]; 
            x++;
        }
        
        // Preset Type
        buffer_B[27] =  bufferDisplay2.presetType[0];         
       
        //Preset (8 bytes)       
        for(y = 0; y < 8; y++)
        {
            buffer_B[28 + y] = 0x00;
        }

        x = 0;
        
        for(y = bufferDisplay2.presetValue[0][0]; y >= 1; y--)
        {
            buffer_B[35 - y] = bufferDisplay2.presetValue[0][y];
            x++;
        }     
 
        // CRC
        buffer_B[36] = verificar_check(buffer_B,37);
        
        side.b.RFstateReport = 0;
        bufferAreadyB = 2;
        FlagTotalB = 0;
        side.b.rfState = RF_IDLE;
        CreditAuth2 = 0;
        AckFlag2 = 0;
    }
  
 }


