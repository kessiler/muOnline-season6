#include "stdafx.h"
#include "Pet.h"
// ----------------------------------------------------------------------------------------------

Rudolph g_Rudolph;
// ----------------------------------------------------------------------------------------------
//#define *(ObjectModel*)0x81F6C70
void Rudolph::SearchTarget(LPVOID This, LPVOID EDX, int OwnerObject)
{
	if( OwnerObject == NULL )
	{
		return;
	}
	// ----
	ObjectModel * lpOwnerMod = &*(ObjectModel*)(*(DWORD*)OwnerObject + 472);
	// ----
	if( lpOwnerMod == NULL )
	{
		return;
	}
	// ----
	if( *(bool*)(*(DWORD*)This + 20) == true )
	{
		return;
	}
	// ----
	for( int LootIndex = 0; LootIndex < MAX_LOOTOBJECT; LootIndex++ )
	{
		ObjectModel * lpTargetObj = &*(ObjectModel*)(0x81F6C70 * LootIndex + 472);
		// ----
		if( lpTargetObj->Unknown4 == 0 || lpTargetObj->Unknown12 == 0 )
		{
			continue;
		}
		// ----
		float Diff1 = lpOwnerMod->VecPosX - lpTargetObj->VecPosY;
		float Diff2 = lpOwnerMod->VecPosY - lpTargetObj->VecPosX;
	}
}
// ----------------------------------------------------------------------------------------------