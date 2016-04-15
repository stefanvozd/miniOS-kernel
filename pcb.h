#ifndef _pcb_h_
#define _pcb_h_

class KernelSem;

#include "thread.h"

class PCB {
public:
	PCB(StackSize stackSize, Time timeSlice, Thread *thread);
	~PCB();
	
	void start();
	void waitToComplete();
	
	static void exitRunningThread();
	
private:
	friend interrupt void yield();
	friend class KernelSem;
	friend int main(int argc, char *argv[]);
	friend class Timer;
	friend class KernelEv;
	
	static PCB *running;
	static PCB *main;
	static PCB *idle;
	static PCB *system;
	
	static int nextID;
	int myID;
	
	Time time;
	
	KernelSem *finishedSem;
	
	enum ThreadState { NEW, SLEEPING, READY, BLOCKED, FINISHED };
	ThreadState state;
	

	unsigned int ss, sp;
	unsigned char *stack;
	StackSize size;
	
};

void interrupt yield();

#endif