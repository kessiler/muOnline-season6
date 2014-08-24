#pragma once
// -------------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------------

class CObjBaseAttack
{
public:
	CObjBaseAttack();
	virtual ~CObjBaseAttack();
	// ----
	int			DecreaseArrow(LPOBJ lpObj);
	int			CheckAttackArea(LPOBJ lpObj, LPOBJ lpTargetObj);
	int			PkCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
	int			ResistanceCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill);
	virtual int MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL & bAllMiss, BYTE byBarrageCount);
	int			MissCheckPvP(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL & bAllMiss, BYTE byBarrageCount);
	int			GetTargetDefense(LPOBJ lpObj, LPOBJ lpTargetObj, BYTE& MsgDamage);
	int			GetPartyMemberCount(LPOBJ lpObj);
};
// -------------------------------------------------------------------------------