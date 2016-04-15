#ifndef _queue_h_
#define _queue_h_

#include "pcb.h"

class PCB;

class Queue {
private:
	class Elem {
	public:
		PCB* pcb;
		Elem* next;
		
		Elem(PCB* p);
	};
	
public:
	Queue();
	~Queue();
	
	void put(PCB* p);
	PCB* take();
	
	int size();
	
private:
	int length;
	Elem* first;
	Elem* last;
	
};

#endif
