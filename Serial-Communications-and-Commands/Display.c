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
// (main).c or .h
// This module contains files necessary to Display Temp and Volts part of Lab 5
// ----------------------------------------------------------------------------
/*******************************************************************************/

//*************************************** Header Files
********************************************
#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"



void displayString(char *Str, unsigned int n)
{
    unsigned char i;
    int k;
    char j;
    unsigned int position[6] = {pos1,pos2,pos3,pos4,pos5,pos6};

    clearLCD();
    for(i=0;i<n;i++)
    {
        j = Str[i];
        k = position[i];
        showChar(j, k);

    }
    return;
}

void displayNumber(unsigned int display_number)
{

    clearLCD();
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
    // Decimal point
    //LCDMEM[pos3+1] |= 0x01;
    return;
}

void displayTemp()
{
    clearLCD();
    displayNumber(deg);
  
    showChar('F',pos6);

    // Degree symbol
    LCDMEM[pos5+1] |= 0x04;
    // Decimal
    LCDMEM[pos4+1] |= 0x01;
    return;
}

void displayVolts()
{
    clearLCD();
    displayNumber(volts);

    showChar('V',pos6);

    // Volts symbol
    LCDMEM[pos5+1] |= 0x04;
    // Decimal
    LCDMEM[pos2+1] |= 0x01;
    return;
}
