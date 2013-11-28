/*
 * ExecutionScheduler.c
 *
 * Created: 2013-11-20 18:20:56
 *  Author: student
 
 1 ustawiamy timer w tct
 2 ustawiamy ocr =250
 3 ustawiamy prescaller na 64
 */

#define size 10 //rozmiar tablicy taskow


#include <avr/io.h>
#include <avr/interrupt.h> 
	
	uint8_t digit[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	uint8_t col[4] ={0b11110111,0b11110011,0b11110001,0b11110000};
	int counter =0;
	int c=0;
	int k=0;m
	typedef void (*Task_PTR)(void); // definicja pointera do funkcji
	int taskCount = 0; 
	
	typedef struct{
		Task_PTR taskPtr;
		uint16_t toGo;
		uint16_t interval;
		uint8_t ready;
	} Task;
	
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
	//funkcja uruchamiana w momencie obslugi przerwania
	void turnOnOff(){
		PORTB =col[c];
		PORTA = digit[k];
		k++;
		k=k%10;
		
		c++;
		c = c%4;
	}
	//obsluga przerwan przerwania
	ISR(TIMER0_COMP_vect){ 
		schedule();
	}

	
	

int main(void)
{
	
	init();
	sei();
	DDRB = 0xFF;
	DDRA = 0xFF;
	
	
	//dodawanie taskow
	addTask(&turnOnOff,1000);

	
	//wlaczenie globalnych przerwan
	 
    while(1)
    {
		execute();
        
    }
}