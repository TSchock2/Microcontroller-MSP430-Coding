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
// This module contains files necessary for serial communication part of Lab 5
// ----------------------------------------------------------------------------
/*******************************************************************************/

//*************************************** Header Files **************************
#include <driverlib.h>
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"

//**************************** THE MAIN ROUTINE ********************************
void main (void)
{
    //****** TI Macro used to stop the watchdog timer
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);

    //****** Disable the GPIO power-on default high-impedance mode
    PMM_unlockLPM5();

    Init_GPIO();                                                                   // New Version Lab #5
    Init_Timer_A0();
    Init_PWM();
    Init_LCD();                                                                    // Lab #4
    Init_Clocks();

    //**************************** Enables all Configured Interrupts *************
    __enable_interrupt();

    //**************************** Establish Command Link ************************
    Communications_Link();                                                         // Beginning in Lab #5

    //******************* THE MAIN INFINITE LOOP ********************************
    while(1)
    {
        /******************************* Serial Communications ********************/
        if(Command_Flag==1)                                                        // A USCI_A1 ISR occurred and a Command was received
        {
            Command_Handler(length);                                               // Read / Write
            parse_Command();                                                       // Decode and execute the Command
        }

        /******************************* Runs Every 100 milliSeconds **************/
        if(ISR_Flag == 1)                                                          // Timer A0 ISR has occurred.
        {
            ISR_Flag = 0;                                                          // Clear Interrupt Service Flag
            if(LED_Test_Mode == TRUE)
            {
              //update_RGB()
            }

            if (Flash == TRUE)
            {
                P2DIR ^= 0x0C;
                P3DIR ^= 0x04;
            }
            else
            {
                P2DIR |= 0x0C;
                P3DIR |= 0x04;
            }
        }

        /******************************* Runs Once a Second ************************/
        if(ISR_Flag_10 == 1)
        {
            // Turn off one LED and turn on the other (alternates RED / GREEN)
            P1OUT ^= BIT0;                                                           // RED LED
            P9OUT ^= BIT7;                                                           // GREEN LED
            ISR_Flag_10 = 0;                                                         // Clear the 1 Second ISR flag
        }
    }
}

//****************************** End of Main Routine **************************

//****************************** END of main.c *********************************

//******************************* Interrupt Service Routing for Timer A0 ******
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    ISR_Flag = 1;
    ISR_Counter++;                                                                   // Count ISRs
    if(ISR_Counter>=10)                                                              // 10 ISRs make 1 second
    {
        ISR_Flag_10 = 1;                                                             // Flag to main()
        ISR_Counter=0;                                                               // Reset 1 Second counter
    }
    return;
}
//************************************************** END OF CODE ****************
