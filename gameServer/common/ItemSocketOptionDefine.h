#pragma once
// -------------------------------------------------------------------------------

#define MAX_SEED_COUNT		50
#define	MAX_SEED_TYPE		6
#define MAX_SEED_EFFECT		12
#define MAX_SPHERE_DROP		10000
#define MAX_SOCKET_SLOT		5
// -------------------------------------------------------------------------------

struct SOCKET_OPTION_LIST	//-> 6 (1.00.92)
{
	void SOCKET_OPTION_LIST::Clear();
	// ----
    BYTE	m_SocketOptionIndex;		//+0
    BYTE	m_SocketOptionValueType;	//+1
    WORD	m_SocketOptionValue;		//+2
    BYTE	m_SocketOptionCount;		//+4
};
// -------------------------------------------------------------------------------

struct BONUS_SOCKET_OPTION	//-> 20 (1.00.92)
{
	void BONUS_SOCKET_OPTION::Clear();
	// ----
    BYTE	OptionIndex;
    BYTE	EffectType;
    BYTE	ItemTypeBegin;
    BYTE	ItemTypeEnd;
    BYTE	OptionValueType;
    BYTE	OptionValue;
    int		ExtractRate;
    BYTE	SocketSlotType[MAX_SOCKET_SLOT];
    BYTE	SocketSlotCount;
};
// -------------------------------------------------------------------------------

struct SET_SOCKET_OPTION //-> 16 (1.00.92)
{
    void	Clear();
	// ----
    BYTE	OptionIndex;
    BYTE	OptionValueType;
    BYTE	OptionValue;
    BYTE	EffectType;
    int		ExtractRate;
    BYTE	SeedTypeTable[MAX_SEED_TYPE];
};
// -------------------------------------------------------------------------------