//	GS-N	1.00.90		JPN		-	Completed
#include "stdafx.h"
#include "SkillHitBox.h"

CSkillHitBox SkillSpearHitBox;
CSkillHitBox SkillElectricSparkHitBox;

CSkillHitBox::CSkillHitBox()
{
	return;
}


CSkillHitBox::~CSkillHitBox()
{
	return;
}

void CSkillHitBox::Init()
{
	memset(this->m_Table, 0, sizeof(this->m_Table));
}



int CSkillHitBox::Check(int dir, int x, int y)
{
	if ( x <0 || x >= 21 || y < 0 || y >= 21 || dir < 0 || dir >= 36 )
	{
		return 0;
	}

	return this->m_Table[dir][y * 21 + x];
}




int CSkillHitBox::HitCheck(int dir, int x, int y, int tx, int ty)
{
	dir /= 10;

	if ( dir < 0 || dir >= 36 )
	{
		return 0;
	}

	int targetx = tx - x;
	int targety = ty - y;

	targetx += 10;
	targety += 10;

	if ( targetx <0 || abs(targetx) >= 21 || targety <0 || abs(targety) >= 21 )
	{
		return 0;
	}

	return this->m_Table[dir][targety * 21 + targetx];
}

BOOL CSkillHitBox::Load(char * filename )
{

	OFSTRUCT ofst;
	HFILE hfile;
	int filesize;
	unsigned long readfilesize;

	this->Init();
	
	hfile = OpenFile( filename, &ofst, OF_READ|OF_SHARE_COMPAT);

	if ( hfile == NULL )
	{
		return false;
	}

	filesize = GetFileSize((HANDLE) hfile, NULL );

	if ( filesize != sizeof(this->m_Table) )
	{
		_lclose(hfile);
		return false;
	}

	if (ReadFile( (HANDLE) hfile, this->m_Table, filesize, &readfilesize, NULL) == 0 )
	{
		_lclose(hfile);
		return false;
	}

	if ( readfilesize != filesize )
	{
		_lclose( hfile);
		return false;
	}

	_lclose( hfile);

	return true;
}