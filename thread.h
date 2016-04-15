// File: thread.h
#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	virtual ~Thread();
	
	static void sleep(Time timeToSleep);
	
	void start();
	void waitToComplete();
	
protected:
	friend class PCB;
	
	
	static void wrapper(Thread *running);
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;
	
};

void dispatch ();

#endif