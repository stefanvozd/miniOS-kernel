#include "pcb.h"
#include "lock.h"
#include "idle.h"
#include "schedule.h"
#include "timer.h"

int lockFlag = 0;
int dispatchCalled = 0;

int userMain(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	// Init.
	
	
	PCB::main = new PCB(defaultStackSize, defaultTimeSlice, 0);
	PCB::main->start();
	PCB::running = Scheduler::get();
	
	IdleThread *idleThread = new IdleThread();
	idleThread->start();
	
	Timer::initialize();
	
	// Call userMain.
	int retval = userMain(argc, argv);
	
	// Restore.
	Timer::restore();
	delete idleThread;
	delete PCB::main;
	
	return retval;
}

