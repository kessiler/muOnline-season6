#ifndef LUCKYCOIN_H
#define LUCKYCOIN_H

#include "User.h"

class CLuckyCoin
{
public:
	CLuckyCoin(void);
	~CLuckyCoin(void);
	// ----
	void Init();
	void ReadFile();
	void OpenRegisterWindow(LPOBJ lpObj);
	void RegisterCoin(LPOBJ lpObj);
	int GetCoinCount(LPOBJ lpObj);
	void DeleteCoin(LPOBJ lpObj);
	int GetCoinPos(LPOBJ lpObj);
	void ExchangeCoin(LPOBJ lpObj,int Type);
	// ----
	char Enabled;
};

extern CLuckyCoin gLuckyCoin;

#endif