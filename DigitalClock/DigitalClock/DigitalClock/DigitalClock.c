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

        addTask(getKeyNumber, 200); //task do wykrywania nacisnietego klawisza, trzeba przemyslec co ile wykonywac zeby dobrze dzialalo podswietlanie i zeby przycisk reagowal szybko
        addTask(manageClock, 1000); //task odpowiadajacy za odliczanie czasu
        addTask(displayClock, 5); //task wyswietlajacy na wyswietlaczu 7-segm.
        addTask(changeMode, 1000); //task do zmiany trybu wyswietlania HH:MM / MM:SS
		addTask(setMinsAndSecs, 200); //*task do zmiany minut i sekund
		addTask(setHoursAndMins, 200); //*task do zmiany godzin i minut
		
        sei();
        while(1){
                execute();
        }
        return 0;
}