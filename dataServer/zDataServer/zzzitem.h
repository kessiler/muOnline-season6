#ifndef __ZZZITEM_H__
#define __ZZZITEM_H__

#include "ItemDef.h"

class CItem
{
public :
	DWORD m_Number;
	char  m_serial;
	short m_Type;
	short m_Level;	
	BYTE  m_Part;
	BYTE  m_Class;
	bool  m_TwoHand;
	BYTE  m_AttackSpeed;
	BYTE  m_WalkSpeed;
	WORD  m_DamageMin;
	WORD  m_DamageMax;
	BYTE  m_SuccessfulBlocking;
	WORD  m_Defense;
	WORD  m_MagicDefense;
	BYTE  m_Speed;

	WORD  m_DamageMinOrigin;
	WORD  m_DefenseOrigin;
	WORD  m_Magic;

	//BYTE  m_Durability;
	float m_Durability;
	WORD  m_DurabilitySmall;
	float m_BaseDurability;

	BYTE  m_SpecialNum;
	BYTE  m_Special     [MAX_ITEM_SPECIAL];
	BYTE  m_SpecialValue[MAX_ITEM_SPECIAL];	// 레벨

	WORD  m_RequireStrength;
	WORD  m_RequireDexterity;
	WORD  m_RequireEnergy;
	WORD  m_RequireLevel;
	WORD  m_RequireVitality;
	WORD  m_RequireLeaderShip;
	WORD  m_Leadership;

	BYTE  m_RequireClass[MAX_CLASSTYPE];
	BYTE  m_Resistance[MAX_RESISTANCE];

	int   m_Value;
	DWORD m_SellMoney;	// 팔때 값
	DWORD m_BuyMoney;	// 살때 값

	int   m_iPShopValue;

	int   m_OldSellMoney;
	int   m_OldBuyMoney;

	BYTE  m_Option1;	// 아이템 옵션 1 세팅되었는지?
	BYTE  m_Option2;	// 아이템 옵션 2 세팅되었는지?
	BYTE  m_Option3;	// 아이템 옵션 3 세팅되었는지?
	
	BYTE  m_NewOption;	//

	float m_DurabilityState[4];
	float m_CurrentDurabilityState;
	BYTE  m_SkillChange;
	BYTE  m_QuestItem;
	BYTE  m_SetOption;
	BYTE  m_SetAddStat;
	bool  m_IsValidItem;
	BYTE  m_SkillResistance[MAX_RESISTANCE];
	int   m_IsLoadPetItemInfo;
	int   m_PetItem_Level;
	int   m_PetItem_Exp;
public :
	CItem();

	void Convert( int type, BYTE Option1=0, BYTE Option2=0, BYTE Option3=0, BYTE Attribute2=0, BYTE SetOption=0, BYTE DbVersion=3);
	void Value();
	void OldValue();
	BOOL GetSize(int &w, int &h);
	void Clear();
	BOOL IsItem();
	int IsSetItem();
	int  GetAddStatType();
	void SetPetItemInfo(int petlevel, int petexp);
	BOOL AddPetItemExp(int petexp);
	int DecPetItemExp(int percent);
	void PetValue();
	int PetItemLevelDown(int exp);
	int ItemDamageMin();
	int  ItemDefense();
	BOOL IsClass(char aClass, int ChangeUP);
	char *GetName();
	int  GetLevel(void);
	void PlusSpecial(int *Value,int Special);
	void PlusSpecialPercent(int* Value,int Special, WORD Percent);
	void PlusSpecialPercentEx(int* Value, int SourceValue, int Special);
	void SetItemPlusSpecialStat(short* Value, int Special);
	int GetWeaponType();
	void PlusSpecialSetRing(BYTE* Value);
	DWORD GetNumber();

	int IsExtItem();
	int IsExtLifeAdd();
	int IsExtManaAdd();
	int IsExtDamageMinus();
	int IsExtDamageReflect();
	int IsExtDefenseSuccessfull();
	int IsExtMonsterMoney();
	int IsExtExcellentDamage();
	int IsExtAttackRate();
	int IsExtAttackRate2();
	int IsExtAttackSpeed();
	int IsExtMonsterDieLife();
	int IsExtMonsterDieMana();
	int IsWingOpGetOnePercentDamage();
	int IsWingOpGetLifeToMonster();
	int IsWingOpGetManaToMoster();
	int IsDinorantReduceAttackDamaege();
	int IsFenrirIncLastAttackDamage();
	int IsFenrirDecLastAttackDamage();
	int SimpleDurabilityDown(int iDur);
	int DurabilityDown(int dur, int aIndex);
	int DurabilityDown2(int dur, int aIndex);
	int NormalWeaponDurabilityDown(int defence, int aIndex);
	int BowWeaponDurabilityDown(int defence, int aIndex);
	int StaffWeaponDurabilityDown(int defence, int aIndex);
	int ArmorDurabilityDown(int damagemin, int aIndex);
	int CheckDurabilityState();
};

class CPetItemExp
{

public:

	int m_DarkSpiritExpTable[52];
	int m_DarkHorseExpTable[52];

public:

	CPetItemExp();
};

extern CPetItemExp	gPetItemExp;
extern int g_MaxItemIndexOfEachItemType[MAX_ITEM_INDEX];

extern int IsItem(int item_num);
extern ITEM_ATTRIBUTE* GetItemAttr(int item_num);

extern BOOL OpenItemScript(char *FileName);
extern BOOL OpenItemNameScript(char* FileName);

extern int  ItemGetNumberMake(int type, int index );
extern void ItemGetSize(int index, int & width, int & height);
extern void ItemByteConvert7(LPBYTE buf, CItem item[], int maxitem);
extern void ItemByteConvert10(LPBYTE buf, CItem item[], int maxitem);
extern void ItemByteConvert16(LPBYTE buf, CItem item[], int maxitem);
extern void ItemByteConvert(LPBYTE buf, CItem item);
extern void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption);
extern int  GetLevelItem(int type, int index, int level);
extern BOOL zzzItemLevel(int type, int index, int level);
extern int GetSerialItem(int type);
extern void BufferItemtoConvert3(LPBYTE buf, int & type, BYTE & level, BYTE & op1, BYTE & op2, BYTE & op3, BYTE & dur);

extern BOOL HasItemDurability(int index);
extern int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem);
extern int ItemGetAttackDurability(int index);
extern int ItemGetDefenseDurability(int index);
extern float GetRepairItemRate(int index);
extern float GetAllRepairItemRate(int index);
extern void CalRepairRate( int itemtype, int itemsubtype, ITEM_ATTRIBUTE* p);
#endif
