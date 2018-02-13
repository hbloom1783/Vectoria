#include "Threading.h"

#include <assert.h>

namespace Threads
{
	#pragma region Mutex

	Mutex::Mutex()
	{
		this->mutexHandle = PTHREAD_MUTEX_INITIALIZER;
	}

	void Mutex::Lock()
	{
		pthread_mutex_lock(&(this->mutexHandle));
	}

	void Mutex::Unlock()
	{
		pthread_mutex_unlock(&(this->mutexHandle));
	}

	#pragma endregion

	#pragma region LockHold

	LockHold::LockHold(Lockable& lockable) : lockable(lockable)
	{
		lockable.Lock();
	}

	LockHold::~LockHold()
	{
		lockable.Unlock();
	}

	#pragma endregion

	#pragma region Thread

	Thread::Thread()
	{
		this->threadIsRunning = false;
	}

	void Thread::Start()
	{
		LockHold hold(stateGuard);

		assert(!threadIsRunning);

		pthread_create(
			&(this->threadHandle),
			NULL, // attr
			Thread::Hook,
			this);

		threadIsRunning = true;
	}

	void Thread::Stop()
	{
		LockHold hold(stateGuard);

		assert(threadIsRunning);

		pthread_cancel((this->threadHandle));

		threadIsRunning = false;
	}

	void Thread::Wait()
	{
		LockHold hold(stateGuard);

		assert(threadIsRunning);

		pthread_join(
			this->threadHandle,
			NULL); // return value (?)

		threadIsRunning = false;
	}

	void Thread::StopGraceful()
	{
	}

	void* Thread::Hook(void* arg)
	{
		Thread* fakeThis = (Thread*)arg;

		fakeThis->Execute();

		return NULL;
	}

	#pragma endregion
}