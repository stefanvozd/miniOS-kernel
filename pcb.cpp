#include "pcb.h"
#include <dos.h>
#include "schedule.h"
#include "ksem.h"
#include "timer.h"

static unsigned int ssold, spold, ssnew, spnew, runseg, runoff, wrapseg, wrapoff;

PCB *PCB::running = 0;
PCB *PCB::main = 0;
PCB *PCB::idle = 0;
PCB *PCB::system = 0;

int PCB::nextID = 1;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *thread) {
	myID = nextID++;
	if (myID == 2)
		PCB::idle = this;
	
	time = timeSlice;
	
	state = NEW;
	
	finishedSem = new KernelSem(0);
	
	size = stackSize;
	if (size > 65536UL)
		size = 65536UL;
	
	stack = new char[size];
	
	
	runseg = FP_SEG(thread);
	runoff = FP_OFF(thread);
	
	
	wrapseg = FP_SEG(Thread::wrapper);
	wrapoff = FP_OFF(Thread::wrapper);
	
	ssnew = FP_SEG(stack + size);
	spnew = FP_OFF(stack + size);
	
	ssold = _SS;
	spold = _SP;
	_SS = ssnew;
	_SP = spnew;
	
	
	asm{
		push runseg;
		push runoff;
		
		sub sp, 4;
		
		mov ax, 200h;
		push ax;
		push wrapseg;
		push wrapoff;
		
		sub sp, 18;
	}
	
	ssnew = _SS;
	spnew = _SP;
	_SS = ssold;
	_SP = spold;
	
	this->ss = ssnew;
	this->sp = spnew;
}

PCB::~PCB() {
	delete[] stack;
	delete finishedSem;
}

void PCB::start() {
	if (state == NEW && this != PCB::idle) {
		Scheduler::put(this);
		state = READY;
	}
}

void PCB::waitToComplete() {
	if (PCB::running != this && PCB::idle != this && this->state != PCB::FINISHED && this->state != PCB::NEW) {
		this->finishedSem->wait();
	}
}

void PCB::exitRunningThread() {
	while (PCB::running->finishedSem->val() < 0) {
		PCB::running->finishedSem->signal();
	}
	
	PCB::running->state = PCB::FINISHED;
}

void interrupt yield() {

	if (lockFlag) {
		dispatchCalled = 1;
		return;
	}
	
	// Definisano u dos.h.
	PCB::running->ss = _SS;
	PCB::running->sp = _SP;
	
	if (PCB::running->state == PCB::READY)	
		Scheduler::put(PCB::running);
	PCB::running = Scheduler::get();
	if (PCB::running == 0)
		PCB::running = PCB::idle;
	
	Timer::startCounting(PCB::running->time);
	
	_SS = PCB::running->ss;
	_SP = PCB::running->sp;
	

}

