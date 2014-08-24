#pragma once
// -------------------------------------------------------------------------------

#define MAX_NEWS_LIST	12
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct NEWS_TITLE
{
	char	Date[12];
	char	Time[7];
	char	Text[70];
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct NEWS_DATA
{
	NEWS_TITLE Title;
	char	Text[700];
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct NEWS_ANS_TITLES
{
	PWMSG_HEAD h;
	BYTE	RealCount;
	NEWS_TITLE Titles[MAX_NEWS_LIST];
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct NEWS_REQ_NEWS
{
	PBMSG_HEAD	h;
	BYTE		ID;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct NEWS_ANS_NEWS
{
	PWMSG_HEAD h;
	BYTE	ID;
	NEWS_DATA News;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

class NewsBoard
{
public:
			NewsBoard();
			~NewsBoard();
	// ----
	void	Init();
	void	ReadListData(char * File);
	void	ReadMainData(char * File);
	void	Load();
	// ----
	void	OpenMain(LPOBJ lpUser);
	void	OpenItem(LPOBJ lpUser, NEWS_REQ_NEWS * Request);
	// ----
	bool	m_ShowOnLogin;
	// ----
private:
	int		m_LoadedCount;
	NEWS_DATA m_Data[MAX_NEWS_LIST];
	// ----
}; extern NewsBoard g_NewsBoard;
// -------------------------------------------------------------------------------