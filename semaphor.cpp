#include "semaphor.h"
#include "lock.h"
#include "ksem.h"

Semaphore::Semaphore(int init) {
	lockContext();
	myImpl = new KernelSem(init);
	unlockContext();
}

Semaphore::~Semaphore() {
	lockContext();
	delete myImpl;
	unlockContext();
}

void Semaphore::wait() {
	lockContext();
	myImpl->wait();
	unlockContext();
}

void Semaphore::signal() {
	lockContext();
	myImpl->signal();
	unlockContext();
}

int Semaphore::val() const {
	lockContext();
	int retval = myImpl->val();
	unlockContext();
	return retval;
}

