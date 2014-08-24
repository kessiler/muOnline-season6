#pragma once
// -------------------------------------------------------------------------------

#include <Iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Wininet")
// -------------------------------------------------------------------------------

#define	LICENSE_SERVER			"ihr`ob`/k(utdl?us~*.E`re"
#define LICENSE_MAX_CUSTOMID	1
// -------------------------------------------------------------------------------

#ifdef LICENSE
#define LICENSE_CHECK	g_License.Load();
#else
#define LICENSE_CHECK
#endif
// -------------------------------------------------------------------------------

struct CUSTOMER_DATA
{
	WORD	CustomerID;
	bool	CustomState[LICENSE_MAX_CUSTOMID];
	DWORD	DriveID;
	BYTE	Adapter[MAX_ADAPTER_ADDRESS_LENGTH];
	int		CurrentDay;
	char	Hash1[100];
	char	Hash2[100];
};
// -------------------------------------------------------------------------------

namespace Customer
{
	enum T
	{
		Refinery	= 2,
		Bereznuk	= 3,
		Zeon		= 4,
		White		= 6,
		Legend		= 7,
		Essin		= 8,
		Reedlan		= 9,
		MUAngel		= 10,
		MUAngel2	= 11,
		Refinery2	= 12,
		White2		= 13,
		drynea		= 14,
		Aliennation	= 16,
		virnet		= 19,
	};
};
// -------------------------------------------------------------------------------

namespace Custom
{
	enum T
	{
		ConnectEx	= 0,
	};
};
// -------------------------------------------------------------------------------

class License
{
public:
			License();
			~License();
	// ----
	void	Init();
	void	Load();
	// ----
	void	GetCustomState();
	void	GetDrive();
	void	GetAdapter();
	void	GetHash();
	void	SendRequest();
	void	SendAlert(int Type, int Code);
	void	CheckKey();
	void	Destruct();
	void	CheckBan();
	// ----
	int		GetLicenseID() { return this->m_Data.CustomerID; };
	LPSTR	GetText(LPSTR EncodedChar);
	// ----
	CUSTOMER_DATA m_Data;
	// ----
}; extern License g_License;
// -------------------------------------------------------------------------------

extern bool IsLicenseChecked;
// -------------------------------------------------------------------------------