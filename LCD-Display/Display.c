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
// This module contains files necessary to LCD Display part of Lab 4
// ----------------------------------------------------------------------------
/*******************************************************************************/

//***** Header Files ***********************************************************/

#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567_Lab3.h"

/********************* Compiler Directives and Definitions **********************/


/******************************** Variables *************************************/
volatile unsigned int display_number = 8765;
unsigned int position[6] = {pos1,pos2,pos3,pos4,pos5,pos6};
volatile char Str[6] = {'T', 'Y', 'L', 'E','R'};
volatile unsigned int n = sizeof(Str);


void displayNumber(unsigned int display_number)
{
    unsigned int num;
    num = display_number;
    clearLCD();
    unsigned int mod;

    if (num > 1000)
        mod = (num / 1000) % 10;
        showChar(mod + 48, pos2);
    if (num > 100)
        mod = (num / 100) % 10;
        showChar(mod + 48, pos3);
    if (num > 10)
        mod = (num / 10) % 10;
        showChar(mod + 48, pos4);
    if (num >= 1)
        num = num % 10;
        showChar(num + 48, pos5);

    return;
}


void displayString(char *Str, unsigned int n)
{
    unsigned int k;
    unsigned int position[6] = {pos1,pos2,pos3,pos4,pos5,pos6};
    clearLCD();

    int i = 0;
    while (i < 6) { // Loop to add each character to display
        k = Str[i];
        showChar(k, position[i]);
        i++;
    }

    return;
}
