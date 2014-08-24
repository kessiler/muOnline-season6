#ifndef LARGERAND_H
#define LARGERAND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_LARGE_RAND_STATE	625

class CLargeRand
{
public:

	CLargeRand();
	virtual ~CLargeRand();

	void  seedMT(DWORD seed);
	DWORD  reloadMT();
	DWORD  randomMT()
	{
		this->left--;

		if ( this->left< 0 )
		{
			return this->reloadMT();
		}

		DWORD y = *next;
		next++;
		y ^= ( (DWORD)y >> (DWORD)11 );
		y ^= ( y << 7 ) & 2636928640;
		y ^= ( y << 15 ) & 4022730752;

		return y ^ ( y >> 18 );
	};
 
private:
 
	DWORD state[MAX_LARGE_RAND_STATE];	// 4
	DWORD * next;	// 9C8
	int left;	// 9CC
};

DWORD GetLargeRand();

#endif