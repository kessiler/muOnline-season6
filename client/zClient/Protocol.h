#pragma once
// ----------------------------------------------------------------------------------------------

class Protocol
{
public:
	void		Load();
	// ----
	static void	DataRecv(DWORD Case, LPBYTE Data, int Len, int aIndex);
	void		DataSend(LPBYTE Data, int Size);
	// ----
}; extern Protocol gProtocol;
// ----------------------------------------------------------------------------------------------