#ifndef _kev_h_
#define _kev_h_

#include "event.h"

class IVTEntry;
class PCB;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	
	void wait();
	void signal();
	
private:
	IVTNo ivtNo;
	IVTEntry* ivtEntry;
	int threadIsBlockedHere;
	PCB* ownerThread;
	
};

#endif