//*******************************************************************************
// ----------------------------------------------------------------------------
// The Ohio State University
// ECE 3567 - Microcontrollers Lab
// Spring Semester 2022
// Instructor: Prof. Gregg Chapman
// Group # 34
// Name Tyler Schock
// (MSP430FR6989 Launchpad) - Texas Instruments Corporation
// Code Composer Studio version 9.0.1
// (Filename) main.c or .h
// This module contains files necessary to alternate Red and Green LEDs at exactly 1 Hz,
// using a Timer A0 ISR, as part of Lab 10 Part 1
// ----------------------------------------------------------------------------
//*******************************************************************************

//***** Header Files ***********************************************************/
#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"

/******************************** Variables *************************************/
volatile unsigned int ISR_Counter;
volatile unsigned int ISR_Flag;
volatile unsigned int ISR_Flag_10;

int main(void) {
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);                           // Stop the watchdog timer
    PMM_unlockLPM5();                                                   // Disable the GPIO power-on default high-impedance mode
    Init_GPIO();                                                        // Configure GPIO for LEDs
    Init_Timer_A0();                                                    // Set-up TA0 for 100 mSec ISR

    // Configure and start the Watchdog Timer
    WDTCTL = 0x5AAD;
    WDT_A_start(__MSP430_BASEADDRESS_WDT_A__);

    __enable_interrupt();                                               // Enable interrupts

    while(1)                                                            // Main Loop
    {
        if(ISR_Flag == 1)                                               // Single 100 ms ISR
        {
            ISR_Flag = 0;                                               // Reset ISR flag
        }

        if(ISR_Flag_10 == 1)                                            // 10 ISRs = 1 second
        {
            P1OUT ^= BIT0;                                              // Toggle RED LED
            P9OUT ^= BIT7;                                              // Toggle GREEN LED
            ISR_Flag_10 = 0;                                            // Clear 1-second flag
        }
    }
}

/************************ Interrupt Service Routine for Timer A0 ********/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    WDT_A_resetTimer(__MSP430_BASEADDRESS_WDT_A__);                     // Reset Watchdog Timer every 100 ms
    ISR_Flag = 1;                                                       // 100 millisecond ISR
    ISR_Counter++;                                                      // Increment ISR counter

    if(ISR_Counter >= 10)                                               // 10 ISRs = 1 Second
    {
        ISR_Flag_10 = 1;                                                // Set 1-second flag
        ISR_Counter = 0;                                                // Reset counter
    }
    return;
}

/****************************** END OF ISR **********************************/
/**************************** END OF CODE ***********************************/
