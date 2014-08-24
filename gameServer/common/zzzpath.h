// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// Status : Completed
#ifndef ZZZPATH_H
#define ZZZPATH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct PATH_t
{
  int PathNum;	// 0
  int PathX[15];	// 4
  int PathY[15];	// 40
  int CurrentPath;	// 7C
};



class PATH
{

private:
	
	int Width; // 0
	int Height; // 4
	int NumPath; // unk8
	BYTE PathX[500];	// C
	BYTE PathY[500];	// 200
	int Dir[16]; // 3F4
	int LastDir; // 434
	int NumFails;	// 438
	LPBYTE Map; // 43C
	LPBYTE HitMap; // 440
	int stx;	// 444
	int sty;	// 448
	int edx;	// 44C
	int edy;	// 450

private:

	

	int GetDist(int x1, int y1, int x2, int y2) // line : 20
	{
		int distx = x2 - x1;
		int disty = y2 - y1;
		return (distx * distx) + (disty * disty);
	}	// line : 23

	int VerifyThatOnPath(int x, int y)
	{
		for ( int i = 0 ; i < this->NumPath ; i++ )
		{
			if ( x ==  this->PathX[i] && y == this->PathY[i] )
			{
				return i;
			}
		}
		return -1;
	}

	BOOL CanWeMoveForward(int x, int y) // line : 33
	{
		int which = x+ y * this->Width  ;
		
		if ( x >= this->Width || x < 0 || y >= this->Height || y < 0 )
		{
			return false;
		}

		if ( this->Map[which] > 1 || this->HitMap[which] != 0 )
		{
			this->HitMap[which] = 1;
			return false;
		}
		return TRUE;
	};	// line : 43

	BOOL CanWeMoveForward2(int x, int y) // line : 46
	{
		int which = x +  y* this->Width ;
		
		if ( x >= this->Width || x < 0 || y >= this->Height || y < 0 )
		{
			return false;
		}

		if ( this->Map[which] >= 1 || this->HitMap[which] != 0 )
		{
			this->HitMap[which] = 1;
			return false;
		}
		return TRUE;
	};
	
	BOOL CanWeMoveForward3(int x, int y) // line : 46
	{
		int which = x +  y* this->Width ;
		
		if ( x >= this->Width || x < 0 || y >= this->Height || y < 0 )
		{
			return false;
		}

		if ( this->Map[which] >= 1 || this->HitMap[which] != 0 )
		{
			this->HitMap[which] = 1;
			return false;
		}
		return TRUE;
	};

	// line : 56


	BOOL IsThisSpotOK(int x, int y) // line : 59
	{
		int pos;

		pos = x+ y * this->Width ;

		if ( x < 0 || x >= this->Width || y < 0 || y >= this->Width )	// Make Deathway fix here maybe
		{
			return false;
		}
		if ( this->HitMap[pos] != 0 )
		{
			return false;
		}
		return true;
	};	// line : 68

private:

	int FindNextDir(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // line : 70
	{
		int MinDist = 10000000;
		long ldir;
		int WhichDir = 0;
		int i;

		if ( First != 0 )
		{
			for ( i = 0 ; i<8 ; i++ )
			{
				ldir = i % 8;
				auto endx = sx + this->Dir[ldir*2];
				auto endy = sy + this->Dir[ldir*2+1];
				auto dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}
		}
		else
		{
			for ( i = dirstart+7 ; i <= dirstart+9 ; i++ )
			{
				ldir = i % 8;
				auto endx = sx + this->Dir[ldir*2];
				auto endy = sy + this->Dir[ldir*2+1];
				auto dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}

			if ( MinDist == 10000000 )
			{
				for ( i = dirstart+2 ; i <= dirstart+6 ; i++ )
				{
					ldir = i % 8;
					auto endx = sx + this->Dir[ldir*2];
					auto endy = sy + this->Dir[ldir*2+1];
					auto dist = this->GetDist(endx, endy, dx, dy);

					if ( MinDist > dist )
					{
						if ( this->IsThisSpotOK(endx, endy) != FALSE )
						{
							MinDist = dist;
							WhichDir = ldir;
						}
					}
				}
			}
		}
		int LastDist;

		this->LastDir = dirstart;
		LastDist = this->GetDist(sx, sy, sx + this->Dir[this->LastDir*2], sy + this->Dir[this->LastDir*2+1]);

		if ( ( LastDist > MinDist - 6 ) && ( LastDist < MinDist + 6 ) && ( MinDist >= 25 ) )
		{
			WhichDir = this->LastDir;
		}

		if ( MinDist == 10000000 )
		{
			return -1;
		}
		
		if ( this->CanWeMoveForward(sx+this->Dir[((WhichDir*2))], sy+this->Dir[((WhichDir*2))+1]) != FALSE )
		{
			int path = this->VerifyThatOnPath(sx, sy);

			if ( path != -1 )
			{
				this->HitMap[sx + sy * this->Width ] = 1;
				this->NumPath = path;
			}

			return WhichDir;
		}

		if ( ErrorCheck != 0 )
		{
			auto dir = WhichDir+7;

			dir %= 8;

			if (this->CanWeMoveForward(sx + this->Dir[dir*2], sy + this->Dir[dir*2+1]) != FALSE )
			{
				auto path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx  + sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir;
			}

			auto dir2 = WhichDir+9;
			dir2 %= 8;
			if (CanWeMoveForward(sx+this->Dir[dir2*2], sy+this->Dir[dir2*2+1]) != FALSE )
			{
				auto path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx + sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir2;
			}
		}
		
		this->HitMap[sx + sy*this->Width]=1;
		return -1;
	};	// line : 155
		
	int FindNextDir2(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // line : 158
	{
		int MinDist = 10000000;
		long ldir;
		int WhichDir = 0;
		int i;

		if ( First != 0 )
		{
			for ( i = 0 ; i<8 ; i++ )
			{
				ldir = i % 8 ;
				auto endx = sx + this->Dir[ldir*2];
				auto endy = sy + this->Dir[ldir*2+1];
				auto dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}
		}
		else
		{
			for ( i = dirstart+7 ; i <= dirstart+9 ; i++ )
			{
				ldir = i % 8;
				auto endx = sx + this->Dir[ldir*2];
				auto endy = sy + this->Dir[ldir*2+1];
				auto dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}

			if ( MinDist == 10000000 )
			{
				for ( i = dirstart+2 ; i <= dirstart+6 ; i++ )
				{
					ldir = i % 8;
					auto endx = sx + this->Dir[ldir*2];
					auto endy = sy + this->Dir[ldir*2+1];
					auto dist = this->GetDist(endx, endy, dx, dy);

					if ( MinDist > dist )
					{
						if ( this->IsThisSpotOK(endx, endy) != FALSE )
						{
							MinDist = dist;
							WhichDir = ldir;
						}
					}
				}
			}
		}
		int LastDist;

		this->LastDir = dirstart;
		LastDist = this->GetDist(sx, sy, sx + this->Dir[this->LastDir*2], sy + this->Dir[this->LastDir*2+1]);

		if ( ( LastDist > MinDist - 6 ) && ( LastDist < MinDist + 6 ) && ( MinDist >= 25 ) )
		{
			WhichDir = this->LastDir;
		}

		if ( MinDist == 10000000 )
		{
			return -1;
		}
		
		if ( this->CanWeMoveForward2(sx+this->Dir[WhichDir*2],sy+ this->Dir[WhichDir*2+1]) != FALSE )
		{
			int path = this->VerifyThatOnPath(sx, sy);

			if ( path != -1 )
			{
				this->HitMap[sx+ sy * this->Width ] = 1;
				this->NumPath = path;
			}

			return WhichDir;
		}

		if ( ErrorCheck != 0 )
		{
			auto dir = WhichDir+7;

			dir %= 8;

			if (this->CanWeMoveForward2(sx + this->Dir[dir*2], sy + this->Dir[dir*2+1]) != FALSE )
			{
				auto path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx+ sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir;
			}

			auto dir2 = WhichDir+9;
			dir2 %= 8;

			if (CanWeMoveForward2(sx+this->Dir[dir2*2], sy+this->Dir[dir2*2+1]) != FALSE )
			{
				auto path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx + sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir2;
			}
		}
		
		this->HitMap[sx + sy*this->Width]=1;
		return -1;
	};
	
	int FindNextDir3(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // line : 158
	{
		int MinDist = 10000000;
		long ldir;
		int WhichDir = 0;
		int i;

		if ( First != 0 )
		{
			for ( i = 0 ; i<8 ; i++ )
			{
				ldir = i % 8 ;
				auto endx = sx + this->Dir[ldir*2];
				auto endy = sy + this->Dir[ldir*2+1];
				auto dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE && this->CanWeMoveForward3(endx, endy) != FALSE)
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}
		}
		else
		{
			for ( i = dirstart+7 ; i <= dirstart+9 ; i++ )
			{
				ldir = i % 8;
				auto endx = sx + this->Dir[ldir*2];
				auto endy = sy + this->Dir[ldir*2+1];
				auto dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE && this->CanWeMoveForward3(endx, endy) != FALSE)
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}

			if ( MinDist == 10000000 )
			{
				for ( i = dirstart+2 ; i <= dirstart+6 ; i++ )
				{
					ldir = i % 8;
					auto endx = sx + this->Dir[ldir*2];
					auto endy = sy + this->Dir[ldir*2+1];
					auto dist = this->GetDist(endx, endy, dx, dy);

					if ( MinDist > dist )
					{
						if ( this->IsThisSpotOK(endx, endy) != FALSE && this->CanWeMoveForward3(endx, endy) != FALSE)
						{
							MinDist = dist;
							WhichDir = ldir;
						}
					}
				}
			}
		}
		
		if ( MinDist == 10000000 )
		{
			return -1;
		}
		
		if ( this->CanWeMoveForward3(sx+this->Dir[WhichDir*2],sy+ this->Dir[WhichDir*2+1]) != FALSE )
		{
			int path = this->VerifyThatOnPath(sx, sy);

			if ( path != -1 )
			{
				this->HitMap[sx+ sy * this->Width ] = 1;
				this->NumPath = path;
			}

			return WhichDir;
		}

		if ( ErrorCheck != 0 )
		{
			auto dir = WhichDir+7;

			dir %= 8;

			if (this->CanWeMoveForward3(sx + this->Dir[dir*2], sy + this->Dir[dir*2+1]) != FALSE )
			{
				auto path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx+ sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir;
			}

			auto dir2 = WhichDir+9;
			dir2 %= 8;

			if (CanWeMoveForward3(sx+this->Dir[dir2*2], sy+this->Dir[dir2*2+1]) != FALSE )
			{
				auto path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx + sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir2;
			}
		}
		
		this->HitMap[sx + sy*this->Width]=1;
		return -1;
	};

	// line : 245

public:

	PATH() // line : 250
	{
		 int dir[16] = {-1, -1,
						0, -1,
						1, -1,
						1, 0,
						1, 1,
						0, 1,
						-1, 1,
						-1, 0} ;

		memcpy(this->Dir, dir, sizeof(dir));
	};	// line : 252

	~PATH() // line : 254
	{
		return;
	};	// line : 255

	bool FindPath(int startx, int starty, int endx,  int endy, bool ErrorCheck) // line : 257
	{
		this->NumPath = 0;

		int WhichDir = 0;
		int NumFails = 0;
		bool First;

		
		memset(this->HitMap, 0, this->Width * this->Height);
		First = true;

		while ( (startx != endx) || (starty != endy) )
		{
			WhichDir = this->FindNextDir( startx, starty, endx, endy, WhichDir, First, ErrorCheck);
			First = 0;
			
			if ( WhichDir >= 0 )
			{
				this->PathX[this->NumPath] = startx;
				this->PathY[this->NumPath] = starty;
				startx += this->Dir[WhichDir*2];
				starty += this->Dir[WhichDir*2+1];
				this->NumPath++;

				if ( this->NumPath >= 15 )
				{
					return false;
				}
			}
			else if ( ErrorCheck != false )
			{
				this->NumPath--;

				if ( this->NumPath < 0 )
				{
					return false;
				}

				startx = this->PathX[this->NumPath];
				starty = this->PathY[this->NumPath];
				NumFails++;

				if ( NumFails >= 10 )
				{
					return false;
				}
			}
			else
			{
				break;
			}
			
		}

		this->PathX[this->NumPath] = startx;
		this->PathY[this->NumPath] = starty;
		this->NumPath++;

		if ( this->NumPath >= 15 )
		{
			return false;
		}

		return true;
	};	// line : 302
	
	bool FindPath2(int startx, int starty, int endx,  int endy, bool ErrorCheck) // line : 305
	{
		this->NumPath = 0;

		int WhichDir=0;
		int NumFails=0;
		bool First;

		
		memset(this->HitMap, 0, this->Width * this->Height);
		First = true;

		while ( (startx != endx) || (starty != endy) )
		{
			WhichDir = this->FindNextDir2(startx, starty, endx, endy, WhichDir, First, ErrorCheck);
			First = 0;
			
			if ( WhichDir >= 0 )
			{
				this->PathX[this->NumPath] = startx;
				this->PathY[this->NumPath] = starty;
				startx += this->Dir[WhichDir*2];
				starty += this->Dir[WhichDir*2+1];
				this->NumPath++;

				if ( this->NumPath >= 14 )
				{
					this->NumPath--;
					break;
				}
			}
			else if ( ErrorCheck != FALSE )
			{
				this->NumPath--;

				if ( this->NumPath < 0 )
				{
					return false;
				}

				startx = this->PathX[this->NumPath];
				starty = this->PathY[this->NumPath];
				NumFails++;

				if ( NumFails >= 10 )
				{
					return false;
				}
			}
			else
			{
				break;
			}
			
		}

		this->PathX[this->NumPath] = startx;
		this->PathY[this->NumPath] = starty;
		this->NumPath++;

		if ( this->NumPath >= 15 )
		{
			return false;
		}

		return true;
	};	// line : 351


	bool FindPath3(int startx, int starty, int endx,  int endy, bool ErrorCheck)
	{
		this->NumPath = 0;

		int WhichDir=0;
		int NumFails=0;
		bool First;

		
		memset(this->HitMap, 0, this->Width * this->Height);
		First = true;

		while ( (startx != endx) || (starty != endy) )
		{
			WhichDir = this->FindNextDir3(startx, starty, endx, endy, WhichDir, First, ErrorCheck);
			First = 0;
			
			if ( WhichDir >= 0 )
			{
				this->PathX[this->NumPath] = startx;
				this->PathY[this->NumPath] = starty;
				startx += this->Dir[WhichDir*2];
				starty += this->Dir[WhichDir*2+1];
				this->NumPath++;

				if ( this->NumPath >= 14 )
				{
					this->NumPath--;
					break;
				}
			}
			else if ( ErrorCheck != FALSE )
			{
				this->NumPath--;

				if ( this->NumPath < 0 )
				{
					return false;
				}

				startx = this->PathX[this->NumPath];
				starty = this->PathY[this->NumPath];
				NumFails++;

				if ( NumFails >= 10 )
				{
					return false;
				}
			}
			else
			{
				break;
			}
			
		}

		this->PathX[this->NumPath] = startx;
		this->PathY[this->NumPath] = starty;
		this->NumPath++;

		if ( this->NumPath >= 15 )
		{
			return false;
		}

		return true;
	};


	void SetMapDimensions(int w, int h, LPBYTE map) // line : 355
	{
		this->Width = w;
		this->Height = h;
		this->Map = (unsigned char*)map;
		this->HitMap = new unsigned char [this->Width * this->Height];
	};

	int GetPath() // line : 360
	{
		return this->NumPath;
	};

	unsigned char * GetPathX()	// line : 364
	{
		return this->PathX;
	};

	unsigned char * GetPathY()	// line : 368
	{
		return this->PathY;
	};



};


#endif