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
// RGB.c
// This module contains files necessary for the RGB_LED for Lab 7
// ----------------------------------------------------------------------------
/*******************************************************************************/

// ----------------------------------------------------------------------------
// RGB_LED.c ('FR6989 Launchpad)
// Gregg Chapman, The Ohio State University, February 2018
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************

#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"

// All globals have been moved to 3567.h NO EXTERNS NECESSARY

/**********************************************************************/


void Init_PWM(void){
    unsigned int period = 0x148;
    duty_cycle_red = 0x100;
    duty_cycle_green = 0x120;
    duty_cycle_blue = 0x120;

    P2DIR |= 0x04;                                     // Set P2.2 as an output
    P2SEL1 |= 0x04;                                    // Set P2.2 to SECONDARY OUTPUT, TB0.4, (10)
    P2SEL0 &= 0xFB;                                    // Set P2.2 to SECONDARY OUTPUT, TB0.4, (10)

    P3DIR |= 0xc0;                                     // Set P3.6 and P3.7 to output
    P3SEL1 |= 0xc0;                                    // Set P3.6 and P3.7 to SECONDARY OUTPUT, TB0.2 and TB0.3, (10)
    P3SEL0 &=0x3F;                                     // Set P3.6 and P3.7 to SECONDARY OUTPUT, TB0.2 and TB0.3, (10)

    TB0CCR0 = period;                                  // PWM Period
    TB0CTL = 0x0110;                                   // ACLK, up mode

    TB0CCTL2 = 0x00E0;                                 // CCR2 reset/set
    TB0CCR2 = duty_cycle_blue;                         // CCR2 PWM duty cycle, TB0.2
    TB0CCTL3 = 0x00E0;                                 // CCR2 reset/set
    TB0CCR3 = duty_cycle_green;                        // CCR2 PWM duty cycle, TB0.3
    TB0CCTL4 = 0x00E0;                                 // CCR5 reset/set
    TB0CCR4 = duty_cycle_red;                          // CCR5 PWM duty cycle, TB0.4
    LED_Color = White;

    return;
}


void update_RGB()
{
    if (Temp_Mode == TRUE)
    {
        if(degLED <= 670)                              //Flashing Violet if Temp is less than lower than 67.1
        {
            LED_Color = Violet;
            Flash = TRUE;
        }
        else if (degLED <= 690)                        // 69 to 67.1, etc.
        {
            LED_Color = Violet;
            Flash = FALSE;
        }
        else if (degLED <= 710)                        // 71 to 69.1
        {
            LED_Color = Blue;
            Flash = FALSE;
        }
        else if (degLED <= 730)                        // 73 to 71.1
        {
            LED_Color = Green;
            Flash = FALSE;
        }
        else if (degLED <= 750)                        // 75 to 73.1
        {
            LED_Color = Yellow;
            Flash = FALSE;
        }
        else if (degLED <= 770)                        // 77 to 75.1
        {
            LED_Color = Orange;
            Flash = FALSE;
        }
        else if (degLED <= 790)                        // 79 to 77.1
        {
            LED_Color = Red;
            Flash = FALSE;
        }
        else if (degLED > 790)                         // Flashing Red if Temp is 79.1 and up
        {
            LED_Color = Red;
            Flash = TRUE;
        }
    }

    switch(LED_Color)
    {
        case Red:                                      // Red element only
            duty_cycle_red = 0x070;
            duty_cycle_green = 0x000;
            duty_cycle_blue = 0x000;
            break;

        case Orange:                                   // Green and Red can make Orange
            duty_cycle_red = 0x0C4;
            duty_cycle_green = 0x024;
            duty_cycle_blue = 0x000;
            break;

        case Yellow:                                   // Green and Red make Yellow too!
            duty_cycle_red = 0x0C4;
            duty_cycle_green = 0x0AB;
            duty_cycle_blue = 0x000;
            break;

        case Green:                                    // Green element only
            duty_cycle_red = 0x000;
            duty_cycle_green = 0x04A;
            duty_cycle_blue = 0x000;
            break;

        case Blue:                                     // Blue element only
            duty_cycle_red = 0x000;
            duty_cycle_green = 0x000;
            duty_cycle_blue = 0x07A;
            break;
        case Violet:                                   // Red and Blue make Purple
            duty_cycle_red = 0x026;
            duty_cycle_green = 0x000;
            duty_cycle_blue = 0x07A;
            break;

        case White:                                    // Turn off LED
            duty_cycle_red = 0x0D0;
            duty_cycle_green = 0x00B0;
            duty_cycle_blue = 0x00A0;
            break;

        case No_Color:                                 // Turn off LED
            duty_cycle_red = 0x000;
            duty_cycle_green = 0x000;
            duty_cycle_blue = 0x000;
            break;

        default:                                       // Turn off LED
            duty_cycle_red = 0x000;
            duty_cycle_green = 0x000;
            duty_cycle_blue = 0x000;
            break;
    }

    TB0CCR2 = duty_cycle_blue;                         // CCR2 PWM duty cycle
    TB0CCR3 = duty_cycle_green;                        // CCR2 PWM duty cycle
    TB0CCR4 = duty_cycle_red;                          // CCR5 PWM duty cycle
    return;
}

/*************************************** Turns off the LED ***********************************************/
void PWM_null()
{
    duty_cycle_red = 0x0000;
    duty_cycle_green = 0x0000;
    duty_cycle_blue = 0x0000;
    TB0CCR2 = 0000;                                   // CCR2 PWM duty cycle
    TB0CCR3 = 0000;                                   // CCR2 PWM duty cycle
    TB0CCR4 = 0000;                                   // CCR5 PWM duty cycle
    return;
}


/*********************************************** END OF CODE ******************************************/
