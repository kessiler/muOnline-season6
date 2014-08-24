#pragma once
// -------------------------------------------------------------------------

#define MAX_FILTER_LIST	500
// -------------------------------------------------------------------------

struct WordList
{
	char Text[180];
};
// -------------------------------------------------------------------------

class ChatFilter
{
public:
	void	Init();
	void	Load();
	void	ReadList(char * File);
	void	ReadMain(char * File);
	// ----
	int		StringToUTF8(CString & Text);
	int		CheckText(char * Text);
	// ----
private:
	WordList Filter[MAX_FILTER_LIST];
	int		Counter;
	// ----
public:
	int		BanTime;
	bool	UsePenalty;
	// ----
}; extern ChatFilter g_ChatFilter;
// -------------------------------------------------------------------------