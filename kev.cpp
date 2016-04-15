#include "kev.h"
#include "schedule.h"
#include "pcb.h"
#include "ivtentry.h"

#include <iostream.h>

KernelEv::KernelEv(IVTNo ivtNo) {
	this->ivtNo = ivtNo;
	ivtEntry = IVTEntry::getFrom(ivtNo);
	ivtEntry->setKEv(this);
	threadIsBlockedHere = 0;
	ownerThread = PCB::running;
}

KernelEv::~KernelEv() {
	ivtEntry->setKEv(0);
	signal();	
}

void KernelEv::wait() {
	if (ownerThread != PCB::running)
		return;
	
	threadIsBlockedHere = 1;
	ownerThread->state = PCB::BLOCKED;
	if (ivtNo == 9) {
		asm {
			mov al, 20h;
			out 20h, al;
		}
	}
	dispatch();
}

void KernelEv::signal() {
	if (threadIsBlockedHere) {
		threadIsBlockedHere = 0;
		ownerThread->state = PCB::READY;
		Scheduler::put(ownerThread);
	}
}

