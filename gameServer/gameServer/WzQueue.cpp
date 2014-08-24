//GameServer 1.00.90 JPN - finished
#include "stdafx.h"
#include "WzQueue.h"

//004B5100  /> \55            PUSH EBP
WZQueue::WZQueue(UINT nMaxNode)
{
	this->m_Max_Node = nMaxNode;
	this->m_pTail = 0;
	this->m_pHead = 0;
	this->m_Count = 0;
	BOOL bSuccess;
	bSuccess=InitializeCriticalSectionAndSpinCount(&this->m_CriticalSection , 4000);
	if (bSuccess == 0 )
	{
		if ( GetLastError() == STATUS_NO_MEMORY )	
		{
			InitializeCriticalSection(&this->m_CriticalSection );
		}
	}
}

void WZQueue::Init()
{
	return;	// Empty
}

void WZQueue::Free()
{
	return;	// Empty
}

WZQueue::~WZQueue()
{
	this->DeleteAllNodes();
	DeleteCriticalSection(&this->m_CriticalSection );
}

UINT WZQueue::GetCount()
{
	int nNum;
	EnterCriticalSection(&this->m_CriticalSection);
	nNum=this->m_Count;
	LeaveCriticalSection(&this->m_CriticalSection);
	return nNum;
}

int WZQueue::AddTail( LPLISTNODE pNewNode)
{
	BOOL bRet;
	EnterCriticalSection(&this->m_CriticalSection);
	bRet=1;

	if( this->m_Count < 1280 )
	{
		pNewNode->lpNodePre  = this->m_pTail  ;
		pNewNode->lpNodeNext = 0;

		if ( this->m_pHead  ==0 )
		{
			this->m_pHead = pNewNode;
		}
		else
		{
			this->m_pTail->lpNodeNext = pNewNode;
		}

		this->m_pTail = pNewNode;
		this->m_Count++;
	}
	else
	{
		bRet=0;
	}

	LeaveCriticalSection(&this->m_CriticalSection);
	return bRet;
}
		

LPLISTNODE WZQueue::GetHeadNode()
{
	LPLISTNODE pNode;
	EnterCriticalSection(&this->m_CriticalSection);

	pNode=0;

	if (this->m_pHead != 0)
	{
		pNode=this->m_pHead;

		if ( this->m_pHead->lpNodeNext   != 0)
		{
			this->m_pHead->lpNodeNext->lpNodePre   = 0; 
			this->m_pHead = this->m_pHead->lpNodeNext ;
		}
		else
		{
			this->m_pTail = 0;
			this->m_pHead = 0;
		}

		this->m_Count --;
		
	}

	LeaveCriticalSection(&this->m_CriticalSection );
	
	return pNode; 
}


BOOL WZQueue::AddToQueue(LPBYTE  const pObject , UINT nSize, BYTE headcode, INT uindex)
{
	BOOL bRet=0;
	LPLISTNODE pNewNode;

	EnterCriticalSection(&this->m_CriticalSection );

	if (nSize < 1 || nSize > 65536 )
	{
		return FALSE;
	}

	pNewNode=(LPLISTNODE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(LISTNODE));

	if (pNewNode != 0)
	{
		char* p;
		p=(char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nSize);

		if (p != 0)
		{
			memcpy(p, pObject, nSize);

			pNewNode->pHeapMemory = p;
			pNewNode->nSize = nSize;
			pNewNode->Headcode = headcode;
			pNewNode->uIndex = uindex;
			pNewNode->bIsDataSending = 0;
			pNewNode->iBytesSended  = 0;

			if (this->AddTail(pNewNode) != 0)
			{
				bRet=1;
			}
		}
		else
		{
			HeapFree(GetProcessHeap(), 0, p);
			HeapFree(GetProcessHeap(), 0, pNewNode);
		}
	}
	else
	{
		HeapFree(GetProcessHeap(), 0, pNewNode);
	}

	LeaveCriticalSection(&this->m_CriticalSection);
	return bRet;
}


LPLISTNODE WZQueue::AddToQueueList(LPBYTE  pObject, UINT nSize, BYTE headcode, INT uindex)
{
	BOOL bRet=0;
	LPLISTNODE pNewNode;

	if (nSize < 1 || nSize > 65536 )
	{
		return FALSE;
	}

	EnterCriticalSection(&this->m_CriticalSection );

	pNewNode=(LPLISTNODE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(LISTNODE));

	if (pNewNode != 0)
	{
		char* p;
		p=(char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nSize);

		if (p != 0)
		{
			memcpy(p, pObject, nSize);

			pNewNode->pHeapMemory = p;
			pNewNode->nSize = nSize;
			pNewNode->Headcode = headcode;
			pNewNode->uIndex = uindex;
			pNewNode->bIsDataSending = 0;
			pNewNode->iBytesSended  = 0;

			if (this->AddTail(pNewNode) != 0)
			{
				bRet=1;
			}
		}
		else
		{
			HeapFree(GetProcessHeap(), 0, p);
			HeapFree(GetProcessHeap(), 0, pNewNode);
		}
	}
	else
	{
		HeapFree(GetProcessHeap(), 0, pNewNode);
	}

	LeaveCriticalSection(&this->m_CriticalSection);
	return pNewNode;
}


BOOL WZQueue::GetFromQueue(LPBYTE pObject, UINT * pSize, BYTE * headcode, INT * uindex)
{
	LPLISTNODE pNode=0;

	pNode = this->GetHeadNode();

	if ( pNode != 0 )
	{
		EnterCriticalSection(&this->m_CriticalSection);

		memcpy(pObject, pNode->pHeapMemory , pNode->nSize);

		*pSize = pNode->nSize;
		*headcode = pNode->Headcode;
		*uindex = pNode->uIndex;

		HeapFree(GetProcessHeap(), 0, pNode->pHeapMemory);
		HeapFree(GetProcessHeap(), 0, pNode);

		LeaveCriticalSection(&this->m_CriticalSection);
		return TRUE;
	}
	return FALSE;
}


void WZQueue::DeleteAllNodes()
{
	LPLISTNODE pNode=0;

	EnterCriticalSection(&this->m_CriticalSection);

	while ( (pNode=this->GetHeadNode() ) != 0 )
	{
		HeapFree(GetProcessHeap(), 0, pNode);
	}

	LeaveCriticalSection(&this->m_CriticalSection);
}


BOOL WZQueue::StartQ()
{
	if ( this->m_pHead != 0 )
	{
		EnterCriticalSection(&this->m_CriticalSection); 
		this->m_pCur = this->m_pHead;
		LeaveCriticalSection(&this->m_CriticalSection);
		return TRUE;
	}
	return FALSE;
}

BOOL WZQueue::NextQ()
{
	if ( this->m_pCur  != 0 )
	{
		if ( this->m_pCur->lpNodeNext  != 0 )
		{
			EnterCriticalSection(&this->m_CriticalSection); 
			this->m_pCur = this->m_pCur->lpNodeNext ;
			LeaveCriticalSection(&this->m_CriticalSection);
			return TRUE;
		}
	}
	return FALSE;
}

LPLISTNODE WZQueue::GetCurData(LPBYTE pObject, UINT* pSize, BYTE * headcode, INT * uindex)
{
	if ( this->m_pCur != 0 )
	{
		if ( this->m_pCur->bIsDataSending == 1 )
		{
			return NULL;
		}

		memcpy(pObject, this->m_pCur->pHeapMemory , this->m_pCur->nSize); 

		*pSize = this->m_pCur->nSize;
		*headcode = this->m_pCur->Headcode;
		*uindex = this->m_pCur->uIndex;

		return this->m_pCur;
	}

	return NULL;
}

BOOL WZQueue::Pop(LPLISTNODE pCur, LPBYTE pObject, int nOfs, int * nSize, int * sendbytes)
{
	BOOL bRet=FALSE;

	if ( pCur != 0 )
	{
		__try
		{
			EnterCriticalSection(&this->m_CriticalSection);
			pCur->iBytesSended  = pCur->iBytesSended  + nOfs;

			if ( (pCur->nSize-nOfs) != 0 )
			{
				memcpy(pObject, &pCur->pHeapMemory[pCur->iBytesSended ], pCur->nSize - pCur->iBytesSended );
				*nSize = pCur->nSize - pCur->iBytesSended ;
				*sendbytes = pCur->iBytesSended ;
				bRet=TRUE;
			}
		}

		__finally
		{
			LeaveCriticalSection(&this->m_CriticalSection);
		}
	}

	return bRet;
}


void WZQueue::SetCurDataSending(LPLISTNODE pNode)
{
	if ( pNode != NULL )
	{
		EnterCriticalSection(&this->m_CriticalSection);
		pNode->bIsDataSending = TRUE;
		LeaveCriticalSection(&this->m_CriticalSection);
	}
}

void WZQueue::DeleteNode(LPLISTNODE pCur)
{
	LPLISTNODE pNode=0;
	LPLISTNODE pPre=0;
	LPLISTNODE pNext=0;

	EnterCriticalSection(&this->m_CriticalSection);

	if ( pCur != NULL )
	{
		pNode = pCur;
		pPre = pCur->lpNodePre;
		pNext= pCur->lpNodeNext;

		if ( pPre == 0 && pNext == 0 )
		{
			this->m_pHead = pCur;
		}
		else if ( pPre == 0 && pNext != 0 )
		{
			pNext->lpNodePre = 0;
			pCur = pNext;
			this->m_pHead=pCur;

			if ( pNext->lpNodeNext == 0 )
			{
				this->m_pTail=pNext;
				this->m_pTail->lpNodeNext =0;
			}
		}
		else if ( pPre != 0 && pNext == 0 )
		{
			pPre->lpNodeNext = 0;
			pCur = pPre;
			this->m_pTail = pCur;

			if ( pPre->lpNodePre == 0 )
			{
				this->m_pHead = pPre;
				this->m_pHead->lpNodePre = 0;
			}
		}
		else if ( pPre != 0 && pNext != 0 )
		{
			pPre->lpNodeNext = pNext;
			pNext->lpNodePre = pPre;

			if ( pPre->lpNodePre == 0 )
			{
				this->m_pHead = pPre;
				this->m_pHead->lpNodePre = NULL;
			}

			if ( pNext->lpNodeNext == NULL )
			{
				this->m_pTail = pNext;
				this->m_pTail->lpNodeNext = NULL;
			}
		}

		HeapFree( GetProcessHeap(), 0, pNode->pHeapMemory  );
		HeapFree( GetProcessHeap(), 0, pNode);

		if ( pPre == NULL && pNext == NULL )
		{
			this->m_pHead = NULL;
			pCur = NULL;
			this->m_pTail = NULL;
		}
		
		this->m_Count --;
		
	}

	LeaveCriticalSection(&this->m_CriticalSection);
}