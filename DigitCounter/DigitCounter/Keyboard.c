/*
 * Keyboard.c
 *
 * Created: 1/12/2014 5:33:24 PM
 *  Author: SG0219139
 */ 
#include "Scheduer.h"


// A utility function to check whether n is power of 2 or not
int isPowerOfTwo(unsigned n)
{  return n && (! (n & (n-1)) ); }
	
// Returns position of the only set bit in 'n'
int findPosition(unsigned n)
{
	if (!isPowerOfTwo(n))
	return -1;
	
	unsigned count = 0;
	
	// One by one move the only set bit to right till it reaches end
	while (n)
	{
		n = n >> 1;
		
		// increment count of shifts
		++count;
	}
	
	return count;
}


int getKeyNumber(){
	int i=0;
	PORTC = 0x00;
	DDRC = 0x0F; // output dla czesci wierszy
	PORTC = 0xF0;
	int wiersz  = findPosition((PORTC & 0xF0));
	for(i=0;i<8;i++){
		
	}
	PINC = 0;
	PORTC = 0x00;
	DDRC = 0xF0; //output dla czesci kolumnowej
	PORTC =	0x0F;
	for(i=0;i<8;i++){
		
	}
	int kolumna = findPosition((PORTC & 0x0F));
	int liczba = (wiersz * 4) + kolumna;
	return liczba; 
}

