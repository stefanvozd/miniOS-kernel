#ifndef _ivtentry_h_
#define _ivtentry_h_

#include "lock.h"

typedef unsigned char IVTNo;

class KernelEv;

class IVTEntry {
public:
	static IVTEntry* getFrom(IVTNo ivtNo);
	
	void setKEv(KernelEv *kev);
	
	IVTEntry(int flagForIntOld, IVTNo ivtNo, PInterrupt intNew);
	~IVTEntry();
	
	void signal();
	
private:
	PInterrupt intOld;
	IVTNo ivtNo;
	KernelEv *kev;
	int flagForIntOld;
	
};

#endif