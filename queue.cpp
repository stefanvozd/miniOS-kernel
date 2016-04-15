#include "queue.h"



Queue::Elem::Elem(PCB* p){
    pcb = p;
    next =0;
}

Queue::Queue(){
	first = last = 0;
	length =0;
}

Queue::~Queue(){
    Elem* old;
	while( first ) {
		old = first;
		first = first->next;
		delete old;
	}          
}


void Queue::put(PCB* p){
	Elem* newElem = new Elem(p);
	if(first == 0)first=last=newElem;
	else{
		last->next = newElem;
		last = newElem;
	}
	length++;
}

PCB* Queue::take(){
	if(first == 0) return 0;
	Elem* oldp = first;
	first = first->next;
	if(first == 0) last = 0;
	length--;
	PCB* p = oldp->pcb;
	delete oldp;
	return p;
}

int Queue::size(){
	return length;
}

