/*
 * Clock.h
 *
 * Created: 1/15/2014 10:29:19 PM
 *  Author: SG0219139
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int isPowerOfTwo(unsigned n);
int findPosition(unsigned n);
void getKeyNumber();
void changeMode();
void displayKey();
void displayMinutesAndSeconds();
void displayHoursAndMinutes();
void displayClock();
void manageClock();
void setMinsAndSecs();
void setHoursAndMins();


#endif /* CLOCK_H_ */