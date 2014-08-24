#ifndef OBJCALCHARACTER_H
#define OBJCALCHARACTER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"
#include "..\common\zzzitem.h"

void gObjCalCharacter(int aIndex);
void GObjExtItemApply(LPOBJ lpObj);
void gObjCalcSetItemStat(LPOBJ lpObj);
void gObjGetSetItemOption(LPOBJ lpObj, LPBYTE pSetOptionTable , LPBYTE pSetOptionCountTable , int * pSetOptionCount);
void gObjCalcSetItemOption(LPOBJ lpObj);
void gObjSetItemStatPlusSpecial(LPOBJ lpObj, int option, int ivalue);
void gObjSetItemPlusSpecial(LPOBJ lpObj, int option, int ivalue);
void gObjSetItemApply(LPOBJ lpObj);
void gObjCalcMLSkillItemOption(LPOBJ lpObj);	//1.01.00
BOOL gObjValidItem(LPOBJ lpObj, CItem * lpItem, int pos);
void gObjCalcShieldPoint(LPOBJ lpObj);

#endif