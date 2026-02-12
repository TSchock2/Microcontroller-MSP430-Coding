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
// (Timer).c or .h
// This module contains files necessary to implement RGB LEDs as part of Lab 3
// ----------------------------------------------------------------------------
/*******************************************************************************/

//***** Header Files ***********************************************************/

#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567_Lab3.h"

/********************* Compiler Directives and Definitions **********************/


/******************************** Variables *************************************/

// Consider placing global variables in a header file, e.g. ECE3567.h

volatile unsigned int duty_cycle_blue;                                                 // Blue duty cycle
volatile unsigned int duty_cycle_green;                                                // Green duty cycle
volatile unsigned int duty_cycle_red;                                                  // Red duty cycle
volatile short LED_Color;                                                              // LED colors
volatile unsigned int step_size = 2;
volatile unsigned int red_count = 0;
int State = 1;
int LED = 1;
void Init_PWM(void) {

        // Initialize duty cycles
        duty_cycle_blue = 0x0000;
        duty_cycle_green = 0x0000;
        duty_cycle_red = 0x0070;

        // Pin directions and Selections
        P3DIR |= 0xC0;
        P2DIR |= 0x04;
        P3SEL1 |= 0xC0;
        P3SEL0 &= 0xFF3F;
        P2SEL1 |= 0x04;
        P2SEL0 &= 0xFFFB;

        // TB0CTL
        TB0CTL = 0x0110;                                                               // Set Control Register for 32.768 kHz, No Pre-Divide, and Timer A0 Up Mode

        // TB0CCTL0
        TB0CCTL0 |= CCIE;                                                              // Enable Interrupt

        // TB0CCTL2
        TB0CCTL2 = 0x00E0;                                                             // Set Reset/Set Mode for PWM

        // TB0CCTL3
        TB0CCTL3 = 0x00E0;                                                             // Set Reset/Set Mode for PWM

        // TB0CCTL4
        TB0CCTL4 = 0x00E0;                                                             // Set Reset/Set Mode for PWM

        // TB0CCR0
        TB0CCR0 = RGB_Period;                                                          // Period

        // TB0CCR2
        TB0CCR2 = duty_cycle_blue;

        // TB0CCR3
        TB0CCR3 = duty_cycle_green;

        // TB0CCR4
        TB0CCR4 = duty_cycle_red;
        return;
}

void update_RGB(void) {

    switch(State)
    {
     case 1:                                                                          // MAX_Red
        red_count++;
        if(red_count >= 15)
        {
            red_count = 0;
            State = 2;                                                                // Go to State 2
        }
        break;
      case 2:                                                                         // Increase duty_cycle_Green
          if(duty_cycle_green < MAX_Green)

              duty_cycle_green += step_size;
          else

              State = 3;                                                             // Go to State 3
          break;
        case 3:                                                                      // Decrease duty_cycle_red
          if(duty_cycle_red >= step_size)

               duty_cycle_red -= step_size;
          else

               State = 4;                                                            // Go to State 4
          break;
        case 4:                                                                      // Increase duty_cycle_blue
          if(duty_cycle_blue < MAX_Blue)

                duty_cycle_blue += step_size;
          else

                State = 5;                                                           // Go to State 5
          break;
        case 5:                                                                      // Decrease duty_cycle_green
          if (duty_cycle_green >= step_size)

                duty_cycle_green -= step_size;
          else

                State = 6;                                                           // Go to State 6
          break;
        case 6:                                                                      // Increase duty_cycle_red
          if (duty_cycle_red < MAX_Red)

                duty_cycle_red += step_size;
          else

                State = 7;                                                           // Go to State 7
          break;
        case 7:                                                                      // Decrease duty_cycle_blue
          if (duty_cycle_blue >= step_size)

                duty_cycle_blue -= step_size;
          else

                State = 1;                                                           // Next State
          break;
        default:                                                                     // Return to State 1
                State = 1;
          break;
}
          // TB0CCR2
          TB0CCR2 = duty_cycle_blue;

          // TB0CCR3
          TB0CCR3 = duty_cycle_green;

          // TB0CCR4
          TB0CCR4 = duty_cycle_red;
          return;
}


void update_RGB2(void) {
    // Initialize LED_Color to Red
    LED_Color = Red;
    LED_Color++;                                                                     // Increment LED_Color to new color
    if(LED_Color > White) {                                                          // If LED_Color is greater than White, change it to Red
        LED_Color = Red;
    }

    switch(LED)
    {
      case 1:                                                                        // Red

          duty_cycle_red = 0x070;
          duty_cycle_green = 0x000;
          duty_cycle_blue = 0x000;

          LED = 2;                                                                   // Go to state 2
        break;
      case 2: // Orange

          duty_cycle_red = 0x00C4;
          duty_cycle_green = 0x0024;
          duty_cycle_blue = 0x0000;

          LED = 3;                                                                  // Got to state 3
        break;
      case 3:                                                                       // Yellow

          duty_cycle_red = 0x00C4;
          duty_cycle_green = 0x00AB;
          duty_cycle_blue = 0x0000;

          LED = 4;                                                                  // Go to State 4
        break;
      case 4:                                                                       // Green

          duty_cycle_red = 0x0000;
          duty_cycle_green = 0x004A;
          duty_cycle_blue = 0x0000;

          LED = 5;                                                                  // Go to State 5
        break;
      case 5:                                                                       // Blue

          duty_cycle_red = 0x0000;
          duty_cycle_green = 0x0000;
          duty_cycle_blue = 0x007A;

          LED = 6;                                                                  // Go to State 6
        break;
      case 6:                                                                       // Violet

          duty_cycle_red = 0x0026;
          duty_cycle_green = 0x0000;
          duty_cycle_blue = 0x007A;

          LED = 7;                                                                  // Go to State 7
        break;
      case 7:                                                                       // White

          duty_cycle_red = 0x00B0;
          duty_cycle_green = 0x0090;
          duty_cycle_blue = 0x00D0;

          LED = 8;                                                                  // Go to State 8
       break;
     case 8:                                                                        // No Color

          duty_cycle_red = 0x0000;
          duty_cycle_green = 0x0000;
          duty_cycle_blue = 0x0000;

          LED = 1;                                                                  // Next State
        break;
      default:                                                                      // Return to State 1 after No Color

          duty_cycle_red = 0x0000;
          duty_cycle_green = 0x0000;
          duty_cycle_blue = 0x0000;

          LED = 1;
        break;
    }

  
       // TB0CCR2
       TB0CCR2 = duty_cycle_blue;

       // TB0CCR3
       TB0CCR3 = duty_cycle_green;

       // TB0CCR4
       TB0CCR4 = duty_cycle_red;
       return;
}

 void PWM_null(void) {

       // TB0CCR2
       TB0CCR2 = 0x0000;

       // TB0CCR3
       TB0CCR3 = 0x0000;

       // TB0CCR4
       TB0CCR4 = 0x0000;

       // Blue
       duty_cycle_blue = 0x0000;

       // Green
       duty_cycle_green = 0x0000;

       // Red
       duty_cycle_red = 0x0000;
       return;
 }
