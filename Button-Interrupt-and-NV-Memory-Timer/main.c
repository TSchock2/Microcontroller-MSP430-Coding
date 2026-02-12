//*******************************************************************************
// ----------------------------------------------------------------------------
// The Ohio State University
// ECE 3567 - Microcontrollers Lab
// Spring Semester 2022
// Instructor: Prof. Gregg Chapman
// Group # 34
// Name 1 Tyler Schock
// Name 2 Seth Fullenkamp
// Name 3 Abdifatah Ashirow
// (MSP430FR6989 Launchpad) - Texas Instruments Corporation
// Code Composer Studio version 9.0.1
// main.c
// This module contains files necessary to Lab 9
// ----------------------------------------------------------------------------
//*******************************************************************************

//***** Header Files ***********************************************************/
#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"

//***** Lab 9 Variables ******************************************************
// For NVRAM make count persistent
#pragma PERSISTENT(count);
// Declare and initialize count
uint16_t count = 0;

/*************************** Function Prototypes *******************************/
void delay(long unsigned int);

/***************************** main () ***********************************/
void main(void) {
    // Stop Watchdog Timer
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);
    // Disable the GPIO power-on default high-impedance mode
    PMM_unlockLPM5();

    //*********************The Initializations*************************************/
    Init_LCD();                                                 // Display, in myLCD.c
    Init_GPIO();                                                // I/O including LED, in myGpio.c
    Init_Timer_A0();                                            // 100mSec, in myClocks.c
    __enable_interrupt();                                       // Enables all defined interrupts

    while(1)
    {
        if(ISR_Flag == 1)                                       // Set every 100mSec
        {
            ISR_Flag = 0;
            // update LCD display with current count value
            displayCount(count);
        }
    }
}

//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************

/******************************* Timer A0 ISR **********************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    ISR_Flag = 1;                                               // 100 millisecond ISR
    return;
}

/******************************* Pushbutton ISR **********************************/
#pragma vector=PORT1_VECTOR
__interrupt void pushbutton_ISR (void)
{
    unsigned long int debounce = 0x2000;                       // adjust debounce here. 0x2000 is 100 mSec
    switch(P1IV) {                                             // Test Port1 Interrupt Vector Register
        case P1IV_NONE: break;                                 // None

        case P1IV_P1IFG1:                                      // Pin 1 (button 1)
            P1OUT |= BIT0;                                     // Red LED ON
            delay(debounce);                                   // debounce
            if ((P1IN & BIT1) == 0x0002) {                     // is button still released?
                count++;                                       // Count up
            }
            P1OUT &= ~BIT0;                                    // Red LED OFF
            P1IFG &= ~BIT1;                                    // Clear interrupt flag
            break;

        case P1IV_P1IFG2:                                      // Pin 2 (button 2)
            P9OUT |= BIT7;                                     // Green LED ON
            delay(debounce);                                   // debounce
            if ((P1IN & BIT2) == 0x0004) {                     // is button still released?
                if (count > 0) {                               // Test for zero
                    count--;                                   // Count down
                }
            }
            P9OUT &= ~BIT7;                                    // Green LED OFF
            P1IFG &= ~BIT2;                                    // Clear interrupt flag
            break;

        default:
            break;
    }
    return;
}

/******************************** Delay **********************************/
void delay(long unsigned int x)
{
    long unsigned int delay_count = x;
    while(delay_count > 0)
    {
        delay_count--;
        __no_operation();
    }
    return;
}

/******************************** END OF CODE **********************************/
// myGpio.c ('FR6989 Launchpad)
// Dr. Gregg Chapman, The Ohio State University, January 2020
// Group # 34
// Name 1 Tyler Schock
// Name 2 Seth Fullenkamp
// Name 3 Abdifatah Ashirow
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "ECE3567.h"
#include "stdio.h"

//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void Init_GPIO(void)
{
    // Configure LED1 and LED2, both OFF
    P1DIR |= BIT0;                                             // Set P1.0 as output (RED LED)
    P9DIR |= BIT7;                                             // Set P9.7 as output (GREEN LED)
    P1OUT &= ~BIT0;                                            // Clear RED LED
    P9OUT &= ~BIT7;                                            // Clear GREEN LED

    // Set up pushbuttons on PORT1 Bit1 and Bit2 for Interrupts

    // Left Push Button: Set As Input Pin With Pull-up Resistor
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    // Left Push Button: Select Interrupt Edge as Low to High
    P1IES &= ~BIT1;
    // Left Push Button: Clear Interrupt P1IFG.1
    P1IFG &= ~BIT1;
    // Left Push Button: Enable Interrupt
    P1IE |= BIT1;

    // Right Push Button: Set As Input Pin With Pull-up Resistor
    P1DIR &= ~BIT2;
    P1REN |= BIT2;
    P1OUT |= BIT2;
    // Right Push Button: Select Interrupt Edge as Low to High
    P1IES &= BIT2;
    // Right Push Button: Clear Interrupt P1IFG.2
    P1IFG &= ~BIT2;
    // Right Push Button: Enable Interrupt
    P1IE |= BIT2;

    return;
}
