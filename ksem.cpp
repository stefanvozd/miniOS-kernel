#include "ksem.h"
#include "queue.h"
#include "schedule.h"


void KernelSem::wait() {
	if (--value < 0) {
		PCB::running->state = PCB::BLOCKED;
		blockedList->put(PCB::running);
		dispatch();
	}
}

void KernelSem::signal() {
	if (value++ < 0) {
		PCB *p = blockedList->take();
		p->state = PCB::READY;
		Scheduler::put(p);
	}
}

int KernelSem::val() const {
	return value;
}

KernelSem::KernelSem(int init) {
	value = init;
	blockedList = new Queue();
}

KernelSem::~KernelSem() {
	while (val() < 0) {
		signal();
	}
	delete blockedList;
}

