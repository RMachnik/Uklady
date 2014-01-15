/*
 * ExecutionScheduler.c
 *
 * Created: 2013-11-20 18:20:56
 *  Author: student
 
 1 ustawiamy timer w tct
 2 ustawiamy ocr =250
 3 ustawiamy prescaller na 64
 */




#include <avr/io.h>
#include <avr/interrupt.h> 
#include "Scheduler.h"
#include "HD44780.h"
#define F_CPU 16000000UL

	uint8_t digits[] = {
		0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b10000010, 0b11111000, 0b10000000, 0b10010000
	};
	uint8_t digit[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	uint8_t col[4] ={0b11110111,0b11110011,0b11110001,0b11110000};
	int c=0;
	int k=0;
	//funkcja uruchamiana w momencie obslugi przerwania
	void turnOnOff(){
		PORTB =col[c];
		PORTA = digit[k];
		k++;
		k=k%10;
		c++;
		c = c%4;
	}


	
	

int main(void)
{
	
	init();
	sei();
	DDRB = 0xFF;
	DDRA = 0xFF;
	
	
	//dodawanie taskow
	//addTask(&turnOnOff,1000);
	LCD_Initalize();
	LCD_Text("cos");

	
	//wlaczenie globalnych przerwan
	 
    while(1)
    {
		execute();
        
    }
}