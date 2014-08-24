// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef SKILLDELAY_H
#define SKILLDELAY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillDelay
{

public:

	CSkillDelay();
	virtual ~CSkillDelay();

	void Init();
	BOOL Check(BYTE skill);

private:

	DWORD LastSkillUseTime[600];	// 4
};

#endif