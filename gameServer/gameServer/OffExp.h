#include "StdAfx.h"

#ifdef OFFEXP

#include "user.h"

#define OFF_EXP_DIR "..\\Data\\Custom\\OffExp.ini"

class cOffExp
{
public:
	int Enable;
	int KickFallen;
	int EnableTime;
	int TimeEnd;

	void LoadConfig();

	void OffExpStart(int aIndex);

	void MainFunction();

	void DarkKnightOff(LPOBJ lpObj);
	void DarkWizardOff(LPOBJ lpObj);
	void FairyElfOff(LPOBJ lpObj);
	void MagicGladiatorOff(LPOBJ lpObj);
	void DarkLordOff(LPOBJ lpObj);
	void SummonerOff(LPOBJ lpObj);
	void RageFighterOff(LPOBJ lpObj);

	void UseMana(int aIndex);

	int SearchTarget(LPOBJ lpObj);

	bool CheckTerrain(LPOBJ lpObj);

	void Close(char * AccountID);
	bool Update(int UserIndex, BYTE Type);

	void TickTimes(int aIndex);
};
extern cOffExp OffExp;

#endif