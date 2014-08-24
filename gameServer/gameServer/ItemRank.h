#pragma once
// -------------------------------------------------------------------------------

#define ITEMRANK_MAX_OPTION		6
// -------------------------------------------------------------------------------

struct ItemRank_List
{
	BYTE	RankID;
	WORD	ItemType;
};
// -------------------------------------------------------------------------------

struct ItemRank_Data
{
	BYTE	RankID;
	short	OptionValue[ITEMRANK_MAX_OPTION];
	std::vector<ItemRank_List> ItemList;
	// ----
	ItemRank_Data()
	{
		ZeroMemory(this, sizeof(*this));
	}
};
// -------------------------------------------------------------------------------

class ItemRank
{
public:
			ItemRank();
			~ItemRank();
	// ----
	void	Init();
	void	Load();
	void	Read(char * File);
	// ----
	bool	IsRankItem(WORD ItemType);
	short	GetValue(WORD ItemType, BYTE OptionType);
	short	GetDataIndex(BYTE RankID);
	// ----
private:
	std::vector<ItemRank_Data> m_Data;
	// ----
}; extern ItemRank g_ItemRank;
// -------------------------------------------------------------------------------