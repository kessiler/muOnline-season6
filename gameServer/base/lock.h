#pragma once

namespace mu
{
	class Lockable
	{
	public:
		Lockable();
		virtual ~Lockable();

		CRITICAL_SECTION m_cs;
	};

	class Mutex : public Lockable
	{
	public:
		Mutex();
		virtual ~Mutex();

		void Lock();
		void Unlock();
	};
}