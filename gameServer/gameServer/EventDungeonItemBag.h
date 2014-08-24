// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------
#pragma once

class CEventItemBagAttr
{
public:
  BYTE type;
  BYTE index;
  BYTE minLevel;
  BYTE maxLevel;
  BYTE skill;
  BYTE luck;
  BYTE option;
  BYTE ExOption;
  BYTE SetOption;
  int DropRate;
};

typedef struct _stEventDungeonItemInfo
{
  int bInfoLoad;
  int levelIndex;
  int minLevel;
  int maxLevel;
  int dropZen;
  int dropCount;
  std::vector<CEventItemBagAttr> EventItemBagAttr;

  _stEventDungeonItemInfo()
  {
	  bInfoLoad = 0;
	  levelIndex = 0;
	  minLevel = 0;
	  maxLevel = 0;
	  dropZen = -1;
	  dropCount = 0;
  }
}stEventDungeonItemInfo;


class CEventDungeonItemBag
{
public:
	CEventDungeonItemBag(void);
	virtual ~CEventDungeonItemBag(void);

	static const int MAX_LEVEL_INDEX = 40;

	void LoadScript(const char *szFileName);
	int GetLevelIndex(int nUserLevel, int nUserReset);
	int GetRegItemCount(int nUserLevel, int nUserReset);
	int GetDropZen(int nUserLevel, int nUserReset);
	int GetDropItemCount(int nUserLevel, int nUserReset);
	int GetLevel(int nMinLevel, int nMaxLevel);
	int DropEventItem(int nIndex, int nMaxUserLevel, int nMaxUserReset);
	int CreateNormalItem(int nIndex, CEventItemBagAttr BagObject);
	int CreateSetItem(int nIndex, CEventItemBagAttr BagObject);

private:
	BOOL m_bScriptLoad;
	int m_ExOptionRate;
	stEventDungeonItemInfo m_EventDungeonItemInfo[MAX_LEVEL_INDEX];
};

