// GS-CS	1.00.90	JPN		-	Completed
#include "stdafx.h"
#include "PacketCheckSum.h"
#include "GameMain.h"
#include "logproc.h"

CPacketCheckSum gPacketCheckSum;

CPacketCheckSum::CPacketCheckSum()
{
	this->Init();
}

CPacketCheckSum::~CPacketCheckSum()
{
	return;
}

void CPacketCheckSum::Init()
{
	this->m_ChecksumTableAllClearState = 0;
	memset(this->m_ChecksumTableClearState, 0, sizeof(this->m_ChecksumTableClearState));
	memset(this->m_ChecksumTableAVGCount, 0, sizeof(this->m_ChecksumTableAVGCount));
	memset(this->m_ChecksumTableAVG, 0, sizeof(this->m_ChecksumTableAVG));
	memset(this->PaketChecksum, 0, sizeof(this->PaketChecksum));
}

void CPacketCheckSum::Check(int aIndex)
{
	if ( gObj[aIndex].m_InWebzen != false )	// Olny for Local Test and Test Mains
	{
		return;
	}

	for ( int i=0 ; i<MAX_PACKET_CHECKSUM_FUNCTION_INDEX ; i++)
	{
		if ( this->PaketChecksum[aIndex].Check[i] == 0 )
		{
			LogAddTD("PacketCheckSum Error [%d][%s][%s] %d",
				aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name, i);
			
			this->ClearCheckSum(aIndex);

			if ( gDisconnectHackUser != FALSE )
			{
				gObjCloseSet(aIndex, 0);
			}

			return;
		}
	}

	LogAddTD("Check PacketCheckSum [%d][%s][%s]", aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name);
	this->ClearCheckSum(aIndex);
}

void CPacketCheckSum::ClearCheckSum(int aIndex)
{
	memset(&this->PaketChecksum[aIndex], 0, sizeof(PAKETCHECKSUM) );
}

BOOL CPacketCheckSum::Add(int aIndex, int funcindex, DWORD checksum)
{
	if ( gObj[aIndex].m_InWebzen != false )
	{
		return true;
	}

	if ( this->m_ChecksumTableAllClearState == 0 )
	{
		this->PaketChecksum[aIndex].Check[funcindex] = checksum;
		return TRUE;
	}
	
	if ( this->m_ChecksumTable[funcindex] == checksum )
	{
		this->PaketChecksum[aIndex].Check[funcindex] = checksum;
	}
	else
	{
		LogAddTD("PacketCheckSum : Compare Fail : [%d][%s][%s] (%d,%d)", aIndex,
			gObj[aIndex].AccountID, gObj[aIndex].Name, funcindex, checksum);

		if ( gDisconnectHackUser != FALSE )
		{
			gObjCloseSet(aIndex, 0);
		}
	}

	return TRUE;
}

void CPacketCheckSum::AddCheckSum(int aIndex, int funcindex, DWORD checksum)
{
	if ( funcindex < 0 || funcindex >= MAX_PACKET_CHECKSUM_FUNCTION_INDEX )
	{
		LogAddTD("PacketCheckSum : Invalid FuncIndex : %d, %d", aIndex, funcindex);
		return;
	}

	this->Add(aIndex, funcindex, checksum);

	if ( this->m_ChecksumTableAllClearState != 0 )
	{
		return;
	}

	if ( this->m_ChecksumTableClearState[funcindex] != 0 )
	{
		return;
	}

	this->m_ChecksumTableAVG[funcindex][this->m_ChecksumTableAVGCount[funcindex] % MAX_CHECKSUM_TABLE_AVG] = checksum;
	this->m_ChecksumTableAVGCount[funcindex]++;

	LogAddTD("Insert PacketCheckSum %d, %d,%d", funcindex, this->m_ChecksumTableAVGCount[funcindex], checksum);

	if ( this->m_ChecksumTableAVGCount[funcindex] >= MAX_CHECKSUM_TABLE_AVG )
	{
		this->SetClearChecksumFunc(funcindex);
	}
}

void CPacketCheckSum::SetClearChecksumFunc(int funcindex)
{
	int iCount = 0;
	int Checksum = this->m_ChecksumTableAVG[funcindex][0];

	for ( int i=0;i<MAX_CHECKSUM_TABLE_AVG;i++)
	{
		if ( this->m_ChecksumTableAVG[funcindex][i] == Checksum )
		{
			iCount++;
		}
	}

	if ( iCount >= MAX_CHECKSUM_TABLE_AVG-1 )
	{
		this->m_ChecksumTableClearState[funcindex] = 1;
		this->m_ChecksumTableAllClearState = TRUE;

		for (int i=0;i<MAX_PACKET_CHECKSUM_FUNCTION_INDEX;i++)
		{
			if ( this->m_ChecksumTableClearState[i] == 0 )
			{
				this->m_ChecksumTableAllClearState = FALSE;
			}

			this->m_ChecksumTable[i] = this->m_ChecksumTableAVG[i][0];
		}

		if ( this->m_ChecksumTableAllClearState != FALSE )
		{
			LogAddTD("Complete Init Checksum");

			for (int i=0;i<MAX_PACKET_CHECKSUM_FUNCTION_INDEX;i++)
			{
				this->m_ChecksumTable[i] = this->m_ChecksumTableAVG[i][0];
				LogAddTD("%d, %d", i, this->m_ChecksumTable[i]);
			}
		}
	}
}