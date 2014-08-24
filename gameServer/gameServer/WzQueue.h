#ifndef WZQUEUE_H
#define WZQUEUE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _ListNode
{
	_ListNode * lpNodePre;	// 0
	_ListNode * lpNodeNext;	// 4
	char* pHeapMemory; // 8
	UINT nSize; // C
	BOOL bIsDataSending; // 10
	int iBytesSended;	// 14
	BYTE Headcode; // 18
	UINT uIndex; // 1C

}LISTNODE, * LPLISTNODE;



class WZQueue
{
public:

	WZQueue(UINT nMaxNode);
	~WZQueue();

	UINT GetCount();
	//BOOL __thiscall IsEmpty();	// Never Used
	int AddToQueue(LPBYTE const pObject , UINT nSize, BYTE headcode, int uindex);
	LPLISTNODE AddToQueueList(LPBYTE pObject, UINT nSize, BYTE headcode, int uindex);
	int GetFromQueue(LPBYTE pObject, UINT* pSize, BYTE* headcode,  int* uindex);
	void Init();
	void Free();
	int StartQ();
	int NextQ();
	LPLISTNODE GetCurData(LPBYTE pObject, UINT* pSize, BYTE * headcode, int* uindex);
	int Pop( LPLISTNODE pCur, LPBYTE pObject, int nOfs, int* nSize, int* sentbytes);
	void DeleteNode( LPLISTNODE pCur);
	void SetCurDataSending( LPLISTNODE pNode);

private:

	LPLISTNODE GetHeadNode();
	int AddTail( LPLISTNODE pNewNode);
	void DeleteAllNodes();

private:

	UINT m_Count;	// 0
	UINT m_Max_Node;	// 4
	LPLISTNODE m_pHead;	// 8
	LPLISTNODE m_pTail;	// C
	LPLISTNODE m_pCur;	// 10
	CRITICAL_SECTION m_CriticalSection;	// 14


/*	UINT m_QueueCount; // unk0
	int m_MaxNode; // unk4
	LPWZNODE m_lpWzNode; // unk8
	LPWZNODE m_lpWzNode2; // unkC
	LPWZNODE m_lpStartQ; // unk10
	CRITICAL_SECTION mWzqcriti;*/
};

#endif