// GameServer 1.00.90 - finished
#include "stdafx.h"
#include "MultiCheckSum.h"
#include "logproc.h"
#include "user.h"


CMultiCheckSum g_MultiChecksum;


CMultiCheckSum::CMultiCheckSum()
{
	this->Init();
}


CMultiCheckSum::~CMultiCheckSum()
{
	return;
}


void CMultiCheckSum::Init()
{
	for (int i=0; i < MAX_MULTICHECKSUM; i++)
	{
		this->m_dwLoaded[i]=0;
	}
	this->m_sFilePath[0]=0;
}


void CMultiCheckSum::SetFilePath(char* filepath)
{
	strcpy(this->m_sFilePath, filepath);
}

int CMultiCheckSum::LoadFile()
{
	char filenames[256];
	int i;

	for (i=0; i<MAX_MULTICHECKSUM; i++)
	{
		if (i == 0)
		{
			wsprintf(filenames, "%s\\CheckSum.dat", this->m_sFilePath);
		}
		else
		{
			wsprintf(filenames, "%s\\CheckSum%d.dat", this->m_sFilePath, i);
		}
		if (this->LoadFile(filenames, i)  != 0)
		{
			this->m_dwLoaded[i]=1;
			LogAddTD("Load Checksum file : %s", filenames);
		}
	}
	return 1;
}


BOOL CMultiCheckSum::LoadFile(char* filename, int index)
{
	FILE* fp;
	
	fp=fopen(filename, "rb");	//ok

	if (fp == 0)
	{
		return 0;
	}

	fread(this->m_dwgCheckSum[index], sizeof(m_dwgCheckSum[index]), 1, fp);
	fclose(fp);
	return TRUE;
}
	
int CMultiCheckSum::CompareCheckSum(int aIndex, DWORD TableNum, DWORD Key)
{
	int i;

	for ( i=0;i<MAX_MULTICHECKSUM;i++)
	{
		if(this->m_dwLoaded[i] != 0 )
		{
			if ( ( this->m_dwgCheckSum[i][TableNum] ) == Key)	
			{
				LogAddTD("CheckSum%d-Exe Compare Success [%s]", i, gObj[aIndex].AccountID );
				return 1;
			}
			else
			{
				LogAddTD("CheckSum%d-Exe Compare Fail %d %d %d %d [%s]", i, this->m_dwgCheckSum[i][TableNum] ,Key, TableNum, gObj[aIndex].AccountID );
			}
		}
	}
	return 0;
}