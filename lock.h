#ifndef _lock_h_
#define _lock_h_

#define lock()		\
asm{				\
	pushf;			\
	cli;			\
}

#define unlock()	\
asm{				\
	popf;			\
}

extern int lockFlag;
extern int dispatchCalled;
void dispatch();

#define lockContext() 	\
lockFlag = 1;

#define unlockContext() \
lockFlag = 0;			\
if (dispatchCalled) 	\
	dispatch();


typedef void interrupt (*PInterrupt) (...);

#endif