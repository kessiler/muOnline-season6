#ifndef TDURMAGICKEYCHECKER_H
#define TDURMAGICKEYCHECKER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_DUR_MAGIC_KEY	60

class TDurMagicKeyChecker
{
public:
	BOOL IsValidDurationTime(BYTE btMagicKey)	// line :15
	{
		if ( btMagicKey <= MAX_DUR_MAGIC_KEY && btMagicKey > 0)
		{
			if ( (GetTickCount() - this->m_dwDurationTime[btMagicKey]) < 15000 )
			{
				return TRUE;
			}
		}
		return FALSE;
	};	// line : 25

	BOOL IsValidCount(BYTE btMagicKey)	// line : 28
	{
		if ( btMagicKey <= MAX_DUR_MAGIC_KEY && btMagicKey > 0)
		{
			if ( this->m_btValidCount[btMagicKey] >= 0 )
			{
				return TRUE;
			}
		}
		return FALSE;
	};	// line : 38

	void SetDurationTime(BYTE btMagicKey, DWORD dwCurrentTickCount)	// line : 41
	{
		if ( btMagicKey <= MAX_DUR_MAGIC_KEY && btMagicKey > 0)
		{
			this->m_dwDurationTime[btMagicKey] = dwCurrentTickCount;
			this->m_btValidCount[btMagicKey] = 7;
		}
	};	// line : 47

	//unsigned char __thiscall DecValidCount(unsigned char);

	int GetValidDurationTime(BYTE btMagicKey)	// line : 63
	{
		if ( btMagicKey <= MAX_DUR_MAGIC_KEY && btMagicKey > 0)
		{
			return this->m_dwDurationTime[btMagicKey];
		}
		return -77;
	};	// line : 70


	int GetValidCount(BYTE btMagicKey)	// line : 73
	{
		if ( btMagicKey <= MAX_DUR_MAGIC_KEY && btMagicKey > 0)
		{		
			return this->m_btValidCount[btMagicKey];
		}
		return -77;
	};	// line : 80

private:
	DWORD m_dwDurationTime[MAX_DUR_MAGIC_KEY];	// 0
	BYTE m_btValidCount[MAX_DUR_MAGIC_KEY];	// F0
};

#endif