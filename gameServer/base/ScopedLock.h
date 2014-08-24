#pragma once

#include "noncopyable.h"
#include "lock.h"

namespace mu
{
	class ScopedLock : Mutex
	{
	public:
		ScopedLock(mu::Mutex &lock);
		~ScopedLock();

		void Lock();
		void Unlock();
	protected:
		Mutex& m_mutex;
	};
}