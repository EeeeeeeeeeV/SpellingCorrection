#ifndef _MUTEXGUARD_H_
#define _MUTEXGUARD_H_

class Mutex;

class MutexGuard
{
	public:
		MutexGuard( Mutex & mutex);
		~MutexGuard();
	private:
		Mutex & m_mutex;
};

#endif
