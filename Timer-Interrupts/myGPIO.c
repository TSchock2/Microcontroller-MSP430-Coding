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
// (myGPIO).c or .h
// This module contains files necessary to initialize GPIO pins as part of Lab 2
// ----------------------------------------------------------------------------
/*******************************************************************************/

//***** Header Files ***********************************************************/

#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"



void Init_GPIO(void) {
    P1OUT &= ~BIT0; // Turn off Red
    P9OUT |= BIT7; // Turn on Green
    return;
}
