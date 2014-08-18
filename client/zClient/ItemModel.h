#pragma once
// -------------------------------------------------------------------------------

#include <vector>
// -------------------------------------------------------------------------------

#define ItemModelPath	"Data\\Custom\\Item\\"
#define ItemTexturePath	"Custom\\Item\\"
// -------------------------------------------------------------------------------

struct ItemModelData
{
	BYTE	ItemType;
	WORD	ItemIndex;
	char	Model[MAX_PATH];
	BYTE	R, G, B;
};
// -------------------------------------------------------------------------------

class ItemModel
{
public:
			ItemModel();
			~ItemModel();
	// ----
	void	Init();
	void	Load();
	void	Read(char * File);
	// ----
	bool	SetColor(int ModelID, VAngle * ColorBuffer);
	void	SetModel();
	void	SetTexture();
	// ----
	std::vector<ItemModelData> m_Data;
	// ----
}; extern ItemModel g_ItemModel;
// -------------------------------------------------------------------------------