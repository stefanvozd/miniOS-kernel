#include "thread.h"
#include "lock.h"
#include "pcb.h"
#include "timer.h"



Thread::~Thread() {
	waitToComplete();
	lockContext();
	delete myPCB;
	unlockContext();
}

void Thread::sleep(Time timeToSleep) {
	lockContext();
	Timer::sleep(timeToSleep);
	unlockContext();
}

void Thread::start() {
	lockContext();
	myPCB->start();
	unlockContext();
}

void Thread::waitToComplete() {
	lockContext();
	myPCB->waitToComplete();
	unlockContext();
}

void Thread::wrapper(Thread *running) {
	running->run();
	
	// Exit thread.
	lockContext();
	PCB::exitRunningThread();
	unlockContext();
	
	dispatch();
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lockContext();
	myPCB = new PCB(stackSize, timeSlice, this);
	unlockContext();
}

void dispatch() {
	lock();
	yield();
	unlock();
}

