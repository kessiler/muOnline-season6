#include "StdAfx.h"

#if __ALIEN__

#include "ItemOption.h"

cItemOption gItemOption;

void cItemOption::Init()
{
	this->Enable = GetPrivateProfileInt("Common","Enable",1,ITEM_OPT_DIR);

	this->PlusDamage = GetPrivateProfileInt("Fenrir","PlusDamage",50,ITEM_OPT_DIR);
	this->PlusAbsurd = GetPrivateProfileInt("Fenrir","PlusAbsurd",50,ITEM_OPT_DIR);
}

void cItemOption::AttackDamage(LPOBJ lpObj, LPOBJ lpTarget, int & AttackDamage)
{
	if(!this->Enable) return;

	if(lpObj->Type == OBJ_USER && lpObj->Inventory1[8].m_Type == ITEMGET(13,37) && lpObj->Inventory1[8].m_NewOption == 4)
	{
		AttackDamage += ( AttackDamage * this->PlusDamage ) / 100;
	}

	if(lpTarget->Type == OBJ_USER && lpTarget->Inventory1[8].m_Type == ITEMGET(13,37) && lpTarget->Inventory1[8].m_NewOption == 4)
	{
		AttackDamage -= ( AttackDamage * this->PlusAbsurd ) / 100;
		if(AttackDamage < 0) AttackDamage = 0;
	}
}

#endif