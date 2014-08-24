// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef CLOGINCOUNT_H
#define CLOGINCOUNT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoginCount
{

public:

	CLoginCount();
	~CLoginCount();

	void Init();
	void Add();
	void Delete();
	int Get();

private:

	int m_count;
};

#endif