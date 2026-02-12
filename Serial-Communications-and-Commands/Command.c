//*******************************************************************************/
// ----------------------------------------------------------------------------
// The Ohio State University
// ECE 3567 - Microcontrollers Lab
// Spring Semester 2025
// Instructor: Prof. Gregg Chapman
// Group # 34
// Name 1 Tyler Schock
// Name 2 Seth Fullenkamp
// Name 3 Abdifatah Ashirow
// (MSP430FR6989 Launchpad) - Texas Instruments Corporation
// Code Composer Studio version 9.1.0
// (main).c or .h
// This module contains files necessary for entire Command Structure part of Lab 5
// ----------------------------------------------------------------------------
/*******************************************************************************/
//***** Header Files **********************************************************
#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"

//***** Global Variables ******************************************************

//***************** Use the UART to R/W from the "Dumb Terminal" *************

void Command_Handler(uint16_t length)
{
    length = Full_Array; //DEFAULT size which means use the full array
    // Read a character from the terminal (we don't validate it's between 1 and 5)
    myUart_readBuf( BACKCHANNEL_UART, (unsigned char *)val, &length );
    myUart_writeBuf( BACKCHANNEL_UART, "", NULL, CRLF );
    myUart_writeBuf( BACKCHANNEL_UART, "Command: ", NULL, CRLF );
    myUart_writeBuf( BACKCHANNEL_UART, (unsigned char *)val,length,CRLF);
    length = 2;
    Command_Flag = 0;
    EUSCI_A_UART_clearInterrupt( EUSCI_A1, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt( EUSCI_A1, EUSCI_A_UART_RECEIVE_INTERRUPT);
    return;
}

//******************************* Decode the Command **************************
void parse_Command()
{
    /*********************** 1) Read in 2 Letter Command *********************/
    if(val[0] == L) /// THIS IS ONLY AN EXAMPLE
    {
        if(val[1] == E )
            Command = 0x0C;
        else if(val[1] == H)
            Command = 0x0F;
        else if (val[1] == R)
            Command = 0x10;
        else if(val[1] == O)
            Command = 0x11;
        else if(val[1] == Y)
            Command = 0x12;
        else if(val[1] == G)
            Command = 0x13;
        else if(val[1] == B)
            Command = 0x14;
        else if(val[1] == V)
            Command = 0x15;
        else if(val[1] == C)
            Command = 0x16;
    }
    else if(val[0] == R)
    {
        if(val[1] == E)
            Command = 0x01;
    }
    else if(val[0] == T)
    {
        if(val[1] == E)
            Command = 0x04;
    }
    else
    {
        Command = NULL;
        myUart_writeBuf( BACKCHANNEL_UART, "UNKNOWN COMMAND ", NULL, CRLF );
        myUart_writeBuf( BACKCHANNEL_UART, "", NULL, CRLF );
        // Ask for the next Command
    }

    /*********************** 2) If there is Data Load up to a 4-digit number, CLEAR BUFFER *********************/
    if(length >=5)
    {
        New_Data = 0;
        New_Data = (val[2]-48)*1000 + (val[3]-48)*100 + (val[4]-48)*10 + (val[5]-48);

        /*********************************** 3) Clear the val[] array ********************************************/
        length = 2;
        val[2]=0;
        val[3]=0;
        val[4]=0;
        val[5]=0;
    }

    /************************************* 4) Act on the Command *************************************************/
    switch(Command)
    {
        case LE: // Enable LED
            LED_Test_Mode = TRUE;
            Temp_Mode = FALSE;
            RC_Voltage_Mode = FALSE;
            clearLCD();
            PWM_null();
            Flash = FALSE;
            LED_Color = No_Color;
            update_RGB();
            break;

        case LH: // Hot! Flash LED Red
            if(LED_Test_Mode == TRUE)
            {
                LED_Color = Red;
                Flash = TRUE;
                update_RGB();
            }
            break;

        case LR: // Red
            if(LED_Test_Mode == TRUE)
            {
                LED_Color = Red;
                Flash = FALSE;
                update_RGB();
            }
            break;

        case LC: // COLD! Flash Purple
            if(LED_Test_Mode == TRUE)
            {
                LED_Color = Violet;
                Flash = TRUE;
                update_RGB();
            }
            break;

        case LO: // Orange
            if(LED_Test_Mode == TRUE)
            {
                LED_Color = Orange;
                Flash = FALSE;
                update_RGB();
            }
            break;

        case LY: // Yellow
            if(LED_Test_Mode == TRUE)
            {
                LED_Color = Yellow;
                Flash = FALSE;
                update_RGB();
            }
            break;

        case LG: // Green
            if(LED_Test_Mode == TRUE)
            {
                LED_Color = Green;
                Flash = FALSE;
                update_RGB();
            }
            break;

        case LB: // Blue
            if(LED_Test_Mode == TRUE)
            {
                LED_Color = Blue;
                Flash = FALSE;
                update_RGB();
            }
            break;

        case LV: // Purple
            if(LED_Test_Mode == TRUE)
            {
                LED_Color = Violet;
                Flash = FALSE;
                update_RGB();
            }
            break;

        case RE: // Display Temp
            Temp_Mode = FALSE;
            RC_Voltage_Mode = TRUE;
            LED_Test_Mode = FALSE;
            clearLCD();
            PWM_null();
            Flash = FALSE;
            LED_Color = No_Color;
            update_RGB();
            volts = 2500;
            displayVolts();
            break;

        case TE: // Display Volts
            Temp_Mode = TRUE;
            RC_Voltage_Mode = FALSE;
            LED_Test_Mode = FALSE;
            clearLCD();
            PWM_null();
            Flash = FALSE;
            LED_Color = No_Color;
            update_RGB();
            deg = 750;
            displayTemp();
            break;

        default:
            break;
    }

    /******************************************** Clear Command and Ask for the next one ********************/
    Command = NULL;
    myUart_writeBuf( BACKCHANNEL_UART, "Please enter the next Command Code: ", NULL, CRLF );
}

/************************************************** END OF CODE *************************************************/
