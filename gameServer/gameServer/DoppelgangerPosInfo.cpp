// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "DoppelgangerPosInfo.h"


#include "logproc.h"
//#include "dirpath.h"
#include "..\common\ReadScript.h"
#include "GameMain.h"

CDoppelgangerPosInfo::CDoppelgangerPosInfo(void):
loaded_(false)
{
}


CDoppelgangerPosInfo::~CDoppelgangerPosInfo(void)
{
}

void CDoppelgangerPosInfo::LoadDoppelgangerPosInfo()
{
	char script_file[MAX_PATH+1] = {0};

	strcpy(script_file, "..\\data\\Event\\DoppelgangerPosition.dat");


	this->loaded_ = false;

	SMDFile = fopen(script_file, "r");

	if ( !SMDFile )
	{
		LogAdd(lMsg.Get(453), script_file);
		return;
	}

    
	while(1)
	{
		SMDToken token = GetToken();

		if (token == SMDToken::END)
			break;

        if(token != SMDToken::NUMBER)
            continue;

        int type = TokenNumber;

        if(type == 0)
        {
            for(int count=0; count < kMaxMonsterType; ++count)
            {
                if(GetToken() == SMDToken::NAME && strcmp("end", TokenString) == 0)
                    break;

                this->posinfo_[type][count][0] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][1] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][2] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][3] = TokenNumber;

            }
        }
        else if(type == 1)
        {
            for(int count=0; count < kMaxMonsterType; ++count)
            {
                if(GetToken() == SMDToken::NAME && strcmp("end", TokenString) == 0)
                    break;

                this->posinfo_[type][count][0] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][1] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][2] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][3] = TokenNumber;

            }
        }
        else if(type == 2)
        {
            for(int count=0; count < kMaxMonsterType; ++count)
            {
                if(GetToken() == SMDToken::NAME && strcmp("end", TokenString) == 0)
                    break;

                this->posinfo_[type][count][0] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][1] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][2] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][3] = TokenNumber;

            }
        }
        else if(type == 3)
        {
            for(int count=0; count < kMaxMonsterType; ++count)
            {
                if(GetToken() == SMDToken::NAME && strcmp("end", TokenString) == 0)
                    break;

                this->posinfo_[type][count][0] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][1] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][2] = TokenNumber;
                GetToken();
                this->posinfo_[type][count][3] = TokenNumber;

            }
        }
        else if(type == 4)
        {
            for(int count=0; count < kMaxMonsterType; ++count)
            {
                if(GetToken() == SMDToken::NAME && strcmp("end", TokenString) == 0)
                    break;

                this->start_posinfo_[count][0] = TokenNumber;
                GetToken();
                this->start_posinfo_[count][1] = TokenNumber;
                GetToken();
                this->start_posinfo_[count][2] = TokenNumber;
                GetToken();
                this->start_posinfo_[count][3] = TokenNumber;

            }
        }
		else
		{
			LogAddTD("Load Unknown Type : DoppelgangerPosInfo.txt");
			break;
		}
	}

	fclose(SMDFile);

    this->loaded_ = true;
}

void CDoppelgangerPosInfo::GetCenterPosition(int map_number, int pos_index, 
                                                BYTE& pos_x, BYTE& pos_y)
{
    int map_type = map_number - kStartEventMapNumber;

    BYTE right_x = posinfo_[map_type][pos_index][2];
    BYTE left_x = posinfo_[map_type][pos_index][0];

    pos_x = (right_x - left_x)/2 + left_x;

    BYTE bottom_y = posinfo_[map_type][pos_index][3];
    BYTE top_y = posinfo_[map_type][pos_index][1];

    pos_y = (bottom_y - top_y)/2 + top_y;
}

int CDoppelgangerPosInfo::GetPosIndex(int map_number, BYTE pos_x, BYTE pos_y)
{
    int pos_index = 0;
    int map_type = map_number - kStartEventMapNumber;
    for (int i = 0; i < kMaxMonsterType; ++i )
    {
        if ( pos_x > posinfo_[map_type][i][0]  && pos_x <= posinfo_[map_type][i][2]
            && pos_y > posinfo_[map_type][i][1] && pos_y <= posinfo_[map_type][i][3] )
        {
            pos_index = i;
            break;
        }
    }

    return pos_index;
}


void CDoppelgangerPosInfo::GetStartMapAttr(int map_number, BYTE &left_x, BYTE &top_y, 
                                                           BYTE &right_x, BYTE &bottom_y)
{
    int map_type = map_number - kStartEventMapNumber;

    left_x = this->start_posinfo_[map_type][0];
    top_y = this->start_posinfo_[map_type][1];
    right_x = this->start_posinfo_[map_type][2];
    bottom_y = this->start_posinfo_[map_type][3];
}

bool CDoppelgangerPosInfo::CheckStartMapTile( int map_number, 
                                                BYTE pos_x, BYTE pos_y )
{
    int map_type = map_number - kStartEventMapNumber;

    if(pos_x > start_posinfo_[map_type][0] 
        && pos_x < start_posinfo_[map_type][2]
        && pos_y > start_posinfo_[map_type][1] 
        && pos_y < start_posinfo_[map_type][3])
    {
        return false;
    }
    else
    {
        return true;
    }
}