//GameServer 1.00.90 finished
#include "stdafx.h"
#include "SkillDelay.h"

CSkillDelay::CSkillDelay()
{
	this->Init();
}


CSkillDelay::~CSkillDelay()
{
	return;
}


void CSkillDelay::Init()
{
	memset(this->LastSkillUseTime, 0, sizeof(this->LastSkillUseTime) );
}


#pragma warning ( disable : 4101 )
int CSkillDelay::Check(BYTE skill)
{
	int skilldelaytime;
	DWORD dwtime;
	return 1;
}
#pragma warning ( default : 4101 )