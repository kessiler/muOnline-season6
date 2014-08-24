#include "StdAfx.h"
#include "lock.h"
#include "panicerror.h"

using namespace mu;

Mutex::Mutex()
{
	int rc = InitializeCriticalSectionAndSpinCount(&this->m_cs,4000);

	if( rc == 0 )
	{
		throw PanicError("Mutex::Mutex() - Cannot initialize critical section");
	}
}

Lockable::Lockable()
{

}

Lockable::~Lockable()
{

}

PanicError::PanicError(const std::string & msg) : m_panicerror(msg)
{
}

PanicError::~PanicError()
{

}

PanicError::PanicError(const PanicError& rhs) : m_panicerror()
{
	this->m_panicerror = rhs.m_panicerror;
}

Mutex::~Mutex()
{
	DeleteCriticalSection(&this->m_cs);
}

void Mutex::Lock()
{
	EnterCriticalSection(&this->m_cs);
}

void Mutex::Unlock()
{
	DeleteCriticalSection(&this->m_cs);
}