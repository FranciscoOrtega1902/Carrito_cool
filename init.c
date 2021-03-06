/***************************************************
*
*	init.c
*
*	Generated by
*	AVR Wizard v1.0.4163.35676
*	Copyright 2011, Chirag Sangani
*	Electronics Club, IIT Kanpur
*	http://students.iitk.ac.in/eclub
*
*	Project Name:   project1
*	Date Created:   28/11/2021 11:00:37 p. m.
*	Author:         
*   Company:        
*	Comments:       
*   
*	Chip:           ATmega16
*	Frequency:      8.00000 MHz
*
***************************************************/

#include "init.h"

void Initialize (void)
{
    //  Enable Interrupts

    sei();

    //  Port A Initialization

    DDRA = 0x00;
    PORTA = 0x00;

    //   Port B Initialization

    DDRB = 0x00;
    PORTB = 0x00;

    //  Port C Initialization

    DDRC = 0x00;
    PORTC = 0x00;

    //  Port D Initialization

    DDRD = 0x00;
    PORTD = 0x00;

    //  Setting up Timer Interrupt Mask

    TIMSK = 0x00;

    //  Timer 0 Initialization

    TCCR0 = 0x00;
    TCNT0 = 0x00;
    OCR0 = 0x00;

    //  Timer 1 Initialization

    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCNT1 = 0x0000;
    OCR1A = 0x0000;
    OCR1B = 0x0000;
    ICR1 = 0x0000;

    //  Timer 2 Initialization

    TCCR2 = 0x00;
    ASSR = 0x00;
    TCNT2 = 0x00;
    OCR2 = 0x00;

    //  Watchdog Timer Initialization

    WDTCR = 0x00;

    //  UART Initialization

    UCSRA = 0x00;
    UCSRB = 0xD8;
    UCSRC = 0x86;
    UBRRL = 0x33;
    UBRRH = 0x00;

    //  SPI Initialization

    SPCR = 0x00;
    SPSR = 0x00;
    
    //  ADC Initialization

    ADMUX = 0x00;
    ADCSRA = 0x00;
    
    SFIOR = 0x00;
    
    //  External Interrupt Initialization

    MCUCR = 0x00;
    MCUCSR = 0x00;
    GICR = 0x00;
    
    //  Analog Comparator Initialization

    ACSR = 0x80;
}

char UARTReadChar (void)
{
    while (!(UCSRA & (1 << RXC)));
    return UDR;
}

void UARTWriteChar (char data)
{
    while (!(UCSRA & (1 << UDRE)));
    UDR = data;
}
