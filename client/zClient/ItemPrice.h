#pragma once
// ----------------------------------------------------------------------------------------------

#define	ITEMPRICE_DEBUG		0
#define MAX_ITEMPRICE_ITEM	512
// ----------------------------------------------------------------------------------------------

struct ITEMPRICE_DATA
{
	WORD	ItemID;
	WORD	ItemIndex;
	BYTE	ItemLevel;
	__int64	Price;
	BYTE	Divisor;
};
// ----------------------------------------------------------------------------------------------

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
	// ----
private:
	int		m_LoadedCount;
	ITEMPRICE_DATA	m_Data[MAX_ITEMPRICE_ITEM];
	// ----
}; extern ItemPrice gItemPrice;
// ----------------------------------------------------------------------------------------------