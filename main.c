/*
 * Carrito_cool.c
 *
 * Created: 11/11/2021 02:52:30 p. m.
 * Author : NOMOS
 */ 

#define F_CPU 8000000UL

//Verificar los valores del sensor 

#define ROJO_LI 1.0 // Rojo Limite Inferior
#define ROJO_LS 1.6 // Rojo Limite Sup
#define NEGRO_LI 2.5
#define NEGRO_LS 3.25
#define BLANCO_LI 3.5 
#define BLANCO_LS 5

#include <avr/io.h>
#include "util/delay.h"
#include "avr/interrupt.h"
#include "prj.h"
#include "includes.h"
#include "init.h"

// Global Variables
volatile char Cmd;
volatile char CanWeMove = 'y';
float vB=0, vQR1=0, vQR2=0, valor1 = 0, valor2 = 0, valor3 = 0, vQR3=0, VTD1 = 0;
float sensor1=0,sensor2=0,sensor3=0,bateria=0;
int flag=0, bandera = 0;
char s1, s2, s3;

 //  Declare functions and global variables here

ISR(USART_RXC_vect)
{
	//  UART Receiver Interrupt

	char Cmd;
	Cmd = UDR;
	if (Cmd == 's') {

		// Uno de los carros llega a la estacion, el otro se detiene igual...
		OCR0 = 0;
	    OCR2 = 0;
		CanWeMove = 'N';
	    // _delay_ms(10000); // Este delay se puede borrar... y que el carro espere 
	    // OCR0 = 150;
	    // OCR2 = 150;
	}
	else if (Cmd=='d') { // El otro carro me dice que le siga...
		OCR0 = 150;
		OCR2 = 150;
		CanWeMove = 'Y';
	}
	else if (Cmd == 'r') {

	}

}

ISR(USART_TXC_vect)
{
	//  UART Transmitter Interrupt

}


//Interrupcion

ISR(ADC_vect){
	if (flag==0)
	{
		sensor1=ADC;
		ADMUX=0b00000001;
		flag=1;
	} else if(flag==1){
		sensor2=ADC;
		ADMUX=0b00000010;
		flag=2;
	} else if(flag==2){
		sensor3=ADC;
		ADMUX=0b00000011;
		flag=3;
	} else if(flag==3){
		bateria=ADC;
		ADMUX=0b00000000;
		flag=0;
	}
	ADCSRA |= (1<<ADSC);
}





void porcentaje_bateria(){
	
	vB=(bateria*0.00488);

	if (vB==0){
		ssd1306_setpos(60, 2), ssd1306_bateria(0);
	}
	else if (vB	>0 && vB<=0.5){
		ssd1306_setpos(60, 2), ssd1306_bateria(1);
	}
	else if (vB>0.5 && vB<=1){
		ssd1306_setpos(60, 2), ssd1306_bateria(2);
	}
	else if (vB>1 && vB<=1.5){
		ssd1306_setpos(60, 2), ssd1306_bateria(3);
	}
	else if (vB>1.5 && vB<=2){
		ssd1306_setpos(60, 2), ssd1306_bateria(4);
	}
	else if (vB>2 && vB<=2.5){
		ssd1306_setpos(60, 2), ssd1306_bateria(5);
	}
	else if (vB>2.5 && vB<=3){
		ssd1306_setpos(60, 2), ssd1306_bateria(6);
	}
	else if (vB>3 && vB<=3.5){
		ssd1306_setpos(60, 2), ssd1306_bateria(7);
	}
	else if (vB>3.5 && vB<=4){
		ssd1306_setpos(60, 2), ssd1306_bateria(8);
	}
	else if (vB>4 && vB<=4.5){
		ssd1306_setpos(60, 2), ssd1306_bateria(9);
	}
	else if (vB>4.5 && vB<=5){
		ssd1306_setpos(60, 2), ssd1306_bateria(10);
	}
	else {
		ssd1306_setpos(60, 2), ssd1306_bateria(0);
	}
}
void QR1(){
	vQR1=(sensor1*0.00488);
	valor1=(int)vQR1;
	s1=valor1+'0';

	if (vQR1 > 3.5)
	{
		ssd1306_setpos(0, 4);	ssd1306_string_font6x8("S1:B"); 
	}
	else if (vQR1 <= 3.5)
	{
		ssd1306_setpos(0, 4);	ssd1306_string_font6x8("S1:N");
	}
	else {
		ssd1306_setpos(0, 4);	ssd1306_string_font6x8("S1:B"); 
	}
}

void QR2(){
	vQR2=(sensor2*0.00488);
	valor2=(int)vQR2;
	s2=valor2+'0';
	
	if (vQR2 > 3.5)
	{
		ssd1306_setpos(40, 4);	ssd1306_string_font6x8("S2:B"); 
	}
	else if (vQR2 <= 3.5)
	{
		ssd1306_setpos(40, 4);	ssd1306_string_font6x8("S2:N"); 
	}
	else {
		ssd1306_setpos(40, 4);	ssd1306_string_font6x8("S2:B"); 
	}
}

void QR3(){
	vQR3=(sensor3*0.00488);
	valor3=(int)vQR3;
	s3=valor3+'0';
	
	if (vQR3 > 3.5)
	{
		ssd1306_setpos(80, 4);	ssd1306_string_font6x8("S3:B"); 
		
	}
	else if (vQR3 <= 3.5)
	{
		ssd1306_setpos(80, 4);	ssd1306_string_font6x8("S3:N"); 
	}
	else {
		ssd1306_setpos(80, 4);	ssd1306_string_font6x8("S3:B"); 
	}
}

void VoltsImprimir(){

	vQR1=(sensor1*0.00488);
	int Entero1 = vQR1;
	int Decimal1 = (vQR1 - Entero1)*100;
	
	unsigned char buffer_OLED1[124]= {};
	
	sprintf((char*)buffer_OLED1,"%01d.%02d", Entero1, Decimal1);

	ssd1306_setpos(0, 6);
	ssd1306_string_font6x8(buffer_OLED1);
	

	vQR2=(sensor2*0.00488);
	int Entero2 = vQR2;
	int Decimal2 = (vQR2 - Entero2)*100;
	
	unsigned char buffer_OLED2[124]= {};
	
	sprintf((char*)buffer_OLED2,"%01d.%02d", Entero2, Decimal2);

	ssd1306_setpos(40, 6);
	ssd1306_string_font6x8(buffer_OLED2);
	
	
	vQR3=(sensor3*0.00488);
	int Entero3 = vQR3;
	int Decimal3 = (vQR3 - Entero3)*100;
	
	unsigned char buffer_OLED3[124]= {};
	
	sprintf((char*)buffer_OLED3,"%01d.%02d", Entero3, Decimal3);

	ssd1306_setpos(80, 6);
	ssd1306_string_font6x8(buffer_OLED3);
	
	}
	
	

void Prueba(){

	vQR1=(sensor1*0.00488);
	vQR2=(sensor2*0.00488);
	vQR3=(sensor3*0.00488);

	if (vQR1 >= BLANCO_LI && vQR1 <= BLANCO_LS && vQR2 >= NEGRO_LI && vQR2 <= NEGRO_LS && vQR3 >= BLANCO_LI && vQR3 <= BLANCO_LS){
		//LINEA RECTA
		//BNB
		OCR0 = 150;
		OCR2 = 150;
	}
	else if (vQR1 >= BLANCO_LI && vQR1 <= BLANCO_LS && vQR2 >= BLANCO_LI && vQR2 <= BLANCO_LS && vQR3 >= BLANCO_LI && vQR3 <= BLANCO_LS){				
	  //En esta condicion los sensores todos detectan blanco, el carro debe detenerse
	  //BBB
	  OCR0 = 0;
	  OCR2 = 0;	
	}	
	else if (vQR1 >= ROJO_LI && vQR1 <= ROJO_LS && vQR2 >= NEGRO_LI && vQR2 <= NEGRO_LS && vQR3 >= ROJO_LI && vQR3 <= ROJO_LS) { //MEJORAR PRRO
     // Cambia los valores de voltaje, por ejemplo 3.5 por los macros BLANCO, ROJO o NEGRO 
	 // El carro llega a la ESTACION
	 //RNR
	 UARTWriteChar('s'); // Dile al otro carro que aguante...
	 OCR0 = 0;
	 OCR2 = 0;
	 _delay_ms(10000); //AJUSTAR TIEMPO PRRO
	 UARTWriteChar('d'); // Dile al otro carro que continue.. 
	 OCR0 = 150;
	 OCR2 = 150;
			
	}	
	
	else if (vQR1 >= NEGRO_LI && vQR1 <= NEGRO_LS && vQR2 >= NEGRO_LI && vQR2 <= NEGRO_LS  && vQR3 >= BLANCO_LI && vQR3 <= BLANCO_LS){		
		//GIRO A LA DERECHA
		//NNB
		OCR0 = 150;
		OCR2 = 100;
	}
	
	else if (vQR1 >= BLANCO_LI && vQR1 <= BLANCO_LS && vQR2 >= NEGRO_LI && vQR2 <= NEGRO_LS  && vQR3 >= NEGRO_LI && vQR3 <= NEGRO_LS){		
		//GIRO A LA IZQUIERDA
		//BNN
		OCR0 = 100;
		OCR2 = 150;
	}	
	
	else if (vQR1 >= NEGRO_LI && vQR1 <= NEGRO_LS && vQR2 >= NEGRO_LI && vQR2 <= NEGRO_LS  && vQR3 >= NEGRO_LI && vQR3 <= NEGRO_LS){
		
		//INTERSECCION
		//NNN
		OCR0 = 100;
		OCR2 = 150;
	}	
}

int main(void)
{
	// ADC
	ADMUX = 0b00000000;
	ADCSRA |= (1 << ADIE) | (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); //Habilitando ADC y su interpretacion
	ADCSRA |= (1 << ADSC); //Iniciando Primera Conversion
	//10 bits phase correct, sin preescalador,  todo el clear
	TCCR0 |= (1 << WGM00) | (1 << COM01) | (1 << CS00);
	TCCR2 |= (1 << WGM20) | (1 << COM21) | (1 << CS20);

	TCCR1A |= (1 << WGM10) | (1 << COM1A1);
	TCCR1B |= (1 << CS10);

	DDRB |= (1 << PB3);
	DDRD |= (1 << PD7);

	// Allow Car to enter 
	CanWeMove = 'Y';

	// Inicializa UART Bus
	Initialize();
	sei();
	i2c_init();
	ssd1306_init();

	ssd1306_clear(); _delay_ms(200);


	ssd1306_setpos(0, 0);	ssd1306_string_font6x8("Bateria");

	ssd1306_setpos(60, 2);	ssd1306_bateria(0);

	sei();
	while (1)
	{
		porcentaje_bateria();
		QR1();
		QR2();
		QR3();
		VoltsImprimir();
		//motores();
		//Ruta1();
		//Ruta2();
		if (CanWeMove == 'Y') {			
			Prueba(); // Evalua el status de los sensores para seguir adelante o no
		}
		else {
			// Implementar un mensaje en la pantalla para indicar que estar detenido
		}
	}
}
