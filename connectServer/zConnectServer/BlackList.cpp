#include "stdafx.h"
#include "BlackList.h"
#include "ReadScript.h"
#include "Window.h"
#include "Settings.h"
// -----------------------------------------------------------------------

BlackList g_BlackList;
// -----------------------------------------------------------------------

BlackList::BlackList()
{
    this->Init();
}
// -----------------------------------------------------------------------

BlackList::~BlackList()
{

}
// -----------------------------------------------------------------------

void BlackList::Init()
{
    this->m_LoadedCount = 0;
    ZeroMemory(this->IP, sizeof(IP));
}
// -----------------------------------------------------------------------

void BlackList::Load()
{
    this->Init();
    this->ReadList(".\\BlackList.txt");
}
// -----------------------------------------------------------------------

void BlackList::ReadList(char * File)
{
    char Line[256];
    FILE * Input = fopen(File, "r");
    // ----
    if( !Input )
    {
        MessageBox(NULL, "Enable to load BlackList.txt", "Loading Error", ERROR);
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
        if( this->m_LoadedCount >= MAX_BLACKLIST_IP )
        {
            g_Window.PrintLog("[Error] Black list count is max");
            break;
        }
        // ----
        strncpy(this->IP[this->m_LoadedCount], IP, 16);
        this->m_LoadedCount++;
    }
    // ----
    rewind(Input);
    fclose(Input);
    g_Window.PrintLog("[%s] Black list loaded [%d]", File, this->m_LoadedCount);
}
// -----------------------------------------------------------------------

bool BlackList::Check(char * IP)
{
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

void BlackList::AddIP(char * IP)
{
    FILE * Input = fopen(".\\BlackList.txt", "a");
    if (Input)
    {
        fprintf(Input, "\n%s", IP);
        fclose(Input);
        this->Load();
    }
}
// -----------------------------------------------------------------------