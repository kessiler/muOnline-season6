//-> Decompiled by DarkSim | 23.01.2013 | 1.01.00 GS-N (All names are original)
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#include "..\common\ItemSocketOptionDefine.h"
// -------------------------------------------------------------------------------

class CSeedItem	//-> Complete
{
public:
	CSeedItem();
	virtual ~CSeedItem();
	// ----
	void	Clear();
	bool	IsMatched(CItem * lpItem);
	// ----
	void	SetOption(BYTE SeedIndex, BYTE SeedType, BYTE ItemLevel, BYTE SeedOptionValueType, LPBYTE SeeedOptionValue, int ExtractRate);
	// ----
public:
	BYTE	m_SeedIndex;	//+0
	BYTE	m_SeedType;		//+1
	BYTE	m_ItemLevel;	//+2
	BYTE	m_SeedOptionValueType;	//+3
	BYTE	m_SeedOptionValue[MAX_SOCKET_SLOT];	//+4
	int		m_ItemCode;	//+12
	int		m_SeedExtractRate;	//+16
};
// -------------------------------------------------------------------------------

class CSphereItem //-> Complete
{
public:
	CSphereItem();
	virtual ~CSphereItem();
	// ----
	void	Clear();
	// ----
	void	SetOption(BYTE SphereLevel, int ValueIncreaseRate);
	// ----
public:
	BYTE	m_SphereLevel;	//+0 (maybe [] or position after SetOption?)
	int		m_ValueIncreaseRate;	//+4
	int		m_ItemCode;	//+8
};
// -------------------------------------------------------------------------------

class CSeedSphereItem //-> Complete
{
public:
	CSeedSphereItem();
	virtual ~CSeedSphereItem();
	// ----
	int		GetItemCode();
	// ----
	void	Clear();
	// ----
	bool	SetOption(CSeedItem * SeedItem, BYTE SphereLevel);
	void	AddOptionValue(BYTE btOptionValue);
	// ----
public:
	BYTE	m_SeedSphereIndex;	//+0
    BYTE	m_SeedIndex;	//+1
    BYTE	m_SeedType;	//+2
    int		m_ItemCode;	//+4
    BYTE	m_ItemLevel;	//+8
    BYTE	m_SeedOptionValueType;	//+9
    WORD	m_SeedOptionValue;	//+10
    BYTE	m_SphereLevel;	//+12
};
// -------------------------------------------------------------------------------