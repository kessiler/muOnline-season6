// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once


typedef struct _stRewardExpInfo
{
	int MinUserLevel;
	int MaxUserLevel;
	int RewardExp;
	_stRewardExpInfo()
	{
		MinUserLevel = 0;
		MaxUserLevel = 0;
		RewardExp = 0;
	}
}stRewardExpInfo;

class CImperialGuardianRewardExp
{
public:
	CImperialGuardianRewardExp(void);
	virtual ~CImperialGuardianRewardExp(void);

	void Init();
	void LoadScript(const char *lpFileName);
	int GetRewardExp(int nUserIndex);
private:
	bool m_bLoadScript;
	std::vector<stRewardExpInfo> m_vtRewardExpInfo;
};

