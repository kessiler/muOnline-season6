#include "StdAfx.h"
#include "user.h"

#define BALANCESYSTEM_DIR "Custom\\BalanceSystem.ini"

class cBalanceSystem
{
public:
	cBalanceSystem();
	virtual ~cBalanceSystem();

	void Load();
	void ReadConfigs(char * File);
	void Main(LPOBJ lpObj, LPOBJ lpTarget, int & AttackDamage);

private:
	bool Enable;

	int DKvsDK;
	int DKvsDW;
	int DKvsELF;
	int DKvsSUM;
	int DKvsMG;
	int DKvsDL;
	int DKvsRF;

	int DWvsDW;
	int DWvsDK;
	int DWvsELF;
	int DWvsSUM;
	int DWvsMG;
	int DWvsDL;
	int DWvsRF;

	int ELFvsELF;
	int ELFvsDK;
	int ELFvsDW;
	int ELFvsSUM;
	int ELFvsMG;
	int ELFvsDL;
	int ELFvsRF;

	int SUMvsSUM;
	int SUMvsDK;
	int SUMvsDW;
	int SUMvsELF;
	int SUMvsMG;
	int SUMvsDL;
	int SUMvsRF;

	int MGvsMG;
	int MGvsDK;
	int MGvsDW;
	int MGvsELF;
	int MGvsSUM;
	int MGvsDL;
	int MGvsRF;

	int DLvsDL;
	int DLvsDK;
	int DLvsDW;
	int DLvsELF;
	int DLvsSUM;
	int DLvsMG;
	int DLvsRF;

	int RFvsRF;
	int RFvsDK;
	int RFvsDW;
	int RFvsELF;
	int RFvsSUM;
	int RFvsMG;
	int RFvsDL;

};
extern cBalanceSystem gBalanceSystem;