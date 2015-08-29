#include "stdafx.h"
#include "zConnectServer.h"
#include "StdAfx.h"
#include "winutil.h"
#include "User.h"

OBJECTSTRUCT	gObj[MAX_CONNECTOBJECT];
int				gObjCount;

void gObjSetTime(int aIndex);

void gObjInit()
{
    gObjCount = 0;
    for( int n=0; n<MAX_CONNECTOBJECT; n++)
    {
        memset(&gObj[n], 0, sizeof(OBJECTSTRUCT));
    }
}

void gObjEnd()
{

}

int gObjAdd(SOCKET aSocket, char *ip)
{
    int n;
    int total;

    n		= gObjCount;
    total	= 0;

    while( TRUE )
    {

        if( gObj[n].Connected == 0 )
        {
            gObj[n].m_Index		= n;
            gObj[n].m_socket	= aSocket;
            gObj[n].Connected	= 1;

            gObjSetTime(n);
            strcpy(gObj[n].m_Ip, ip);

            gObjCount++;
            if( gObjCount >= MAX_CONNECTOBJECT ) gObjCount = 0;

            return n;
        }
        else
        {
            if( GetTickCount() > gObj[n].Time )
            {
                CloseClient( n );
            }
        }
        n++;
        if( n >= MAX_CONNECTOBJECT ) n = 0;

        total ++;
        if( total >= MAX_CONNECTOBJECT ) break;
    }
    return -1;
}

void gObjSetTime(int aIndex)
{
    if( aIndex < 0 || aIndex > MAX_CONNECTOBJECT-1 ) return;
    if( gObj[aIndex].Connected < 1 ) return;

    gObj[aIndex].Time = GetTickCount()+(60*1000);
}

int gObjDel(int index)
{
    if( !CHECK_LIMIT( index, MAX_CONNECTOBJECT ) )
    {
        printf("Error : index error %s %d",__FILE__, __LINE__);
        return FALSE;
    }

    gObj[index].Connected = 0;

    return TRUE;
}

int gObjGetSocket(SOCKET socket)
{
    for( int n=0; n<MAX_CONNECTOBJECT; n++)
    {
        if( gObj[n].Connected )
        {
            if( gObj[n].m_socket == socket )
            {
                return n;
            }
        }
    }
    return -1;
}
