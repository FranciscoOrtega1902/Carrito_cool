/*

 * USART_ATmega16.c

 *

 * Created: 17/07/2021 10:08:00

 * Author : efrainmendezflores

 */ 

 

// Slave code

#define F_CPU 8000000UL /* Define frequency here its 8MHz */

#include <avr/io.h>

#include <avr/interrupt.h>

 

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

 

int botonMenu = 0;

 

#define LED_OFF PORTA &=~ (1<<PA0)

#define LED_ON PORTA |= (1<<PA0)

 

void UART_init(long USART_BAUDRATE)

{

    UCSRB |= (1 << RXEN) | (1 << RXCIE);/* Turn on the transmission and reception */

    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */

 

    UBRRL = BAUD_PRESCALE; /* Load lower 8-bits of the baud rate */

    UBRRH = (BAUD_PRESCALE >> 8); /* Load upper 8-bits */

}

 

ISR(USART_RXC_vect)

{

    botonMenu=UDR;

}

 

int main()

{

    UART_init(9600);

    

    DDRA |= (1<<PA0);

    LED_OFF; // Apagando LED de inicio

    

    sei();

    

    while(1){

        if (botonMenu == '0')

        {

            LED_OFF;

        }

        else if (botonMenu == '1')

        {

            LED_ON;

        }

    }

}
