// TStatistics.h: interface for the TStatistics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSTATISTICS_H__7D95F743_60FD_49DB_99CD_16AB68F8EEDC__INCLUDED_)
#define AFX_TSTATISTICS_H__7D95F743_60FD_49DB_99CD_16AB68F8EEDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum MONSTER_AI_STATE_TRANSITION_TYPE
{
	MAI_STATE_TRANS_NO_ENEMY = 0x0,
	MAI_STATE_TRANS_IN_ENEMY = 0x1,
	MAI_STATE_TRANS_OUT_ENEMY = 0x2,
	MAI_STATE_TRANS_DIE_ENEMY = 0x3,
	MAI_STATE_TRANS_DEC_HP = 0x4,
	MAI_STATE_TRANS_DEC_HP_PER = 0x5,
	MAI_STATE_TRANS_IMMEDIATELY = 0x6,
	MAI_STATE_TRANS_AGRO_UP = 0x7,
	MAI_STATE_TRANS_AGRO_DOWN = 0x8,
	MAI_STATE_TRANS_GROUP_SOMMON = 0xa,
	MAI_STATE_TRANS_GROUP_HEAL = 0xb,
};


enum MONSTER_AI_STATE_TRANSITION_VALUE_TYPE
{
	MAI_STATE_TRANS_VALUE_ = 0x0,
};



class TMonsterAIState
{

public:

	TMonsterAIState()
	{
		this->Reset();
	}

	void Reset()
	{
		this->m_iPriority = -1;
		this->m_iCurrentState = -1;
		this->m_iNextState = -1;
		this->m_iTransitionType = -1;
		this->m_iTransitionRate = -1;
		this->m_iTransitionValueType = -1;
		this->m_iTransitionValue = -1;
		this->m_iDelayTime = -1;
		memset(this->m_szStateTransitionDesc, 0, sizeof(this->m_szStateTransitionDesc));
	}

public:

	int m_iPriority;	// 0
	int m_iCurrentState;	// 4
	int m_iNextState;	// 8
	int m_iTransitionType;	// C
	int m_iTransitionRate;	// 10
	int m_iTransitionValueType;	// 14
	int m_iTransitionValue;	// 18
	int m_iDelayTime;	// 1C
	char m_szStateTransitionDesc[100];	// 20

};

#endif // !defined(AFX_TSTATISTICS_H__7D95F743_60FD_49DB_99CD_16AB68F8EEDC__INCLUDED_)
