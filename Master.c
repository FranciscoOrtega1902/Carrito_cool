/*

 * MasterCode.c

 *

 * Created: 24/07/2021 3:17:57

 * Author : efrainmendezflores

 */ 

 

#define F_CPU 8000000UL /* Define frequency here its 8MHz */

#include <avr/io.h>

#include <util/delay.h>

#include <stdlib.h>

#include <stdio.h>

 

//#define USART_BAUDRATE 9600

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

 

 

void UART_init(long USART_BAUDRATE)

{

    UCSRB |= (1 << RXEN) | (1 << TXEN);/* Turn on transmission and reception */

    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */

    UBRRL = BAUD_PRESCALE; /* Load lower 8-bits of the baud rate value */

    UBRRH = (BAUD_PRESCALE >> 8); /* Load upper 8-bits*/

}

 

unsigned char UART_RxChar()

{

    while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */

    return(UDR); /* Return the byte*/

}

 

void UART_TxChar(char ch)

{

    while (! (UCSRA & (1<<UDRE))); /* Wait for empty transmit buffer*/

    UDR = ch ;

}

 

void UART_SendString(char *str)

{

    unsigned char j=0;

    

    while (str[j]!=0) /* Send string till null */

    {

        UART_TxChar(str[j]);

        j++;

    }

}

 

int main()

{

    char c;

    UART_init(9600);

   

    

    while(1)

    {

        UART_TxChar('1');

        _delay_ms(500);

        UART_TxChar('0');

        _delay_ms(500);

    }

}
