#pragma once
// ----------------------------------------------------------------------------------------------

#define TRAYMODE_ICON_ID		0x444
#define TRAYMODE_ICON_MESSAGE	0x445
// ----------------------------------------------------------------------------------------------

class TrayMode
{
public:
	void	Load();
	// ----
	void	SwitchState();
	void	ShowNotify(bool Mode);
	void	ShowMessage(DWORD Type, char * Title, char * Message);
	// ----
	LRESULT	TempWindowProc;
	HICON	TempIconProc;
	bool	InTray;
	// ----
}; extern TrayMode gTrayMode;
// ----------------------------------------------------------------------------------------------