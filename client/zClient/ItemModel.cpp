#include "stdafx.h"
#include "Object.h"
#include "ItemModel.h"
#include "ScriptEncode.h"
#include "Import.h"
#include "TDebugLog.h"
// -------------------------------------------------------------------------------

ItemModel g_ItemModel;
// -------------------------------------------------------------------------------

ItemModel::ItemModel()
{
	Init();
}
// -------------------------------------------------------------------------------

ItemModel::~ItemModel()
{

}
// -------------------------------------------------------------------------------

void ItemModel::Init()
{
	m_Data.clear();
}
// -------------------------------------------------------------------------------

void ItemModel::Load()
{
	Read("Data\\Custom\\ItemModel.z");
}
// -------------------------------------------------------------------------------

void ItemModel::Read(char * File)
{
	int Token;
	// ----
	if( !g_ScriptEncode.ReadScript(File) )
	{
		//MessageBox(NULL, "file not found", "[ItemModel]", ERROR);
		return;
	}
	// ----
	while(true)
	{
		Token = g_ScriptEncode.GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		ItemModelData lpData;
		// ----
		lpData.ItemType	= g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		lpData.ItemIndex = g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		strcpy(lpData.Model, g_ScriptEncode.GetString());
		// ----
		g_ScriptEncode.GetToken();
		lpData.R = g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		lpData.G = g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		lpData.B = g_ScriptEncode.GetNumber();
		// ----
		TDEBUG("[%d] %d %d - %s", m_Data.size(), lpData.ItemType, lpData.ItemIndex, lpData.Model);

		m_Data.push_back(lpData);
	}
}
// -------------------------------------------------------------------------------

bool ItemModel::SetColor(int ModelID, VAngle * ColorBuffer)
{
	for( int i = 0; i < m_Data.size(); i++ )
	{
		if( ITEM2(m_Data[i].ItemType, m_Data[i].ItemIndex) == ModelID )
		{
			ColorBuffer->X = (float)(m_Data[i].R / 255.0);
			ColorBuffer->Y = (float)(m_Data[i].G / 255.0);
			ColorBuffer->Z = (float)(m_Data[i].B / 255.0);
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

void ItemModel::SetModel()
{
	for( int i = 0; i < m_Data.size(); i++ )
	{
		if( strcmp(m_Data[i].Model, "") == 0 )
		{
			continue;
		}
		// ----
		int ModelID = ITEM2(m_Data[i].ItemType, m_Data[i].ItemIndex);
		int Option	= -1;
		// ----
		/*if( m_Data[i].ItemType >= 0 && m_Data[i].ItemType <= 11 )
		{
			Option = m_Data[i].ItemIndex + 1;
			if ( a7 == -1 )
			{
				sub_9CF52A((int)&a3a, "%s.bmd", a6);
			}
			else
			{
				if ( a7 >= 10 )
					sub_9CF52A((int)&a3a, "%s%d.bmd", a6);
				else
					sub_9CF52A((int)&a3a, "%s0%d.bmd", a6);
			}
		}*/
		// ----
		pLoadModel(ModelID, ItemModelPath, m_Data[i].Model, Option);
	}
}
// -------------------------------------------------------------------------------

void ItemModel::SetTexture()
{
	for( int i = 0; i < m_Data.size(); i++ )
	{
		if( strcmp(m_Data[i].Model, "") == 0 )
		{
			continue;
		}
		// ----
		int ModelID = ITEM2(m_Data[i].ItemType, m_Data[i].ItemIndex);
#ifdef __LEGEND__
		if( m_Data[i].ItemType >= 13 )
		{
			pLoadTexture(ModelID, ItemTexturePath, GL_REPEAT, GL_NEAREST, GL_TRUE);
		}
		else
		{
			pLoadTexture(ModelID, "Item\\", GL_REPEAT, GL_NEAREST, GL_TRUE);
		}
#else
		pLoadTexture(ModelID, ItemTexturePath, GL_REPEAT, GL_NEAREST, GL_TRUE);
#endif
	}
}
// -------------------------------------------------------------------------------