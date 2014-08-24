#include "StdAfx.h"
#include "ScopedLock.h"

using namespace mu;

ScopedLock::ScopedLock(Mutex& lock) : m_mutex(lock)
{
	this->m_mutex.Lock();
}

ScopedLock::~ScopedLock()
{
	this->m_mutex.Unlock();
}