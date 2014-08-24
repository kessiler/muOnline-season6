// KanturuStateInfo.h: interface for the CKanturuStateInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUSTATEINFO_H__A947E484_9C13_41C3_8A7A_C55DCF7A987F__INCLUDED_)
#define AFX_KANTURUSTATEINFO_H__A947E484_9C13_41C3_8A7A_C55DCF7A987F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKanturuStateInfo
{

public:

	CKanturuStateInfo();
	virtual ~CKanturuStateInfo();

	void ResetTimeInfo()
	{
		this->SetStateInfo(0);
		this->SetCondition(0);
		this->SetValue(0);
	}


	void SetStateInfo(int iStateTimeInfo)
	{
		this->m_iState = iStateTimeInfo;
	}

	//int __thiscall GetStateInfo();
	void SetCondition(int iCondition)
	{
		this->m_iCondition = iCondition;
	}


	int GetCondition()
	{
		return this->m_iCondition;
	}

	void SetValue(int iValue)
	{
		this->m_iValue = iValue;
	}

	int GetValue()
	{
		return this->m_iValue;
	}
	void SetAppliedTime()
	{
		this->m_dwAppliedTickCount = GetTickCount();
	}

	int GetAppliedTime()
	{
		return this->m_dwAppliedTickCount;
	}

	//void __thiscall ResetAppliedTime();
	int GetElapsedTime()
	{
		int iElapsedTime = GetTickCount() - this->GetAppliedTime();
		return iElapsedTime;
	}

	int GetRemainTime()
	{
		int iRemainTime = 0;

		if ( this->GetCondition() == 1 )
		{
			iRemainTime = this->m_iValue - this->GetElapsedTime();

			if ( iRemainTime <= 0 )
			{
				return 0;
			}

			iRemainTime /= 1000;
		}

		return iRemainTime;
	}

	BOOL IsTimeOut()
	{
		if ( this->GetRemainTime() <= 0 )
		{
			return TRUE;
		}

		return FALSE;
	}


private:

	int m_iState;	// 4
	int m_iCondition;	// 8
	int m_iValue;	// C
	DWORD m_dwAppliedTickCount;	// 10

};

#endif // !defined(AFX_KANTURUSTATEINFO_H__A947E484_9C13_41C3_8A7A_C55DCF7A987F__INCLUDED_)
