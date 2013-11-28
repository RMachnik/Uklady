/*
 * GccApplication1.c
 *
 * Created: 2013-11-13 18:29:01
 *  Author: student
 */ 


#define F_CPU 16000000UL  //flaga taktowania procesora

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>




void initialise(){
	TCCR0 |= (1<<WGM00) | (1<<WGM01); //ustawianie flag
	TCCR0 |=(0<<COM00) | (0<<COM01); // ustawiamy flagi dla TCCRO
	TCCR0 |=(1<<CS00) | (0<<CS01) | (0<<CS02);
	TIMSK |=(1<<OCIE0) |(1<<TOIE0); //konfiguracja TIMERA
	DDRB |= (1<<PB3); // ustawiam DDRB na PB3 -> to jest port obslugiwany sprzetowo przez TCCRO
}

ISR(TIMER0_COMP_vect){ //funkcja ktora obsluguje przerwania
	PORTA = 0x00;
}
ISR(TIMER0_OVF_vect){
	PORTA = 0xFF;
}
int main(void)
{

	DDRA = 0xFF;
	initialise();
	sei();  //globalne przerwania
    while(1)
    {
			_delay_ms(30);
        	OCR0++;
    }
}