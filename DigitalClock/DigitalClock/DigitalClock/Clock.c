/*
 * Clock.c
 *
 * Created: 1/15/2014 10:28:20 PM
 *  Author: SG0219139
 */ 


#include "Clock.h"
int keyPressed = 0; //ostatnio wcisniety klawisz
int displayMode = 1; //0 - godzina, 1 - minuty i sekundy
int seconds = 0;
int minutes = 0;
int hours = 0;
int stopClock = 0; //zmienna mowiaca czy zegar ma chodzic (0) czy sie zatrzymac (1)
uint8_t Digits[] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000};
uint8_t Column[] = {0b11111110, 0b11111101, 0b11111011, 0b11111011};

int i = 0; //obecna pozycja w tablicy cyfr
int j = 0; //numer obecnej kolumny na wyswietlaczu 7-segm.

int isPowerOfTwo(unsigned n)
{
	return n && (! (n & (n-1)) );
	
}
//sprawdza na ktorej pozycji w podanym bajcie bit jest niezerowy, jesli jest wiecej niz 1 niezerowy bit zwraca 0
int findPosition(unsigned n)
{

	if (!isPowerOfTwo(n))
	return 0;
	unsigned count = 0;

	while (n)
	{
		n = n >> 1;
		++count;
	}
	return count;
}

void getKeyNumber(){
	int i=0;
	PORTC = 0x00;
	DDRC = 0x0F; // output dla czesci wierszy
	PORTC = 0xF0;
	for(i=0;i<8;i++){}
	int wiersz  = findPosition((PINC ^ 0xF0)) - 4;

	PORTC = 0x00;
	DDRC = 0xF0; //output dla czesci kolumnowej
	PORTC = 0x0F;
	for(i=0;i<8;i++){}
	int kolumna = findPosition((PINC ^ 0x0F));

	int liczba = ((wiersz-1) * 4) + kolumna;
	if(liczba < 0) liczba = 0;
	DDRA = 0xFF;
	PORTA = 0x00;
	PORTA = liczba;
	keyPressed = liczba;
}

void changeMode(){
	if(keyPressed == 1){
		displayMode++;
		displayMode = displayMode % 2;
	}
}

void setHoursAndMins(){ //zmiana czasu na ekranie HH:MM
	stopClock = 1; //zatrzymaj odliczanie zegara
	if(displayMode == 0){ //jesli wyswietlane jest HH:MM
		if(keyPressed == 3){ //zwieksz godziny
			hours++;
			if(hours > 23){
				hours = hours % 24;
			}
		}
		if(keyPressed == 2){ //zwieksz minuty
			minutes++;
			if(minutes > 59){
				minutes = minutes % 60;
			}
		}
	}	
	stopClock = 0; //wznow prace zegara
}

void setMinsAndSecs(){ //zmiana czasu na ekranie MM:SS
	stopClock = 1; //zatrzymaj odliczanie zegara
	if(displayMode == 1){ //jesli wyswietlane jest MM:SS
		if(keyPressed == 3){ //zwieksz minuty
			minutes++;
			if(minutes > 59){
				minutes = minutes % 60;
			}
		}
		if(keyPressed == 2){ //zwieksz sekundy
			seconds++;
			if(seconds > 59){
				seconds = seconds % 60;
			}
		}
	}
	stopClock = 0; //wznow prace zegara
}

void displayKey(){
	PORTB = 0xFF;
	PORTA = Digits[keyPressed];
	PORTB = Column[j];
	i++;
	i = i % 10;
	j++;
	j = j % 4;
}

void displayMinutesAndSeconds(){
	PORTB = 0xFF;
	int dec = seconds / 10;
	int one = seconds % 10;
	int minDec = minutes / 10;
	int minOne = minutes % 10;
	if(j == 0){
		PORTB = Column[0];
		PORTA = Digits[one];
	}
	else if(j == 1){
		PORTB = Column[1];
		PORTA = Digits[dec];
	}
	else if(j == 2){
		PORTB = Column[2];
		PORTA = Digits[minOne];
	}
	else if(j == 3){
		PORTB = Column[3];
		PORTA = Digits[minDec];
	}

	i++;
	i = i % 10;
	j++;
	j = j % 4;
}

void displayHoursAndMinutes(){
	PORTB = 0xFF;
	int dec = minutes / 10;
	int one = minutes % 10;
	int HrsDec = hours / 10;
	int HrsOne = hours % 10;
	if(j == 0){
		PORTB = Column[0];
		PORTA = Digits[one];
	}
	else if(j == 1){
		PORTB = Column[1];
		PORTA = Digits[dec];
	}
	else if(j == 2){
		PORTB = Column[2];
		PORTA = Digits[HrsOne];
	}
	else if(j == 3){
		PORTB = Column[3];
		PORTA = Digits[HrsDec];
	}

	i++;
	i = i % 10;
	j++;
	j = j % 4;
}

void displayClock(){
	if(displayMode == 1){
		displayMinutesAndSeconds();
	}
	if(displayMode == 0){
		displayHoursAndMinutes();
	}
}

void manageClock(){
	if(stopClock == 0){ //jesli zegar ma chodzic to dodaj sekunde
		seconds++;
		if(seconds > 59){
			minutes++;
			seconds = seconds % 60;
		}
		if(minutes > 59){
			hours++;
			minutes = minutes % 60;
		}
		if(hours > 23){
			hours = hours % 24;
		}				
	}
}