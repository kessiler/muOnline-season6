#pragma once
// -------------------------------------------------------------------------------

#ifdef QUESTSYSTEM

#include "user.h"
// -------------------------------------------------------------------------------

#define QUEST_DEBUG			1
#define QUEST_GUI_ENABLED	1
// -------------------------------------------------------------------------------

#define QUEST_MAX_COUNT		40
#define QUEST_MAX_HUNT		5
// -------------------------------------------------------------------------------

#define QUEST_TYPE_HUNT		0
#define QUEST_TYPE_SEARCH	1
#define QUEST_TYPE_TALK		2
// -------------------------------------------------------------------------------

#define QUEST_REW_MONEY		0
#define QUEST_REW_ITEM		1
#define QUEST_REW_EXP		2
#define QUEST_REW_POINT		3
// -------------------------------------------------------------------------------

struct QuestType_Hunt
{
	short	Index;
	BYTE	Count;
	WORD	MonsterClass;
	short	MonsterMap;
};
// -------------------------------------------------------------------------------

struct QuestType_Search
{
	short	Index;
	BYTE	Count;
	WORD	ItemType;
	WORD	ItemIndex;
	BYTE	MinLevel;
	BYTE	MaxLevel;
	BYTE	MinOption;
	BYTE	MaxOption;
	BYTE	IsLuck;
	BYTE	IsSkill;
	bool	IsExcellent;
	bool	IsAncient;
};
// -------------------------------------------------------------------------------

struct QuestType_Talk
{
	short	Index;
	short	NPC;
	short	MapNumber;
	short	X;
	short	Y;
	char	Answer[MAX_CHAT_LEN];
};
// -------------------------------------------------------------------------------

struct QuestReward_Item
{
	BYTE	Count;
	WORD	ItemType;
	WORD	ItemIndex;
	BYTE	Level;
	BYTE	Option;
	BYTE	Luck;
	BYTE	Skill;
	BYTE	Excellent;
	BYTE	Ancient;
};
// -------------------------------------------------------------------------------

struct QuestReward
{
	__int64 Money;
	short Experience;
	std::vector<QuestReward_Item> Item;
	int LevelPoint;
	int WCoinC;
	int WCoinP;
	int WCoinG;
	int Contribute;
};
// -------------------------------------------------------------------------------

struct QuestEpisode
{
	short	Index;
	BYTE	Type;
	int		PreviousEpisode;
	char	Class;
	WORD	MinLevel;
	WORD	MaxLevel;
	WORD	MinReset;
	WORD	MaxReset;
	char	Title[MAX_CHAT_LEN];
	std::vector<QuestType_Hunt>		Hunt;
	std::vector<QuestType_Search>	Search;
	std::vector<QuestType_Talk>		Talk;
	QuestReward Reward;
};
// -------------------------------------------------------------------------------

struct QuestManager
{
	short	Index;
	WORD	ClassID;
	BYTE	MapNumber;
	BYTE	X;
	BYTE	Y;
	char	Folder[MAX_PATH];
	char	File[MAX_PATH];
	bool	Repeat;
	DWORD	TimeOut;
	short	EpisodeCount;
	char	Class;
	WORD	MinLevel;
	WORD	MaxLevel;
	WORD	MinReset;
	WORD	MaxReset;
	char	Title[MAX_CHAT_LEN];
	std::vector<QuestEpisode> Episode;
};
// -------------------------------------------------------------------------------

/*struct QuestUser
{
	short	SceneIndex;
	short	EpisodeIndex;
	BYTE	HuntData[QUEST_MAX_HUNT];
	BYTE	TalkIndex;
	bool	Reward;
};*/
// -------------------------------------------------------------------------------

class QuestSystem
{
public:
			QuestSystem();
			~QuestSystem();
	// ----
	void	Init();
	void	Load();
	// ----
	void	ReadManagerList(LPSTR File);
	void	ReadSceneList(QuestManager * lpScene);
	void	ReadEpisode(QuestEpisode * lpEpisode, LPSTR File);
	// ----
	short	GetSceneIndex(LPOBJ lpNpc);
	short	GetQuestIndex(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	bool	IsManager(LPOBJ lpNpc);
	bool	IsValidScene(LPOBJ lpUser, short SceneIndex);
	bool	IsValidEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	bool	IsFinishedEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	bool	IsSplitItem(WORD ItemID);
	// ----
	void	AddUser(LPOBJ lpUser);
	bool	Dialog(LPOBJ lpUser, LPOBJ lpNpc);
	// ----
	void	AddEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	void	DelEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	void	EndEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	// ----
	void	MakeReward(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	void	MakeNotify(LPOBJ lpUser, short SceneIndex, short EpisodeIndex, char * Text, ...);
	void	MakeNotifyQuest(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	void	MakeNotice(LPOBJ lpUser, char * Sender, char * Message, ...);
	void	MakeNoticeItem(LPOBJ lpUser, QuestEpisode lpEpisode, short ItemIndex);
	void	MakeNoticeItem(LPOBJ lpUser, BYTE ItemPos);
	short	GetItemCount(LPOBJ lpUser, QuestEpisode lpEpisode, short ItemIndex);
	bool	SearchItem(LPOBJ lpUser, short SceneIndex, short EpisodeIndex, short ItemIndex);
	bool	DeleteItem(LPOBJ lpUser, short SceneIndex, short EpisodeIndex, short ItemIndex);
	// ----
	void	RunHunt(LPOBJ lpUser, LPOBJ lpMonster);
	void	RunItem(LPOBJ lpUser, LPOBJ lpNpc, BYTE ItemPos);
	void	RunTalk(LPOBJ lpUser, LPOBJ lpNpc);
	// ----
	void	DB_ReqQuestList(LPOBJ lpUser);
	void	DB_AnsQuestList();
	void	DB_SetQuestList(LPOBJ lpUser);
	void	SendQuestList(LPOBJ lpUser);
	// ----
	void	DB_ReqEpisodeState(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	void	DB_SetEpisodeState(LPOBJ lpUser, short SceneIndex, short EpisodeIndex);
	// ----
private:
	std::vector<QuestManager>	m_Scene;
	// ----
}; extern QuestSystem g_QuestSystem;
// -------------------------------------------------------------------------------

#endif