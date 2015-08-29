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

BOOL WZQueue::AddTail(ListNode* pNewNode)
{
    EnterCriticalSection(&m_CriticalSection);

    BOOL bRet = TRUE;
    if( m_Count < MAX_NODE )
    {
        pNewNode->pUpLink	= m_pTail;
        pNewNode->pDownLink = NULL;

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

BOOL WZQueue::AddToQueue(const BYTE* pObject, unsigned int nSize, BYTE headcode, int uindex)
{
    BOOL bRet = FALSE;

    //ASSERT( pObject );

    if( nSize < 1 || nSize > 65536 )
    {
        return FALSE;
    }

    ListNode* pNewNode = (ListNode*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ListNode));

    if( pNewNode )
    {
        BYTE* pMsg = (BYTE*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nSize);

        if( pMsg )
        {
            memcpy(pMsg, pObject, nSize);

            pNewNode->pObject = pMsg;
            pNewNode->nSize = nSize;
            pNewNode->headcode = headcode;
            pNewNode->uindex   = uindex;

            if( AddTail(pNewNode) ) bRet = TRUE;
        }
        else
        {
            HeapFree(GetProcessHeap(), 0, pNewNode);
        }
    }

    return bRet;
}

BOOL WZQueue::GetFromQueue(BYTE* pObject, unsigned int * pSize, BYTE * headcode, int * uindex)
{
    ListNode*	pNode = NULL;

    //ASSERT( pObject );

    pNode = GetHeadNode();
    if(pNode)
    {
        memcpy(pObject, pNode->pObject, pNode->nSize);
        *pSize = pNode->nSize;
        *headcode =pNode->headcode;
        *uindex = pNode->uindex;

        HeapFree(GetProcessHeap(), 0, pNode->pObject);
        HeapFree(GetProcessHeap(), 0, pNode);
        return TRUE;
    }

    return FALSE;
}

void WZQueue::DeleteAllNodes()
{
    ListNode*	pNode = NULL;

    // DO NOT DELETE Object that pointed by pObject!!!!
    while(pNode = GetHeadNode())
    {
        HeapFree(GetProcessHeap(), 0, pNode);
    }
}
