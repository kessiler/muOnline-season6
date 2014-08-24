#pragma once
// -------------------------------------------------------------------------------

enum eImperialState
{
	IMPERIAL_STATE_READY	= 0,
	IMPERIAL_STATE_ATTACK	= 1,
	IMPERIAL_STATE_WAIT		= 2,
	IMPERIAL_STATE_LOOT		= 3,
	IMPERIAL_STATE_WARP		= 4,
	IMPERIAL_STATE_FAIL		= 5,
	IMPERIAL_STATE_KICK		= 6,
	IMPERIAL_STATE_CLEAR	= 7,
};
// -------------------------------------------------------------------------------

#define IMPERIAL_MAX_ZONE				4
#define IS_IMPERIAL_ZONE(nZoneIndex)	(nZoneIndex >= 0 && nZoneIndex <= 3)
// -------------------------------------------------------------------------------