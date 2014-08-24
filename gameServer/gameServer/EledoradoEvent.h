// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef ELEDORADOEVENT_H
#define ELEDORADOEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEledoradoEvent
{

public:

	CEledoradoEvent();
	virtual ~CEledoradoEvent();

	void Init();
	void SetEventState(int State);
	void Run();
	void RegenGoldGoblen();
	void RegenTitan();
	void RegenGoldDercon();
	void RegenDevilLizardKing();
	void RegenKantur();

	void RegenGoldenRabbit();
	void RegenGoldenDarkKnight();
	void RegenGoldenDevil();
	void RegenGoldenMonster();
	void RegenGoldenCrust();
	void RegenGoldenSatiros();
	void RegenGoldenTwintail();
	void RegenGoldenIronKnight();
	void RegenGoldenNeipin();
	void RegenGoldenGreatDragon();

	void CheckGoldDercon(int MapNumber);
	void SetMenualStart(BOOL bMenualStart){this->m_bMenualStart = bMenualStart;}	// line : 57
	void Start_Menual();
	void End_Menual();

public:

	int EventState;	// 4
	DWORD GoldGoblenEventStartTime;	// 8
	DWORD TitanEventStartTime;	// C
	DWORD GoldDerconEventStartTime;	// 10
	DWORD DevilLizardKingEventStartTime;	// 14
	DWORD KanturEventStartTime;	// 18
	DWORD GoldenRabbitRegenTime;
	DWORD GoldenDarkKnightRegenTime;
	DWORD GoldenDevilRegenTime;
	DWORD GoldenMonsterRegenTime;
	DWORD GoldenCrustRegenTime;
	DWORD GoldenSatirosRegenTime;
	DWORD GoldenTwintailRegenTime;
	DWORD GoldenIronKnightRegenTime;
	DWORD GoldenNeipinRegenTime;
	DWORD GoldenGreatDragonRegenTime;

	int m_BossTitanMapNumber;	// 1C
	int m_BossTitanMapX;	// 20
	int m_BossTitanMapY;	// 24
	int m_BossDevilLizardKingMapNumber;	// 28
	int m_BossDevilLizardKingMapX;	// 2C
	int m_BossDevilLizardKingMapY;	// 30
	int m_BossKanturMapNumber;	// 34
	int m_BossKanturMapX;	// 38
	int m_BossKanturMapY;	// 3C
	int m_bMenualStart;	// 40
	int m_BossGoldDerconMapNumber[3];	// 44

};

extern CEledoradoEvent gEledoradoEvent;

#endif