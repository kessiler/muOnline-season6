// CrywolfAltar.h: interface for the CCrywolfAltar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFALTAR_H__4D5C2361_CB6E_4B57_8509_CB4BD40525D7__INCLUDED_)
#define AFX_CRYWOLFALTAR_H__4D5C2361_CB6E_4B57_8509_CB4BD40525D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_CRYWOLF_ALTAR	5
#define MAX_ALTAR_CONTRACTS	2

#define CRYWOLF_ALTAR_CLASS_RANGE(x)  ( ((x)< 205 )?FALSE:((x) > 209 )?FALSE:TRUE   ) 
#define MIN_ELF_LEVEL_ALTAR	260

class CCrywolfAltarInfo
{

public:

	CCrywolfAltarInfo()
	{
		this->Reset();
	}


	void Reset()
	{
		this->m_iAltarState = 0;
		this->m_iUserIndex = -1;
		this->m_iAltarIndex = -1;
		this->m_iContractCount = 0;
		this->m_dwAppliedContractTime =0 ;
		this->m_dwValidContractTime =0 ;
		this->m_dwLastValidContractTime =0 ;
	}

	int GetRemainContractCount()
	{
		int iRemainCount = MAX_ALTAR_CONTRACTS - this->m_iContractCount;
		return (iRemainCount<0)?0:iRemainCount;
	}

	void SetAltarState(int iState)
	{
		this->m_iAltarState = iState;
	}

	int GetAltarState()
	{
		return this->m_iAltarState;
	}

	void SetAltarIndex(int iAltarIndex)
	{
		this->m_iAltarIndex = iAltarIndex;
	}

	//int __thiscall GetAltarIndex();
	//int __thiscall GetAltarUserIndex();

public:

	int m_iAltarState;	// 0
	int m_iAltarIndex;	// 4
	int m_iUserIndex;	// 8
	int m_iContractCount;	// C
	int m_dwAppliedContractTime;	// 10
	int m_dwValidContractTime;	// 14
	int m_dwLastValidContractTime;	// 18
};

class CCrywolfAltar  
{

public:

	CCrywolfAltar();
	virtual ~CCrywolfAltar();

	void CrywolfAltarAct(int iIndex);
	void SetAltarObjIndex(int iClass, int iObjIndex);
	void SetAllAltarObjectIndex();
	int GetAlatarNumber(int iClass);
	void SetAltarValidContract(int iAltarObjIndex, int iClass);
	BOOL CheckAltarValidContract(int iClass);
	int GetAltarState(int iClass);
	int GetAltarRemainContractCount(int iClass);
	BOOL SetAltarUserIndex(int iAltarObjIndex, int iClass, int iUserIndex);
	void ResetAltarUserIndex(int iAltarObjIndex, int iClass);
	int GetAltarUserIndex(int iClass);
	int GetContractedAltarCount();
	int GetPriestHPSum();
	int GetPriestMaxHPSum();
	void ResetAllAltar();
	void ResetAltar(int iClass);
	void SetAllAltarViewState(int iAltarState);
	void SetAltarViewState(int iClass, int iAltarState);

private:

	CCrywolfAltarInfo m_AltarInfo[MAX_CRYWOLF_ALTAR];	// 4
};


extern CCrywolfAltar g_CrywolfNPC_Altar;

#endif // !defined(AFX_CRYWOLFALTAR_H__4D5C2361_CB6E_4B57_8509_CB4BD40525D7__INCLUDED_)
