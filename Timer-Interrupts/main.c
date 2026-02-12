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
// This module contains files necessary to demonstrate timers and interrupts for blinking LEDs as part of Lab 2
// ----------------------------------------------------------------------------
/*******************************************************************************/

//***** Header Files ***********************************************************/

#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"

/********************* Compiler Directives and Definitions **********************/


/******************************** Variables *************************************/

// Consider placing global variables in a header file, e.g. ECE3567.h

volatile unsigned int ISR_Counter; // Used to count to 10 in order to delay exactly 1 second
volatile unsigned char ISR_Flag = 0; // Flag to tell main() that a Timer A0 interrupt occurred
volatile unsigned char ISR_Flag_10 = 0; // Flag to tell main() that a Timer A0 interrupt occurred 10 times

/*************************** Function Prototypes *******************************/
// Consider placing function prototypes in a header file, e.g. ECE3567.h


/***************************** First Function***********************************/


void main(void) {

WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);                                             // Disable watchdog timer
PMM_unlockLPM5();                                                                     // Release all pins on MCU

Init_GPIO();                                                                          // Initialize GPIO pins
Init_Timer_A0();                                                                      // Initialize Timer A0

P1DIR |= BIT0;                                                                        // Initialize Bit 0 to pin 1 for Red
P9DIR |= BIT7;                                                                        // Initialize Bit 7 to pin 9 for Green
__enable_interrupt();                                                                 // Enable interrupts

    while(1)
    {
        if(ISR_Flag==1)                                                               // Timer A0 has occurred.
        {
            ISR_Flag = 0;
        }
        if(ISR_Flag_10 ==1)                                                           // 1 Sec interval
        {
            ISR_Flag_10 = 0;

            P1OUT ^= BIT0;                                                            // Toggle bit value for Red LED
            P9OUT ^= BIT7;                                                            // Toggle bit value for Green LED
        }
    }


}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void) {
        ISR_Flag = 1;                                                                 // Set ISR_Flag to 1 for Timer A0
        ISR_Counter++;                                                                // Increment ISR_Counter

        if(ISR_Counter >= 10)                                                         // Check if ISR_Counter is greater than or equal to 10
        {
            ISR_Flag_10 = 1;                                                          // Set ISR_Flag to 1 for Timer A0 to occur
            ISR_Counter = 0;                                                          // Reset ISR_Counter to 0
        }

        return;
}




/******************************** END OF CODE **********************************/
