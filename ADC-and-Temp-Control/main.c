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
// This module contains files necessary for Main in Lab 7
// ----------------------------------------------------------------------------
/*******************************************************************************/

// ----------------------------------------------------------------------------
// main.c (for ECE 3567) ('FR6989 Launchpad) - Dr. Gregg J. Chapman Sp2020
// ----------------------------------------------------------------------------

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
    // Stop Watchdog Timer
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);

    // Disable the GPIO power-on default high-impedance mode
    // Activates previously configured port settings
    PMM_unlockLPM5();

    //*********************The Initializations*************************************/
    Init_LCD();                                                  // Display, in myLCD.c
    Init_GPIO();                                                 // I/O including LED, in myGpio.c
    Init_Timer_A0();                                             // 100mSec or 62.5 msec depending on Lab, in myClocks.c
    Init_Clocks();                                               // For UART, in myClocks.c
    Init_PWM();                                                  // For RGB LED, in RGB_LED.c
    // Init_RC();                                                // For RC voltage and feedback, in RCMode.c
    tempSensorModeInit();                                        // For Temperature Mode and feedback, in tempSensorMode.c
    Init_ADC12();                                                // For ADC in Temp_Mode and RC_Voltage_Mode, in ADC12.c
    __enable_interrupt();                                        // Enables all defined interrupts
    LED_Alternate = 0;                                           // Used in LED Flash timing

    //**************************** Establish Command Link ***************************/
    Communications_Link();                                       // Init UARTS and open Command Channel

    //******************* THE MAIN INFINITE LOOP *************************************/
    while(1)
    {
        if(Command_Flag==1)                                      // A USCI_A1 ISR occurred and a Command was received
        {
            Command_Handler(length);                             // Read / Write
            parse_Command();                                     // Decode and execute the Command
        }
        if(ISR_Flag == 1)                                        // Every 62.5 milliseconds for this Lab
        {
            ISR_Flag = 0;
            if(RC_Voltage_Mode == TRUE)                          // RE - Calculate RC Voltage,
            { // which also calls RC_Feedback and update_RGB
                // RC_Voltage();
            }
            else if(Temp_Mode == TRUE)                           // TE - Calculate Temperature display on LCD
            {
                tempSensor();
            }
            else if(LED_Test_Mode == TRUE)
            {
                update_RGB();                                    // LE - Updates LED only
            }
            if(LED_Alternate == TRUE)                            // Test for 1/8 second timing
            {
                if(Flash == TRUE)                                // Alternate pin directions for LED Flash Mode
                {
                    P3DIR ^= 0xC0;
                    P2DIR ^= 0x04;
                }
                else // Restore as outputs for no Flash
                {
                    P3DIR |= 0xC0;
                    P2DIR |= 0x04;                               // Set direction register PWM pins to outputs
                }
            }
        }
        // Toggle Red and Green LEDs on Launchpad
        if(ISR_Flag_16 == 1)                                     // New Timer A0 counter for 1 full second
        { // counts to 16 not 10
            ISR_Flag_16 = 0;                                     // Toggle Red and Green LEDs
            P1OUT ^= BIT0;
            P9OUT ^= BIT7;
        }
    }
}

//********************************* END of main.c ******************************************/
//************************* Interrupt Service Routing for Timer A0 **************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    ISR_Flag = 1;                                                 // 62.5 millisecond ISR for Lab 8
    ISR_Counter++;
    LED_Alternate = !LED_Alternate;                               // Used for 1/8 second Flash timing in main loop
    if(ISR_Counter>=16)
    {
        ISR_Flag_16 = 1;                                          // 1 Second
        ISR_Counter=0;
    }
    return;
}

/******************************** END of ISR ***********************************************/
/******************************** END OF CODE **********************************************/
