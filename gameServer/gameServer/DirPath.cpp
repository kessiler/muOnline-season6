//GameServer 1.00.77 JPN - Completed
#include "stdafx.h"
#include "dirpath.h"

CDirPath::CDirPath()
{

}

CDirPath::~CDirPath()
{

}

void CDirPath::SetFirstPath(char* dir_path)
{
	strcpy(this->m_path, dir_path);
}

char* CDirPath::GetNewPath(const char* npath)
{
	strcpy(this->m_newpath ,this->m_path);
	strcat(this->m_newpath,npath);

	return this->m_newpath;
}