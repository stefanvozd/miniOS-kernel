#include "ivtentry.h"
#include "kev.h"
#include "thread.h"
#include "lock.h"
#include <dos.h>


IVTEntry* ivtEntries[256] = { 0 };

IVTEntry* IVTEntry::getFrom(IVTNo ivtNo) {
	return ivtEntries[ivtNo];
}

void IVTEntry::setKEv(KernelEv *kev) {
	lock();
	this->kev = kev;
	unlock();
}

IVTEntry::IVTEntry(int flagForIntOld, IVTNo ivtNo, PInterrupt intNew) {
	lock();
	this->ivtNo = ivtNo;
	intOld = getvect(ivtNo);
	setvect(ivtNo, intNew);
	kev = 0;
	this->flagForIntOld = flagForIntOld;
	
	ivtEntries[ivtNo] = this;
	unlock();
}

IVTEntry::~IVTEntry() {
	lock();
	setvect(ivtNo, intOld);
	
	ivtEntries[ivtNo] = 0;
	if (ivtNo == 9) {
		asm {
			mov al, 20h;
			out 20h, al;
		}
	}
	unlock();
}

void IVTEntry::signal() {
	if (flagForIntOld)
		(*intOld)();
	
	if (kev)
		kev->signal();
	
	dispatch();
}

