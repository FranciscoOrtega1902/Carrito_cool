
//#define USART_BAUDRATE 9600

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void UART_SendString(char* str);