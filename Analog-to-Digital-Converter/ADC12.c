//*******************************************************************************/
// ----------------------------------------------------------------------------
// The Ohio State University
// ECE 3567 - Microcontrollers Lab
// Spring Semester 2025
// Instructor: Prof. Gregg Chapman
// Group # 34
// Name 1 Tyler Schock
// Name 2
// Name 3 Abdifatah Ashirow
// (MSP430FR6989 Launchpad) - Texas Instruments Corporation
// Code Composer Studio version 9.1.0
// (main).c or .h
// This module contains files necessary for Analog to Digital Converters and Temperature
// Sensor part of Lab 6

#include "driverlib.h"
#include "stdio.h"
#include "string.h"
#include "ECE3567.h"

//
// All globals have been moved to 3567.h NO EXTERNS NECESSARY

void Init_ADC12(void)
{
    // Set-up the Sampling Timer for ADC12 sample clock. Timer A1 is free-running in PWM Mode
    /********** Initialize Timer A1 as Sample Clock *********************/

    TA1CTL = 0x0000; // Begin by clearing the register
    TA1CTL |= 0x0100; // Select ACLK, and No Divide by
    TA1CCTL1 |= 0x00E0; // Select Compare Mode, Reset/Set Output Mode, and Interrupts DISABLED (rest are default)
    TA1CCR0 = 0x33; // Set Period for 640 Hz. This gets divided by 64 later for a sample rate of 10Hz
    TA1CCR1 = 0x19; // Select 50% duty cycle
    TA1CTL |= 0x0010; // Set Timer to UP MODE

    // Set-up Internal Voltage Reference
    REFCTL0 = 0x0001;

    // Configure P8.6 for ADC 5
    P8SEL0 |= BIT6; // P8.6 is Tertiary Module function [11], ADC channel A5
    P8SEL1 |= BIT6; // P8.6 is Tertiary Module function [11], ADC channel A5

    // Initialize the ADC12B Module
    ADC12CTL0 |= 0x0490; // Sample and Hold is 64 clock cycles for MEM0 - MEM7, SHI is rising edge, ADCON
    ADC12CTL1 |= 0x100E; // No Pre-divide/Timer A1/Div1/ACLK/Repeat-Sequence-of-Channels

    // ADC12CTL2 is set-up for single ended channels and no differential pairs by default

    ADC12CTL3 |= 0x0080; // Set bit 7 ADC30 is internal Temperature Sensor for ADC Start Address

    // Set-up Memory Channels and sampling sequence
    ADC12MCTL0 |= 0x011E; // VREF and AVSS / Not end of Sequence / A30 /
    ADC12MCTL1 |= 0x0085; // AVCC and AVSS / MEM1 is A5 and End of Sequence

    /***************** START FREE RUNNING CONVERSIONS **************************/
    ADC12IFGR0 &= ~0x0002; // Clear pending MEM1 interrupt flag
    ADC12IER0 |= 0x0002; // Enable MEM1 Interrupt
    ADC12CTL0 |= 0x0003; // Start Converter
}

//******************************* Interrupt Service Routing for A to D Converter *****************/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    switch(__even_in_range(ADC12IV,12))
    {
        case 0: break; // Vector 0: No interrupt
        case 2: break; // Vector 2: ADC12BMEMx Overflow
        case 4: break; // Vector 4: Conversion time overflow
        case 6: break; // Vector 6: ADC12BHI
        case 8: break; // Vector 8: ADC12BLO
        case 10: break; // Vector 10: ADC12BIN
        case 12: break; // Vector 12: ADC12BMEM0 Interrupt
        case 14:
            ADC12_B_clearInterrupt(ADC12_B_BASE, 0, ADC12_B_IFG1); // TI Macro Clears IFG1
            ADCValue0 = ADC12MEM0; // Transfer Temperature Sensor voltage to variable
            ADCValue1 = ADC12MEM1; // Transfer RC Voltage to variable
            break; // Vector 14: ADC12BMEM1
        case 16: break; // Vector 16: ADC12BMEM2
        case 18: break; // Vector 18: ADC12BMEM3
        case 20: break; // Vector 20: ADC12BMEM4
        case 22: break; // Vector 22: ADC12BMEM5
        case 24: break; // Vector 24: ADC12BMEM6
        case 26: break; // Vector 26: ADC12BMEM7
        case 28: break; // Vector 28: ADC12BMEM8
        case 30: break; // Vector 30: ADC12BMEM9
        case 32: break; // Vector 32: ADC12BMEM10
        case 34: break; // Vector 34: ADC12BMEM11
        case 36: break; // Vector 36: ADC12BMEM12
        case 38: break; // Vector 38: ADC12BMEM13
        case 40: break; // Vector 40: ADC12BMEM14
        case 42: break; // Vector 42: ADC12BMEM15
        case 44: break; // Vector 44: ADC12BMEM16
        case 46: break; // Vector 46: ADC12BMEM17
        case 48: break; // Vector 48: ADC12BMEM18
        case 50: break; // Vector 50: ADC12BMEM19
        case 52: break; // Vector 52: ADC12BMEM20
        case 54: break; // Vector 54: ADC12BMEM21
        case 56: break; // Vector 56: ADC12BMEM22
        case 58: break; // Vector 58: ADC12BMEM23
        case 60: break; // Vector 60: ADC12BMEM24
        case 62: break; // Vector 62: ADC12BMEM25
        case 64: break; // Vector 64: ADC12BMEM26
        case 66: break; // Vector 66: ADC12BMEM27
        case 68: break; // Vector 68: ADC12BMEM28
        case 70: break; // Vector 70: ADC12BMEM29
        case 72: break; // Vector 72: ADC12BMEM30
        case 74: break; // Vector 74: ADC12BMEM31
        case 76: break; // Vector 76: ADC12BRDY
        default: break;
    }
}

//****************************** END of ISR for A-to-D ******************************************/
