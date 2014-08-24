#pragma once
// -------------------------------------------------------------------------------

class Timer
{
public:
			Timer(bool bOnceWake);
	void	Init();
			~Timer();
	// ----
	bool	IsWake();
	void	SetTimer(DWORD dwMilliSecond);
	// ----
private:
	DWORD	m_iWakeTick;	// +0x0(0x4)
	bool	m_bOnceWake;	// +0x4(0x1)
	bool	m_bActive;		// +0x5(0x1)
};
// -------------------------------------------------------------------------------

class PacketStatistics
{
public:
	enum PROCESS_RETURN
	{ 
		PS_NORMAL	= 1,
		PS_RESET	= 2,
	};
	// ----
			PacketStatistics();
	void	Init(DWORD iCheckDuration);
			~PacketStatistics();
	// ----
	PROCESS_RETURN Process();
	// ----
	void	AddPacketInfo(bool bEncrypt, DWORD iPackSize);
	void	GetStatistics(int & iOutPacketCount, int & iOutEncryptPacketCount, DWORD & iOutPacketTotalSize);
	// ----
private:
	Timer	m_kTimer;				// +0x0(0x8)
	DWORD	m_iCheckDuration;		// +0x8(0x4)
	int		m_iPacketCount;			// +0xc(0x4)
	int		m_iEncryptPacketCount;	// +0x10(0x4)
	DWORD	m_iPacketTotalSize;		// +0x14(0x4)
};
// -------------------------------------------------------------------------------