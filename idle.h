#ifndef _idle_h_
#define _idle_h_

#include "thread.h"

#include <iostream.h>

class IdleThread : public Thread {
public:
	IdleThread::IdleThread() : repeat(1) {}
	virtual ~IdleThread() {
		repeat = 0;
	}
	
	virtual void run();
	
private:
	volatile int repeat;
	
};

void IdleThread::run() {
	while (repeat) {
		
	}
}

#endif