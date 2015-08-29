#pragma once
// -----------------------------------------------------------------------

#define MAX_BLACKLIST_IP	500
// -----------------------------------------------------------------------

class BlackList
{
public:
    BlackList();
    ~BlackList();
    // ----
    void	Init();
    void	Load();
    void	ReadList(char * File);
    bool	Check(char * IP);
    void	AddIP(char * IP);
    // ----
    char	IP[17][MAX_BLACKLIST_IP];
    // ----
private:
    int		m_LoadedCount;
    // ----
};
extern BlackList g_BlackList;
// -----------------------------------------------------------------------