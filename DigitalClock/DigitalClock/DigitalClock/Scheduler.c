/*
 * Scheduler.c
 *
 * Created: 1/15/2014 10:13:59 PM
 *  Author: SG0219139
 */

#include "Scheduler.h"

int tasks_num = 0;

void init(){

	for(int i = 0; i < MAX_TASKS; i++){
		tasks[i].Interval = 0;
		tasks[i].ready = 0;
		tasks[i].t = 0;
		tasks[i].ToGo = 0;
	}
}

void initCounter(){
		// prescaler na 64
		OCR0 = 250;
		TCCR0 |= (0 << WGM00) | (1 << WGM01) | (0 << CS02) | (1 << CS01) | (1 << CS00) | (0 << COM01) | (0 << COM00);
		TIMSK |= (1 << TOIE0) | (1 << OCIE0);

		DDRA = 0xFF;
		DDRB = 0xFF;
		//PORTC = 0b000000100; //to niepotrzebne
}

void addTask(TASK_PTR task, uint16_t period){
	if(tasks_num < MAX_TASKS){
		tasks[tasks_num].Interval = period;
		tasks[tasks_num].ToGo = period;
		tasks[tasks_num].t = task;
		tasks_num++;
	}
}

void addOneShot(TASK_PTR task, uint16_t period){
	if(tasks_num < MAX_TASKS){
		tasks[tasks_num].Interval = 0;
		tasks[tasks_num].ToGo = period;
		tasks[tasks_num].t = task;
		tasks_num++;
	}
}

//wykonywana w przerwaniu
void schedule(){
	//int i = 0;
	for(int i = 0; i < MAX_TASKS; i++){
		//jesli task nie jest pusty zmiejszamy ToGo
		if(tasks[i].t != 0){
			tasks[i].ToGo--;
			//jesli ToGo = 0 zwiekszamy ready (periodyczny)
			if(tasks[i].ToGo == 0 && tasks[i].Interval != 0){
				tasks[i].ready++;
				tasks[i].ToGo = tasks[i].Interval;
			}
			//(nie jest periodyczny)
			else if(tasks[i].ToGo == 0 && tasks[i].Interval == 0){
				tasks[i].ready++;
			}
		}
	}
}

void execute(){
	for(int i=0; i < MAX_TASKS; i++){
		if(tasks[i].ready > 0){
			tasks[i].t();
			tasks[i].ready--;
			//usuwanie taska jesli zostal wykonany i nie jest periodyczny
			if(tasks[i].ready == 0 && tasks[i].Interval == 0){
				tasks[i].Interval = 0;
				tasks[i].ToGo = 0;
				tasks[i].t = 0;
			}
		}
	}
}


ISR(TIMER0_COMP_vect){
	schedule();
}