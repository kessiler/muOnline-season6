// DirPath.h: interface for the CDirPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRPATH_H__C5754B23_D772_44FB_9239_D0F9C806703B__INCLUDED_)
#define AFX_DIRPATH_H__C5754B23_D772_44FB_9239_D0F9C806703B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirPath  
{
public:
	CDirPath();
	virtual ~CDirPath();

	void SetFirstPath(char* dir_path);
	char* GetNewPath(char* npath);

private:

	char m_path[260];
	char m_newpath[260];
};

#endif // !defined(AFX_DIRPATH_H__C5754B23_D772_44FB_9239_D0F9C806703B__INCLUDED_)
