//-> Revised by DarkSim | 27.06.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#include "user.h"
#include "MagicInf.h"
#include "..\\common\\zzzitem.h"
#include "ObjBaseAttack.h"
// -------------------------------------------------------------------------------

#define RAVEN_ATTACK_DISTANCE		7
// -------------------------------------------------------------------------------

struct PMSG_SEND_PET_ITEMINFO
{
	PBMSG_HEAD h;
	// ----
	BYTE	PetType;
	BYTE	InvenType;
	BYTE	nPos;
	BYTE	Level;
	int		Exp;
	BYTE	Life;
};
// -------------------------------------------------------------------------------

class CDarkSpirit : public CObjBaseAttack
{
public:
	int		m_AttackDamageMin;		//+4
	int		m_AttackDamageMax;		//+8
	int		m_AttackSpeed;			//+12
	int		m_SuccessAttackRate;	//+16
	float	m_CriticalAttackRate;	//+20
	float	m_ExcellentAttackRate;	//+24
	int		m_Unknown28;			//+28
	int		m_Unknown32;			//+32
	int		m_iMasterIndex;			//+36
	int		m_iTargetIndex;			//+40
	DWORD	m_dwLastAttackTime;		//+44
	// ----
	enum ePetItem_Mode
	{
		PetItem_Mode_Normal				= 0,
		PetItem_Mode_Attack_Random		= 1,
		PetItem_Mode_Attack_WithMaster	= 2,
		PetItem_Mode_Attack_Target		= 3,

	} m_ActionMode;					//+48
	CItem *	m_pPetItem;				//+52
	// ----
public:
			CDarkSpirit();
  virtual	~CDarkSpirit();
  // ----
  void		Init();
  void		Run();
  void		ModeNormal();
  void		ModeAttackRandom();
  void		ModeAttackWithMaster();
  void		ModeAttakTarget();
  void		SetTarget(int aTargetIndex);
  void		ReSetTarget(int aTargetIndex);
  void		Set(int aIndex, CItem* pPetItem);
  void		SetMode(ePetItem_Mode mode, int iTargetindex);
  int		Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic, int criticaldamage, int iActionType);
  int		GetAttackDamage(LPOBJ lpObj, int targetDefense, int bIsOnDuel,  int criticaldamage);
  int		MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, int& bAllMiss);
  int		MissCheckPvP(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, int& bAllMiss);
  void		ChangeCommand(int command, int targetindex);
  void		RangeAttack(int aIndex, int aTargetIndex);
  void		SendAttackMsg(int aIndex, int aTargetIndex, int criticaldamage, int iActionType);
  int		GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);
  // ----
  static void __cdecl CDarkSpirit::SendLevelmsg(int aIndex, int nPos, int PetType, int InvenType);
  // ----
}; extern CDarkSpirit gDarkSpirit[OBJMAX];
// -------------------------------------------------------------------------------