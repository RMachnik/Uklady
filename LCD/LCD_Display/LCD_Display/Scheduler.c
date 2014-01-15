/*
 * Scheduler.c
 *
 * Created: 1/8/2014 9:24:27 PM
 *  Author: SG0219139
 */ 
#include "Scheduler.h"


	int counter =0;

	
	int taskCount = 0;
	
	
	
	Task tasksTab[size]; //tablica taskow
	
	//dodawanie pojedynczego tasku wykonujacego sie cyknicznie co okreslony czas - period
	void addTask(Task_PTR t,int period){
		if(taskCount<size){
			
			tasksTab[taskCount].toGo = period;
			tasksTab[taskCount].interval = period;
			tasksTab[taskCount].ready = 0;
			tasksTab[taskCount].taskPtr = t;
			taskCount++;
		}
		
	}
	//task ktory wykonuje sie jedynie raz w momencie gdy uplynie okreslony czas - period
	void addOneShot(Task_PTR t,int period){
		if(taskCount<size){
			tasksTab[taskCount].toGo = period;
			tasksTab[taskCount].interval = 0;
			tasksTab[taskCount].ready = 0;
			tasksTab[taskCount].taskPtr = t;
			taskCount++;
		}
	}
	
	//scheduler uruchamia sie podczas obslugi przerwania jego czas dzialania to 1ms
	//funkcja nadzoruje liczniki czasu jaki czekaja poszczegolne zadania
	void schedule(){
		int k=0;
		//ten ustawia taskom ready tym ktore interval maja juz 0
		for(k=0;k<size;k++){
			tasksTab[k].toGo--;
			if(tasksTab[k].toGo==0){
				tasksTab[k].ready++;
				tasksTab[k].toGo = tasksTab[k].interval;
			}
		}
		
	}
	
	//egzekutor ktory uruchamia poszczegolne zadania
	void execute(){
		int cur = 0;
		for(cur=0;cur<size;cur++){
			if(tasksTab[cur].ready>0 && tasksTab[cur].interval>=0){
				tasksTab[cur].toGo = tasksTab[cur].interval;
				tasksTab[cur].taskPtr();
				tasksTab[cur].ready--;
				cur = 0;
				//ustawianie interwalu na -1 dla oneShotTask aby nie uruchamiac go ponownie
				if(tasksTab[cur].interval == 0){
					tasksTab[cur].interval = -1;
				}
			}
		}
	}
	
	
	//inicjalizacja licznik
	void init(){
		
		TCCR0 |= (0<<WGM00) | (1<<WGM01); //ustawianie flag
		TCCR0 |=(0<<COM00) | (0<<COM01); // ustawiamy flagi dla TCCRO
		TCCR0 |=(1<<CS00) | (1<<CS01) | (0<<CS02);
		TIMSK |=(1<<OCIE0);
		OCR0 = 250;
		int i=0;
		for(i=0;i<size;i++){
			tasksTab[i].interval = 0;
			tasksTab[i].toGo = 0;
			tasksTab[i].ready=0;
		}
	}
	
		//obsluga przerwan przerwania
		int ISR(TIMER0_COMP_vect){
			schedule();
			return 0;
		}