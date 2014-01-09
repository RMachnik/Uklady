/*
 * Scheduer.h
 *
 * Created: 1/8/2014 9:21:04 PM
 *  Author: SG0219139
 */ 


#ifndef SCHEDUER_H_
#define SCHEDUER_H_
#include <avr/io.h>
#define size 10 //rozmiar tablicy taskow

typedef void (*Task_PTR)(void); // definicja pointera do funkcji

typedef struct{
	Task_PTR taskPtr;
	uint16_t toGo;
	uint16_t interval;
	uint8_t ready;
} Task;

void addTask(Task_PTR t,int period);
void addOneShot(Task_PTR t,int period);
void schedule();
void execute();
void init();

#endif /* SCHEDUER_H_ */