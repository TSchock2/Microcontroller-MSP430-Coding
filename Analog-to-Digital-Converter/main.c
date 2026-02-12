//*******************************************************************************/
// ----------------------------------------------------------------------------
// The Ohio State University
// ECE 3567 - Microcontrollers Lab
// Spring Semester 2025
// Instructor: Prof. Gregg Chapman
// Group # 34
// Name 1 Tyler Schock
// Name 2
// Name 3 Abdifatah Ashirow
// (MSP430FR6989 Launchpad) - Texas Instruments Corporation
// Code Composer Studio version 9.1.0
// (main).c or .h
// This module contains files necessary for Analog to Digital Converters and Temperature
// Sensor part of Lab 6
// ----------------------------------------------------------------------------
/*******************************************************************************/
//***** Header Files **********************************************************
#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"

//***** Global Variables ******************************************************

// All globals have been moved to 3567.h NO EXTERNS NECESSARY

//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Stop watchdog timer
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PMM_unlockLPM5();

    //*********************The Initializations*************************************/
    Init_LCD();                                 // Display
    Init_GPIO();                                // I/O including LEDs and Pushbuttons
    Init_Timer_A0();
    Init_Clocks();
    Init_PWM();
    Init_RC();
    tempSensorModeInit();
    Init_ADC12();
    // PWM_null();
    __enable_interrupt();

    /****************** Set up Pushbutton to switch between Hardware Modes ***************/

    //**************************** Establish Command Link ***************************/
    Communications_Link();

    //******************* THE MAIN INFINITE LOOP *************************************/
    while(1)
    {
        if(Command_Flag==1)                       // A USCI_A1 ISR occurred and a Command was received
        {
            Command_Handler(length);              // Read / Write
            parse_Command();                      // Decode and execute the Command
        }

        if(ISR_Flag == 1)                         // Every 62.5 milliseconds
        {
            ISR_Flag = 0;
            if(RC_Voltage_Mode == TRUE)           // Calculate RC Voltage which will also call RC_Feedback, and update_RGB
            {
                RC_Voltage();
            }
            else if(Temp_Mode == TRUE)            // Calculate Temperature display on LCD
            {
                tempSensor();
                sam = 0;
            }
            else if(LED_Test_Mode == TRUE)
            {
                update_RGB();
            }
        }

        if(ISR_Flag_10 == 1)                      // New Timer A0 counter for 1 full second
        {
            ISR_Flag_10 = 0;                      // Toggle Red and Green LEDs
            P1OUT ^= BIT0;
            P9OUT ^= BIT7;
            if(Flash == TRUE)                     // Alternate pin directions
            {
                P3DIR ^= 0xC0;
                P2DIR ^= 0x04;
            }
            else                                  // Restore as outputs
            {
                P3DIR |= 0xC0;
                P2DIR |= 0x04;
            }
        }
    }
}

//****************************** END of main.c ******************************************/

//******************************* Interrupt Service Routing for Timer A0 **************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    ISR_Flag = 1;                                 // 62.5 millisecond ISR
    ISR_Counter++;
    if(ISR_Counter>=10)
    {
        ISR_Flag_10 = 1;                          // 1 Second
        ISR_Counter=0;
    }

    return;
}

/******************************** END of ISR ***********************************************/

/******************************** END OF CODE ***********************************************/
