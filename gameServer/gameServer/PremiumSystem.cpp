#include "StdAfx.h"
#include "PremiumSystem.h"
#include "GameMain.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "GameShop.h"

PremiumSystem g_PremiumSystem;

PremiumSystem::PremiumSystem()
{

}

PremiumSystem::~PremiumSystem()
{

}

void PremiumSystem::Init()
{
	this->m_Price.clear();
	this->m_ChaosMachineRateIncrease = 0;
	this->m_ChaosMachineMoneyDecrease = 0;
	this->m_DieMoneyPenaltyPercent = 0;
	this->m_ResetDelayMin = 0;
	this->m_DamagePowerIncrease = 0;
	this->m_DefenseIncrease = 0;
	this->m_CriticalDamageIncrease = 0;
	this->m_ExcellentDamageIncrease = 0;
	this->m_MaxLifeIncrease = 0;
	this->m_MaxManaIncrease = 0;
	this->m_MaxSDIncrease = 0;
	this->m_MaxAGIncrease = 0;
}

void PremiumSystem::Load()
{
	this->Init();
	this->ReadPrice(gDirPath.GetNewPath("Custom\\PremiumPrice.txt"));
	this->ReadPrice(gDirPath.GetNewPath("Custom\\PremiumMain.ini"));
}

void PremiumSystem::ReadPrice(char * File)
{
	int Token;
	SMDFile = fopen(File, "r");
	// ----
	if( SMDFile == 0 )
	{
		MsgBox(lMsg.Get(MSGGET(0, 112)), File);
		return;
	}
	// ----
	while(true) 
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		else
		{
			PremiumSystemPrice lpPrice;
			lpPrice.Days = TokenNumber;
			Token = GetToken();
			lpPrice.PayType = TokenNumber;
			Token = GetToken();
			lpPrice.PayAmount = TokenNumber;
			this->m_Price.push_back(lpPrice);
		}
	}
	// ----
	fclose(SMDFile);
}

void PremiumSystem::ReadMain(char * File)
{
	this->m_ChaosMachineRateIncrease	= GetPrivateProfileInt("Bonus", "ChaosMachineRateIncrease", 0, File);
	this->m_ChaosMachineMoneyDecrease	= GetPrivateProfileInt("Bonus", "ChaosMachineMoneyDecrease", 0, File);
	this->m_DieMoneyPenaltyPercent		= GetPrivateProfileInt("Bonus", "DieMoneyPenaltyPercent", 0, File);
	this->m_ResetDelayMin				= GetPrivateProfileInt("Bonus", "ResetDelayMin", 0, File);
	this->m_DamagePowerIncrease			= GetPrivateProfileInt("Bonus", "DamagePowerIncrease", 0, File);
	this->m_DefenseIncrease				= GetPrivateProfileInt("Bonus", "DefenseIncrease", 0, File);
	this->m_CriticalDamageIncrease		= GetPrivateProfileInt("Bonus", "CriticalDamageIncrease", 0, File);
	this->m_ExcellentDamageIncrease		= GetPrivateProfileInt("Bonus", "ExcellentDamageIncrease", 0, File);
	this->m_MaxLifeIncrease				= GetPrivateProfileInt("Bonus", "MaxLifeIncrease", 0, File);
	this->m_MaxManaIncrease				= GetPrivateProfileInt("Bonus", "MaxManaIncrease", 0, File);
	this->m_MaxSDIncrease				= GetPrivateProfileInt("Bonus", "MaxSDIncrease", 0, File);
	this->m_MaxAGIncrease				= GetPrivateProfileInt("Bonus", "MaxAGIncrease", 0, File);
}

bool PremiumSystem::IsPremiumUser(int UserIndex)
{
	return gObj[UserIndex].m_PremiumState;
}

void PremiumSystem::ApplyBonus(int UserIndex)
{
	if( !gObj[UserIndex].m_PremiumState )
	{
		return;
	}
	// ----

}

void PremiumSystem::BuyPremium(int UserIndex, int Days)
{
	bool Result = false;
	// ----
	for( int i = 0; i < this->m_Price.size(); i++ )
	{
		if( m_Price[i].Days != Days )
		{
			continue;
		}
		// ----
		switch(m_Price[i].PayType)
		{
		case 0:
			if( gObj[UserIndex].GameShop.WCoinC < m_Price[i].PayAmount )
			{
				GCServerMsgStringSend("You do not have enough WCoinC", UserIndex, 1);
				return;
			}
			gObj[UserIndex].GameShop.WCoinC -= m_Price[i].PayAmount;
			Result = true;
			break;
		case 1:
			if( gObj[UserIndex].GameShop.WCoinP < m_Price[i].PayAmount )
			{
				GCServerMsgStringSend("You do not have enough WCoinP", UserIndex, 1);
				return;
			}
			gObj[UserIndex].GameShop.WCoinP -= m_Price[i].PayAmount;
			Result = true;
			break;
		case 2:
			if( gObj[UserIndex].GameShop.GoblinPoint < m_Price[i].PayAmount )
			{
				GCServerMsgStringSend("You do not have enough WCoinG", UserIndex, 1);
				return;
			}
			gObj[UserIndex].GameShop.GoblinPoint -= m_Price[i].PayAmount;
			Result = true;
			break;
		}
	}
	// ----
	if( !Result )
	{
		GCServerMsgStringSend("Wrong premium period", UserIndex, 1);
		return;
	}
	// ----
	//Request to database
	// ----
	gGameShop.GDSavePoint(UserIndex);
}

void PremiumSystem::RemovePremium(int UserIndex)
{
	
}

void PremiumSystem::CheckEndTime(int UserIndex)
{

}