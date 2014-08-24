#ifndef SIMPLEMODULUS_H
#define SIMPLEMODULUS_H


#pragma pack(1)

struct ENCDEC_FILEHEADER
{
	short sFileHeader;
	int dwSize;
};

typedef DWORD dwXORTable[4];

#pragma pack()




class CSimpleModulus
{

public:

	CSimpleModulus();
	virtual ~CSimpleModulus();

	void Init();

	DWORD m_dwModulus[4];	// 4
	DWORD m_dwEncryptionKey[4];	// 14
	DWORD m_dwDecryptionKey[4];	// 24
	DWORD m_dwXORKey[4];	// 34

protected:

	static DWORD s_dwSaveLoadXOR[4];

public:

	int Encrypt(void * lpDest, void * lpSource, int iSize);
	int Decrypt(void * lpDest, void * lpSource, int iSize);

protected:

	int EncryptBlock(void *, void *, int);
	int DecryptBlock(void *, void *);
	int CSimpleModulus::AddBits(void*lpDest,int iDestBitPos,void*lpSource,int iBitSourcePos,int iBitLen);
	void Shift(void*lpBuff,int iSize,int ShiftLen);
	int GetByteOfBit(int);

public:

	BOOL SaveAllKey(LPSTR lpszFileName);
	BOOL LoadAllKey(LPSTR lpszFileName);
	BOOL SaveEncryptionKey(LPSTR lpszFileName);
	BOOL LoadEncryptionKey(LPSTR lpszFileName);
	BOOL SaveDecryptionKey(LPSTR lpszFileName);
	BOOL LoadDecryptionKey(LPSTR lpszFileName);

protected:

	BOOL SaveKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bSaveModulus, BOOL bSaveEncKey, BOOL bSaveDecKey, BOOL bSaveXORKey);
	BOOL LoadKey(LPSTR lpszFileName, WORD wFileHeader, BOOL bLoadModulus, BOOL bLoadEncKey, BOOL bLoadDecKey, BOOL bLoadXORKey);
};

//DWORD CSimpleModulus::s_dwSaveLoadXOR[4];

#endif



/*class CSimpleModulus {

public:

	CSimpleModulus();
	virtual ~CSimpleModulus();

	VOID Init();
	BOOL Encrypt(unsigned char*Dest,unsigned char*Src,int Len);
	BOOL Decrypt(unsigned char*Dest,unsigned char*Src,int Len);
	BOOL EncryptBlock(unsigned char*Dest,unsigned char*Src,int Len);
	BOOL DecryptBlock(unsigned char*Dest,unsigned char*Src);
	INT AddBits(unsigned char*Dest,int Param10,unsigned char*Src,int Param18,int Param1c);
	VOID Shift(unsigned char*Buff,int Len,int ShiftLen);
	BYTE GetByteOfBit(int btByte);
	BOOL SaveAllKey(char* filename);
	BOOL LoadAllKey(char* filename);
	BOOL SaveEncryptionKey(char* filename);
	BOOL LoadEncryptionKey(char* filename);
	BOOL SaveDecryptionKey(char* filename);
	BOOL LoadDecryptionKey(char * filename);
	BOOL SaveKey(char * filename, int FileHeader, BOOL IsXORC1, BOOL IsXORC2, BOOL IsXORC3, BOOL IsXORC4);
	BOOL LoadKey(char * filename, int FileHeader, BOOL IsXORC1, BOOL IsXORC2, BOOL IsXORC3, BOOL IsXORC4);

private:

	dwXORTable m_XORTableC1;
	dwXORTable m_XORTableC2;
	dwXORTable m_XORTableC3;
	dwXORTable m_XORTableC4;

};*/
