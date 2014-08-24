//	GS-CS	1.00.90	JPN	-	Completed
#include "stdafx.h"
#include "IpCache.h"

CIpCache IpCache;

CIpCache::CIpCache()
{
	this->Init();	
}

CIpCache::~CIpCache()
{

}

void CIpCache::Init()
{
	for (int n=0;n<MAX_IP_CACHE; n++)
	{
		this->m_IpInfo[n].IpUsed = FALSE;
	}
}

BOOL CIpCache::AddIp(char* ip)
{
	int n;	// Iterator
	int blank=-1;
	
	for (n=0;n<MAX_IP_CACHE;n++)
	{
		if (this->m_IpInfo[n].IpUsed != FALSE)
		{
			if (GetTickCount()>this->m_IpInfo[n].Time )
			{
				this->m_IpInfo[n].IpUsed = FALSE;

				if(blank < 0)
				{
					blank = n;	// Set Array
				}
			}
			else if (this->m_IpInfo[n].IpAddress[0]==ip[0])	// Faster reduce
			{
				if (this->m_IpInfo[n].IpAddress[4]==ip[4])	// Faster Reduce
				{
					if (!strcmp(this->m_IpInfo[n].IpAddress, ip))
					{
						return 0;
					}
				}
			}
		}
		else
		{
			if (blank < 0)
			{
				blank = n;	// Set Array
			}
		}
	}

	if (blank >= 0 )
	{
		strcpy(this->m_IpInfo[blank].IpAddress, ip);

		this->m_IpInfo[blank].IpUsed = TRUE;
		
		this->m_IpInfo[blank].Time = GetTickCount()+ 30000;		
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void CIpCache::DelIp(char* ip)
{
	for (int n=0; n<MAX_IP_CACHE;n++)
	{
		if (this->m_IpInfo[n].IpUsed != FALSE )	// Check if it is used
		{
			if (this->m_IpInfo[n].IpAddress[0] == ip[0])	// Fastest Discart
			{
				if (this->m_IpInfo[n].IpAddress[4] == ip[4])	// Fastest Discard
				{
					if (!strcmp(this->m_IpInfo[n].IpAddress , ip))	// Full Discard
					{
						this->m_IpInfo[n].IpUsed = FALSE;	// Ip- Disconnected
					}
				}
			}
		}
	}	// Iterator
}