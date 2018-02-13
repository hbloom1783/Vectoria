#pragma once

#define HAVE_STRUCT_TIMESPEC
#include "pthread.h"

namespace Threads
{
	class Lockable
	{
	public:
		virtual void Lock() = 0;
		virtual void Unlock() = 0;
	};

	class Mutex : public Lockable
	{
	public:
		Mutex();
		virtual void Lock();
		virtual void Unlock();

	private:
		pthread_mutex_t mutexHandle;

		Mutex(Mutex& other) = delete;
		Mutex& operator=(Mutex& other) = delete;
	};

	class LockHold
	{
	public:
		LockHold(Lockable& lockable);
		~LockHold();

	private:
		Lockable& lockable;

		LockHold(LockHold& other) = delete;
		LockHold& operator=(LockHold& other) = delete;
	};

	class Thread
	{
	public:
		Thread();
		void Start();
		void Stop();
		void Wait();
		virtual void StopGraceful();

	private:
		pthread_t threadHandle;
		bool threadIsRunning;
		Mutex stateGuard;

		virtual void Execute() = 0;
		static void* Hook(void* arg);
	};
}