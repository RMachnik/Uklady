/*
 * Scheduler.h
 *
 * Created: 1/15/2014 10:11:35 PM
 *  Author: SG0219139
 */ 



#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#define MAX_TASKS 10

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

typedef void(*TASK_PTR) (void);

struct tsk{
	TASK_PTR t;
	uint16_t ToGo;
	uint16_t Interval;
	uint8_t ready;
};
struct tsk tasks[MAX_TASKS];

void init();
void addTask(TASK_PTR task, uint16_t period);
void addOneShot(TASK_PTR task, uint16_t period);
void schedule();
void execute();
void initCounter();

typedef void(*TASK_PTR) (void);





#endif /* SCHEDULER_H_ */