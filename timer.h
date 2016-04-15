#ifndef _timer_h_
#define _timer_h_

#include "lock.h"
#include "thread.h"

class Timer {
public:
	static void initialize();
	static void restore();
	
	static void startCounting(Time time);
	
	static void sleep(Time time);
	
private:
	struct Elem {
		PCB *pcb;
		Time time;
		struct Elem *next;
		
		Elem(PCB *pcb, Time time) {
			this->pcb = pcb;
			this->time = time;
			this->next = 0;
		}
	};
	
	static Elem *first;
	
	static PInterrupt pOldTimerInterrupt;
	static void interrupt newTimerInterrupt(...);
	static char isInitialized;
	static Time time;
	
	Timer() {}
	
};

#endif