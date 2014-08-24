// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef VIEWPORTGUILD_H
#define VIEWPORTGUILD_H

#include "user.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CViewportGuild
{

public:

	CViewportGuild();
	virtual ~CViewportGuild();

	void Init();
	BOOL Add(int number, int TargetObj);

private:

	short m_GuildNumber[MAX_VIEWPORT];
	short m_GuildCount;
};

#endif