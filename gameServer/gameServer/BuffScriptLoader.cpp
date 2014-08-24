#include "StdAfx.h"
#include "BuffScriptLoader.h"
#include "..\include\ReadScript.h"
#include "..\common\zzzitem.h"

CBuffScriptLoader g_BuffScript;

// Constructor / Destructor

CBuffScriptLoader::CBuffScriptLoader()
{

}

CBuffScriptLoader::~CBuffScriptLoader()
{

}

void CBuffScriptLoader::Initialize()
{
	this->iLoadCount = 0;	

	for ( int i = 0; i < MAX_BUFF_EFFECT-1; i++ )
	{
		this->m_EffectData[i].btIndex			= 0xFF;
		this->m_EffectData[i].btType			= 0;
		this->m_EffectData[i].btClearType		= 0;
		this->m_EffectData[i].btNotification	= 0;
		this->m_EffectData[i].szBuffInfo[0]		= 0;
	}
}

bool CBuffScriptLoader::Load(char* pchFileName)
{
	this->Initialize();

	if ( (SMDFile = fopen(pchFileName, "r")) == NULL )	//ok
	{
		return false;
	}

	SMDToken Token;
	int iType = 0;
	BYTE btBuffIndex = 0;
	BYTE btBuffEffectType = 0;
	BYTE ItemType = 0;
	BYTE ItemIndex = 0;
	BYTE btBuffType = 0;
	BYTE btNoticeType = 0;
	BYTE btClearType = 0;
	char chBuffName[20];
	char chBuffDescript[100];

	while ( TRUE )
	{
		Token = GetToken();

		if ( Token == END )
		{
			break;
		}
		
		if ( Token == NUMBER )
		{
			iType = TokenNumber;

			while ( TRUE )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				btBuffIndex			= (int)TokenNumber; Token = GetToken();
				btBuffEffectType	= (int)TokenNumber; Token = GetToken();
				ItemType			= (int)TokenNumber; Token = GetToken();
				ItemIndex			= (int)TokenNumber; Token = GetToken();

				memcpy(chBuffName, TokenString, sizeof(chBuffName));

				Token = GetToken();

				btBuffType		= (int)TokenNumber; Token = GetToken();
				btNoticeType	= (int)TokenNumber; Token = GetToken();
				btClearType		= (int)TokenNumber; Token = GetToken();

				memcpy(chBuffDescript, TokenString, sizeof(chBuffDescript));

				this->AddBuffEffectData(btBuffIndex, btBuffEffectType, ItemType, ItemIndex, chBuffName, btBuffType, btNoticeType, btClearType, chBuffDescript);
			}
		}
	}

	fclose(SMDFile);
	return true;
}

bool CBuffScriptLoader::AddBuffEffectData(BYTE btBuffIndex,BYTE btBuffEffectType,BYTE btItemType,BYTE btItemIndex,char* pchName,BYTE btBuffType,BYTE btNoticeType,BYTE btClearType,char* pchDescript)
{
	this->m_EffectData[this->iLoadCount].btIndex = btBuffIndex;
	this->m_EffectData[this->iLoadCount].btViewPortIndex = btBuffEffectType;
	this->m_EffectData[this->iLoadCount].btItemType = btItemType;
	this->m_EffectData[this->iLoadCount].btItemIndex = btItemIndex;
	this->m_EffectData[this->iLoadCount].btType = btBuffType;
	this->m_EffectData[this->iLoadCount].btNotification = btNoticeType;
	this->m_EffectData[this->iLoadCount].btClearType = btClearType;

	memcpy(this->m_EffectData[this->iLoadCount].szBuffName,	pchName, sizeof(this->m_EffectData[this->iLoadCount].szBuffName));
	memcpy(this->m_EffectData[this->iLoadCount].szBuffInfo,	pchDescript, sizeof(this->m_EffectData[this->iLoadCount].szBuffInfo));
	this->iLoadCount++;
	return true;
}

bool CBuffScriptLoader::CheckVaildBuffEffect(BYTE btBuffIndex)
{
	if ( btBuffIndex < 0 || btBuffIndex > MAX_BUFF_EFFECT-1 )
	{
		return false;
	}

	return true;
}

LPBUFF_EFFECT_DATE CBuffScriptLoader::GetBuffData(BYTE btBuffIndex)
{
	LPBUFF_EFFECT_DATE lpBuffData = 0;

	if ( this->CheckVaildBuffEffect(btBuffIndex) == false )
		return NULL;

	for ( int i = 0; i < MAX_BUFF_EFFECT-1; i++ )
	{
		if ( this->m_EffectData[i].btIndex == btBuffIndex )
		{
			return &this->m_EffectData[i];
		}
	}
	return NULL;
}

LPBUFF_EFFECT_DATE CBuffScriptLoader::GetBuffDataFromItemCode(WORD wItemCode)
{
	LPBUFF_EFFECT_DATE lpBuffData = 0;

	for ( int i = 0; i < MAX_BUFF_EFFECT-1; i++ )
	{
		if( ITEMGET(this->m_EffectData[i].btItemType,this->m_EffectData[i].btItemIndex) == wItemCode )
		{
			return &this->m_EffectData[i];
		}
	}
	return NULL;
}