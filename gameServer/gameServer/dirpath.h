#ifndef DIRPATH_H
#define DIRPATH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirPath
{

public:

	CDirPath();
	virtual ~CDirPath();

	void SetFirstPath(char* dir_path);
	LPSTR GetNewPath(const char* npath);

private:

	char m_path[260];	// 4
	char m_newpath[260];	// 108
};

#endif