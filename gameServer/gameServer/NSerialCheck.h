// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef NSERIALCHECK_H
#define NSERIALCHECK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class NSerialCheck
{

public:

	NSerialCheck();
	virtual ~NSerialCheck();

	void init();
	BOOL InCheck(BYTE serial);
	int GetSerial();
	BYTE GetSendSerial();

private:

	BYTE m_serial;	// 4
	BYTE m_SendSeiral;	// 5
};

#endif