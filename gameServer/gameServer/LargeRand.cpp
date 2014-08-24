//GameServer 1.00.90 - completed
#include "stdafx.h"
#include "LargeRand.h"

CLargeRand g_LargLand;

DWORD GetLargeRand()
{
	return g_LargLand.randomMT();
}


CLargeRand::CLargeRand()
{
	this->left = -1;
}



CLargeRand::~CLargeRand()
{
	return;
}


void CLargeRand::seedMT(DWORD seed)
{
	DWORD x = seed | 1;
	DWORD * s = this->state;
	this->left = 0;
	*s = x;
	s++;

	for ( int j = MAX_LARGE_RAND_STATE-1 ; --j != 0 ;  x *= 69069, *s = x, s++);
}



DWORD CLargeRand::reloadMT()
{
	DWORD * p0 = this->state;
	DWORD * p2 = &this->state[2];
	DWORD * pM = &this->state[397];

	if ( this->left < -1 )
		this->seedMT(0x1105);

	this->left = MAX_LARGE_RAND_STATE-2;
	this->next = &this->state[1];
	DWORD s0 = this->state[0];	
	DWORD s1 = this->state[1];

	for ( int j= 228; --j != 0 ; s0 = s1, s1 = *p2, p2++)
	{
		*p0++ = (*pM++ ^ (((s0 & 0x80000000) | (s1 & 0x7FFFFFFF)) >> 1)) ^ (((s1&1!=0)?-1:0)&0x9908B0DF);
	}

	pM = this->state;

	for(int j = 0x18D; --j != 0 ; s0=s1 , s1 = *p2 , p2++ )
	{
		*p0++ = (*pM++ ^ (((s0 & 0x80000000) | (s1 & 0x7FFFFFFF)) >> 1)) ^ (((s1&1!=0)?-1:0)&0x9908B0DF);
	}

	s1 = this->state[0];
	*p0 = (*pM ^ (((s0 & 0x80000000) | (s1 & 0x7FFFFFFF)) >> 1)) ^ (((s1&1!=0)?-1:0)&0x9908B0DF);
	s1 ^= s1 >> 11;
	s1 ^= (s1 << 7) & 0x9D2C5680;
	s1 ^= (s1 << 15) & 0xEFC60000;

	return s1 ^ ( s1 >> 18 );
}
