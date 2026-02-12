/*******************************************************************************/
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
// (Timer).c or .h
// This module contains files necessary to implement Timer A0 as part of Lab 3
// ----------------------------------------------------------------------------
/*******************************************************************************/

//***** Header Files ***********************************************************/

#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567_Lab3.h"



void Init_Timer_A0(void) {

    // TA0CTL
    TA0CTL = 0x0110; // Set Control Register for 32.768 kHz, No Pre-Divide, and Timer A0 Up Mode

    // TA0CCTL0
    TA0CCTL0 |= CCIE; // Enable Interrupt

    // TA0CCTL1
    TA0CCTL1 = 0x00E0; // Set Reset/Set Mode for PWM

    // TA0CCR0
    TA0CCR0 = 0x0CCD; // 100 mSec period

    // TA0CCR1
    TA0CCR1 = 0x0666; // 50% Duty Cycle

    return;
}
