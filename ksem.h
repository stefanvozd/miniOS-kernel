#ifndef _ksem_h_
#define _ksem_h_

class Queue;

class KernelSem {
public:
	void wait();
	void signal();
	
	int val() const;
	
	KernelSem(int init);
	~KernelSem();
	
private:
	int value;
	Queue *blockedList;
	
};

#endif