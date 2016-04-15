#ifndef _event_h_
#define _event_h_

#include "ivtentry.h"

#define PREPAREENTRY(ivtNo, flagForIntOld) \
void interrupt intNew##ivtNo(...); \
IVTEntry ivtEntry##ivtNo(flagForIntOld, ivtNo, intNew##ivtNo); \
void interrupt intNew##ivtNo(...) { \
	ivtEntry##ivtNo.signal(); \
}

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event(IVTNo ivtNo);
	~Event();
	
	void wait();
	
protected:
	void signal();
	
private:
	KernelEv *myImpl;
	
};

#endif