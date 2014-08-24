//-> Revised by DarkSim | 18.02.2013 | 1.01.00 GS-N (Without changes)
// -------------------------------------------------------------------------------

#include "stdafx.h"
#include "DirPath.h"
#include "ItemBag.h"
#include "GameMain.h"
#include "..\include\readscript.h"
#include "logproc.h"
// -------------------------------------------------------------------------------

//004b2990	-> 100%
CItemBag::CItemBag()
{
	return;
}
// -------------------------------------------------------------------------------

//004b2a80	-> 100%
CItemBag::~CItemBag()
{
	return;
}
// -------------------------------------------------------------------------------

//004b2ab0	-> 100%
void CItemBag::Init(char * name)
{
	this->BagObjectCount		= 0;
	this->BagNormalItemCount	= 0;
	this->BagExItemCount		= 0;
	// ----
	this->LoadItem(gDirPath.GetNewPath(name));
}
// -------------------------------------------------------------------------------

//004b2b20	-> 100%
void CItemBag::LoadItem(char * script_file)
{
	SMDFile = fopen(script_file, "r");	//ok
	// ----
	if( !SMDFile )
	{
		LogAdd(lMsg.Get(MSGGET(1, 197)), script_file);
		return ;
	}
	// ----
	int Token;
	int n = 0;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		if( Token == NUMBER )
		{
			n = this->BagObjectCount;
			// ----
			this->BagObject[n].m_type	= TokenNumber;
			// ----
			Token = GetToken();
			this->BagObject[n].m_index	= TokenNumber;
			// ----
			Token = GetToken();
			this->BagObject[n].m_level	= TokenNumber;
			// ----
			Token = GetToken();
			this->BagObject[n].m_op1	= TokenNumber;
			// ----
			Token = GetToken();
			this->BagObject[n].m_op2	= TokenNumber;
			// ----
			Token = GetToken();
			this->BagObject[n].m_op3	= TokenNumber;
			// ----
			if( this->BagObject[n].m_op2 != 0 )
			{
				this->BagExItemCount++;
			}
			else
			{
				this->BagNormalItemCount++;
			}
			// ----
			this->BagObjectCount++;
			// ----
			if( this->BagObjectCount > MAX_ITEMBAG_ATTR - 1 )
			{
				break;
			}
		}
	}
	// ----
	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1, 198)), script_file);
}
// -------------------------------------------------------------------------------

//004b31e0	-> 100%
BYTE CItemBag::GetType(int n)
{
	if( n < 0 || n > MAX_ITEMBAG_ATTR - 1 )
	{
		return 0;
	}
	// ----
	return this->BagObject[n].m_type;
}
// -------------------------------------------------------------------------------

//004b3230	-> 100%
BYTE CItemBag::GetIndex(int n)
{
	if( n < 0 || n > MAX_ITEMBAG_ATTR - 1 )
	{
		return 0;
	}
	// ----
	return this->BagObject[n].m_index;
}
// -------------------------------------------------------------------------------

//004b3280	-> 100%
BYTE CItemBag::GetLevel(int n)
{
	if( n < 0 || n > MAX_ITEMBAG_ATTR - 1 )
	{
		return 0;
	}
	// ----
	return this->BagObject[n].m_level;
}
// -------------------------------------------------------------------------------

//004b32d0	-> 100%
BYTE CItemBag::GetOp1(int n)
{
	if( n < 0 || n > MAX_ITEMBAG_ATTR - 1 )
	{
		return 0;
	}
	// ----
	return this->BagObject[n].m_op1;
}
// -------------------------------------------------------------------------------

//004b3320	-> 100%
BYTE CItemBag::GetOp2(int n)
{
	if( n < 0 || n > MAX_ITEMBAG_ATTR - 1 )
	{
		return 0;
	}
	// ----
	return this->BagObject[n].m_op2;
}
// -------------------------------------------------------------------------------

//004b3370	-> 100%
BYTE CItemBag::GetOp3(int n)
{
	if( n < 0 || n > MAX_ITEMBAG_ATTR - 1 )
	{
		return 0;
	}
	// ----
	return this->BagObject[n].m_op3;
}
// -------------------------------------------------------------------------------