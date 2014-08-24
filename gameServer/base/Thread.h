#pragma once

#include "Atomic.h"
#include "noncopyable.h"

namespace mu
{
	class Thread : public Noncopyable
	{
	public:
		Thread();
		virtual ~Thread();

		bool IsRunning();
		bool Start();
		virtual BOOL Run();
		void Suspend();
		void Resume();
		void Stop();

	protected:
		virtual void helpStop();

	private:
		HANDLE	m_hThread;
		DWORD	m_ThreadId;

		Atomic<long> m_run;
		Atomic<long> m_suspend;
	};

	BOOL ThreadFunc(void* t);
}