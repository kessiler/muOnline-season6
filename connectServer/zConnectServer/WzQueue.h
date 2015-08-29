#ifndef __WZQUEUE_H__
#define __WZQUEUE_H__

#include <windows.h>

typedef struct _ListNode
{
    struct _ListNode*	pUpLink;
    struct _ListNode*	pDownLink;
    unsigned char *		pObject;
    unsigned int		nSize;
    BYTE	headcode;
    int     uindex;
} ListNode;

#define MAX_NODE		128
#define SPIN_COUNT		4000

class  WZQueue
{
public:
    WZQueue(unsigned int nMaxNode = MAX_NODE);
    ~WZQueue();

    unsigned int	GetCount();
    BOOL			IsEmpty();
    BOOL			AddToQueue(const BYTE* pObject, unsigned int nSize, BYTE headcode, int uindex);
    BOOL			GetFromQueue(BYTE* pObject, unsigned int * pSize, BYTE * headcode, int * uindex);
    //BOOL			GetFromQueue(BYTE* pObject, unsigned int * pSize);

private:

    ListNode*	GetHeadNode();
    BOOL		AddTail(ListNode* pNewNode);
    void		DeleteAllNodes();

    unsigned int		m_Count;
    unsigned int		m_Max_Node;
    ListNode*		m_pHead;
    ListNode*		m_pTail;

    CRITICAL_SECTION	m_CriticalSection;
};

#endif
