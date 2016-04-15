#include "event.h"
#include "lock.h"
#include "kev.h"


Event::Event(IVTNo ivtNo) {
	lock();
	myImpl = new KernelEv(ivtNo);
	unlock();
}

Event::~Event() {
	lock();
	delete myImpl;
	unlock();
}

void Event::wait() {
	lock();
	myImpl->wait();
	unlock();
}

void Event::signal() {
	lock();
	myImpl->signal();
	unlock();
}

