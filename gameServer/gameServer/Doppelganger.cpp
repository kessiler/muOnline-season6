// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "Doppelganger.h"
#include "logproc.h"
#include "BuffEffectSlot.h"
#include "GameMain.h"
#include "..\common\winutil.h"
//#include "..\Lua\include\lua.h"
#include "Luna.h"
#include "LuaFun.h"
#include "gObjMonster.h"
#include "TNotice.h"
#include "DoppelgangerLua.h"
#include "DSProtocol.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "IllusionTempleEvent.h"
#include "user.h"
#ifdef __CUSTOMS__
#include "ShopPointEx.h"
#endif

CDoppelganger g_DoppleganerEvent;

bool g_bDoppelGangerEvent;
int g_nMarksOfDimensionDropRate;
float g_fDPMasterLevelExpRate;
int g_RegDoppelgangerClear;

CDoppelgangerMonsterHerd::CDoppelgangerMonsterHerd()
{
    this->pos_index_ = 0;
    this->ukn50 = 0;
    this->end_pos_index_ = 0;
    this->should_move_ = true;
}

void CDoppelgangerMonsterHerd::MonsterAttackAction(OBJECTSTRUCT *lpObj, 
                                                    OBJECTSTRUCT *lpTargetObj)
{
    if( gObjCheckUsedBuffEffect(lpObj, 57) )
        return;

    if( !lpObj || !lpTargetObj )
        return;

    if( lpObj->Connected < 3 || lpTargetObj->Connected < 3)
        return;

    if( lpObj->Type == OBJ_MONSTER )
    {
        lpObj->NextActionTime = 1000;
    }
}

int CDoppelgangerMonsterHerd::AddMonsterEX(int monster_type, 
                                            int should_attack_first, 
                                            int monster_level, 
                                            int monster_hp, 
                                            int attack_min, 
                                            int attack_max, 
                                            int monster_def)
{
    if( !this->m_bHasInfo )
    {
        return -1;
    }

    int map_number = this->m_iMapNumber;
    BYTE pos_x = 0;
    BYTE pos_y = 0;

    if ( !GetRandomLocation(pos_x, pos_y) )
    {
        return -1;
    }


    int monster_index = gObjAddMonster(this->m_iMapNumber);
    if ( monster_index < 0 )
    {
        LogAddTD("[Doppelganger] AddMonsterEx Fail(%d) [Class: %d][%d][%d, %d]",
                    monster_index, monster_type,
                    m_iMapNumber, pos_x, pos_y);

        return -1;
    }


    gObj[monster_index].m_PosNum = -1;
    gObj[monster_index].X = pos_x;
    gObj[monster_index].Y = pos_y;

    gObj[monster_index].MapNumber = this->m_iMapNumber;
    gObj[monster_index].TX = gObj[monster_index].X;
    gObj[monster_index].TY = gObj[monster_index].Y;
    gObj[monster_index].m_OldX = gObj[monster_index].X;
    gObj[monster_index].m_OldY = gObj[monster_index].Y;
    gObj[monster_index].StartX = gObj[monster_index].X;
    gObj[monster_index].StartY = gObj[monster_index].Y;

    MONSTER_ATTRIBUTE* monster_attribute = gMAttr.GetAttr(monster_type);

    if( !monster_attribute )
    {
        gObjDel(monster_index);
        return 0;
    }


    gObj[monster_index].Level = monster_attribute->m_Level;
    gObjSetMonster(monster_index, monster_type);
    gObj[monster_index].Level = monster_level;
    gObj[monster_index].Life = monster_hp;
    gObj[monster_index].MaxLife = monster_hp;
	gObj[monster_index].m_Defense = monster_def;
    gObj[monster_index].m_AttackDamageMin = attack_min;
    gObj[monster_index].m_AttackDamageMax = attack_max;
    gObj[monster_index].m_SuccessfulBlocking = monster_def;
    gObj[monster_index].MaxRegenTime = 1000;
    gObj[monster_index].Dir = rand() % 8;
    gObj[monster_index].m_bIsInMonsterHerd = 1;
    gObj[monster_index].m_bIsMonsterAttackFirst = should_attack_first;
    gObj[monster_index].m_lpMonsterHerd = this;

    EnterCriticalSection(&this->m_critMonsterHerd);

    _MONSTER_HERD_DATA monster_herd_data;
    monster_herd_data.m_iIndex = monster_index;
    monster_herd_data.m_iType = monster_type;
    monster_herd_data.m_iX = pos_x;
    monster_herd_data.m_iY = pos_y;
    monster_herd_data.m_bRegen = 0;
    m_mapMonsterHerd.insert(std::pair<int, _MONSTER_HERD_DATA>(monster_index, monster_herd_data));

    LeaveCriticalSection(&this->m_critMonsterHerd);

    return monster_index;
}

int CDoppelgangerMonsterHerd::GetRandomLocation(BYTE& pos_x, BYTE& pos_y)
{
    if ( this->m_bHasInfo )
    {
        int count = 100;
        while ( count-- )
        {
            pos_x = this->m_iCUR_X + ((rand() % 2 != 0 ? 2 : 0) - 1) 
                                    * (rand() % (m_iRADIUS + 1));

            pos_y = this->m_iCUR_Y + ((rand() % 2 != 0 ? 2 : 0) - 1) 
                                    * (rand() % (m_iRADIUS + 1));

            int map_attribute = MapC[m_iMapNumber].GetAttr(pos_x, pos_y);

            if( map_attribute == 0 || map_attribute == 2 )
                return 1;
        }
    }


    return 0;
}

int CDoppelgangerMonsterHerd::SetTotalInfo(int map_number, int radius, 
                                            int pos_info, int should_move)
{

    if( !MAX_MAP_RANGE(map_number) ) 
    {
        return 0;
    }

    if( radius < 0 || radius > 15)
    {
        return 0;
    }


    BYTE pos_x = 0;
    BYTE pos_y = 0;
    g_DoppleganerEvent.GetCenterPosition(map_number, pos_info, pos_x, pos_y);

    this->m_iMapNumber = map_number;
    this->m_iCUR_X = pos_x;
    this->m_iCUR_Y = pos_y;
    this->m_iRADIUS = radius;
    this->ukn50 = pos_info;
    this->pos_index_ = pos_info;
    this->should_move_ = should_move;
    this->m_bHasInfo = 1;

    return 1;
}

void CDoppelgangerMonsterHerd::MonsterBaseAct(OBJECTSTRUCT *lpObj)
{
    OBJECTSTRUCT* lpTargetObj = nullptr;

    if( lpObj->TargetNumber < 0 )
    {
        lpObj->m_ActState.Emotion = 0;
    }
    else
    {
        lpTargetObj = &gObj[lpObj->TargetNumber];
    }

    if(lpObj->m_ActState.Emotion == 0)
    {
        if( lpObj->m_ActState.Attack )
        {
            lpObj->m_ActState.Attack = 0;
            lpObj->TargetNumber = -1;
            lpObj->NextActionTime = 100;
        }

        if( lpObj->m_MoveRange > 0 )
        {
            if( !gObjCheckUsedBuffEffect(lpObj, 57) )
                this->MonsterMoveAction(lpObj);
        }

        if( lpObj->Class == 529 || lpObj->Class == 530 )
            g_DoppleganerEvent.AngerKillerAttack(lpObj);

        if( lpObj->m_bIsMonsterAttackFirst )
        {
            lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, 1);
            if( lpObj->TargetNumber >= 0 )
            {
                if( lpObj->Class == 533 )
                {
                    lpObj->m_ActState.EmotionCount = 30;
                    lpObj->NextActionTime = 100;
                    lpObj->m_ActState.Move = 0;
                    lpObj->PathStartEnd = 0;
                }
                else
                {
                    lpObj->m_ActState.EmotionCount = 10;
                }

                lpObj->m_ActState.Emotion = 1;
            }
        }
    }
    else if( lpObj->m_ActState.Emotion == 1 )
    {
        if( lpObj->Class == 529 || lpObj->Class == 530 )
        {
            lpObj->m_ActState.Emotion = 0;
            lpObj->m_ActState.EmotionCount = 0;
        }

        if( lpObj->m_ActState.EmotionCount )
        {
            lpObj->m_ActState.EmotionCount--;
        }
        else
        {
            lpObj->m_ActState.Emotion= 0;
        }

        if ( lpObj->TargetNumber >= 0 )
        {
            if( !lpObj->PathStartEnd )
            {
                int dis = gObjCalDistance(lpObj, lpTargetObj);
                int attack_range = 0;

                if( lpObj->m_AttackType < 100 )
                    attack_range = lpObj->m_AttackRange;
                else
                    attack_range = lpObj->m_AttackRange + 2;

                if( dis > attack_range )
                {
                    if( gObjMonsterGetTargetPos(lpObj) )
                    {
                        if( MapC[lpObj->MapNumber].CheckWall(
                                                        lpObj->X, lpObj->Y, 
                                                        lpObj->MTX, lpObj->MTY))
                        {
                            lpObj->m_ActState.Move = 1;
                            lpObj->NextActionTime = 100;
                            lpObj->Dir = GetPathPacketDirPos(
                                                    lpTargetObj->X - lpObj->X, 
                                                    lpTargetObj->Y - lpObj->Y);
                        }
                        else
                        {
                            this->MonsterMoveAction(lpObj);

                            lpObj->m_ActState.Emotion = 0;
                            lpObj->m_ActState.EmotionCount = 10;
                        }
                    }
                    else
                    {
                        this->MonsterMoveAction(lpObj);
                        lpObj->m_ActState.Emotion = 0;
                        lpObj->m_ActState.EmotionCount = 10;
                    }
                }
                else
                {
                    int target_user_index = lpObj->TargetNumber;
                    int map = gObj[target_user_index].MapNumber;
                    if( MapC[map].CheckWall(lpObj->X, lpObj->Y, 
                        gObj[target_user_index].X, gObj[target_user_index].Y) )
                    {
                        if( (MapC[map].GetAttr(gObj[target_user_index].X, 
                                        gObj[target_user_index].Y) & 1) == 1 )
                        {
                            lpObj->TargetNumber = -1;
                            lpObj->m_ActState.Emotion = 1;
                        }
                        else
                        {
                            lpObj->m_ActState.Attack = 1;
                        }

                        lpObj->Dir = GetPathPacketDirPos(
                                                lpTargetObj->X - lpObj->X, 
                                                lpTargetObj->Y - lpObj->Y);

                        lpObj->NextActionTime = 100;
                    }
                }
            }
        }
    }
    else if( lpObj->m_ActState.Emotion == 3 )
    {
        if( lpObj->m_ActState.EmotionCount )
        {
            lpObj->m_ActState.EmotionCount--;
        }
        else
        {
            lpObj->m_ActState.Emotion = 0;
        }

        lpObj->m_ActState.Move = 0;
        lpObj->m_ActState.Attack = 0;
        lpObj->NextActionTime = 400;
    }

}


int CDoppelgangerMonsterHerd::MoveHerd()
{
    if ( this->m_bHasInfo )
    {
        int map_type = this->m_iMapNumber - 65;
        int next_pos_info = this->pos_index_ + 1;

        if ( next_pos_info > 22 )
            next_pos_info = 22;

        if ( !this->should_move_ )
            next_pos_info = this->pos_index_;

        BYTE pos_x = 0;
        BYTE pos_y = 0;
        g_DoppleganerEvent.GetCenterPosition(this->m_iMapNumber,
                                        next_pos_info, pos_x, pos_y);

        BYTE map_attribute = MapC[m_iMapNumber].GetAttr(pos_x, pos_y);
        if( !(map_attribute & 1) && !(map_attribute & 4) 
                    && !(map_attribute & 8) )
        {
            this->m_iCUR_X = pos_x;
            this->m_iCUR_Y = pos_y;

            return 1;
        }
		else
		{
			LogAddTD("%s() Failed, pos_index:%d, nex_pos_index:%d", __FUNCTION__,
											pos_index_, next_pos_info);
		}

    }

    return 0;
}

void CDoppelgangerMonsterHerd::ArrangeMonsterHerd()
{
    if ( IsActive() )
    {
        auto it = m_mapMonsterHerd.begin();
        auto end = m_mapMonsterHerd.end();

        for( ; it != end; ++it)
        {          
            if ( gObj[it->second.m_iIndex].Live == 1 )
                break;
        }
        
        if( it == end )
            SetActive(0);
    }
}

int CDoppelgangerMonsterHerd::IsActive()
{
    return this->m_bHerdActive;
}

void CDoppelgangerMonsterHerd::SetActive(bool is_active)
{
    this->m_bHerdActive = is_active;
}

CDoppelganger::CDoppelganger(void)
{
	InitializeCriticalSection(&user_crit_section_);
	InitializeCriticalSection(&treasure_box_crit_section_);
	InitializeCriticalSection(&m_cs3);

}


CDoppelganger::~CDoppelganger(void)
{
	DeleteCriticalSection(&user_crit_section_);
	DeleteCriticalSection(&treasure_box_crit_section_);
	DeleteCriticalSection(&m_cs3);
}


void CDoppelganger::DoppelgangerProcessInit()
{
//    TRACE_LOG("Doppelganger Init()");

    lua_State* lua_state = this->mu_lua_.GetLua();
    if ( lua_state )
    {
        Luna<CDoppelgangerLua>::Register(lua_state);
        
        //this->mu_lua_.DoFile(gDirPath.GetNewPath("Lua_Doppelganger.lua"));
        this->mu_lua_.DoFile("../Data/Lua/Doppelganger.lua");
        g_Generic_Call(this->mu_lua_.GetLua(), "FN_LuaDopplegangerInit", ">");

        this->pos_info_.LoadDoppelgangerPosInfo();
        this->event_info_.LoadDopplegangereventinfo();
        this->event_item_bag_.LoadDoppelgangerItemBag();

        for( int i = 0; i < 11; ++i )
            this->monster_attribute_[i] = GetDefaultMonsterInfo(i + 529);

        SetDoppelgangerState(0);
    }
    else
    {
        LogAddTD("[%s()] - Error - [%s] [%d]", 
                    __FUNCTION__, __FILE__, __LINE__);
    }
}


void CDoppelganger::DoppelgangerInfoLoad()
{

    g_bDoppelGangerEvent = GetPrivateProfileIntA("GameServerInfo", 
                                    "bDoppelgangerEvent", 
                                    0, 
                                    ("../Data/commonserver.cfg"));

    g_nMarksOfDimensionDropRate = GetPrivateProfileIntA("GameServerInfo", 
                                      "MarksOfDimensionDropRate", 0, 
                                      ("../Data/commonserver.cfg"));

    char float_string[6] = {0};
    GetPrivateProfileStringA("GameServerInfo", "DPMasterLevelExpRate", "0.5", 
                                float_string, 
                                5, 
                                ("../Data/commonserver.cfg"));

    g_fDPMasterLevelExpRate = atof(float_string);

    g_RegDoppelgangerClear = GetPrivateProfileIntA("GameServerInfo", 
                                "RegDoppelgangerClear", 
                                0, 
                               ("../Data/commonserver.cfg"));
}

void CDoppelganger::SetDoppelgangerState(BYTE state)
{

    switch( state )
    {
    case 0:
        SetDoppelgangerStateNone();
        break;
    case 1:
        SetDoppelgangerStateReady();
        break;
    case 2:
        SetDoppelgangerStatePlaying();
        break;
    case 3:
        SetDoppelgangerStateEnd();
        break;
    default:
        break;
    }

    state_start_tick_	= GetTickCount();
    current_state_		= state;
}

BYTE CDoppelganger::GetDoppelgangerState()
{
    return this->current_state_;
}

void CDoppelganger::SetDoppelgangerStateNone()
{

    LogAddTD("[Doppelganger] State NONE");

    this->ukn2c4 = 0;
    this->user_max_level_ = 0;
    this->user_min_level_ = 0;

    for(int i = 0; i < kMaxMiddleTreasureBox; ++i)
    {
        if( this->middle_treasure_box_index_[i] != -1 )
        {
            gObjDel(this->middle_treasure_box_index_[i]);
            this->middle_treasure_box_index_[i] = -1;
        }
    }

    if( this->last_treasure_box_index_ != -1 )
    {
        gObjDel(this->last_treasure_box_index_);
        this->last_treasure_box_index_ = -1;
    }

    if( GetUserCount() > 0 )
    {
        for(int j = 0; j < MAX_PARTYUSER; ++j )
        {
            if( this->user_info_[j].IsUser() )
            {
                if( gObj[user_info_[j].object_index].Connected > PLAYER_LOGGED )
                    gObjMoveGate(this->user_info_[j].object_index, 267);
            }
        }
    }

    ClearUserData();
    this->start_user_count_ = 0;
}

void CDoppelganger::SetDoppelgangerStateReady()
{
    LogAddTD("[Doppelganger] State READY");

    this->ukn3fc = -1;
    this->monster_pos_ = 0;
    this->monster_goal_count_ = 0;
    this->ukn314 = 0;
    this->mission_result_ = 1;
	this->ice_walker_dead = 0;//addition
    this->ukn2c8 = 0;
    this->ukn2cc = 0;
    this->ukn2d0 = 0;
    this->ukn2d4 = 0;

    for(int i = 0; i < kMaxIceWorker; ++i )
        this->ice_worker_index_[i] = -1;

    for(int j = 0; j < kMaxLarva; ++j )
        this->larva_index_[j] = -1;

    for(int k = 0; k < kMaxMiddleBoss; ++k )
    {
        this->middle_boss_index_[k] = -1;
        this->middle_boss_killed_[k] = 0;
    }

    this->killer_state_ = 1;

    this->is_middle_treasure_box_spawned_ = 0;
    SetKillerState(0);
    SetAngerKillerState(0);
    this->boos_regen_order_ = 0;

    EnterCriticalSection(&this->treasure_box_crit_section_);
    this->is_treasure_box_opened_ = 1;
    this->is_last_treasure_box_opened_ = 1;
    LeaveCriticalSection(&this->treasure_box_crit_section_);

    SendDoppelgangerState(1);
//    TRACE_LOG("Set Doppelganger State Ready");
}

void CDoppelganger::SetDoppelgangerStatePlaying()
{
    LogAddTD("[Doppelganger] State PLAY");

    DWORD current_time = GetTickCount();

    this->ukn344 = current_time;
    this->add_herd_monster_time_ = current_time;
    this->add_boss_monster_time_ = current_time;
    this->ukn350 = current_time;
    this->start_user_count_ = this->user_count_;
    SetUserAverageLevel();

    if( event_info_.SetDoppelgangerInfo(GetUserCount(), GetUserMaxLevel()) )
    {
        char player_log[512] = "";

        wsprintf(player_log, "[Doppelganger] PlayUser [%d][%d] ", 
                            this->map_number_, this->start_user_count_);

        EnterCriticalSection(&this->user_crit_section_);

        for(int i = 0; i < MAX_PARTYUSER; ++i )
        {
            if( this->user_info_[i].IsUser() )
            {
                if( gObj[this->user_info_[i].object_index].Connected > 2 )
                {
                    wsprintf(player_log + strlen(player_log),
                                "(%s)(%s) ",
                                gObj[user_info_[i].object_index].AccountID,
                                gObj[user_info_[i].object_index].Name);
                }
            }
        }

        LeaveCriticalSection(&this->user_crit_section_);

        LogAddTD(player_log);
        SendDoppelgangerState(2);
        SendMapTileInfoAll(1);
        SendMonsterGoalCount();

//        TRACE_LOG("Set Doppelganger State Play");

    }
    else
    {
        LogAddTD("[Doppelganger] SetDoppelgangerInfo is NULL return");
        SetDoppelgangerState(3);
    }
}


void CDoppelganger::SetDoppelgangerStateEnd()
{
    //LogAddTD("[Doppelganger] State END");

    for (int i = 0; i < kMaxMonsterHard; ++i )
    {
        this->monster_herd_[i].Stop();
        this->monster_herd_[i].SetPosIndex(0);
    }

    this->ukn4c24 = 0;
    DelLarvaAll();
    SendDoppelgangerState(3);
    SendDoppelgangerResultAll();

    if ( this->user_count_ > 0 )
    {
        if ( this->monster_goal_count_ < 3 )
        {
            this->ukn314 = 1;

            int j;
            for ( j = 0;
                j < MAX_PARTYUSER
                && ( !this->user_info_[j].IsUser()
                || gObj[this->user_info_[j].object_index].Connected != 3);
            ++j )
                ;

            if ( j < MAX_PARTYUSER )
            {
                AddLastTreasureBox(gObj[this->user_info_[j].object_index].X, 
                    gObj[this->user_info_[j].object_index].Y);
            }
        }
    }
}

void CDoppelgangerMonsterHerd::SetPosIndex(int pos_index)
{
    this->pos_index_ = pos_index;
}


void CDoppelganger::ProcDoppelganger(int current_time)
{
    if( g_bDoppelGangerEvent )
    {
        switch( this->current_state_ )
        {
        case 0:
            ProcDoppelgangerState_None(current_time);
            break;
        case 1:
            ProcDoppelgangerState_Ready(current_time);
            break;
        case 2:
            ProcDoppelgangerState_Playing(current_time);
            break;
        case 3:
            ProcDoppelgangerState_End(current_time);
            break;
        default:
            return;
        }
    }
}


void CDoppelganger::ProcDoppelgangerState_None(int current_time)
{
    if( !this->ukn2c4 )
    {
        if ( this->state_start_tick_ > 30000 )
            this->ukn2c4 = 1;
    }

    if( GetUserCount() > 0 )
    {
        if ( GetTickCount() - this->ukn330 > 30000 )
            SetDoppelgangerState(1);
    }
}


void CDoppelganger::ProcDoppelgangerState_Ready(int current_time)
{

    if( (60000 * this->ready_time_ - (GetTickCount() - this->state_start_tick_)) <= 30000 )
    {
        if( !this->ukn2cc )
        {
            this->ukn2cc = 1;
            SendDoppelgangerTimerMsg(17);
        }
    }

    if( current_time - this->state_start_tick_ > (60000 * this->ready_time_) )
        SetDoppelgangerState(2);
}

void CDoppelganger::ProcDoppelgangerState_Playing(int current_time)
{
    lua_State *lua_state;


    if( this->user_count_ == 0)
    {
        SetDoppelgangerState(3);
        return;
    }

    if( (current_time - this->ukn350) >= 1000 )
    {
        SendDoppelgangerMonserPos();
        SendDoppelgangerUserPos();
        this->ukn350 = current_time;
    }

    if( current_time - this->ukn344 >= 1000 )
    {
        ArrangeMonsterHerd();
        lua_state = this->mu_lua_.GetLua();
        if ( !lua_state )
        {
            LogAddTD("[%s()] - Error - [%s] [%d]", 
                        __FUNCTION__, __FILE__, __LINE__ );
            return;
        }

        g_Generic_Call(lua_state, "FN_LuaDoppelgangerCallback", "i>", 
                        current_time);

        this->ukn344 = current_time;
    }

    for(int i = 0; i < kMaxMonsterHard; ++i )
    {
        if( this->monster_herd_[i].IsActive() )
            this->monster_herd_[i].MoveHerd();
    }

    if( this->is_middle_treasure_box_spawned_ == 1 )
    {
        if( GetTickCount() - this->middle_treasure_box_spawn_time_ >= 60000 )
        {
            OpenTreasureBox();
            for(int j = 0; j < kMaxMiddleTreasureBox; ++j )
            {
                if ( this->middle_treasure_box_index_[j] != -1 )
                {
                    gObjDel(this->middle_treasure_box_index_[j]);
                    this->middle_treasure_box_index_[j] = -1;
                }
            }
            this->is_middle_treasure_box_spawned_ = 0;
        }
    }

    if( !this->ukn2d0 )
    {
        if(this->ice_walker_dead = 0)//if( this->killer_state_ == 0) original
        {
            if ( GetTickCount() - this->ice_walker_spawn_time >= 30000 )//this->ukn36c >= 30000 original
            {
                this->ukn2d0 = 1;
                SendDoppelgangerTimerMsg(18);
            }
        }
    }

    if ( current_time - this->state_start_tick_ > (60000 * this->play_time_) )
        SetDoppelgangerState(3);

}


void CDoppelganger::ProcDoppelgangerState_End(int current_time)
{

    if( current_time - this->state_start_tick_ > (10000 * this->end_time_) )
    {
        if( GetUserCount() > 0 )
        {
            if( !this->ukn314 )
            {
                for( int i = 0; i < MAX_PARTYUSER; ++i )
                {
                    if( this->user_info_[i].IsUser() )
                    {
                        if( gObj[user_info_[i].object_index].Connected <= 2 )
                            DelDoppelgangerUser(user_info_[i].object_index);
                        else
                            gObjMoveGate(this->user_info_[i].object_index, 267);
                    }
                }
            }
        }
    }

    if ( !this->ukn2d4 )
    {
        if ( current_time - this->state_start_tick_ 
                > (unsigned int)(30000 * this->end_time_) )
        {
            this->ukn2d4 = 1;
            SendDoppelgangerTimerMsg(19);
        }
    }

    if ( current_time - this->state_start_tick_ > (unsigned int)(60000 * this->end_time_))
        SetDoppelgangerState(0);

    if( this->party_number_ > -1 )
    {
        if( this->party_number_ < 10000 )
        {
            if( gParty.GetCount(this->party_number_) <= 1
               || gObj[gParty.m_PartyS[party_number_].Number[0]].Connected != 3)
            {
                for(int j = 0; j < MAX_PARTYUSER; ++j )
                {
                    int user_index = gParty.m_PartyS[party_number_].Number[j];
                    if ( user_index >= 0 )
                    {
                        gParty.Delete(this->party_number_, j);
                        gObj[user_index].PartyNumber = -1;
                        gObj[user_index].PartyTargetUser = -1;
                        GCPartyDelUserSend(user_index);
                    }
                }

                gParty.Destroy(this->party_number_);
            }
            else
            {
                for(int k = 0; k < MAX_PARTYUSER; ++k )
                {
                    int user_index = gParty.m_PartyS[party_number_].Number[k];
                    if ( user_index >= 0 )
                    {
                        if ( gObj[user_index].Connected < 3 )
                        {
                            gParty.Delete(this->party_number_, k);
                            gObj[user_index].PartyNumber = -1;
                            gObj[user_index].PartyTargetUser = -1;
                        }
                    }
                }
            }

            CGPartyListAll(this->party_number_);
            this->party_number_ = -1;
        }
    }
}


int CDoppelganger::PickupMarkOfDimensionItem(OBJECTSTRUCT *lpObj, 
                                                CMapItem *lpItem, int item_num)
{
    PMSG_ITEMGETRESULT result_msg;

    PHeadSetB((LPBYTE)&result_msg, 34, sizeof(result_msg));

    result_msg.result = -1;

    for(int pos = 0; pos < INVENTORY_SIZE; ++pos )
    {
        if( (lpObj->pInventory[pos].IsItem()) == 1
            && lpObj->pInventory[pos].m_Type == 0x1C6E
            && lpObj->pInventory[pos].m_Level == lpItem->m_Level )
        {
            int map_number = lpObj->MapNumber;

            int item_durability = lpObj->pInventory[pos].m_Durability;
            if ( !(item_durability >= 0 && item_durability <= 4) )
                return 0;

            if ( MapC[map_number].ItemGive(lpObj->m_Index, item_num, 1) != 1 )
                return 0;



            BYTE NewOption[MAX_EXOPTION_SIZE];
            ItemIsBufExOption(NewOption, lpItem);

            LogAddTD(lMsg.Get(477), lpObj->AccountID, lpObj->Name,
                    map_number, lpObj->X, lpObj->Y,
                    lpItem->m_Number, lpItem->GetName(), lpItem->m_Type,
                    lpItem->m_Level, lpItem->m_Option1, lpItem->m_Option2, 
                    lpItem->m_Option3,
                    lpItem->m_Durability, 
                    NewOption[0], NewOption[1], NewOption[2],
                    NewOption[3], NewOption[4], NewOption[5], NewOption[6],
                    lpItem->m_SetOption, lpItem->m_ItemOptionEx >> 7,
                    g_kJewelOfHarmonySystem.GetItemStrengthenOption(lpItem),
                    g_kJewelOfHarmonySystem.GetItemOptionLevel(lpItem));

            result_msg.result = -3;
            DataSend(lpObj->m_Index, (LPBYTE)&result_msg, result_msg.h.size);
            lpObj->pInventory[pos].m_Durability = 
                    lpObj->pInventory[pos].m_Durability + lpItem->m_Durability;

            if ( lpObj->pInventory[pos].m_Durability < 5.0 )
            {
                GCItemDurSend(lpObj->m_Index, pos, 
                                lpObj->pInventory[pos].m_Durability, 0);
                return 1;
            }

            int durability_gap = (lpObj->pInventory[pos].m_Durability - 5.0);

            gObjInventoryItemSet(lpObj->m_Index, pos, -1);
            lpObj->pInventory[pos].Clear();

            GCInventoryItemDeleteSend(lpObj->m_Index, pos, 1);

            ItemSerialCreateSend(lpObj->m_Index, 235, lpObj->X, lpObj->Y,
                                0x1C6Fu, lpItem->m_Level,
                                0, 0, 0, 0, lpObj->m_Index, 0, 0);

            LogAddTD("[%s][%s] Make BOUNDARY OF_DIMENSION "
                        "(Left Dimension Mark:%d)",
                        lpObj->AccountID, lpObj->Name, durability_gap);

            if( durability_gap <= 0 )
                return 1;

            lpItem->m_State = 2;
            lpItem->Give = 0;
            lpItem->live = 1;
            lpItem->m_Durability = durability_gap;
        }
    }

    return 0;
}

bool CDoppelganger::EnterDoppelgangerEvent(int user_index, BYTE item_pos)
{
    PMSG_DOPPELGANGER_ENTER_ANS result_msg;

    OBJECTSTRUCT* lpObj = &gObj[user_index];
    PHeadSubSetB((LPBYTE)&result_msg, 0xBFu, 0xEu, 5);
    result_msg.result = 0;
	
    if( lpObj->Type != OBJ_USER || lpObj->Connected <= 2 )
	{
        return false;
	}
    if( lpObj->m_IfState.use && (lpObj->m_IfState.type != 12) )
	{
		return false;
	}
    if( /*gFreeServer == 1 &&*/ lpObj->Level < 12 )
    {
        result_msg.result = 5;
        DataSend(user_index, (LPBYTE)&result_msg, result_msg.h.size);
        return false;
    }

    if( lpObj->m_bPShopOpen == 1 )
	{
        CGPShopReqClose(lpObj->m_Index);
	}

    if( GetDoppelgangerState() != 0 || !this->ukn2c4 )
    {
        result_msg.result = 2;
        DataSend(user_index, (LPBYTE)&result_msg, result_msg.h.size);
        return false;
    }

    int pk_flag = 0;
    if( lpObj->PartyNumber < 0 )
    {
        if ( lpObj->m_PK_Level >= 4 )
		{
            pk_flag = 1;
		}
    }
    else
    {
        if( gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= 5 )
            pk_flag = 1;
    }

    if( pk_flag == 1 )
    {
        result_msg.result = 3;
        DataSend(user_index, (LPBYTE)&result_msg, result_msg.h.size);
        return false;
    }

    int event_ticket_pos = item_pos + 12;
    CItem * event_ticket_item = &lpObj->pInventory[event_ticket_pos];

    if( !event_ticket_item->IsItem() )
    {
        result_msg.result = 1;
        DataSend(user_index, (LPBYTE)&result_msg, result_msg.h.size);
        return false;
    }

    if( event_ticket_item->m_Type != ITEMGET(14, 111) && event_ticket_item->m_Type != ITEMGET(13, 125) )
    {
        result_msg.result = 1;
        DataSend(user_index, (LPBYTE)&result_msg, result_msg.h.size);
        return false;
    }

    if( event_ticket_item->m_Type == 0x1A7D && event_ticket_item->m_Durability < 1.0 )
    {
        result_msg.result = 1;
        DataSend(user_index, (LPBYTE)&result_msg, result_msg.h.size);
        return false;
    }


    if( !AddDoppelgangerUser(user_index) )
    {
        result_msg.result = 2;
        DataSend(user_index, (LPBYTE)&result_msg, result_msg.h.size);
        return false;
    }

    if( event_ticket_item->m_Type == 0x1C6F )
    {
        gObjInventoryDeleteItem(user_index, event_ticket_pos);
        GCInventoryItemDeleteSend(user_index, event_ticket_pos, 0);
    }
    else if( event_ticket_item->m_Type ==  0x1A7D )
    {
        event_ticket_item->m_Durability = 
                            event_ticket_item->m_Durability - 1.0;

        if ( event_ticket_item->m_Durability > 0.0 )
        {
            GCItemDurSend2(user_index, 
                            event_ticket_pos, 
                            event_ticket_item->m_Durability, 
                            0 /*flag*/);
        }
        else
        {
            gObjInventoryDeleteItem(user_index, event_ticket_pos);
            GCInventoryItemDeleteSend(user_index, 
                                        event_ticket_pos, 
                                        1 /*flag*/);
        }
    }

    DataSend(user_index, (LPBYTE)&result_msg, result_msg.h.size);
    gObjMoveGate(lpObj->m_Index, this->ukn320);
    SendMapTileInfo(lpObj, 4);

    LogAddTD("[Doppelganger] Entered Player [%s][%s]", 
                lpObj->AccountID, lpObj->Name);

    return true;
}

bool CDoppelganger::LeaveDoppelgangerEvent(int aIndex)
{
    DelDoppelgangerUser(aIndex);
    LogAddTD("[Doppelganger] Leaved Player [%s][%s]", 
                gObj[aIndex].AccountID, gObj[aIndex].Name);
    return true;
}

bool CDoppelganger::AddDoppelgangerUser(int user_index)
{
    EnterCriticalSection(&this->user_crit_section_);

    if( this->user_count_ )
    {
        if( gObj[user_index].PartyNumber == -1 
                || gObj[user_index].PartyNumber != this->party_number_ )
        {
            LeaveCriticalSection(&this->user_crit_section_);
            return false;
        }

        for( int i = 0; i < MAX_PARTYUSER; ++i )
        {
            if( !this->user_info_[i].IsUser() )
            {
                user_info_[i].object_index = user_index;
				user_info_[i].user_level = gObj[user_index].m_nMasterLevel
                                            + gObj[user_index].Level;

                break;
            }

            if( i == MAX_PARTYUSER-1 )
            {
                LeaveCriticalSection(&this->user_crit_section_);
                return false;
            }
        }
    }
    else
    {
        int rand_value = GetRandomValue(4);
		this->map_number_ = rand_value + CDoppelgangerPosInfo::kStartEventMapNumber;
        this->ukn320 = rand_value + 329;
        this->party_number_ = gObj[user_index].PartyNumber;
        this->user_info_[0].object_index = user_index;
        this->user_info_[0].user_level = 
			gObj[user_index].m_nMasterLevel + gObj[user_index].Level;

        this->ukn330 = GetTickCount();

        if( this->party_number_ >= 0 )
        {
            PMSG_SET_DEVILSQUARE pMsg;
            PHeadSetB((LPBYTE)&pMsg, 0x92u, sizeof(pMsg));
            pMsg.Type = 16;

            for( int j = 0; j < MAX_PARTYUSER; ++j )
            {
                int party_user_index = gParty.m_PartyS[party_number_].Number[j];

                if( party_user_index < 0 )
                    continue;

                int map_number = gObj[party_user_index].MapNumber;

                if( BC_MAP_RANGE(map_number) 
                    || CC_MAP_RANGE(map_number)
                    || DS_MAP_RANGE(map_number)
                    || IT_MAP_RANGE(map_number))
                {
                    continue;
                }


                DataSend(party_user_index, (LPBYTE)&pMsg, pMsg.h.size);
            }
        }
    }

    ++this->user_count_;
    CalUserLevel();

    LeaveCriticalSection(&this->user_crit_section_);

    return true;
}

void CDoppelganger::DelDoppelgangerUser(int user_index)
{
    EnterCriticalSection(&this->user_crit_section_);

    for( int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() 
                && this->user_info_[i].object_index == user_index )
        {
            this->user_info_[i].object_index = -1;
            this->user_info_[i].user_level = 0;
            break;
        }

        if ( i == MAX_PARTYUSER-1 )
        {
            LeaveCriticalSection(&this->user_crit_section_);
            return;
        }
    }

    --this->user_count_;

    if ( this->user_count_ <= 0 )
    {
        this->user_count_ = 0;
        this->ukn330 = 0;
    }

    CalUserLevel();

    LeaveCriticalSection(&this->user_crit_section_);
}


void CDoppelganger::CalUserLevel()
{
    this->user_max_level_ = 0;
    this->user_min_level_ = 0;

    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( this->user_max_level_ < this->user_info_[i].user_level )
                this->user_max_level_ = this->user_info_[i].user_level;

            if( this->user_min_level_ )
            {
                if( this->user_min_level_ > this->user_info_[i].user_level )
                    this->user_min_level_ = this->user_info_[i].user_level;
            }
            else
            {
                this->user_min_level_ = this->user_info_[i].user_level;
            }
        }
    }
}

void CDoppelganger::ClearUserData()
{

    EnterCriticalSection(&this->user_crit_section_);

    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        this->user_info_[i].object_index = -1;
        this->user_info_[i].user_level = 0;
    }
    this->user_count_ = 0;
    this->party_number_ = -1;
    this->ukn330 = 0;
    this->user_max_level_ = 0;
    this->user_min_level_ = 0;

    LeaveCriticalSection(&this->user_crit_section_);
}

int CDoppelganger::GetUserMaxLevel()
{
    return this->user_max_level_;
}

int CDoppelganger::GetUserMinLevel()
{
    return this->user_min_level_;
}

int CDoppelganger::GetUserAverageLevel()
{
    return this->user_average_level_;
}

void CDoppelganger::SetUserAverageLevel()
{
    if( GetUserCount() != 0)
    {
        int level_sum = 0;

        for(int i = 0; i < MAX_PARTYUSER; ++i )
        {
            if( this->user_info_[i].IsUser() == 1 )
                level_sum += gObj[this->user_info_[i].object_index].Level;
        }

        this->user_average_level_ = level_sum / GetUserCount();
    }
}

int CDoppelganger::GetUserCount()
{
    return this->user_count_;
}

int CDoppelganger::GetStartUserCount()
{
    return this->start_user_count_;
}

void CDoppelganger::SendNoticeMessage(char* message)
{

    PMSG_NOTICE notice_msg;
    TNotice::MakeNoticeMsg(&notice_msg, 0, message);

    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( gObj[this->user_info_[i].object_index].Connected > 2 )
                DataSend(this->user_info_[i].object_index, (LPBYTE)&notice_msg, 
                            notice_msg.h.size);
        }
    }
}

void CDoppelganger::SendDoppelgangerState(char state)
{
    PMSG_DOPPELGANGER_STATE state_msg;

    PHeadSubSetB((LPBYTE)&state_msg, 0xBFu, 0x10u, sizeof(state_msg));
    state_msg.state = state;

    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if ( gObj[this->user_info_[i].object_index].Connected > 2 )
            {
                DataSend(this->user_info_[i].object_index, (LPBYTE)&state_msg, 
                            state_msg.h.size);
            }
        }
    }
}


void CDoppelganger::PlatformLugardAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj)
{
    if( lpObj->m_bPShopOpen )
        CGPShopReqClose(lpObj->m_Index);

    if( lpObj->m_PK_Level >= 4 )
    {
		//lMsg.Get(3400)
        GCServerMsgStringSend("I can't help you while you is killer", lpObj->m_Index, 1);
        return;
    }

    char rest_time = 0;
    if( GetDoppelgangerState() != 0)
    {
        if( GetDoppelgangerState() == 1 )
        {
            rest_time = this->end_time_ + this->ready_time_
                        - (GetTickCount() - this->state_start_tick_) / 1000 / 60
                        + this->play_time_ + 1;
        }
        else
        {
            if ( GetDoppelgangerState() == 2 )
            {
                rest_time = this->play_time_ - (GetTickCount() - this->state_start_tick_) 
                            / 0x3E8 / 0x3C + this->end_time_ + 1;
            }
            else
            {
                rest_time = this->end_time_ - (GetTickCount() - this->state_start_tick_) 
                            / 0x3E8 / 0x3C + 1;
            }
        }
    }
    else
    {
        if( this->ukn2c4 == 1 )
        {
            if( GetUserCount() > 0 )
            {
                if( lpObj->PartyNumber < 0 )
                {
                    rest_time = this->play_time_ + this->end_time_ + this->ready_time_;
                }
                else
                {
                    if ( lpObj->PartyNumber == this->party_number_ )
                        rest_time = 0;
                    else
                        rest_time = this->play_time_ + this->end_time_ + this->ready_time_;
                }
            }
			else	//fix
			{
				rest_time = 0;
			}
        }
        else
        {
            rest_time = this->end_time_ + this->ready_time_ + this->play_time_ + 1;
        }
    }
	rest_time = 0; //addition
    PMSG_TALKRESULT talk_result_msg;

    talk_result_msg.h.c = 0xC3;
    talk_result_msg.h.headcode = 48; //0x30; //30
    talk_result_msg.h.size = sizeof(talk_result_msg); //A0
    talk_result_msg.result = 35;
    talk_result_msg.level1 = rest_time;
	talk_result_msg.level2 = rest_time;
	talk_result_msg.level3 = rest_time;
	talk_result_msg.level5 = rest_time;
	talk_result_msg.level6 = rest_time;
	talk_result_msg.level7 = rest_time;

    DataSend(lpObj->m_Index, (LPBYTE)&talk_result_msg, sizeof(talk_result_msg));
}


void CDoppelganger::MiddleTreasureAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj)
{
    if ( lpNpc->m_State == 0)
        return;

    if(abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3)
        return;


    if ( OpenTreasureBox() )
    {
        lpNpc->Life = 0;
        gObjLifeCheck(lpNpc, lpObj, 0, 1, 0, 0, 0, 0);

        for(int i = 0; i < kMaxMiddleTreasureBox; ++i )
        {
            if ( this->middle_treasure_box_index_[i] != -1 )
            {
                if ( this->middle_treasure_box_index_[i] != lpNpc->m_Index )
                {
                    gObjDel(this->middle_treasure_box_index_[i]);
                    this->middle_treasure_box_index_[i] = -1;
                }
            }
        }

        LogAddTD("[Doppelganger] MiddleTreasureBox Open [%d][%d, %d][%s][%s]",
                this->map_number_, lpNpc->X, lpNpc->Y,
                lpObj->AccountID, lpObj->Name);

        int item_drop = GetRandomValue(1000);
        if( item_drop <= 100 || item_drop >= 500 )
        {
            int user_count = GetStartUserCount();
            int monster_level = (GetMonsterLevelWeight() 
                                    * GetDefaultMonsterLevel(532));

            int monster_hp = (GetMonsterHpWeight() * GetDefaultMonsterHp(532));

            int monster_attack_min = (GetMonsterAttackWeight() 
                                        * GetDefaultMonsterAttackMin(532));

            int monster_attack_max = (GetMonsterAttackWeight() 
                                        * GetDefaultMonsterAttackMax(532));

            int monster_def = (GetMonsterDefenseWeight() 
                                        * GetDefaultMonsterDefense(532));

            for( int j = 0; j < user_count; ++j )
            {
                BYTE pos_x = lpNpc->X;
                BYTE pos_y = lpNpc->Y;

                if( !GetRandomLocation(pos_x, pos_y) )
                {
                    pos_x = lpNpc->X;
                    pos_y = lpNpc->Y;
                }

                int larva_index = AddMonsterLarva(this->map_number_,
                                            pos_x, pos_y,
                                            monster_level, monster_hp,
                                            monster_attack_min, 
                                            monster_attack_max,
                                            monster_def);

                if ( larva_index != -1 )
                    AddLarvaIndex(larva_index);

                LogAddTD("[Doppelganger] Create Larva (%d)[%d][%d, %d]", 
                        larva_index, this->map_number_, pos_x, pos_y);
            }
        }
        else
        {
            LogAddTD("[Doppelganger] MiddleTreasureBox ItemDrop [%d][%d, %d]", 
                        this->map_number_, lpNpc->X, lpNpc->Y);

            int loot_count = this->event_info_.GetMiddleItemDropCnt();
            for(int k = 0; k < loot_count; ++k )
            {
                BYTE pos_x = lpNpc->X;
                BYTE pos_y = lpNpc->Y;

                if( !GetRandomLocation(pos_x, pos_y) )
                {
                    pos_x = lpNpc->X;
                    pos_y = lpNpc->Y;
                }

                int reward_value = GetRandomValue(3);
                int item_bag_index = 
                    this->event_info_.GetMiddleItemBagIndex(reward_value);

                int drop_rate = 
                        this->event_info_.GetMiddleItemDropRate(reward_value);

                this->event_item_bag_.DoppelgangerItemDrop(lpObj, 
                                                            item_bag_index, 
                                                            drop_rate, 
                                                            0 /*flag*/);
            }
        }
    }
}


void CDoppelganger::LastTreasureAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj)
{
    if( lpNpc->m_State == 0)
        return;

    if( abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3 )
        return;


    if( OpenLastTreasureBox() )
    {
        lpNpc->Life = 0;
        gObjLifeCheck(lpNpc, lpObj, 0, 1, 0, 0, 0, 0);
        LogAddTD("[Doppelganger] LastTreasureBox Open [%d][%d, %d][%s][%s]",
                this->map_number_, lpNpc->X, lpNpc->Y,
                lpObj->AccountID, lpObj->Name);

        int loot_count = this->event_info_.GetLastItemDropCnt();
        for(int i = 0; i < loot_count; ++i )
        {
            BYTE pos_X = lpNpc->X;
            BYTE pos_y = lpNpc->Y;

            if ( !GetRandomLocation(pos_X, pos_y) )
            {
                pos_X = lpNpc->X;
                pos_y = lpNpc->Y;
            }

            int reward_value = GetRandomValue(6);
            int item_bag_index = 
                        this->event_info_.GetLastItemBagIndex(reward_value);
            int drop_rate = this->event_info_.GetLastItemDropRate(reward_value);
            this->event_item_bag_.DoppelgangerItemDrop(lpObj, 
                                                        item_bag_index, 
                                                        drop_rate, 
                                                        1 /*flag*/);
        }
    }

}


bool CDoppelganger::OpenTreasureBox()
{
    EnterCriticalSection(&this->treasure_box_crit_section_);

    if ( this->is_treasure_box_opened_ == 1 )
    {
        LeaveCriticalSection(&this->treasure_box_crit_section_);
        return false;
    }
    else
    {
        this->is_treasure_box_opened_ = 1;
        LeaveCriticalSection(&this->treasure_box_crit_section_);
        return true;
    }

}

bool CDoppelganger::OpenLastTreasureBox()
{

    EnterCriticalSection(&this->treasure_box_crit_section_);

    if ( this->is_last_treasure_box_opened_ == 1 )
    {
        LeaveCriticalSection(&this->treasure_box_crit_section_);
        return false;
    }
    else
    {
        this->is_last_treasure_box_opened_ = 1;
        LeaveCriticalSection(&this->treasure_box_crit_section_);
        return true;
    }
}

int CDoppelganger::GetRandomValue(int range)
{
    return (rand() / (32768.0 / range));
}

int CDoppelganger::GetMonseterHerdIndex()
{
    int ret = -1;
    for(int i = 0; i < kMaxMonsterHard; ++i )
    {
        if( this->monster_herd_[i].GetMonsterSize() == 0)
        {
            ret = i;
            break;
        }
    }

    if ( ret == -1 )
    {
        LogAddTD("[Doppelganger] Error Not Enought HerdIndex");
        ret = -1;
    }

    return ret;
}

void CDoppelganger::IncMonseterHerdIndex()
{
    ;
}

void CDoppelganger::SetHerdStartPosInfo(int herd_index, int pos_info, 
                                        int should_move)
{
    if ( herd_index < kMaxMonsterHard )
    {
        this->monster_herd_[herd_index].SetTotalInfo(this->map_number_,
                                            4 /*Radius*/ , 
                                            pos_info, 
                                            should_move);
    }
}

void CDoppelganger::SetHerdEndPosInfo(int herd_index, int pos_info)
{
    if ( herd_index < kMaxMonsterHard )
        this->monster_herd_[herd_index].SetEndPosition(pos_info);
}

void CDoppelganger::AddMonsterHerd(int herd_index, int monster_class, 
                                    int should_attack_first)
{

    if( herd_index >= kMaxMonsterHard )
        return;

    // CHECK THIS
    if ( monster_class >= 529 || monster_class <= 539 )
    {
        float weight = 1.0;

        if ( GetMissionResult() == 0)
            weight = 2.0;


        int monster_level = (GetMonsterLevelWeight() 
                                * GetDefaultMonsterLevel(monster_class));

        int monster_hp = (GetMonsterHpWeight() 
                                * GetDefaultMonsterHp(monster_class) * weight);

        int monster_attack_min = (GetMonsterAttackWeight() 
                                * GetDefaultMonsterAttackMin(monster_class) 
                                * weight);

        int monster_attack_max = (GetMonsterAttackWeight() 
                                * GetDefaultMonsterAttackMax(monster_class) 
                                * weight);

        int monster_def = (GetMonsterDefenseWeight() 
                            * GetDefaultMonsterDefense(monster_class) * weight);


        if( monster_class == 530 )
        {
            if ( GetKillerState() == 3 )
                monster_hp = GetKillerHp();
        }

        if( monster_class == 529 )
        {
            if ( GetAngerKillerState() == 3 )
                monster_hp = GetAngerKillerHp();
        }

        int monster_index = this->monster_herd_[herd_index].AddMonsterEX(
                                    monster_class, should_attack_first,
                                    monster_level, monster_hp,
                                    monster_attack_min,
                                    monster_attack_max, monster_def);

        if ( monster_index < 0 )
        {
            LogAddTD("Error AddMonsterEx Index is %d", monster_index);
            return;
        }

        if( monster_class == 531 )
        {
            AddIceWorkerIndex(monster_index);

            LogAddTD("[Doppelganger] Iceworker monster Regen [%d][%d, %d] (%d)",
                gObj[monster_index].MapNumber, 
                gObj[monster_index].X, gObj[monster_index].Y,
                monster_index);
        }
        else if( monster_class == 529 )
        {
            SetAngerKillerHp(monster_hp);
            SetAngerKillerState(2);

            LogAddTD("[Doppelganger] AngerKiller monster Regen "
                "[%d][%d, %d] (%d)",
                gObj[monster_index].MapNumber,
                gObj[monster_index].X, gObj[monster_index].Y,
                monster_index);
        }            
        else if ( monster_class == 530 )
        {
            SetMiddleBossIndex(530, monster_index);
            SetKillerHp(monster_hp);
            SetKillerState(2);
            LogAddTD("[Doppelganger] Killer monster Regen [%d][%d, %d] (%d)",
                gObj[monster_index].MapNumber,
                gObj[monster_index].X,
                gObj[monster_index].Y,
                monster_index);
        }
    }
}

BYTE CDoppelganger::GetKillerState()
{
    return this->killer_state_;
}

int CDoppelganger::GetKillerHp()
{
    return this->killer_hp_;
}

void CDoppelganger::SetKillerHp(int killer_hp)
{
    this->killer_hp_ = killer_hp;
}

BYTE CDoppelganger::GetAngerKillerState()
{
    return this->anger_killer_state_;
}

int CDoppelganger::GetAngerKillerHp()
{
    return this->anger_killer_hp_;
}

void CDoppelganger::SetAngerKillerHp(int anger_killer_hp)
{
    this->anger_killer_hp_ = anger_killer_hp;
}

void CDoppelganger::AddMonsterBoss(int herd_index, int monster_class, 
                                    int should_attack_first)
{
    ;
}

void CDoppelganger::AddMonsterNormal(int monster_class, int pos_x, int pos_y)
{
    ;
}

void CDoppelganger::SetIceWorkerRegen(BYTE pos_info)
{
    PMSG_DOPPELGANGER_ICEWORKER_STATE state_msg;
    char msg_notice[256];

    sprintf(msg_notice, lMsg.Get(3442));
    SendNoticeMessage(msg_notice);

    PHeadSubSetB((LPBYTE)&state_msg, 0xBFu, 0x11u, sizeof(state_msg));

    this->ice_walker_dead = 0;//this->ukn370 = 0;
    state_msg.state = 0;
    state_msg.pos_info = pos_info;
    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() == 1 )
        {
            if( gObj[this->user_info_[i].object_index].Connected > 2 )
            {
                DataSend(this->user_info_[i].object_index, (LPBYTE)&state_msg, 
                            state_msg.h.size);
            }
        }
    }

    this->ice_walker_spawn_time = GetTickCount();//this->ukn36c = GetTickCount();
}

void CDoppelganger::AddIceWorkerIndex(int monster_index)
{
    for(int i = 0; i < kMaxIceWorker; ++i )
    {
        if ( this->ice_worker_index_[i] == -1 )
        {
            this->ice_worker_index_[i] = monster_index;
            return;
        }
    }
}

void CDoppelganger::DelIceWorkerIndex(int monster_index)
{

    for(int i = 0; i < kMaxIceWorker; ++i )
    {
        if ( this->ice_worker_index_[i] == monster_index )
        {
            this->ice_worker_index_[i] = -1;
            break;
        }
    }

    CheckIceWorker();
}


bool CDoppelganger::CheckIceWorker()
{

    for(int i = 0; i < kMaxIceWorker; ++i )
    {
        if( this->ice_worker_index_[i] != -1 )
            return false;
    }

    this->ice_walker_dead = 1;//this->ukn370 = 1;
    this->mission_result_ = 1;

    char pMsgNotice[256];
    sprintf(pMsgNotice, lMsg.Get(3443));
    SendNoticeMessage(pMsgNotice);

    PMSG_DOPPELGANGER_ICEWORKER_STATE pStateMsg;

    PHeadSubSetB((LPBYTE)&pStateMsg, 0xBFu, 0x11u, 6);
    pStateMsg.state = 1;
    pStateMsg.pos_info = 0;

    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() == 1 )
        {
            if ( gObj[this->user_info_[i].object_index].Connected > 2 )
            {
                DataSend(this->user_info_[i].object_index, (LPBYTE)&pStateMsg, 
                            pStateMsg.h.size);
            }
        }
    }

    SendDoppelgangerTimerMsg(-1);
    LogAddTD("[Doppelganger] Iceworker Die - Mission Clear");

    return true;
}

void CDoppelganger::AddLarvaIndex(int monster_index)
{
    for(int i = 0; i < kMaxLarva; ++i )
    {
        if ( this->larva_index_[i] == -1 )
        {
            this->larva_index_[i] = monster_index;
            return;
        }
    }
}

void CDoppelganger::DelLarvaAll()
{
    for(int i = 0; i < kMaxLarva; ++i )
    {
        if ( this->larva_index_[i] != -1 )
        {
            gObjDel(this->larva_index_[i]);
            this->larva_index_[i] = -1;
        }
    }
}


void CDoppelganger::MonsterHerdStart(int herd_index)
{
    if ( herd_index <= 199 )
        this->monster_herd_[herd_index].Start();
}

void CDoppelganger::CheckDoppelgangerMosterPos(OBJECTSTRUCT *lpObj)
{
    int pos_num = GetDoppelgangerPosIndex(lpObj->X, lpObj->Y);

    int last_pos = 22;

    if ( !lpObj->m_bIsInMonsterHerd )
        return;

    if ( lpObj->Class == 531 )
        last_pos = -1;

    if ( pos_num == last_pos )
    {
        lpObj->Live = 0;
        ++this->monster_goal_count_;
        LogAddTD("[Doppelganger] Monster MagicCircle in: %d, MonsterType(%d)", 
                            this->monster_goal_count_, lpObj->Class);

		
		gObjAddBuffEffect(lpObj, 105, 0, 0, 0, 0, 10);

        SendMonsterGoalCount();
        if ( lpObj->Class == 530 )
        {
            SetKillerHp(lpObj->Life);
            SetKillerState(3);
        }

        if ( lpObj->Class == 529 )
        {
            SetAngerKillerHp(lpObj->Life);
            SetAngerKillerState(3);
        }

        if ( this->monster_goal_count_ >= 3 )
        {
            this->ukn314 = 0;
            SetDoppelgangerState(3);
        }
    }
    else
    {
        CDoppelgangerMonsterHerd* monster_herd = 
                        (CDoppelgangerMonsterHerd*)lpObj->m_lpMonsterHerd;

        if( !monster_herd )
            return;

        if( pos_num > monster_herd->GetPosIndex() )
        {
            if( monster_herd->IsMove() )
            {
                int tmp_pos_num = pos_num + 1;
                if ( pos_num + 1 > last_pos )
                    tmp_pos_num = pos_num;

                monster_herd->SetPosIndex(tmp_pos_num);
            }
            else
            {
                monster_herd->SetPosIndex(monster_herd->GetEndPosIndex());
            }
        }

        if ( lpObj->Class != 531 )
        {

            if ( this->ukn3fc == -1 )
            {
                this->ukn3fc = lpObj->m_Index;
                this->monster_pos_ = pos_num;
            }
            else
            {
                if ( gObj[this->ukn3fc].Live )
                {
                    if ( pos_num > this->monster_pos_ )
                    {
                        this->ukn3fc = lpObj->m_Index;
                        this->monster_pos_ = pos_num;
                    }
                }
                else
                {
                    this->ukn3fc = lpObj->m_Index;
                    this->monster_pos_ = pos_num;
                }
            }
        }
    }

}

int CDoppelganger::GetDoppelgangerPosIndex(BYTE pos_x, BYTE pos_y)
{
    return pos_info_.GetPosIndex(this->map_number_, pos_x, pos_y);
}

void CDoppelganger::SendMonsterGoalCount()
{
    PMSG_DOPPELGANGER_MONSTERGOALCOUNT pMsg; // [sp+50h] [bp-Ch]@1

    PHeadSubSetB((LPBYTE)&pMsg, 0xBFu, 0x14u, sizeof(pMsg));
    pMsg.state = 3;
    pMsg.count = this->monster_goal_count_;

    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( gObj[this->user_info_[i].object_index].Connected > 2 )
            {
                DataSend(this->user_info_[i].object_index, 
                            (LPBYTE)&pMsg, pMsg.h.size);
            }
        }
    }
}

void CDoppelganger::SendDoppelgangerMonserPos()
{
    PMSG_DOPPELGANGER_MONSTERPOS pMsg;

    PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0xF, sizeof(pMsg));

    pMsg.monster_pos = this->monster_pos_;

    for( int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( gObj[this->user_info_[i].object_index].Connected > 2 )
            {
                DataSend(this->user_info_[i].object_index, 
                            (LPBYTE)&pMsg, pMsg.h.size);
            }
        }
    }
}

void CDoppelganger::SendDoppelgangerUserPos()
{
    DOPPELGANGER_POS_INFO user_pos_info; // [sp+4Ch] [bp-11Ch]@5
    PMSG_DOPPELGANGER_USERPOS user_pos_msg; // [sp+58h] [bp-110h]@1
    char send_buffer[256]; // [sp+60h] [bp-108h]@5


    int offset = 0;
    user_pos_msg.time = 60 * this->play_time_ - 
                        (GetTickCount() - this->state_start_tick_) / 1000;

    offset = 8;
    int tmp_count = 0;

    for( int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( gObj[this->user_info_[i].object_index].Connected > 2 )
            {
                user_pos_info.obj_index = this->user_info_[i].object_index;
                user_pos_info.map_number = this->map_number_;
                user_pos_info.pos_index = GetDoppelgangerPosIndex(
                                gObj[this->user_info_[i].object_index].X,
                                gObj[this->user_info_[i].object_index].Y);

                ++tmp_count;

                memcpy(&send_buffer[offset], &user_pos_info, 
                                        sizeof(user_pos_info));

                offset += sizeof(user_pos_info);
            }
        }
    }

    user_pos_msg.user_count = tmp_count;
    PHeadSubSetB((LPBYTE)&user_pos_msg, 0xBFu, 0x12u, offset);
    memcpy(send_buffer, &user_pos_msg, 8u);

    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser()  )
        {
            if( gObj[this->user_info_[i].object_index].Connected > 2 )
                DataSend(this->user_info_[i].object_index, (LPBYTE)send_buffer, 
                            user_pos_msg.h.size);
        }
    }
}


void CDoppelganger::SelfExplosion(int index, CMagicInf* magic, int target_index)
{
    OBJECTSTRUCT* lpObj = &gObj[index];

    if( lpObj->X < 0  || lpObj->X > 255)
        return;

    if( lpObj->Y < 0  || lpObj->Y > 255)
        return;

    int min_x = lpObj->X - 2;
    int max_x = lpObj->X + 2;
    int min_y = lpObj->Y - 2;
    int max_y = lpObj->Y + 2;

    if ( min_x < 0 )
        min_x = 0;

    if ( min_y < 0 )
        min_y = 0;

    if ( min_x > 255 )
        min_x = 255;

    if ( min_y > 255 )
        min_y = 255;

    lpObj->m_PosNum = -1;
    lpObj->Life = 0;
    gObjLifeCheck(lpObj, lpObj, 0, 1, 0, 0, 0, 0);
    GCMagicAttackNumberSend(lpObj, magic->m_Skill, target_index, 1);

    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() 
            && gObj[this->user_info_[i].object_index].Connected > 2
            && gObj[this->user_info_[i].object_index].X >= min_x
            && gObj[this->user_info_[i].object_index].X <= max_x
            && gObj[this->user_info_[i].object_index].Y >= min_y
            && gObj[this->user_info_[i].object_index].Y <= max_y )
        {
            gObjAddAttackProcMsgSendDelay(lpObj, 
                                            50,     // MsgCode
                                            this->user_info_[i].object_index, 
                                            500,    // delay
                                            magic->m_Skill, 
                                            0);     // SubCode2
            return;
        }
    }
}


void CDoppelganger::AngerKillerAttack(OBJECTSTRUCT *lpObj)
{

    if ( lpObj->X < 0  || lpObj->X > 255)
        return;

    if ( lpObj->Y < 0  || lpObj->Y > 255)
        return;

    int min_x = lpObj->X - 2;
    int max_x = lpObj->X + 2;
    int min_y = lpObj->Y - 2;
    int max_y = lpObj->Y + 2;

    if ( min_x < 0 )
        min_x = 0;

    if ( min_y < 0 )
        min_y = 0;

    if ( min_x > 255 )
        min_x = 255;

    if ( min_y > 255 )
        min_y = 255;


    for(int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if ( this->user_info_[i].IsUser() 
            && gObj[this->user_info_[i].object_index].Connected > 2)
        {

            if ( gObj[this->user_info_[i].object_index].X >= min_x 
                && gObj[this->user_info_[i].object_index].X <= max_x
                && gObj[this->user_info_[i].object_index].Y >= min_y
                && gObj[this->user_info_[i].object_index].Y <= max_y)
            {

                gObjAttack(lpObj, &gObj[this->user_info_[i].object_index], 0, 0, 0, 0, 0, 0, 0);
                gObjBackSpring(&gObj[this->user_info_[i].object_index], lpObj);

            }
        }
    }
}

void CDoppelganger::SendDoppelgangerResult(OBJECTSTRUCT *lpObj, BYTE result)
{
    PMSG_DOPPELGANGER_RESULT result_msg;

    int reward_exp = 0;

    PHeadSubSetB((LPBYTE)&result_msg, 0xBFu, 0x13u, 12);

    result_msg.result = result;
    result_msg.reward_exp = reward_exp;

    DataSend(lpObj->m_Index, (LPBYTE)&result_msg, result_msg.h.size);
    LogAddTD("[Doppelganger] Event fail (%d) [%s][%s]", 
                result, lpObj->AccountID, lpObj->Name);
}


void CDoppelganger::SendDoppelgangerResultAll()
{
    PMSG_DOPPELGANGER_RESULT result_msg;

    int reward_exp = 0;
    PHeadSubSetB((LPBYTE)&result_msg, 0xBFu, 0x13u, sizeof(result_msg));

    if( this->monster_goal_count_ < 3 )
        result_msg.result = 0;
    else
        result_msg.result = 2;

    for( int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( gObj[this->user_info_[i].object_index].Connected == 3 )
            {
#if defined __CUSTOMS__
				g_ShopPointEx.AddEventBonus(this->user_info_[i].object_index, ShopPointExEvent::DG);
#endif
			// ----
                result_msg.reward_exp = reward_exp;
                DataSend(this->user_info_[i].object_index, (LPBYTE)&result_msg, 
                        result_msg.h.size);

                if( g_RegDoppelgangerClear == 1 )
                {
                    //if( !result_msg.result )
                    //    GEReqdoppelgangerClear(user_info_[i].object_index);
                }

                LogAddTD(
                    "[Doppelganger] Event Result (%d) [%s][%s]",
                    result_msg.result,
                    gObj[this->user_info_[i].object_index].AccountID,
                    gObj[this->user_info_[i].object_index].Name);
            }
        }
    }

    LogAddTD("[Doppelganger] Event Complete");
}

void CDoppelganger::ArrangeMonsterHerd() //check it twice
{
    PMSG_DOPPELGANGER_ICEWORKER_STATE state_msg;

    char msg_notice[256];

    if( this->ice_walker_dead = 0 )//if( !this->ukn370 )
    {
        if( GetTickCount() - this->ice_walker_spawn_time > 60000 )//this->ukn36c > 60000 )
        {
            this->mission_result_ = 0;

            sprintf(msg_notice, lMsg.Get(3444));

            SendNoticeMessage(msg_notice);

            for( int i = 0; i < MAX_PARTYUSER; ++i )
            {
                if( this->ice_worker_index_[i] != -1 )
                {
                    gObj[this->ice_worker_index_[i]].Live = 0;
                    gObjDel(this->ice_worker_index_[i]);
                    this->ice_worker_index_[i] = -1;
                }
            }

            this->ice_walker_dead = 1;//this->ukn370 = 1;
            PHeadSubSetB((LPBYTE)&state_msg, 0xBF, 0x11, sizeof(state_msg));
            state_msg.state = 1;
            state_msg.pos_info = 0;

            for(int i = 0; i < MAX_PARTYUSER; ++i )
            {
                if( this->user_info_[i].IsUser()  )
                {
                    if( gObj[this->user_info_[i].object_index].Connected > 2 )
                    {
                        DataSend(this->user_info_[i].object_index, 
                                (LPBYTE)&state_msg, state_msg.h.size);
                    }
                }
            }
            LogAddTD("[Doppelganger] Iceworker Disappear Mission Fail");
        }
    }

    for(int j = 0; j < kMaxMonsterHard; ++j )
    {
        if( !this->monster_herd_[j].IsActive() )
        {
            this->monster_herd_[j].Stop();
            this->monster_herd_[j].SetPosIndex(0);
        }

        this->monster_herd_[j].ArrangeMonsterHerd();
    }
}

void CDoppelganger::MoveDoppelgangerMonsterProc()
{
    for(int i = 0; i < OBJ_MAXMONSTER; ++i )
    {
        OBJECTSTRUCT* lpObj = &gObj[i];

        if( gObj[i].Connected != 3 )
            continue;

        if( lpObj->m_State != 2 )
            continue;

        if( lpObj->Type != OBJ_MONSTER )
            continue;

        if ( !(lpObj->MapNumber >= 65 && lpObj->MapNumber <= 68) )
            continue;

        if( lpObj->PathCount )
        {
            if( !gObjCheckUsedBuffEffect(lpObj, 57) 
                && !gObjCheckUsedBuffEffect(lpObj, 61)
                && !gObjCheckUsedBuffEffect(lpObj, 72))
            {
                int delay_time = 0;

                if( lpObj->DelayLevel )
                    delay_time = 300;

                if( lpObj->Type == OBJ_MONSTER )
                {
                    if( lpObj->m_bIsInMonsterHerd  )
                        lpObj->m_MoveSpeed = 250;
                }

                int move_time = 0;

                if( lpObj->PathDir[lpObj->PathCur] % 2 )
                    move_time = delay_time + lpObj->m_MoveSpeed;
                else
                    move_time = ((delay_time + lpObj->m_MoveSpeed) * 1.3);

                if( GetTickCount() - lpObj->PathTime > move_time )
                {
                    int tmp = GetTickCount();
                    if( lpObj->PathCur == 1 )
                        lpObj->PathTime = tmp;

                    if( lpObj->PathCur < 14 )
                    {
                        int next_x = lpObj->PathX[lpObj->PathCur];
                        int next_y = lpObj->PathY[lpObj->PathCur];
                        int attr = MapC[lpObj->MapNumber].GetAttr(next_x, 
                                                                    next_y);

                        if( lpObj->Type != OBJ_USER || (attr & 4) != 4 
                                && (attr & 8) != 8 )
                        {
                            lpObj->X = lpObj->PathX[lpObj->PathCur];
                            lpObj->Y = lpObj->PathY[lpObj->PathCur];
                            lpObj->Dir = lpObj->PathDir[lpObj->PathCur];
                            lpObj->PathTime = GetTickCount();
                            ++lpObj->PathCur;
                            if( lpObj->PathCur >= lpObj->PathCount )
                            {
                                lpObj->PathCur = 0;
                                lpObj->PathCount = 0;
                                if ( lpObj->Type == 2 || lpObj->Type == 3 )
                                    lpObj->PathStartEnd = 0;
                            }
                        }
                        else
                        {
                            LogAddTD(
                                "[ CHECK POSITION ] MoveMosterProc "
                                "[%s][%s] Map[%d]-(%d,%d) "
                                "User(%d,%d) Can not Move Position Attr[%d]",
                                lpObj->AccountID, lpObj->Name,
                                lpObj->MapNumber, next_x, next_y,
                                lpObj->X, lpObj->Y, attr);

                            for( int i = 0; i < 15; ++i )
                            {
                                lpObj->PathX[i] = 0;
                                lpObj->PathY[i] = 0;
                                lpObj->PathOri[i] = 0;
                            }

                            lpObj->PathCount = 0;
                            lpObj->PathCur = 0;
                            lpObj->PathTime = GetTickCount();

                            if ( lpObj->Type == OBJ_MONSTER 
                                    || lpObj->Type == OBJ_NPC )
                            {
                                lpObj->PathStartEnd = 0;
                            }

                            if ( lpObj->Type == OBJ_USER )
                            {
                                gObjSetPosition(lpObj->m_Index, 
                                                lpObj->X, 
                                                lpObj->Y);
                            }
                        }

                    }
                }
            }
        }

        CreateFrustrum(lpObj->X, lpObj->Y, i);
    }
}


bool CDoppelganger::GetRandomLocation(BYTE& pos_x, BYTE& pos_y)
{
    int count = 50;
    while( count-- )
    {
        pos_x += ((rand() % 2 != 0 ? 2 : 0) - 1) * rand() % 3;
        pos_y += ((rand() % 2 != 0 ? 2 : 0) - 1) * rand() % 3;

        BYTE map_attribute = MapC[this->map_number_].GetAttr(pos_x, pos_y);

        if ( map_attribute == 0 || map_attribute == 2 )
            return true;
    }

    return false;
}

bool CDoppelganger::GetRandomLocation(BYTE& pos_x, BYTE& pos_y, int seed)
{
    if ( seed >= 0 || seed <= 7 )
    {
        int dir[8][2];

        dir[0][0] = -2;
        dir[0][1] = -2;
        dir[1][0] = 0;
        dir[1][1] = -2;
        dir[2][0] = 2;
        dir[2][1] = -2;
        dir[3][0] = 2;
        dir[3][1] = 0;
        dir[4][0] = 2;
        dir[4][1] = 2;
        dir[5][0] = 0;
        dir[5][1] = 2;
        dir[6][0] = -2;
        dir[6][1] = 2;
        dir[7][0] = -2;
        dir[7][1] = 0;
		pos_x += dir[seed][0];
		pos_y += dir[seed][1];

        BYTE map_attribute = MapC[this->map_number_].GetAttr(pos_x, pos_y);

        if ( map_attribute == 0 || map_attribute == 2 )
            return true;
    }

    return false;
}

bool CDoppelganger::AddMiddleTreasureBoxAll(BYTE pos_x, BYTE pos_y)
{

    EnterCriticalSection(&this->treasure_box_crit_section_);
    this->is_treasure_box_opened_ = 1;
    LeaveCriticalSection(&this->treasure_box_crit_section_);

    int seed = 0;
    for(int i = 0; i < kMaxMiddleTreasureBox; ++i )
    {
        BYTE final_pos_x = pos_x; 
        BYTE final_pos_y = pos_y;

        if( i )
        {
            bool result = false;

            for( int j = seed; j < 8; ++j )
            {
                result = GetRandomLocation(final_pos_x, final_pos_y, j);

                if( result )
                {
                    seed = j + 1;
                    break;
                }

                final_pos_x = pos_x;
                final_pos_y = pos_y;
            }

            if( !result )
            {
                LogAddTD("MiddleTreasureBox RandLocation Error [%d, %d]",
                        final_pos_x, final_pos_y);

                final_pos_x = pos_x;
                final_pos_y = pos_y;
            }
        }
        else
        {
            final_pos_x = pos_x;
            final_pos_y = pos_y;
        }

        if ( this->middle_treasure_box_index_[i] != -1 )
        {
            gObjDel(this->middle_treasure_box_index_[i]);
            this->middle_treasure_box_index_[i] = -1;
        }

        this->middle_treasure_box_index_[i] = 
                                AddMiddleTreasureBox(this->map_number_, 
                                                     final_pos_x, final_pos_y);

    }

    this->middle_treasure_box_spawn_time_ = GetTickCount();
    this->is_middle_treasure_box_spawned_ = 1;

    EnterCriticalSection(&this->treasure_box_crit_section_);
    this->is_treasure_box_opened_ = 0;
    LeaveCriticalSection(&this->treasure_box_crit_section_);

    return true;
}


int CDoppelganger::AddMiddleTreasureBox(int map_number, BYTE pos_x, 
                                                        BYTE pos_y)
{
    int monster_index = gObjAddMonster(map_number);
    if ( monster_index < 0 )
    {
        LogAddTD("[Doppelganger] Create MiddleTreasureBox Fail "
            "[%d][%d, %d] (%d)", 
            this->map_number_, pos_x, pos_y, monster_index);

        return monster_index;
    }

    gObj[monster_index].m_PosNum = -1;
    gObj[monster_index].X = pos_x;
    gObj[monster_index].Y = pos_y;
    gObj[monster_index].MapNumber = map_number;
    gObj[monster_index].TX = gObj[monster_index].X;
    gObj[monster_index].TY = gObj[monster_index].Y;
    gObj[monster_index].m_OldX = gObj[monster_index].X;
    gObj[monster_index].m_OldY = gObj[monster_index].Y;
    gObj[monster_index].StartX = gObj[monster_index].X;
    gObj[monster_index].StartY = gObj[monster_index].Y;

    MONSTER_ATTRIBUTE* monster_attribute = gMAttr.GetAttr(541);

    if( !monster_attribute )
    {
        gObjDel(monster_index);
        return -1;
    }
    
    gObj[monster_index].Level = monster_attribute->m_Level;
    gObjSetMonster(monster_index, 541);
    gObj[monster_index].Life = 100.0;
    gObj[monster_index].MaxLife = 100.0;
    gObj[monster_index].m_Defense = 100;
    gObj[monster_index].m_AttackDamageMin = 100;
    gObj[monster_index].m_AttackDamageMax = 100;
    gObj[monster_index].MaxRegenTime = 1000;
    gObj[monster_index].Dir = 1;

    LogAddTD("[Doppelganger] Create MiddleTreasureBox Success "
                "[%d][%d, %d] (%d)",
                this->map_number_, pos_x, pos_y, monster_index);

    return monster_index;
}

bool CDoppelganger::AddLastTreasureBox(BYTE pos_x, BYTE pos_y)
{
    int monster_index = gObjAddMonster(this->map_number_);

    if ( monster_index < 0 )
    {
        LogAddTD( "[Doppelganger] Create LastTreasureBox Fail "
                    "[%d][%d, %d] (%d)",
                    this->map_number_, pos_x, pos_y, monster_index);

        return false;
    }

    gObj[monster_index].m_PosNum = -1;
    gObj[monster_index].X = pos_x;
    gObj[monster_index].Y = pos_y;
    gObj[monster_index].MapNumber = this->map_number_;
    gObj[monster_index].TX = gObj[monster_index].X;
    gObj[monster_index].TY = gObj[monster_index].Y;
    gObj[monster_index].m_OldX = gObj[monster_index].X;
    gObj[monster_index].m_OldY = gObj[monster_index].Y;
    gObj[monster_index].StartX = gObj[monster_index].X;
    gObj[monster_index].StartY = gObj[monster_index].Y;

    MONSTER_ATTRIBUTE* monster_attribute = gMAttr.GetAttr(542);

    if( !monster_attribute )
    {
        gObjDel(monster_index);
        return false;
    }

    gObj[monster_index].Level = monster_attribute->m_Level;
    gObjSetMonster(monster_index, 542);
    gObj[monster_index].Life = 100.0;
    gObj[monster_index].MaxLife = 100.0;
    gObj[monster_index].m_Defense = 100;
    gObj[monster_index].m_AttackDamageMin = 100;
    gObj[monster_index].m_AttackDamageMax = 100;
    gObj[monster_index].MaxRegenTime = 1000;
    gObj[monster_index].Dir = 1;

    this->last_treasure_box_index_ = monster_index;

    EnterCriticalSection(&this->treasure_box_crit_section_);
    this->is_last_treasure_box_opened_ = 0;
    LeaveCriticalSection(&this->treasure_box_crit_section_);

    LogAddTD("[Doppelganger] Create LastTreasureBox Success [%d][%d, %d] (%d)",
        this->map_number_, pos_x, pos_y, monster_index);


    return true;
}

int CDoppelganger::AddMonsterLarva(int map_number, BYTE pos_x, BYTE pos_y, 
                                    int monster_level, 
                                    int monster_hp, 
                                    int monster_attack_min, 
                                    int monster_attack_max, 
                                    int monster_def)
{

    int monster_index = gObjAddMonster(map_number);
    if ( monster_index < 0 )
    {
        LogAddTD("[Doppelganger] Create Larva Fail [%d][%d, %d]", 
                    this->map_number_, pos_x, pos_y);
        return -1;
    }

    gObj[monster_index].m_PosNum = -1;
    gObj[monster_index].X = pos_x;
    gObj[monster_index].Y = pos_y;
    gObj[monster_index].MapNumber = map_number;
    gObj[monster_index].TX = gObj[monster_index].X;
    gObj[monster_index].TY = gObj[monster_index].Y;
    gObj[monster_index].m_OldX = gObj[monster_index].X;
    gObj[monster_index].m_OldY = gObj[monster_index].Y;
    gObj[monster_index].StartX = gObj[monster_index].X;
    gObj[monster_index].StartY = gObj[monster_index].Y;

    MONSTER_ATTRIBUTE* monster_attribute = gMAttr.GetAttr(532);
    if( !monster_attribute )
    {
        gObjDel(monster_index);
        return -1;
    }

    gObj[monster_index].Level = monster_attribute->m_Level;
    gObjSetMonster(monster_index, 532);

    gObj[monster_index].Level = monster_level;
    gObj[monster_index].Life = (double)monster_hp;
    gObj[monster_index].MaxLife = (double)monster_hp;
    gObj[monster_index].m_Defense = monster_def;
    gObj[monster_index].m_AttackDamageMin = monster_attack_min;
    gObj[monster_index].m_AttackDamageMax = monster_attack_max;
    gObj[monster_index].m_SuccessfulBlocking = monster_def;
    gObj[monster_index].MaxRegenTime = 1000;
    gObj[monster_index].Dir = rand() % 8;

    return monster_index;
}

void CDoppelganger::DelTreasureBox(int object_index, int monster_class)
{
    if( monster_class == 541 )
    {
        for( int i = 0; i < kMaxMiddleTreasureBox; ++i )
        {
            if( this->middle_treasure_box_index_[i] == object_index )
            {
                gObjDel(object_index);
                this->middle_treasure_box_index_[i] = -1;
            }
        }
    }
    else
    {
        if( monster_class == 542 )
        {
            gObjDel(object_index);
            this->last_treasure_box_index_ = -1;
        }
    }
}

int CDoppelganger::CheckMapTile(int map_number, BYTE pos_x, BYTE pos_y)
{
    return pos_info_.CheckStartMapTile(map_number, pos_x, pos_y);
}

void CDoppelganger::SendMapTileInfo(OBJECTSTRUCT *lpObj, char map_set_type)
{
    char cTEMP_BUF[256];
    PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

    PHeadSetB((LPBYTE)lpMsg, 0x46, 
                sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);

    PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];

    lpMsg->btType = 0;
    lpMsg->btCount = 1;
    lpMsg->btMapAttr = 4;
    lpMsg->btMapSetType = map_set_type;

    pos_info_.GetStartMapAttr(this->map_number_, 
                                lpMsgBody[0].btX, lpMsgBody[0].btY, 
                                lpMsgBody[1].btX, lpMsgBody[1].btY);

    DataSend(lpObj->m_Index, (LPBYTE)lpMsg, lpMsg->h.size);
}

void CDoppelganger::SendMapTileInfoAll(BYTE map_set_type)
{
    for( int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( gObj[this->user_info_[i].object_index].Connected > 2 )
            {
                SendMapTileInfo(&gObj[this->user_info_[i].object_index], 
                                    map_set_type);
            }
        }
    }
}

MONSTER_ATTRIBUTE* CDoppelganger::GetDefaultMonsterInfo(int monster_class)
{
    MONSTER_ATTRIBUTE* monster_attribute = gMAttr.GetAttr(monster_class);

    if( !monster_attribute )
    {
        MsgBox(lMsg.Get(509), monster_class, __FILE__, __LINE__ );
    }

    return monster_attribute;
}

int CDoppelganger::GetDefaultMonsterLevel(int monster_class)
{
    if(monster_class >= 529 && monster_class <= 539)
    {
        return monster_attribute_[monster_class-529]->m_Level;
    }
    else
    {
        return 1;
    }
}

int CDoppelganger::GetDefaultMonsterHp(int monster_class)
{
    if(monster_class >= 529 && monster_class <= 539)
    {
        return monster_attribute_[monster_class-529]->m_Hp;
    }
    else
    {
        return 1;
    }
}


int CDoppelganger::GetDefaultMonsterAttackMax(int monster_class)
{
    if(monster_class >= 529 && monster_class <= 539)
    {
        return monster_attribute_[monster_class-529]->m_DamageMax;
    }
    else
    {
        return 1;
    }
}

int CDoppelganger::GetDefaultMonsterAttackMin(int monster_class)
{
    if(monster_class >= 529 && monster_class <= 539)
    {
        return monster_attribute_[monster_class-529]->m_DamageMin;
    }
    else
    {
        return 1;
    }
}

int CDoppelganger::GetDefaultMonsterDefense(int monster_class)
{
    if(monster_class >= 529 && monster_class <= 539)
    {
        return monster_attribute_[monster_class-529]->m_Defense;
    }
    else
    {
        return 1;
    }
}

float CDoppelganger::GetMonsterLevelWeight()
{
    return this->event_info_.GetMonsterLevelWeight();
}

float CDoppelganger::GetMonsterHpWeight()
{
    return this->event_info_.GetMonsterHpWeight();
}

float CDoppelganger::GetMonsterAttackWeight()
{
    return this->event_info_.GetMonsterAttackWeight();
}

float CDoppelganger::GetMonsterDefenseWeight()
{
    return this->event_info_.GetMonsterDefenseWeight();
}

int CDoppelganger::GetMissionResult()
{
    return this->mission_result_;
}

void CDoppelganger::SetMiddleBossIndex(int monster_class, int monster_index)
{
    if( monster_class == 530 )
    {
        for( int i = 0; i < 4; ++i )
        {
            if( this->middle_boss_index_[i] == -1 )
                this->middle_boss_index_[i] = monster_index;
        }
    }
}

void CDoppelganger::SetMiddleBossKill(int monster_class, int monster_index)
{

    if( monster_class == 531 )
    {
        this->middle_boss_index_[3] = 1;
    }
    else if ( monster_class == 529 )
    {
        this->middle_boss_index_[2] = 1;
    }
    else
    {
        for( int i = 0; i < 4; ++i )
        {
            if( this->middle_boss_index_[i] == monster_index )
            {
                this->middle_boss_killed_[i] = 1;
                return;
            }
        }
    }
}

int CDoppelganger::GetMiddleKillBossKill(int number)
{
    return this->middle_boss_killed_[number];
}


void CDoppelganger::SendDoppelgangerTimerMsg(int message)
{
    PMSG_SET_DEVILSQUARE pMsg;

    PHeadSetB((LPBYTE)&pMsg, 0x92u, sizeof(pMsg));
    pMsg.Type = message;

    for( int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( gObj[this->user_info_[i].object_index].Connected > 2 )
            {
                DataSend(user_info_[i].object_index, (LPBYTE)&pMsg,
                        pMsg.h.size);
            }
        }
    }
}

void CDoppelganger::SaveDoppelgangerTmpInfo()
{
    LogAddTD("[DoppelgangerTmpInfo] +--ã¦¬¬");

    for(int i = 0; i < 3; ++i )
    {
        if ( this->middle_treasure_box_index_[i] == -1 )
        {
            LogAddTD("[DoppelgangerTmpInfo] +--ã¦¬¬");
        }
        else
        {
            OBJECTSTRUCT* lpObj = &gObj[this->middle_treasure_box_index_[i]];
            LogAddTD("[DoppelgangerTmpInfo] +--ã¦¬¬",
                lpObj->m_Index, lpObj->Class, lpObj->Connected,
                lpObj->MapNumber, lpObj->DieRegen, lpObj->Live,
                lpObj->m_State);
        }
    }

    LogAddTD("[DoppelgangerTmpInfo] +ã+-¦¬¬");

    if ( this->last_treasure_box_index_ == -1 )
    {
        LogAddTD("[DoppelgangerTmpInfo] +ã+-¦¬¬");
    }
    else
    {
        OBJECTSTRUCT* lpObj = &gObj[last_treasure_box_index_];
        LogAddTD(
            "[DoppelgangerTmpInfo] +ã+-¦¬¬",
            lpObj->m_Index, lpObj->Class, lpObj->Connected,
            lpObj->MapNumber, lpObj->DieRegen, lpObj->Live,
            lpObj->m_State);
    }
}

int CDoppelganger::ChangeUserIndex(int ex_user_index, int current_user_index)
{

    EnterCriticalSection(&this->user_crit_section_);

    for( int i = 0; i < 5; ++i )
    {
        if( this->user_info_[i].IsUser()/* && gObj[ex_user_index].Connected == 4*/ && this->user_info_[i].object_index == ex_user_index )
        {
			
            this->user_info_[i].object_index = current_user_index;
            LeaveCriticalSection(&this->user_crit_section_);
            return 1;
        }
    }
    LeaveCriticalSection(&this->user_crit_section_);
    return 0;
}

void CDoppelganger::SendDoppelgangerStateToSpecificUser(int user_index, 
                                                        char state)
{
    if ( OBJMAX_RANGE(user_index) )
    {
        PMSG_DOPPELGANGER_STATE state_msg;

        PHeadSubSetB((LPBYTE)&state_msg, 0xBFu, 0x10u, sizeof(state_msg));
        state_msg.state = state;

        for( int i = 0; i < 5; ++i )
        {
            if( user_info_[i].IsUser() && gObj[user_index].Connected == 3 )
            {
                DataSend(user_index, (LPBYTE)&state_msg, state_msg.h.size);
                return;
            }
        }
    }
}

int CDoppelganger::GetPlayUserCountRightNow()
{
    int really_play_user_count = 0;
    for( int i = 0; i < MAX_PARTYUSER; ++i )
    {
        if( this->user_info_[i].IsUser() )
        {
            if( gObj[this->user_info_[i].object_index].Connected == 3 )
                ++really_play_user_count;
        }
    }

    return really_play_user_count;
}

