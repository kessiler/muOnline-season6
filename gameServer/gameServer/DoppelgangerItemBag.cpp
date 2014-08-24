// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "DoppelgangerItemBag.h"


#include "logproc.h"
//#include "dirpath.h"
#include "../common/ReadScript.h"
#include "user.h"
#include "ItemBagEx.h"
#include "GameMain.h"
#include "LargeRand.h"
#include "DSProtocol.h"

CDoppelgangerItemBag::CDoppelgangerItemBag(void)
{
}


CDoppelgangerItemBag::~CDoppelgangerItemBag(void)
{
}

void CDoppelgangerItemBag::DopplegangerItemBagInit()
{
    drop_item_groups_.clear();
}

void CDoppelgangerItemBag::LoadDoppelgangerItemBag()
{
    char script_file[MAX_PATH+1] = {0};

    strcpy(script_file, "..\\data\\Event\\DoppelgangerItemBag.txt");

    this->loaded_ = false;
    SMDFile = fopen(script_file, "r");

    if( !SMDFile )
    {
        LogAdd(lMsg.Get(453), script_file);
        return;
    }

    int droprate_per_item_count = 0;
    int rate_count = 0;
    int count = 0;

    while(1)
    {
        SMDToken token = GetToken();

        if ( token == SMDToken::END )
            break;

        if ( token != SMDToken::NUMBER )
            continue;

        int type = TokenNumber;

        if(type == 0)
        {
            count = 0;
            while ( 1 )
            {
                token = GetToken();

                if ( token == SMDToken::NAME && strcmp("end", TokenString) == 0 )
                   break;

                DOPPELGANGER_DROP_ITEM_GROUP_INFO itemgroup_info;

                itemgroup_info.number = count;
                itemgroup_info.option1_rate = TokenNumber;
                GetToken();
                itemgroup_info.option2_rate = TokenNumber;
                GetToken();
                itemgroup_info.option3_rate = TokenNumber;
                GetToken();
                itemgroup_info.excellent_option_rate = TokenNumber;
                drop_item_groups_.push_back(itemgroup_info);

                ++count;
            }        
        }
        else if(type == 1)
        {
            while ( 1 )
            {
                token = GetToken();

                if ( token == SMDToken::NAME && strcmp("end", TokenString) == 0 )
                    break;

                this->option3_weights_[0] = TokenNumber;
                GetToken();
                this->option3_weights_[1] = TokenNumber;
                GetToken();
                this->option3_weights_[2] = TokenNumber;
                GetToken();
                this->option3_weights_[3] = TokenNumber;

            }
        }
        else if(type == 2)
        {
            while ( 1 )
            {
                token = GetToken();

                if ( token == SMDToken::NAME && strcmp("end", TokenString) == 0 )
                    break;

                this->excellent_option_count_weights_[0] = TokenNumber;
                GetToken();
                this->excellent_option_count_weights_[1] = TokenNumber;
            }
        }
        else if(type == 3)
        {
            while ( 1 )
            {
                token = GetToken();

                if ( token == SMDToken::NAME && strcmp("end", TokenString) == 0 )
                    break;

                this->excellent_option_weights_[0] = TokenNumber;
                GetToken();
                this->excellent_option_weights_[1] = TokenNumber;
                GetToken();
                this->excellent_option_weights_[2] = TokenNumber;
                GetToken();
                this->excellent_option_weights_[3] = TokenNumber;
                GetToken();
                this->excellent_option_weights_[4] = TokenNumber;
                GetToken();
                this->excellent_option_weights_[5] = TokenNumber;
            }
        }
		else
		{

			int group_index = type - 4;

			if ( drop_item_groups_.size() <= group_index )
			{
				LogAddTD("Load Unknown Type : DoppelgangerItemBag.txt");
				return;
			}
       
			while ( 1 )
			{
				token = GetToken();

				if ( token == SMDToken::NAME && strcmp("end", TokenString) == 0 )
					break;

				CItemBagAttrEx itembag_attr;
				itembag_attr.m_type = TokenNumber;
				GetToken();
				itembag_attr.m_index = TokenNumber;
				GetToken();
				itembag_attr.m_minLevel = TokenNumber;
				GetToken();
				itembag_attr.m_maxLevel = TokenNumber;
				GetToken();
				itembag_attr.m_isskill = TokenNumber;
				GetToken();
				itembag_attr.m_isluck = TokenNumber;
				GetToken();
				itembag_attr.m_isoption = TokenNumber;
				GetToken();
				itembag_attr.m_isexitem = TokenNumber;


				drop_item_groups_[group_index].itembag_attributes.push_back(itembag_attr);
			}
		}
    }

    this->loaded_ = true;
    return;
}

bool CDoppelgangerItemBag::DoppelgangerItemDrop(OBJECTSTRUCT *lpObj, 
                                                    int itembag_index, 
                                                    int droprate, 
                                                    BYTE flag)
{
    if( !this->loaded_ )
        return false;

    bool should_item_drop = true;

    int tmp_itembag_index = itembag_index - 4;

    if( itembag_index - 4 > drop_item_groups_.size() )
        return false;

    DWORD randvalue = GetLargeRand() % 1000000;

    if( randvalue >= droprate )
        should_item_drop = 0;

    BYTE drop_pos_x = lpObj->X;
    BYTE drop_pos_y = lpObj->Y;

    if(should_item_drop)
    {
        if( !gObjGetRandomItemDropLocation(lpObj->MapNumber, 
                                            drop_pos_x, drop_pos_y,
                                            4 /*RangeX*/, 4 /*RangeY*/, 
                                            10 /*LoopCount*/) )
        {
            drop_pos_x = lpObj->X;
            drop_pos_y = lpObj->Y;
        }

        int item_index = drop_item_groups_[tmp_itembag_index].itembag_attributes.size();
        if(item_index == 0)
        {
            LogAddTD("Error : DOPPELGANGER ItemBag is count 0");
            return false;
        }

        float item_durability = 0.0;

        CItemBagAttrEx& itembag_attr = drop_item_groups_[tmp_itembag_index].itembag_attributes[GetLargeRand() % item_index];

        int option1 = 0;
        int option2 = 0;
        int option3 = 0;
        int excellent_option_code = 0;
        int item_level = 0;

        if ( itembag_attr.m_maxLevel == itembag_attr.m_minLevel )
        {
            item_level = itembag_attr.m_maxLevel;
        }
        else
        {
            item_level = GetLargeRand() % (itembag_attr.m_maxLevel - itembag_attr.m_minLevel + 1)
                                         + itembag_attr.m_minLevel;
        }

        int item_type = ItemGetNumberMake(itembag_attr.m_type, itembag_attr.m_index);

        if( item_type == -1 )
            return false;

        if( itembag_attr.m_isskill )
        {
            if ( GetLargeRand() % 1000000 < drop_item_groups_[tmp_itembag_index].option1_rate )
                option1 = 1;
        }

        if( itembag_attr.m_isluck )
        {
            if ( GetLargeRand() % 1000000 < drop_item_groups_[tmp_itembag_index].option2_rate )
                option2 = 1;
        }

        if( itembag_attr.m_isoption )
        {
            if ( GetLargeRand() % 1000000 <  drop_item_groups_[tmp_itembag_index].option3_rate )
            {
                DWORD randvalue = GetLargeRand() % 1000000;
                if ( randvalue < this->option3_weights_[0] )
                {
                    option3 = 1;
                }
                else if ( randvalue < this->option3_weights_[1] + this->option3_weights_[0] )
                {
                    option3 = 2;
                }
                else if ( randvalue < this->option3_weights_[2] + this->option3_weights_[1] + this->option3_weights_[0] )
                {
                    option3 = 3;
                }
                else if ( randvalue < (this->option3_weights_[3] + this->option3_weights_[2]
                                       + this->option3_weights_[1] + this->option3_weights_[0]) )
                {
                    option3 = 4;
                }
            }
        }

        if( itembag_attr.m_isexitem > 0 )
        {
            bool should_excellent_item_drop = false;
            if( itembag_attr.m_isexitem == 2 )
            {
                should_excellent_item_drop = true;
            }
            else
            {
                if( GetLargeRand() % 1000000 < drop_item_groups_[tmp_itembag_index].excellent_option_rate )
                    should_excellent_item_drop = true;
            }

            if( should_excellent_item_drop )
            {
                int excellent_option_count = 0;
                DWORD randvalue = GetLargeRand() % 1000000;
                if( randvalue < this->excellent_option_count_weights_[0] )
                {
                    excellent_option_count = 1;
                }

                else if( randvalue < excellent_option_count_weights_[1] 
                                           + excellent_option_count_weights_[0] )
                {
                   excellent_option_count = 2;
                }

                int loop_count = 50;
                while( loop_count-- )
                {
                    if( excellent_option_count <= 0 )
                        break;

                    int excellent_option_index = rand() % 6;
                    if( (excellent_option_code & (1 << excellent_option_index)) == 0) 

                    {
                        if ( (GetLargeRand() % 1000000) < excellent_option_count_weights_[excellent_option_index] )
                        {
                            excellent_option_code |= (1 << excellent_option_index);
                            --excellent_option_count;
                        }
                    }
                }

                if ( excellent_option_code )
                {
                    option2 = 0;
                    option1 = 1;
                    item_level = 0;
                }
            }
        }
        if ( item_type == 0x180F || item_type == 0x1C0D 
                || item_type == 0x1C0E || item_type == 0x1C16 )
        {
            option1 = 0;
            option2 = 0;
            option3 = 0;
            item_level = 0;
        }

        if ( item_type == 0x1A00
            || item_type == 0x1A01
            || item_type == 0x1A02
            || item_type == 0x1A08
            || item_type == 0x1A09
            || item_type == 0x1A0C
            || item_type == 0x1A0D
            || item_type == 0x1A26
            || item_type == 0x1A27
            || item_type == 0x1A28
            || item_type == 0x1A29 )
        {
            item_level = 0;
            item_durability = 255.0;
        }

        ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, drop_pos_x, drop_pos_y,
                                item_type, item_level, item_durability,
                                option1, option2, option3, lpObj->m_Index,
                                excellent_option_code, 0);
        LogAddTD("[Doppelganger] Drop Item(%d) [%s][%s] : (%d)(%d/%d) "
                  "Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
                    flag, lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
                    drop_pos_x, drop_pos_y, ItemAttribute[item_type].Name,
                    item_type, item_level, option1, option2, option3, excellent_option_code);
    }
    else
    {
        DWORD randvalue = rand() % 3 + 2;
        for (int item_count = 0; item_count < randvalue; ++item_count )
        {
            drop_pos_x = lpObj->X;
            drop_pos_y = lpObj->Y;

            if ( !gObjGetRandomItemDropLocation( lpObj->MapNumber,
                                          drop_pos_x, drop_pos_y,
                                          4 /*RangeX*/, 4 /*RangeY*/, 10 /*LoopCount*/) )
            {
                drop_pos_x = lpObj->X;
                drop_pos_y = lpObj->Y;
            }

            MapC[lpObj->MapNumber].MoneyItemDrop(10000, drop_pos_x, drop_pos_y);
            LogAddTD("[Doppelganger] Drop Zen(%d) [%s][%s] [%d Zen]", 
                        flag, lpObj->AccountID, lpObj->Name, 10000);
        }
    }

    return true;
}