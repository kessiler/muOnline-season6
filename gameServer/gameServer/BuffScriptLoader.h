#pragma once

#define MAX_BUFF_EFFECT 255
#define MAX_STATE_COUNT	16

typedef struct
{
	BYTE btIndex;			// 0x00 - CBuffEffect + 0x08
	BYTE btViewPortIndex;	// 0x01 - CBuffEffect + 0x09
	BYTE btItemType;		// 0x02 - CBuffEffect + 0x0A
	BYTE btItemIndex;		// 0x03 - CBuffEffect + 0x0B
	char szBuffName[20];	// 0x04 - CBuffEffect + 0x0C
	BYTE btType;			// 0x24 - CBuffEffect + 0x20
	BYTE btNotification;	// 0x25 - CBuffEffect + 0x21
	BYTE btClearType;		// 0x26 - CBuffEffect + 0x22
	char szBuffInfo[100];	// 0x27 - CBuffEffect + 0x23
} BUFF_EFFECT_DATE, *LPBUFF_EFFECT_DATE;

class CBuffScriptLoader
{
public:
	CBuffScriptLoader();
	virtual ~CBuffScriptLoader();

	void Initialize();
	bool Load(char* pchFileName);
	bool AddBuffEffectData(BYTE btBuffIndex,BYTE btBuffEffectType,BYTE btItemType,BYTE btItemIndex,char* pchName,BYTE btBuffType,BYTE btNoticeType,BYTE btClearType,char* pchDescript);
	bool CheckVaildBuffEffect(BYTE btBuffIndex);
	LPBUFF_EFFECT_DATE GetBuffData(BYTE btBuffIndex);
	LPBUFF_EFFECT_DATE GetBuffDataFromItemCode(WORD wItemCode);

public:
	int iLoadCount;
	BUFF_EFFECT_DATE m_EffectData[MAX_BUFF_EFFECT];
};

extern CBuffScriptLoader g_BuffScript;