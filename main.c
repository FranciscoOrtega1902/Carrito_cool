/*
 * Carrito_cool.c
 *
 * Created: 11/11/2021 02:52:30 p. m.
 * Author : NOMOS
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include "util/delay.h"
#include "avr/interrupt.h"
#include "prj.h"

float vB=0, vQR1=0, vQR2=0, valor1 = 0, valor2 = 0, valor3 = 0, vQR3=0, VTD1 = 0;
float sensor1=0,sensor2=0,sensor3=0,bateria=0;
int flag=0, bandera = 0;
char s1, s2, s3;

//Interrupción

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

int main(void)
{
	// ADC
	ADMUX = 0b00000000;
	ADCSRA |= (1 << ADIE) | (1 << ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); //Habilitando ADC y su interpretacion
	ADCSRA |= (1<<ADSC); //Iniciando Primera Conversión
	//10 bits phase correct, sin preescalador,  todo el clear
	TCCR0 |= (1<<WGM00) |  (1<<COM01) |  (1<<CS00);
	TCCR2 |= (1<<WGM20) | (1<<COM21) | (1<<CS20);
	
	TCCR1A |= (1<<WGM10) | (1<<COM1A1);
	TCCR1B |= (1<<CS10);
	
	DDRB |= (1<<PB3);
	DDRD |= (1<<PD7);
	
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
		Prueba();
		
	}
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
	
	
	
	
/*
void motores(){
	vQR1=(sensor1*0.00488);
	vQR2=(sensor2*0.00488);
	vQR3=(sensor3*0.00488);
	
	//2.00 - 3.30 ES B
	//3.80 - 4.30 ES N
	
	
	//Adelante
	if ( vQR1 < 3.5 && vQR2 > 3.25 && vQR3 < 3.5){
		
		OCR0 = 115;
		OCR2 = 120;
		
	}
	
	//Izquierda
	else if (  vQR1 > 3.25 && vQR2 > 3.25 && vQR3 < 3.5){
		
		OCR0 = 0;
		OCR2 = 120;
	}
	
	//Derecha
	else if (vQR1 < 3.5 && vQR2 > 3.25 && vQR3 > 3.25){
		
		OCR0 = 120;
		OCR2 = 0;
	}
	//izq
	else if (  vQR1 > 3.25 && vQR2 < 3.25 && vQR3 < 3.5){
		
		OCR0 = 0;
		OCR2 = 120;
	}
	
	//Derecha
	else if (vQR1 < 3.5 && vQR2 < 3.25 && vQR3 > 3.25){
		
		OCR0 = 120;
		OCR2 = 0;
	}
	
	//Alto
	else if (  vQR1 < 3.5 && vQR2 < 3.5 && vQR3 < 3.5 ) {
		
		OCR0 = 0;
		OCR2 = 0;
		
	}
	
	else {
		
		OCR0 = 0;
		OCR2 = 0;
	}
}*/

/*
void motores2(){
	OCR0 = 115;
	OCR2 = 120;
}*/

/*
void Rutas(){
	if (bandera == 0)
	{
		Ruta1();
		bandera = 1;
	}
	else if (bandera == 1)
	{
		Ruta2();
		bandera = 0;
	}
	
	
}*/

/*
void Ruta1(){
	vQR1=(sensor1*0.00488);
	vQR2=(sensor2*0.00488);
	vQR3=(sensor3*0.00488);
	
	
	if (vQR1 < 3.5 && vQR2 < 3.5 && vQR3 < 3.5){
		//adelante
		//BBB
		OCR0 = 115;
		OCR2 = 120; 
		
	}
	
	if (vQR1 < 3.5 && vQR2 > 3.25 && vQR3 < 3.5){
		//adelante
		//BNB
		OCR0 = 115;
		OCR2 = 120;
	}
	
	else if (vQR1 > 3.25 && vQR2 > 3.25 && vQR3 < 3.5){
		//Adelante
		//NBN
		OCR0 = 115;
		OCR2 = 120;
	}
	
	else if (vQR1 > 3.25 && vQR2 < 3.5 && vQR3 < 3.5){
		//Izquierda
		//NBB
		OCR0 = 0;
		OCR2 = 120;
	}
	
	else if (vQR1 > 3.25 && vQR2 > 3.25 && vQR3 < 3.5){
		//Adelante
		//NNB
		OCR0 = 15;
		OCR2 = 120;
	}
	
	else if (vQR1 > 3.25 && vQR2 < 3.5 && vQR3 > 3.25){
		//Izquierda
		OCR0 = 0;
		OCR2 = 120;
	}
	
	else if (vQR1 > 3.25 && vQR2 > 3.25 && vQR3 > 3.25){
		//Izquierda
		OCR0 = 0;
		OCR2 = 120;
	}
	
}*/
	

/*
void Ruta2(){
	vQR1=(sensor1*0.00488);
	vQR2=(sensor2*0.00488);
	vQR3=(sensor3*0.00488);
	
	int banderaNNB = 0;
	int banderaNNN = 0;
	int banderaBNN = 0;
	
	if (vQR1 < 3.5 && vQR2 < 3.5 && vQR3 < 3.5){
		//adelante
		//BBB
		OCR0 = 115;
		OCR2 = 120;
	}
	
	if (vQR1 < 3.5 && vQR2 > 3.25 && vQR3 < 3.5  ){
		//adelante
		//BNB
		OCR0 = 115;
		OCR2 = 120;
	}
	
	if (vQR1 > 3.25 && vQR2 > 3.25 && vQR3 < 3.5 ){
		
		if (banderaNNB == 0)
		{
			//adelante
			//NNB
			OCR0 = 115;
			OCR2 = 120;
			banderaNNB = 1;
			
			if (banderaNNB == 1){
				
				if (vQR1 > 3.25 && vQR2 > 3.25 && vQR3 < 3.5 )
				{
					//izquierda
					//NNB
					OCR0 = 0;
					OCR2 = 120;
					banderaNNB = 2;
				}
			}
		}
	}
	
	if (vQR1 > 3.25 && vQR2 < 3.5 && vQR3 < 3.5){
		//Izquierda
		//NBB
		OCR0 = 0;
		OCR2 = 120;
	}
	
	if (vQR1 > 3.25 && vQR2 > 3.25 && vQR3 > 3.25 ){
		
		if (banderaNNN == 0)
		{
			//izquierda
			//NNN
			OCR0 = 0;
			OCR2 = 120;
			banderaNNN = 1;
		}
			
		if (banderaNNN == 1)
		{
			if (vQR1 > 3.25 && vQR2 > 3.25 && vQR3 > 3.25 ){
				//adelante
				//NNN
				OCR0 = 115;
				OCR2 = 120;
				banderaNNN = 0;
			}
		}
	}
	
	
	if (vQR1 < 3.5 && vQR2 > 3.25 && vQR3 > 3.25 ){
		
		if (banderaBNN == 0)
		{
			//izquierda
			//BNN
			OCR0 = 0;
			OCR2 = 120;
			banderaBNN = 1;
			
			if (banderaBNN == 1){
				
				if (vQR1 < 3.5 && vQR2 > 3.25 && vQR3 > 3.25 )
				{
					//Derecha
					//BNN
					OCR0 = 115;
					OCR2 = 0;
					banderaBNN = 0;
					
					
					if (banderaNNB == 2){
						
						if (vQR1 > 3.25 && vQR2 > 3.25 && vQR3 < 3.5 )
						{
							//izquierda
							//NNB
							OCR0 = 0;
							OCR2 = 120;
							banderaNNB = 0;
						}
					}
				}
			}	
		}	
	}
	
}*/


void Prueba(){
	vQR1=(sensor1*0.00488);
	vQR2=(sensor2*0.00488);
	vQR3=(sensor3*0.00488);

	if (vQR1 >= 3.5 && vQR1 <= 5 && vQR2 >= 2 && vQR2 <= 3.25 && vQR3 >= 3.5 && vQR3 <= 5){

	
		//LÍNEA RECTA
		//BNB
		OCR0 = 150;
		OCR2 = 150;
	}
	else if (vQR1 >= 3.5 && vQR1 <= 5 && vQR2 >= 3.5 && vQR2 <= 5 && vQR3 >= 3.5 && vQR3 <= 5){
				
				//STOP
				//BBB
				OCR0 = 0;
				OCR2 = 0;	
	}	
		else if (vQR1 >= 3.5 && vQR1 <= 5 && vQR2 >= 3.5 && vQR2 <= 5 && vQR3 >= 3.5 && vQR3 <= 5){ //MEJORAR PRRO
			
			//ESTACIÓN
			//RNR
			OCR0 = 0;
			OCR2 = 0;
			
			_delay_ms(10000); //AJUSTAR TIEMPO PRRO
			
			OCR0 = 150;
			OCR2 = 150;			
			
	}	
	
	else if (vQR1 >= 2 && vQR1 <= 3.25 && vQR2 >= 2 && vQR2 <= 3.25  && vQR3 >= 3.5 && vQR3 <= 5){
		
		//GIRO A LA DERECHA
		//NNB
		OCR0 = 150;
		OCR2 = 100;
	}
	
	else if (vQR1 >= 3.5 && vQR1 <= 5 && vQR2 >= 2 && vQR2 <= 3.25  && vQR3 >= 2 && vQR3 <= 3.25){
		
		//GIRO A LA IZQUIERDA
		//BNN
		OCR0 = 100;
		OCR2 = 150;
	}	
	
	else if (vQR1 >= 2 && vQR1 <= 3.25 && vQR2 >= 2 && vQR2 <= 3.25  && vQR3 >= 2 && vQR3 <= 3.25){
		
		//INTERSECCIÓN
		//NNN
		OCR0 = 100;
		OCR2 = 150;
	}	
}