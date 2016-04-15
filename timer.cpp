#include "timer.h"
#include <dos.h>
#include "schedule.h"
#include "pcb.h"

void tick();

Timer::Elem *Timer::first = 0;
PInterrupt Timer::pOldTimerInterrupt = 0;
char Timer::isInitialized = 0;
Time Timer::time = 0;

void Timer::initialize() {
	lock();
	if (Timer::isInitialized == 0) {
		Timer::first = 0;
		
		Timer::pOldTimerInterrupt = getvect(0x08);

		setvect(0x08, Timer::newTimerInterrupt);
		Timer::isInitialized = 1;
	}
	unlock();
}

void Timer::restore() {
	lock();
	if (Timer::isInitialized != 0) {
		while (first) {
			first->pcb->state = PCB::READY;
			Scheduler::put(first->pcb);
			Elem *old = first;
			first = first->next;
			delete old;
		}
		
		
		setvect(0x08, Timer::pOldTimerInterrupt);
		Timer::isInitialized = 0;
	}
	unlock();
}

void Timer::startCounting(Time time) {
	lock();
	Timer::time = time;
	unlock();
}

void Timer::sleep(Time time) {
	lock();
	Elem *prev = 0, *curr = Timer::first;
	while (curr && curr->time <= time) {
		time -= curr->time;
		prev = curr;
		curr = curr->next;
	}
	
	Elem *elem = new Elem(PCB::running, time);
	
	if (prev)
		prev->next = elem;
	else
		Timer::first = elem;
	
	elem->next = curr;
	if (curr != 0)
		curr->time -= time;
	
	PCB::running->state = PCB::SLEEPING;
	dispatch();
	unlock();
}

void interrupt Timer::newTimerInterrupt(...) {
	(*Timer::pOldTimerInterrupt)();
	tick();
	

	if (Timer::first != 0)
		Timer::first->time--;
	
	while (Timer::first != 0 && Timer::first->time == 0) {
		PCB *pcb = Timer::first->pcb;
		pcb->state = PCB::READY;
		Scheduler::put(pcb);
		
		Elem *old = Timer::first;
		Timer::first = Timer::first->next;
		delete old;
	}
	

	if (PCB::running == PCB::idle) {
		yield();
	}
	else if (PCB::running->time > 0) {
		if (Timer::time > 0) {
			--Timer::time;
		}
		
		if (Timer::time == 0) {
			yield();
		}
	}
}

