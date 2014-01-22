/*
 * Clock.c
 *
 * Created: 1/15/2014 10:28:20 PM
 *  Author: SG0219139
 */ 


#include "Clock.h"
int keyPressed = 0; 
int lastKeyPressed =-1;//ostatnio wcisniety klawisz
int displayMode = 1; //0 - godzina, 1 - minuty i sekundy
int seconds = 0;
int minutes = 0;
int hours = 12;
int stopClock = 0; //zmienna mowiaca czy zegar ma chodzic (0) czy sie zatrzymac (1)

//stale oznaczajace poszczegolne przyciski, tryby ktore sa obslugiwane
const int HOURS = 0;
const int MINUTES = 1;
const int CHANGE_MODE_RIGHT_COLS = 1;
const int CHANGE_MODE_LEFT_COLS = 2;
const int CHANGE_MODE_DISABLED = 0;
const int BUTTON_CHANGE_MODE = 2;
const int BUTTON_CHANGE_VALUE = 3;

int ONE_CLICK_GUARD = 0; //do zabezpieczania przed wielokrotnym klikiem
int changeMode = 0; //1 zmieniaj kolumny po prawej, 2 zmieniaj kolumny po lewej, 0 pal wszystkie

uint8_t Digits[] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000};
uint8_t Column[] = {0b11111110, 0b11111101, 0b11111011, 0b11110111};

int i = 0; //obecna pozycja w tablicy cyfr
int j = 0; //numer obecnej kolumny na wyswietlaczu 7-segm.

void displayClock(){
	if(displayMode == 1){
		displayMinutesAndSecondsWithChangingValueMode();
	}
	if(displayMode == 0){
		displayHoursAndMinutesWithChangingValueMode();
	}
}


//pobieranie numeru przycisku jaki zostal nacisniety
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
	if(liczba < 0 || liczba > 4) liczba = 0;
	DDRA = 0xFF;
	lastKeyPressed = keyPressed;
	keyPressed = liczba;
	handleChangeMode();
	if(liczba==0){
		ONE_CLICK_GUARD = 1;
	}
}

//zmiana trybu wyswietlanie czasu lub ustawianie czasu
void handleChangeMode(){
	if(keyPressed == BUTTON_CHANGE_MODE && ONE_CLICK_GUARD==1){
		ONE_CLICK_GUARD=0;
		changeMode++;
		changeMode = changeMode % 3;
	}
	if(changeMode == CHANGE_MODE_LEFT_COLS || changeMode == CHANGE_MODE_RIGHT_COLS){
		stopClock =1;
	}
	if(changeMode == CHANGE_MODE_DISABLED){
		stopClock =0;
	}
}

//handler ktory zmienia godziny lub minuty w zaleznosci od trybu
void handleAddValue(){
	
	if(displayMode == HOURS && changeMode == CHANGE_MODE_RIGHT_COLS){
		if(keyPressed == BUTTON_CHANGE_VALUE && ONE_CLICK_GUARD==1){
			minutes++;	
			ONE_CLICK_GUARD=0;		
			if(minutes > 59){
				minutes = minutes % 60;
			}
		}
	}
	if(displayMode == HOURS && changeMode == CHANGE_MODE_LEFT_COLS){
		if(keyPressed == BUTTON_CHANGE_VALUE && ONE_CLICK_GUARD==1){
		hours++;
		ONE_CLICK_GUARD=0;			
		if(hours > 23){
			hours = hours % 24;
		}
		}
	}
	if(displayMode == MINUTES && changeMode == CHANGE_MODE_RIGHT_COLS){
		if(keyPressed == BUTTON_CHANGE_VALUE && ONE_CLICK_GUARD==1){
			seconds++;	
			ONE_CLICK_GUARD=0;			
			if(seconds > 59){
				seconds = seconds % 60;
			}
		}
	}
	if(displayMode == MINUTES && changeMode == CHANGE_MODE_LEFT_COLS){
			if(keyPressed == BUTTON_CHANGE_VALUE && ONE_CLICK_GUARD==1){
				minutes++;
				ONE_CLICK_GUARD=0;	
				if(minutes > 59){
					minutes = minutes % 60;
				}
			}
	}
}

//zmienia tryb wyswietlania z godziny/minuty na minuty/sekundy
void changeDisplayMode(){
	if(keyPressed == 1){
		displayMode++;
		displayMode = displayMode % 2;
	}
}



//pomocnicza funkcja do wyswietlania numeru przycisku nieuzywana w projekcie
void displayKey(){
	PORTB = 0xFF;
	PORTA = Digits[keyPressed];
	PORTB = Column[j];
	i++;
	i = i % 10;
	j++;
	j = j % 4;
}




//zarzadca czasu zegara
void manageClock(){
	if(stopClock == 0){ //jesli zegar ma chodzic to dodaj sekunde
		seconds++;
		if(seconds > 59){
			minutes++;			
		}
		seconds = seconds % 60;
		if(minutes > 59){
			hours++;			
		}
		minutes = minutes % 60;
		//if(hours > 23){
			hours = hours % 24;
		//}				
	}
}
//wyswietlanie godzin i minut w zaleznosci od trybu
void displayHoursAndMinutesWithChangingValueMode(){
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
	j++;
	if(changeMode == CHANGE_MODE_LEFT_COLS){
		j = (j%2)+2;
	}
	if(changeMode == CHANGE_MODE_RIGHT_COLS){
		j = j%2;
	}
	if(changeMode == CHANGE_MODE_DISABLED)	{
		j = j % 4;
	}


}

//wyswietlanie minut i sekund w zaleznosci od trybu
void displayMinutesAndSecondsWithChangingValueMode(){
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
	j++;
	if(changeMode == CHANGE_MODE_LEFT_COLS){
		j = (j%2)+2;
	}
	if(changeMode == CHANGE_MODE_RIGHT_COLS){
		j = j%2;
	}
	if(changeMode == CHANGE_MODE_DISABLED)	{
		j = j % 4;
	}
}

//sprawdzanie czy podany bajt jest potega dwojki
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