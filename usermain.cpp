#include "thread.h"
#include "semaphor.h"
#include "event.h"

#include <iostream.h>

PREPAREENTRY(9,1);

static Semaphore *mutex = 0;

void tick() {}

int i;

class Znak : public Thread {
public:
	Znak(char c, int n) : Thread(), c(c), n(n) {}
	virtual ~Znak() { waitToComplete(); }
	
	virtual void run();
	
private:
	char c;
	int n;
	
};

void Znak::run() {
	for (int i = 0; i < n; i++) {
		mutex->wait();
		cout << c;
		mutex->signal();
		
		for (int j = 0; j < 10000; j++)
			for (int k = 0; k < 20000; k++);
		
//		dispatch();
	}
	
	mutex->wait();
	cout << endl << c << " zavrsio" << endl;
	mutex->signal();
}

class Key : public Thread {
public:
	Key(int n) : Thread(), n(n) {}
	virtual ~Key() { waitToComplete(); }
	
	void run();
	
private:
	int n;
	
};

void Key::run() {
	Event event9(9);
	
	for (int i = 0; i < n; i++) {
		mutex->wait();
		cout << endl << "key wait " << (i + 1) << endl;
		mutex->signal();
		
		event9.wait();
		
		mutex->wait();
		cout << endl << "key signal " << (i + 1) << endl;
		mutex->signal();
	}
	
	mutex->wait();
	cout << endl << "key zavrsio" << endl;
	mutex->signal();
}



int userMain(int argc, char *argv[]) {
	mutex = new Semaphore(1);
	
	Znak *a = new Znak('a', 10);
	Znak *b = new Znak('b', 15);
	Znak *c = new Znak('c', 20);
//	Znak *d = new Znak('d', 25);
//	Znak *e = new Znak('e', 30);
	Key *k = new Key(20);
	
	a->start();
	b->start();
	c->start();
//	d->start();
//	e->start();
	k->start();
	
	delete a;
	delete b;
	delete c;
//	delete d;
//	delete e;
	delete k;
	
	mutex->wait();
	cout << endl << "userMain zavrsio" << endl;
	mutex->signal();
	
	delete mutex;
	
	return 0;
}

