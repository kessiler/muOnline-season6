#pragma once
// -------------------------------------------------------------------------------

#ifdef __CUSTOMS__
// -------------------------------------------------------------------------------

#define	ITEMPRICE_DEBUG		0
#define MAX_ITEMPRICE_ITEM	512
// -------------------------------------------------------------------------------

struct ITEMPRICE_DATA
{
	WORD	ItemID;
	WORD	ItemIndex;
	BYTE	ItemLevel;
	int		Price;
	BYTE	Divisor;
};
// -------------------------------------------------------------------------------

class ItemPrice
{
public:
			ItemPrice();
			~ItemPrice();
	// ----
	void	Init();
	void	ReadList(char * File);
	void	Load();
	bool	CheckItem(short ItemCode, BYTE ItemLevel, __int64 * Price, int * Divisor);
	bool	CheckItem(short ItemCode, BYTE ItemLevel);
	// ----
private:
	int		m_LoadedCount;
	ITEMPRICE_DATA	m_Data[MAX_ITEMPRICE_ITEM];
	// ----
}; extern ItemPrice g_ItemPrice;
// -------------------------------------------------------------------------------

#endif
// -------------------------------------------------------------------------------