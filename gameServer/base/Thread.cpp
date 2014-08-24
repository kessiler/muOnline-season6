#include "StdAfx.h"
#include "Thread.h"
#include "panicerror.h"

using namespace mu;
void nassert(bool cond,const char* cs,int line,const char* file);

Thread::Thread() : m_hThread(0),m_ThreadId(0),m_run(0),m_suspend(0)
{

}

Thread::~Thread()
{
	nassert(this->IsRunning()==0,"!IsRunning()",__LINE__,__FILE__);
}

bool Thread::IsRunning()
{
	return (this->m_run.Get() == 1);
}

bool Thread::Start()
{
	DWORD tid = 0;
	this->m_run = 1;
	this->m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)mu::ThreadFunc,this,0,&tid);

	if( m_hThread == 0 )
	{
		this->m_run = 0;
		return false;
	}
	this->m_ThreadId = tid;
	Sleep(100);
	return true;
}

BOOL Thread::Run()
{
	return false;
}

void Thread::Suspend()
{
	nassert(this->m_hThread!=0,"thread_ != 0",__LINE__,__FILE__);

	if( this->m_hThread == 0 )
		return;
	
	if( SuspendThread(this->m_hThread) < 0 )
	{
		throw PanicError("Failed to suspend thread");
	}

	this->m_suspend = 1;
}

void Thread::Resume()
{
	nassert(this->m_hThread!=0,"thread_ != 0",__LINE__,__FILE__);

	if( this->m_hThread == 0 )
		return;
	
	if( SuspendThread(this->m_hThread) < 0 )
	{
		throw PanicError("Failed to resume thread");
	}

	this->m_suspend = 0;
}

void Thread::Stop()
{
	if( this->m_run.Get() == 0 )
		return;

	if( this->m_hThread == 0 )
		return;

	this->m_run = 0;
	this->helpStop();

	WaitForSingleObject(this->m_hThread,-1);
	CloseHandle(this->m_hThread);
	this->m_hThread = 0;
	this->m_suspend = 0;
}

void Thread::helpStop()
{

}

BOOL mu::ThreadFunc(void* t)
{
	Thread* thread = (Thread*)t;

	if( thread )
	{
		return thread->Run();
	}
	return -1;
}