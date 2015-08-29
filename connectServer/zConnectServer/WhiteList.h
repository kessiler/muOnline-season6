#pragma once
// -----------------------------------------------------------------------

#define MAX_WHITELIST_IP	500
// -----------------------------------------------------------------------

class WhiteList
{
public:
    WhiteList();
    ~WhiteList();
    // ----
    void	Init();
    void	Load();
    void	ReadList(char * File);
    bool	Check(char * IP);
    void	AddIP(char * IP);
    // ----
    char	IP[17][MAX_WHITELIST_IP];
    // ----
private:
    int		m_LoadedCount;
    // ----
};
extern WhiteList g_WhiteList;
// -----------------------------------------------------------------------