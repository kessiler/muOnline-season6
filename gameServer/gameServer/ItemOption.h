#include "StdAfx.h"

#if __ALIEN__

#include "user.h"

#define ITEM_OPT_DIR "..\\Data\\Custom\\ItemOption.ini"

class cItemOption
{
private:
	bool Enable;
	int PlusDamage;
	int PlusAbsurd;
public:
	void Init();
	void AttackDamage(LPOBJ lpObj, LPOBJ lpTarget, int & AttackDamage);

};
extern cItemOption gItemOption;

#endif