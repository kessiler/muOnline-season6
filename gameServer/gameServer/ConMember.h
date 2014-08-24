// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------

#ifndef CONMEMBER_H
#define CONMEBMER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConMember
{
public:

	CConMember();
	virtual ~CConMember();

	void Init();
	BOOL IsMember(LPSTR AccountID);
	void Load(LPSTR filename);

private:
	
	std::map<std::string, int> m_szAccount;

};



extern CConMember ConMember;

#endif