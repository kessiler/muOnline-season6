//-> Revised by DarkSim | 18.02.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#define MAX_ITEMBAG_ATTR	150	//OK
// -------------------------------------------------------------------------------

class CItemBagAttr	//-> Complete
{
public:
	CItemBagAttr()	//004b29e0	-> 100%
	{
		this->m_type	= 0;
		this->m_index	= 0;
		this->m_level	= 0;
		this->m_op1		= 0;
		this->m_op2		= 0;
		this->m_op3		= 0;
	}
	// ----
public:
	BYTE	m_type;
	BYTE	m_index;
	BYTE	m_level;
	BYTE	m_op1;
	BYTE	m_op2;
	BYTE	m_op3;
};
// -------------------------------------------------------------------------------

class CItemBag	//-> Complete
{
public:
				CItemBag();
	virtual		~CItemBag();
	// ----
	void		Init(LPSTR name);
	void		LoadItem(LPSTR script_file);
	// ----
	int			GetBagCount()
	{
		return this->BagObjectCount;
	};
	// ----
	BYTE		GetType(int n);
	BYTE		GetIndex(int n);
	BYTE		GetLevel(int n);
	BYTE		GetOp1(int n);
	BYTE		GetOp2(int n);
	BYTE		GetOp3(int n);
	// ----
	int			GetNormalItemCount()
	{
		return this->BagNormalItemCount;
	};
	// ----
	int			GetExItemCount()
	{
		return this->BagExItemCount;
	};
	// ----
private:
	CItemBagAttr	BagObject[MAX_ITEMBAG_ATTR];
	int				BagObjectCount;
	int				BagNormalItemCount;
	int				BagExItemCount;
};
// -------------------------------------------------------------------------------