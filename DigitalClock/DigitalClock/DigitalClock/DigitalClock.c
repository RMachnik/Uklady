/*
 * DigitalClock.c
 *
 * Created: 1/15/2014 10:09:59 PM
 */ 




#include "Scheduler.h"
#include "Clock.h"

int main(void)
{


        init();
		initCounter();

        addTask(getKeyNumber, 1000);
        addTask(manageClock, 1000);
        addTask(displayClock, 5);
        addTask(changeMode, 1000);
        sei();
        while(1){
                execute();
        }
        return 0;
}