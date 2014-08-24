// GS-CS 1.00.90	JPN - Completed
#include "stdafx.h"
#include "CLoginCount.h"

CLoginCount::CLoginCount()
{
	this->Init();
}

CLoginCount::~CLoginCount()
{
	return;
}

void CLoginCount::Init()
{
	this->m_count=0;
}

void CLoginCount::Add()
{
	this->m_count++;
}

void CLoginCount::Delete()
{
	this->m_count--;

	if ( this->m_count < 0 )
	{
		this->m_count = 0;
	}
}

int CLoginCount::Get()
{
	return this->m_count;
}

