// DirPath.cpp: implementation of the CDirPath class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dataserver.h"
#include "DirPath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirPath::CDirPath()
{

}

CDirPath::~CDirPath()
{

}

void CDirPath::SetFirstPath(char* dir_path)
{
	strcpy(m_path ,dir_path);
}

char* CDirPath::GetNewPath(char* npath)
{
	strcpy(m_newpath ,m_path);
	strcat(m_newpath,npath);

	return m_newpath;
}