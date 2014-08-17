//Decompilation Completed -> All Same as WebZen
#include "stdafx.h"
#include "WZQueue.h"

WZQueue::WZQueue(unsigned int nMaxNode)
{
	m_Max_Node = nMaxNode;
	m_pHead    = m_pTail = NULL;
	m_Count    = 0;

	BOOL bSuccess =	InitializeCriticalSectionAndSpinCount(&m_CriticalSection, SPIN_COUNT);
	if(!bSuccess)
	{
		if( GetLastError() == STATUS_NO_MEMORY )
			InitializeCriticalSection(&m_CriticalSection);
	}
}

void WZQueue::Init()
{
	/*m_Max_Node = nMaxNode;
	m_pHead    = m_pTail = NULL;
	m_Count    = 0;

	BOOL bSuccess =	InitializeCriticalSectionAndSpinCount(&m_CriticalSection, SPIN_COUNT);
	if(!bSuccess)
	{
		if( GetLastError() == STATUS_NO_MEMORY )
			InitializeCriticalSection(&m_CriticalSection);
	}
	*/
}

void WZQueue::Free()
{
	//DeleteAllNodes();
	//DeleteCriticalSection(&m_CriticalSection);
}


WZQueue::~WZQueue()
{
	DeleteAllNodes();
	DeleteCriticalSection(&m_CriticalSection);
}

unsigned int WZQueue::GetCount()
{
	EnterCriticalSection(&m_CriticalSection);
	DWORD nNum = m_Count;
	LeaveCriticalSection(&m_CriticalSection);
	return nNum;
}

unsigned int WZQueue::GetCount_NoLock()
{
	return m_Count;
}

BOOL WZQueue::AddTail(ListNode* pNewNode)
{
	EnterCriticalSection(&m_CriticalSection);
	BOOL bRet = TRUE;
	if( m_Count < MAX_NODE )
	{
		// crosslink two nodes.
		pNewNode->pUpLink	= m_pTail;
		pNewNode->pDownLink = NULL;

		// check headnode.
		if(m_pHead == NULL)
			m_pHead = pNewNode;
		else
			m_pTail->pDownLink = pNewNode;

		m_pTail = pNewNode;

		m_Count++;
	}
	else 
		bRet = FALSE;
	LeaveCriticalSection(&m_CriticalSection);
	return bRet;
}

ListNode* WZQueue::GetHeadNode()
{
	EnterCriticalSection(&m_CriticalSection);	
	ListNode* pNode = NULL;

	if(m_pHead)
	{
		pNode = m_pHead;
		if(m_pHead->pDownLink)
		{
			m_pHead->pDownLink->pUpLink = NULL;
			m_pHead = m_pHead->pDownLink;
		}
		else
		{
			m_pHead = m_pTail = NULL;
		}

		m_Count--;
	}	
	LeaveCriticalSection(&m_CriticalSection);
	return pNode;
}

BOOL WZQueue::AddToQueue(const BYTE* pObject, unsigned int nSize, BYTE headcode, int uindex, int iSessionId)
{
	BOOL bRet = FALSE;

	//ASSERT( pObject );	
	if( nSize < 1 || nSize > 65536 ) 
	{
		return FALSE;
	}

	EnterCriticalSection(&m_CriticalSection);
	ListNode* pNewNode = (ListNode*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ListNode));
	
	if( pNewNode )
	{
		BYTE* pMsg = (BYTE*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nSize);
		if( pMsg )
		{
			memcpy(pMsg, pObject, nSize);

			pNewNode->pObject	= pMsg;
			pNewNode->nSize		= nSize;
			pNewNode->headcode	= headcode;
			pNewNode->uindex	= uindex;
			pNewNode->bSending	= 0;
			pNewNode->nOfs		= 0;
			pNewNode->iSessionId = iSessionId; //new
			if( AddTail(pNewNode) ) bRet = TRUE;
		}
		else
		{
			HeapFree(GetProcessHeap(), 0, pMsg);
			HeapFree(GetProcessHeap(), 0, pNewNode);
		}
	}
	else
		HeapFree(GetProcessHeap(), 0, pNewNode);
	
	LeaveCriticalSection(&m_CriticalSection);
	return bRet;
}

ListNode* WZQueue::AddToQueueList(const BYTE* pObject, unsigned int nSize, BYTE headcode, int uindex)
{
	BOOL bRet = FALSE;

	//ASSERT( pObject );
	if( nSize < 1 || nSize > 65536 ) 
	{
		return NULL;
	}

	EnterCriticalSection(&m_CriticalSection);
	ListNode* pNewNode = (ListNode*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ListNode));
	
	if( pNewNode )
	{
		BYTE* pMsg = (BYTE*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nSize);
		if( pMsg )
		{
			memcpy(pMsg, pObject, nSize);

			pNewNode->pObject	= pMsg;
			pNewNode->nSize		= nSize;
			pNewNode->headcode	= headcode;
			pNewNode->uindex	= uindex;
			pNewNode->bSending	= 0;
			pNewNode->nOfs		= 0;
			if( AddTail(pNewNode) ) bRet = TRUE;
		}
		else
		{
			HeapFree(GetProcessHeap(), 0, pMsg);
			HeapFree(GetProcessHeap(), 0, pNewNode);
		}
	}
	else
		HeapFree(GetProcessHeap(), 0, pNewNode);
	
	LeaveCriticalSection(&m_CriticalSection);
	return pNewNode;
}

BOOL WZQueue::GetFromQueue(BYTE* pObject, unsigned int * pSize, BYTE * headcode, int * uindex, int * iSessionId)
{
	ListNode*	pNode = NULL;
	//ASSERT( pObject );
	
	//EnterCriticalSection(&m_CriticalSection); //wz removed criti for some reason hmm...
	pNode = GetHeadNode();
	if(pNode)
	{
		
		memcpy(pObject, pNode->pObject, pNode->nSize);
		*pSize = pNode->nSize;
		*headcode =pNode->headcode;
		*uindex = pNode->uindex;
		*iSessionId = pNode->iSessionId; //new
		
		HeapFree(GetProcessHeap(), 0, pNode->pObject);
		HeapFree(GetProcessHeap(), 0, pNode);
		//LeaveCriticalSection(&m_CriticalSection);
		return TRUE;
	}

	//LeaveCriticalSection(&m_CriticalSection);
	return FALSE;
}

void WZQueue::DeleteAllNodes()
{
	ListNode*	pNode = NULL;

	EnterCriticalSection(&m_CriticalSection);
	// DO NOT DELETE Object that pointed by pObject!!!!
	while(pNode = GetHeadNode())
	{
		HeapFree(GetProcessHeap(), 0, pNode);
	}
	LeaveCriticalSection(&m_CriticalSection);
}

BOOL WZQueue::StartQ()
{
	if( m_pHead )
	{
		EnterCriticalSection(&m_CriticalSection);
		m_pCur = m_pHead;
		LeaveCriticalSection(&m_CriticalSection);
		return TRUE;
	}
	return FALSE;
}

BOOL WZQueue::NextQ()
{
	if( m_pCur )
	{
		if( m_pCur->pDownLink != NULL )
		{
			EnterCriticalSection(&m_CriticalSection);
			m_pCur = m_pCur->pDownLink;
			LeaveCriticalSection(&m_CriticalSection);
			return TRUE;
		}
	}
	return FALSE;
}

ListNode* WZQueue::GetCurData(BYTE* pObject, unsigned int * pSize, BYTE * headcode, int * uindex)
{
	if( m_pCur )
	{		
		if( m_pCur->bSending == 1 ) return FALSE;

		memcpy(pObject, m_pCur->pObject, m_pCur->nSize);
		*pSize = m_pCur->nSize;
		*headcode =m_pCur->headcode;
		*uindex = m_pCur->uindex;
		return m_pCur;
	}
	return NULL;
}

BOOL WZQueue::Pop(ListNode* pCur, BYTE *pObject, int nOfs, int *nSize, int *sentbytes)
{
	BOOL bRet=FALSE;

	if( pCur )
	{
		__try
		{
			EnterCriticalSection(&m_CriticalSection);
			pCur->nOfs += nOfs;
			if( pCur->nSize-nOfs )
			{
				memcpy(pObject, pCur->pObject+pCur->nOfs, pCur->nSize-pCur->nOfs);
				*nSize = pCur->nSize-pCur->nOfs;
				*sentbytes = pCur->nOfs;
				bRet = TRUE;
				__leave;
			}
		}
		__finally
		{
			LeaveCriticalSection(&m_CriticalSection);
		}
	}
	return bRet;
}

void WZQueue::SetCurDataSending(ListNode* pNode)
{	
	if( pNode )
	{
		EnterCriticalSection(&m_CriticalSection);
		pNode->bSending = 1;
		LeaveCriticalSection(&m_CriticalSection);
	}
}

void WZQueue::DeleteNode(ListNode* pCur)
{
	ListNode* pNode = NULL;
	ListNode* pPre = NULL;
	ListNode* pNext = NULL;
	EnterCriticalSection(&m_CriticalSection);
	
	if( pCur )
	{
		pNode = pCur;
		pPre  = pCur->pUpLink;
		pNext = pCur->pDownLink;

		if( pPre == NULL && pNext == NULL )
		{
			m_pHead = pCur;
		}
		else if( pPre == NULL && pNext != NULL )
		{
			pNext->pUpLink = NULL;
			pCur = pNext;
			m_pHead = pCur;
			if( pNext->pDownLink == NULL ) {
				m_pTail = pNext;
				m_pTail->pDownLink = NULL;
			}
		}
		else if( pPre != NULL && pNext == NULL )
		{
			pPre->pDownLink = NULL;
			pCur = pPre;
			m_pTail = pCur;
			if( pPre->pUpLink == NULL ) {
				m_pHead = pPre;
				m_pHead->pUpLink = NULL;
			}
		}		
		else if( pPre && pNext )
		{
			pPre->pDownLink = pNext;
			pNext->pUpLink  = pPre;
			if( pPre->pUpLink == NULL ) {
				m_pHead = pPre;
				m_pHead->pUpLink = NULL;
			}
			if( pNext->pDownLink == NULL ) {
				m_pTail = pNext;
				m_pTail->pDownLink = NULL;
			}
		}
		HeapFree(GetProcessHeap(), 0, pNode->pObject);
		HeapFree(GetProcessHeap(), 0, pNode);
		if( pPre == NULL && pNext == NULL ) {
			m_pHead = NULL;
			pCur    = NULL;
			m_pTail = NULL;
		}
		m_Count--;
	}
	LeaveCriticalSection(&m_CriticalSection);
}
