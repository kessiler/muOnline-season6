#include "stdafx.h"
#include "WhiteList.h"
#include "ReadScript.h"
#include "Window.h"
#include "Settings.h"
// -----------------------------------------------------------------------

WhiteList g_WhiteList;
// -----------------------------------------------------------------------

WhiteList::WhiteList()
{
    this->Init();
}
// -----------------------------------------------------------------------

WhiteList::~WhiteList()
{

}
// -----------------------------------------------------------------------

void WhiteList::Init()
{
    this->m_LoadedCount = 0;
    ZeroMemory(this->IP, sizeof(IP));
}
// -----------------------------------------------------------------------

void WhiteList::Load()
{
    this->Init();
    this->ReadList(".\\WhiteList.txt");
}
// -----------------------------------------------------------------------

void WhiteList::ReadList(char * File)
{
    char Line[256];
    FILE * Input = fopen(File, "r");
    // ----
    if( !Input )
    {
        MessageBox(NULL, "Enable to load WhiteList.txt", "Loading Error", ERROR);
        return;
    }
    // ----
    rewind(Input);
    // ----
    while(fgets(Line, 255, Input) != NULL)
    {
        if(Line[0] == '/')continue;
        if(Line[0] == ';')continue;
        if(Line[0] == 'e' && Line[1] == 'n' && Line[2] == 'd')
        {
            continue;
        }
        // ----
        if( strlen(Line) < 7 )
        {
            continue;
        }
        // ----
        char IP[16] = { 0 };
        sscanf(Line, "%16s", &IP);
        // ----
        if( this->m_LoadedCount >= MAX_WHITELIST_IP )
        {
            g_Window.PrintLog("[Error] White list count is max");
            break;
        }
        // ----
        strncpy(this->IP[this->m_LoadedCount], IP, 16);
        this->m_LoadedCount++;
    }
    // ----
    rewind(Input);
    fclose(Input);
    g_Window.PrintLog("[%s] White list loaded [%d]", File, this->m_LoadedCount);
}
// -----------------------------------------------------------------------

bool WhiteList::Check(char * IP)
{
    if( !gSettings.WhiteList )
    {
        return true;
    }
    // ----
    for( int i = 0; i < this->m_LoadedCount; i++ )
    {
        if( strcmp(IP, this->IP[i]) == false )
        {
            return true;
        }
    }
    // ----
    return false;
}
// -----------------------------------------------------------------------

void WhiteList::AddIP(char * IP)
{
    FILE * Input = fopen(".\\WhiteList.txt", "a");
    if (Input)
    {
        fprintf(Input, "\n%s", IP);
        fclose(Input);
        this->Load();
    }
}
// -----------------------------------------------------------------------