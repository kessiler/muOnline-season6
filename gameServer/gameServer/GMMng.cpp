#include "stdafx.h"
#include "GMMng.h"
#include "LogToFile.h"
#include "logproc.h"
#include "GameMain.h"
#include "protocol.h"
#include "..\common\winutil.h"
#include "MoveCommand.h"
#include "GuildClass.h"
#include "TNotice.h"
#include "BattleSoccerManager.h"
#include "giocp.h"
#include "Kanturu.h"
#include "SkillAdditionInfo.h"
#include "DSProtocol.h"
#include "BuffEffectSlot.h"
#include "MasterLevelSystem.h"
#include "ObjCalCharacter.h"
#include "MasterLevelSkillTreeSystem.h"
#include "MapClass.h"
#include "MonsterAttr.h"
#include "..\common\SetItemOption.h"
#include "ItemAddOption.h"
#include "DarkSpirit.h"
#include "IllusionTempleEvent.h"
#include "Raklion.h"
#include "RaklionBattleUserMng.h"
#include "ChaosCard.h"
#include "PCBangPointSystem.h"
#include "..\include\readscript.h"
#include "MapServerManager.h"
#include "User.h"
#include "ObjUseSkill.h"
#include "QuestUtil.h"
#include "Protocol.h"
#include "EDSProtocol.h"
#ifdef PERIOD
#include "PeriodItemEx.h"
#endif

#ifdef IMPERIAL
#include "ImperialGuardian.h"
#endif

#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
#include "ConnectEx.h"
#endif

#include "OfflineTrade.h"

#ifdef OFFEXP
#include "OffExp.h"
#endif

CLogToFile g_PostLog("Post", ".\\LOG\\Chat", TRUE);

CGMMng cManager;

CGMMng::CGMMng()
{
	this->ManagerInit();
}

CGMMng::~CGMMng()
{
	return;
}

void CGMMng::Init()
{
	this->cCommand.Init();
	this->LoadCommands(gDirPath.GetNewPath("Custom\\AccessData.ini"));
	this->WatchTargetIndex = -1;
}

void CGMMng::LoadCommands(LPSTR szFileName)
{
	if ( (SMDFile = fopen(szFileName, "r")) == NULL )	//ok
	{
		return;
	}

	SMDToken Token;
	int iType = 0;
	int SubNameCount = 0;
	int CommandsCount = 0;

	while ( true )
	{
		Token = GetToken();

		if (Token == END )
		{
			break;
		}

		iType = TokenNumber;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
			{
				break;
			}

			if ( iType == 0 )
			{
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				int CommandIndex;
				char szCommandName[30];
				int CommandRights;
				// ----
				CommandIndex = TokenNumber;
				// ----
				Token = GetToken();
				memcpy(szCommandName, TokenString, sizeof(szCommandName));
				// ----
				Token = GetToken();
				CommandRights = TokenNumber;
				// ----
				this->cCommand.Add(szCommandName, CommandIndex, CommandRights, true);
				// ----
				CommandsCount++;
			}
		}
	}
	// ----
	fclose(SMDFile);
}

char * CGMMng::GetSubName(int auth_code)
{
	switch(auth_code)
	{
	case 1:
		return "User";
	case 2:
		return "Admin";
	case 32:
		return "GameMaster";
		
	}

	return NULL;
}

void CGMMng::ManagerInit()
{
	for ( int n = 0;n<MAX_GAME_MASTER ; n++ )
	{
		this->ManagerIndex[n] = -1;
	}
}


int CGMMng::ManagerAdd(char* name, int aIndex)
{
	for ( int n=0 ; n<MAX_GAME_MASTER ; n++ )
	{
		if ( this->ManagerIndex[n] == -1 )
		{
			strcpy(this->szManagerName[n], name);
			this->ManagerIndex[n] = aIndex;
			return n;
		}
	}

	return -1;
}


void CGMMng::ManagerDel(char* name)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++)
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			if ( strcmp(this->szManagerName[n], name) == 0 )
			{
				this->ManagerIndex[n] = -1;
				//break;
				return; //Original Code (GS 0.65)
			}
		}
	}
}


void CGMMng::ManagerSendData(LPSTR szMsg, int size)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			GCServerMsgStringSend(szMsg, this->ManagerIndex[n], 0);
		}
	}
}

void CGMMng::DataSend(LPBYTE szMsg, int size)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			::DataSend(this->ManagerIndex[n], szMsg, size);
		}
	}
}


void CGMMng::BattleInfoSend(char* Name1, BYTE score1, char* Name2, BYTE score2)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			GCGoalSend(this->ManagerIndex[n], Name1, score1, Name2, score2);
		}
	}
}


int CGMMng::GetCmd(char* szCmd)
{
	for ( int n	=0; n< MAX_GM_COMMAND ; n++ )
	{
		if ( this->cCommand.nCmdCode[n] > 0 )
		{
			if ( stricmp(szCmd, this->cCommand.szCmd[n]) == 0 )
			{
				return this->cCommand.nCmdCode[n];
			}
		}
	}

	return 0;
}

// 005708B0 -> calls 1x from ManagementProc
BYTE CGMMng::GetData(char *szCmd,int &command_code,int &gamemaster_code)
{
	for ( int n	=0; n< MAX_GM_COMMAND ; n++ )
	{
		if ( this->cCommand.nCmdCode[n] > 0 )
		{
			if ( stricmp(szCmd, this->cCommand.szCmd[n]) == 0 )
			{
				command_code = this->cCommand.nCmdCode[n];
				gamemaster_code = this->cCommand.nGMCode[n];
				return TRUE;
			}
		}
	}

	return 0;
}

char* CGMMng::GetTokenString()
{
	//Original Code (GS 0.65)
	char seps[] = " ";
	return strtok(NULL, seps);
}

int CGMMng::GetTokenNumber()
{
	char seps[] = " ";
	char * szToken;

	szToken = strtok(NULL, seps);

	if( szToken != NULL )
	{
		return atoi(szToken);
	}
	return 0;
}

BOOL CGMMng::CommandActive(int command_code)
{
	if ( this->cCommand.nCmdActive[command_code] == 1 )
	{
		return TRUE;
	}
	
	return FALSE;
}

void CGMMng::MsgYellow(int aIndex,char* szMsg,...)
{
	char szTemp[1024];
	BYTE *Packet;

	va_list pArguments;
	va_start(pArguments,szMsg);
	vsprintf(szTemp,szMsg,pArguments);
	va_end(pArguments);

	Packet = (BYTE*)malloc(200);
	memset(Packet,0x00,200);
	*Packet=0xC1;
	*(Packet+2)=0x02;

	memcpy((Packet+13),szTemp,strlen(szTemp));

	int Len=(strlen(szTemp)+0x13);
	*(Packet+1)=Len;

	::DataSend(aIndex,Packet,Len);
	free(Packet);
}

void MsgSrv(char *Sender,char *Message, int Type)
{		 
	BYTE *Packet;
	Packet = (BYTE*) malloc(200);
	memset(Packet, 0x00, 200);
	*Packet = 0xC1;
	if(Type)
		*(Packet+2) = 0x02;	  
	else				   
		*(Packet+2) = 0x00;
	memcpy((Packet+3), Sender, strlen(Sender));
	memcpy((Packet+13), Message, strlen(Message));
	int Len = (strlen(Message) + 0x13);
	*(Packet+1) = Len;
	DataSendAll(Packet, Len);
	free (Packet);
}

void MessageSendEx(int Type, char * Sender, char * Message, ...)
{
	char szTemp[1024];
	va_list pArguments;
	va_start(pArguments, Message);
	vsprintf(szTemp, Message, pArguments);
	va_end(pArguments);
	// ----
	CHAT_WHISPER_EX pMessage;
	memcpy(pMessage.Name, Sender, 10);
	memcpy(pMessage.Message, szTemp, 90);
	pMessage.Type = Type;
	pMessage.Head.set((LPBYTE)&pMessage, 2, sizeof(CHAT_WHISPER_EX));
	DataSendAll((LPBYTE)&pMessage, pMessage.Head.size);
}

void CGMMng::MessageAll(int Type, int Type2, char *Sender, char *Msg,...)
{					  
	char Messages[1024];
	ZeroMemory(Messages, sizeof(Messages));
	char Temp[255]; 
	strcpy(Temp,Msg);
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages, Temp, pArguments1);
	va_end(pArguments1); 

	if(Type == 2)
	{
		MsgSrv(Sender, Messages, Type2);
	}
	else
	{
		for(int i = OBJMAX - OBJMAXUSER; i <= OBJMAX; i++)
		{  	 			
			LPOBJ lpObj = &gObj[i];
			if(lpObj->Connected < PLAYER_PLAYING)	continue;
			GCServerMsgStringSend(Messages, i, Type);
		}
	}
}

int CGMMng::ManagementProc(LPOBJ lpObj, char* szCmd, int aIndex) //00570A00
{
	//Original Code (GS 0.65)
	char seps[] = " ";
	char *szCmdToken;
	char string[256];
	char szId[20];
	char *pId = szId;

	int len= strlen(szCmd);
	if( len < 1 || len > 250 ) return FALSE;

	memset(szId, 0, 20);

	strcpy(string, szCmd);
	
	szCmdToken = strtok(string, seps);
	int command_number = GetCmd(szCmdToken);

	int command_code,gamemaster_code;//Season 4.5 addon

	if(this->GetData(szCmdToken,command_code,gamemaster_code) == FALSE)//Season 4.5 addon
	{
		return FALSE;
	}
	
	if( lpObj->Authority < gamemaster_code )
	{
		return FALSE;
	}

	if( this->CommandActive(command_code) == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(3425),lpObj->m_Index, 1);
		return FALSE;
	}

	switch ( command_code )
	{
	case COMMAND_MAKEBLESS://Identical
		{
			for(int i = 0; i < 15; i++)
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(14,13), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
		}
		break;
	case COMMAND_MAKESOUL://Identical
		{
			for(int i = 0; i < 15; i++)
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(14,14), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
		}
		break;
	case COMMAND_RANDOMSETITEM://Identical
		{
			for(int i = 0; i < 20; i++)
			{
				MakeRandomSetItem(aIndex); 
			}
		}
		break;
	case COMMAND_SETITEM://Identical
		{
			char * szSetItemName = this->GetTokenString();

			if ( szSetItemName == NULL )
			{
				return 0;
			}

			int iLevel;
			int iOption1 = 0;
			int iOption2 = 0;
			int iOption3 = 0;
			BYTE NewOption = 0;
			int iDur = BYTE(-1);

			iLevel = this->GetTokenNumber();
			iDur = this->GetTokenNumber();
			iOption1 = this->GetTokenNumber();
			iOption2 = this->GetTokenNumber();

			int iOptionLevel = this->GetTokenNumber();
			int iOptionType = this->GetTokenNumber();

			int iSetOption;

			if(iOptionLevel == 4)
			{
				iOption3 = 1;
			}
			else if(iOptionLevel == 8)
			{
				iOption3 = 2;
			}
			else if(iOptionLevel == 12)
			{
				iOption3 = 3;
			}
			else if(iOptionLevel == 16)
			{
				iOption3 = 4;
			}

			for(int i = 0; i < MAX_ITEMS; i++)
			{
				if(strcmp(gSetItemOption.GetSetOptionName(i, 1), szSetItemName) == 0)
				{
					iSetOption = 1;

					if(iOptionType == 0)
					{
						if( rand()%100 < 80 ) 
								iSetOption |= 4;
							else 
								iSetOption |= 8;
					}
					else if(iOptionType == 1)
					{
						iSetOption |= 4;
					}
					else
					{
						iSetOption |= 8;
					}

					ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, i, iLevel, iDur, iOption1, iOption2, iOption3, -1, NewOption, iSetOption); 
				}

				if(strcmp(gSetItemOption.GetSetOptionName(i, 2), szSetItemName) == 0)
				{
					iSetOption = 2;

					if(iOptionType == 0)
					{
						if( rand()%100 < 80 ) 
								iSetOption |= 4;
							else 
								iSetOption |= 8;
					}
					else if(iOptionType == 1)
					{
						iSetOption |= 4;
					}
					else
					{
						iSetOption |= 8;
					}

					ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, i, iLevel, iDur, iOption1, iOption2, iOption3, -1, NewOption, iSetOption); 
				
				}
			}
		}
		break;
	case COMMAND_RESETPK://identical
		{
			TNotice pNotice(1);
			pNotice.SendToUser(lpObj->m_Index, "PK Reset | Setting Normal Pk user");
			lpObj->m_PK_Count = 0;
			lpObj->m_PK_Level = 3;
			lpObj->m_PK_Time = 0;

			if(lpObj->PartyNumber >= 0)
			{
				gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
				gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
			}
		}
		break;
	case COMMAND_VIEWCLASSHP://Identical
		{
			int iTokenNumber = this->GetTokenNumber();

			if(iTokenNumber == 0)
			{
				TNotice pNotice(1);
				pNotice.SendToUser(lpObj->m_Index, "cannot find class by id : %d",iTokenNumber);
			}
			else
			{
				for(int n = 0; n < MAX_VIEWPORT; n++)
				{
					if(lpObj->VpPlayer[n].state != 0)
					{
						if(lpObj->VpPlayer[n].type == OBJ_MONSTER)
						{
							if ( lpObj->VpPlayer[n].number >= 0 )
							{
								LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].number];

								if ( lpTarget->Class == iTokenNumber )
								{
									TNotice pNotice(1);
									pNotice.SendToUser(lpObj->m_Index, "Info For Viewport class %d :  HP = %7.0f / %7.0f",iTokenNumber, lpTarget->Life, lpTarget->MaxLife);
								}
							}
						}
					}
				}
			}
		}
		break;
	case COMMAND_NOTICEALL://Identical
		{
			TNotice pNotice(0);
			pNotice.SendToAllUser("%s",this->GetTokenString());
		}
		break;
	case COMMAND_SETPKLEVEL://Identical
		{
			int iTokenNumber = this->GetTokenNumber();

			if(iTokenNumber == 0)
			{
				iTokenNumber = 3;
			}

			if(iTokenNumber == 1)
			{
				lpObj->m_PK_Level = 1;
			}
			else if(iTokenNumber == 2)
			{
				lpObj->m_PK_Level = 2;
			}
			else if(iTokenNumber == 3)
			{
				lpObj->m_PK_Time = 0;
				lpObj->m_PK_Level = 3;
			}
			else if(iTokenNumber == 4)
			{
				lpObj->m_PK_Level = 4;
				lpObj->m_PK_Count = 1;
			}
			else if(iTokenNumber == 5)
			{
				lpObj->m_PK_Level = 5;
				lpObj->m_PK_Count = 2;
			}
			else if(iTokenNumber == 6)
			{
				lpObj->m_PK_Level = 6;
				lpObj->m_PK_Count = 3;
			}
			else
			{
				lpObj->m_PK_Level = 3;
			}

			if(lpObj->m_PK_Level == 4)
			{
				GCServerMsgStringSend(lMsg.Get(1137), lpObj->m_Index, 1);
			}

			if(lpObj->m_PK_Level <= 6)
			{
				GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
			}

			if(lpObj->PartyNumber >= 0)
			{
				gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
				gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
			}						
		}
		break;
	case COMMAND_MAKEITEMBYNAME://
        {
            int iItemType;//5d0
            int iItemIndex;//5D4
            int iLevel;//5d8
            int iOption1 = 0;//5dc
            int iOption2 = 0;//5e0
            int iOption3 = 0;//5e4
            BYTE NewOption = 0;//5e8
            int iDur = (BYTE)255;//5ec
 
            char szText[100] = {0};
            char szItemName[20] = {0};
 
            try
			{
				memset(szItemName, 0, sizeof(szItemName));
				strcpy(szItemName, this->GetTokenString()); //local created automatically
			}
			catch(...)
			{
				wsprintf(szItemName, "Unknow");
			}
 
            int iType;//668
            int iIndex;//66c
			int i;
 
            for( i = 0; i < MAX_ITEMS; i++ )//670
            {
                if(strcmp(szItemName, ItemAttribute[i].Name) == 0)
                {
                    iType = i / 512;
                    iIndex = i - (iType << 9) ;
                    sprintf(szItemName,"[ %s ] Founded item = %d, %d",ItemAttribute[i].Name,iType,iIndex);//maybe need translation
                    break;
                }
            }
 
            if( i == MAX_ITEMS ) //failure condition
            {
                sprintf(szText, "Cannot find item by name");
                GCServerMsgStringSend(szText, lpObj->m_Index, 1);
                return FALSE;
            }
 
            GCServerMsgStringSend(szText, lpObj->m_Index, 1);
           
            iItemType = iType;//5d0
            iItemIndex = iIndex;//5d4
            iLevel = this->GetTokenNumber();//5d8
            iDur = this->GetTokenNumber();//5ec
            iOption1 = this->GetTokenNumber();//5dc
            iOption2 = this->GetTokenNumber();//5e0
            int iItemOption3 = this->GetTokenNumber();//674
       
            if(iItemType == 13 && iItemIndex == 3)
            {
                if(iItemOption3 < 8)
                {
                    iOption3 = iItemOption3;
                }
            }
            else
            {
                if(iItemOption3 == 4)
                {
                    iOption3 = 1;
                }
                else if(iItemOption3 == 8)
                {
                    iOption3 = 2;
                }
                else if(iItemOption3 == 12)
                {
                    iOption3 = 3;
                }
                else if (iItemOption3 == 16)
                {
                    iOption3 = 4;
                }
            }
 
            int EBP678 = this->GetTokenNumber();
            int EBP67C = this->GetTokenNumber();
 
            if(EBP678 != 0 )
            {
                if( EBP67C == 0)
                {
                    if((rand()%100) < 80)
                    {
                        EBP678 |= 4;
                    }
                    else
                    {
                        EBP678 |= 8;
                    }
                }
                else if (EBP67C == 1)
                {
                    EBP678 |= 4;
                }
                else
                {
                    EBP678 |= 8;
                }
            }
           
            int EBP680 = this->GetTokenNumber();
 
            if(EBP680 > 0)
            {
                NewOption |= 0x20;
            }
 
            int EBP684 = this->GetTokenNumber();
 
            if(EBP684 > 0)
            {
                NewOption |= 0x10;
            }
 
            int EBP688 = this->GetTokenNumber();
 
            if(EBP688 > 0)
            {
                NewOption |= 0x08;
            }
 
            int EBP68C = this->GetTokenNumber();
 
            if(EBP68C > 0)
            {
                NewOption |= 0x04;
            }
 
            int EBP690 = this->GetTokenNumber();
 
            if(EBP690 > 0)
            {
                NewOption |= 0x02;
            }
 
            int EBP694 = this->GetTokenNumber();
 
            if(EBP694 > 0)
            {
                NewOption |= 0x01;
            }
 
           
            if( iItemType >= 0 && iItemType < 512 &&
                iItemIndex >= 0 && iItemIndex < 512)
            {
                int iItemNumber = ItemGetNumberMake(iItemType,iItemIndex);
               
                if( iItemNumber == ITEMGET(0,19) ||
                    iItemNumber == ITEMGET(4,18) ||
                    iItemNumber == ITEMGET(5,10) ||
                    iItemNumber == ITEMGET(2,13))
                {
                    NewOption = 63;
                }
               
                if(iItemNumber == ITEMGET(13,4) || iItemNumber ==  ITEMGET(13,5))
                {
                    PetItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, iItemNumber, iLevel, iDur ,iOption1,  iOption2, iOption3, -1, NewOption, EBP678);
                }
                else
                {
                    ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, iItemNumber, iLevel, iDur ,iOption1,  iOption2, iOption3, -1, NewOption, EBP678);
                }
            }
        }
        break; 
	case COMMAND_FINDITEMBYNAME:
		{
			char szText[100] = {0};//6fc
			char szItemName[20] = {0};//710

			strcpy(szItemName, this->GetTokenString());
			int i;
			for( i = 0; i < MAX_ITEMS; i++)//714
			{
				if(strcmp(szItemName, ItemAttribute[i].Name) == 0)
				{
					int iType = i / 512;//718
					int iIndex = i - (iType << 9) ;//71c
					sprintf(szText,"[ %s ] Founded item = %d, %d",ItemAttribute[i].Name,iType,iIndex);
					break;
				}
			}

			if(i == MAX_ITEMS) //failure condition
			{
				sprintf(szText, "Cannot find item by name");
			}

			GCServerMsgStringSend(szText, lpObj->m_Index, 1);
		}
		break;
	case COMMAND_SETSTATS:
		{
			int iClass = lpObj->Class;//720
			int iStrength = this->GetTokenNumber();//ebp724
			int iDexterity = this->GetTokenNumber();//ebp728
			int iVitality = this->GetTokenNumber();//ebp72C
			int iEnergy = this->GetTokenNumber();//ebp730
			int iStandartPoints;//ebp734
			int iAllPoints = iStrength + iDexterity + iVitality + iEnergy;//ebp738

			if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA)
			{
				iStandartPoints = 7 * (lpObj->Level -1 ) +
						 DCInfo.DefClass[iClass].Strength +
						 DCInfo.DefClass[iClass].Dexterity +
						 DCInfo.DefClass[iClass].Vitality +
						 DCInfo.DefClass[iClass].Energy;
			}
			else
			{
				iStandartPoints = 5 * (lpObj->Level -1 ) +
						 DCInfo.DefClass[iClass].Strength +
						 DCInfo.DefClass[iClass].Dexterity +
						 DCInfo.DefClass[iClass].Vitality +
						 DCInfo.DefClass[iClass].Energy; 
			}

			if( iStrength	< DCInfo.DefClass[iClass].Strength	||
				iDexterity  < DCInfo.DefClass[iClass].Dexterity ||
				iVitality	< DCInfo.DefClass[iClass].Vitality	||
				iEnergy		< DCInfo.DefClass[iClass].Energy)
			{
				
				GCServerMsgStringSend("You cannot set stats smaller standart", lpObj->m_Index, 1);
				return FALSE;
			}

			if ( iAllPoints > iStandartPoints )
            {
				GCServerMsgStringSend("You cannot set stats bigger standart", lpObj->m_Index, 1);
				return FALSE;
            }
			
			lpObj->ChangeUP = 0;
			lpObj->Experience = 0;

			lpObj->Strength = iStrength;
			lpObj->Dexterity = iDexterity;
			lpObj->Vitality = iVitality;
			lpObj->Energy = iEnergy;
		
			lpObj->Life = (iVitality - DCInfo.DefClass[iClass].Vitality) * DCInfo.DefClass[iClass].LevelLife + DCInfo.DefClass[iClass].Life;
			lpObj->MaxLife = (iVitality - DCInfo.DefClass[iClass].Vitality) * DCInfo.DefClass[iClass].LevelLife + DCInfo.DefClass[iClass].MaxLife;
			lpObj->Mana = (iVitality - DCInfo.DefClass[iClass].Energy) * DCInfo.DefClass[iClass].LevelMana + DCInfo.DefClass[iClass].Mana;
			lpObj->MaxMana = (iVitality - DCInfo.DefClass[iClass].Energy) * DCInfo.DefClass[iClass].LevelMana + DCInfo.DefClass[iClass].MaxMana;
	
			lpObj->LevelUpPoint = iStandartPoints - iAllPoints;
			
			GCServerMsgStringSend("PointsUpdated!!", lpObj->m_Index, 1);
			
			gObjCloseSet(lpObj->m_Index, 1);
		}
		break;
	case COMMAND_SETSTATSNULLEXP:
		{
			int iClass = lpObj->Class;//73C
			int iStrength = this->GetTokenNumber();//740
			int iDexterity = this->GetTokenNumber();//744
			int iVitality = this->GetTokenNumber();//748
			int iEnergy = this->GetTokenNumber();//74C
			
			lpObj->ChangeUP = 0;
			lpObj->Experience = 0;
			lpObj->Strength = iStrength;
			lpObj->Dexterity = iDexterity;
			lpObj->Vitality = iVitality;
			lpObj->Energy = iEnergy;


			lpObj->Life = (iVitality - DCInfo.DefClass[iClass].Vitality) * DCInfo.DefClass[iClass].LevelLife + DCInfo.DefClass[iClass].Life;
			lpObj->MaxLife = (iVitality - DCInfo.DefClass[iClass].Vitality) * DCInfo.DefClass[iClass].LevelLife + DCInfo.DefClass[iClass].MaxLife;
			lpObj->Mana = (iVitality - DCInfo.DefClass[iClass].Energy) * DCInfo.DefClass[iClass].LevelMana + DCInfo.DefClass[iClass].Mana;
			lpObj->MaxMana = (iVitality - DCInfo.DefClass[iClass].Energy) * DCInfo.DefClass[iClass].LevelMana + DCInfo.DefClass[iClass].MaxMana;
	
			GCServerMsgStringSend("Modi.Stat Complete", lpObj->m_Index, 1);
			
			gObjCloseSet(lpObj->m_Index, 1);
		}
		break;
	case COMMAND_UPDBCLASS:
		{
			int iDbClass = lpObj->DbClass;//750
			
			if(iDbClass == 0)
			{
				lpObj->DbClass = 1;
			}
			else if(iDbClass == 16)
			{
				lpObj->DbClass = 17;
			}
			else if(iDbClass == 32)
			{
				lpObj->DbClass = 33;
			}
			else if(iDbClass == 80)
			{
				lpObj->DbClass = 81;
			}

			GCServerMsgStringSend("DBCLass is modifyed", lpObj->m_Index, 1);
			
			gObjCloseSet(lpObj->m_Index, 1);
		}
		break;
	case COMMAND_UPCLASS:
		{
			if( lpObj->Level >= 320 )
			{
				lpObj->ThirdChangeUp = 1;
				lpObj->DbClass |= 2;
				gObjCloseSet(lpObj->m_Index, 1);
			}
		}
		break;
	case COMMAND_RESETCLASS:
		{
			lpObj->ChangeUP = 0;
			lpObj->ThirdChangeUp = 0;

			switch(lpObj->DbClass)
			{
			case 1:
            case 3:
                lpObj->DbClass = 0;
				break;
            case 17:
            case 19:
				lpObj->DbClass = 16;
				break;
            case 33:
			case 35:
                lpObj->DbClass = 32;
                break;
			case 50:
                lpObj->DbClass = 48;
                break;
			case 66:
                lpObj->DbClass = 64;
                break;
			case 98:
				lpObj->DbClass = 96;
				break;
			case 81:
			case 83:
                lpObj->DbClass = 80;
                break;
			}

			GCServerMsgStringSend("Reset class complete", lpObj->m_Index, 1);
			gObjCloseSet(lpObj->m_Index, 1);
		}
		break;
	case COMMAND_MAKEITEMS:
		{
			int iType = this->GetTokenNumber();//75C
			int iIndex = this->GetTokenNumber();//760
			int iItemCount = this->GetTokenNumber();//764

			if(iItemCount <= 0)
			{
				iItemCount = 1;
			}
			else if(iItemCount > 50)
			{
				iItemCount = 50;
			}

			for(int i = 0; i <iItemCount; i++)//678
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(iType,iIndex), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
		}
		break;
	case COMMAND_SETMONEY:
		{
			int iMoney = this->GetTokenNumber();
			gObj[aIndex].Money = iMoney;
			GCMoneySend(aIndex, gObj[aIndex].Money);
		}
		break;
	case COMMAND_MODIFYFENDUR:
		{
			int iTokenNumber = this->GetTokenNumber();

			MsgOutput(lpObj->m_Index, "[Modify Standart Fenrir dur] %d -> %d", g_iFenrirDefaultMaxDurSmall, iTokenNumber);

			g_iFenrirDefaultMaxDurSmall = iTokenNumber;
		}
		break;
	case COMMAND_MODIFYELFFENDUR: //??
		{
			int iTokenNumber = this->GetTokenNumber();

			MsgOutput(lpObj->m_Index, "[Modify Elf Fenrir dur] %d -> %d", g_iFenrirElfMaxDurSmall, iTokenNumber);

			g_iFenrirElfMaxDurSmall = iTokenNumber;
		}
		break;
	case COMMAND_MODIFYSD: //??
		{
			int iShield = this->GetTokenNumber();

			MsgOutput(lpObj->m_Index, "[Shield Gage] %d -> %d", lpObj->iShield, iShield);

			if(iShield > (lpObj->iMaxShield + lpObj->iAddShield))
			{
				iShield = lpObj->iMaxShield + lpObj->iAddShield;
			}

			lpObj->iShield = iShield;

			GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		}
		break;
	case COMMAND_SHOWHPSD: //??
		{
			MsgOutput(lpObj->m_Index, "HP:%7.1f/%7.1f , SD:%d/%d", lpObj->Life, lpObj->AddLife + lpObj->MaxLife, lpObj->iShield, lpObj->iMaxShield + lpObj->iAddShield);
		}
		break;
	case COMMAND_SETOPTIONEFFECT: //??
		{
			int iEffectType = this->GetTokenNumber();
			int iEffectValue = this->GetTokenNumber();
			int iEffectDur = this->GetTokenNumber();

			g_ItemAddOption.SetManualItemEffect(lpObj, iEffectType, iEffectValue, iEffectDur);

			GCReFillSend(lpObj->m_Index, lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
			GCManaSend(lpObj->m_Index, lpObj->AddMana + lpObj->MaxMana, -2, 0, lpObj->MaxBP + lpObj->AddBP);
		}
		break;
	case COMMAND_MAKEPRIZE:
		{
			for(int i = 0; i < 5; i++)
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(12,15), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
			for(int i = 0; i < 5; i++)
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(14,13), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
			for(int i = 0; i < 5; i++)
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(14,14), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
		}
		break;
	case COMMAND_MAKEITEM:
		{
			int ItemType;
			int ItemIndex;
			int EBP798;
			int EBP79C = 0;
			int EBP7A0 = 0;
			int EBP7A4 = 0;
			BYTE EBP7A8 = 0;
			int iDur = (BYTE)-1;//7AC
			int EBP7B0;

			ItemType  = this->GetTokenNumber();//790
			ItemIndex  = this->GetTokenNumber();//794
			EBP798 = this->GetTokenNumber();
			iDur = this->GetTokenNumber();
			EBP79C = this->GetTokenNumber();
			EBP7A0 = this->GetTokenNumber();
			EBP7B0 = this->GetTokenNumber();

			if(ItemType == 13 && ItemIndex == 3)
			{
				if(	EBP7B0 < 8)
				{
					EBP7A4 = EBP7B0;
				}
			}
			else
			{
				if(EBP7B0 == 4)
				{
					EBP7A4 = 1;
				}
				else if(EBP7B0 == 8)
				{
					EBP7A4 = 2;
				}
				else if(EBP7B0 == 12)
				{
					EBP7A4 = 3;
				}
				else if(EBP7B0 == 16)
				{
					EBP7A4 = 4;
				}
			}

			int EBP7B4 = this->GetTokenNumber();
			int EBP7B8 = this->GetTokenNumber();
			
			if(EBP7B4 != 0 )
			{
				if( EBP7B8 == 0)
				{
					if((rand()%100) < 80)
					{
						EBP7B4 |= 4; 
					}
					else
					{
						EBP7B4 |= 8; 
					}
				}
				else if (EBP7B8 == 1) 
				{
					EBP7B4 |= 4; 
				}
				else
				{
					EBP7B4 |= 8; 
				}
			}
			
			int EBP7BC = this->GetTokenNumber();

			if(EBP7BC > 0)
			{
				EBP7A8 |= 0x20;
			}

			int EBP7C0 = this->GetTokenNumber();

			if(EBP7C0 > 0)
			{
				EBP7A8 |= 0x10;
			}

			int EBP7C4 = this->GetTokenNumber();

			if(EBP7C4 > 0)
			{
				EBP7A8 |= 0x08;
			}

			int EBP7C8 = this->GetTokenNumber();

			if(EBP7C8 > 0)
			{
				EBP7A8 |= 0x04;
			}

			int EBP7CC = this->GetTokenNumber();

			if(EBP7CC > 0)
			{
				EBP7A8 |= 0x02;
			}

			int EBP7D0 = this->GetTokenNumber();

			if(EBP7D0 > 0)
			{
				EBP7A8 |= 0x01;
			}

			if( ItemType >= 0 && ItemType < 512 &&
				ItemIndex >= 0 && ItemIndex < 512)
			{
				int iItemNumber = ItemGetNumberMake(ItemType,ItemIndex);//7d4
				
				if( iItemNumber == ITEMGET(0,19) || 
					iItemNumber == ITEMGET(4,18) || 
					iItemNumber == ITEMGET(5,10) || 
					iItemNumber == ITEMGET(2,13))
				{
					EBP7A8 = 63;	
				}

				//for(int i =0;i < 1;i++)
				//{
					ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, iItemNumber, EBP798, iDur, EBP79C, EBP7A0, EBP7A4, -1, EBP7A8, EBP7B4); 
				//}
			}
		}
		break;
	case COMMAND_MAKECHAOS: //??
		{
			for(int i = 0; i < 15; i++)
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(12,15), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
		}
		break;
	case COMMAND_MAKELIFE: //??
		{
			for(int i = 0; i < 15; i++)
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(14,16), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
		}
		break;
	case COMMAND_MAKECREATION: //??
		{
			for(int i = 0; i < 15; i++)
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(14,22), 0, 0, 0, 0, 0, -1, 0, 0); 
			}
		}
		break;
	case COMMAND_MONEYZEROSET: //??
		{
			gObj[aIndex].Money = 0;
			GCMoneySend(aIndex, 0);
		}
		break;
	case COMMAND_MASTERLEVEL:
		{
			int iTokenNumber = this->GetTokenNumber();

			if(iTokenNumber < 0)
			{
				iTokenNumber = 0;
			}
			else if(iTokenNumber > 200)
			{
				iTokenNumber = 200;
			}

			if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) == 0 || (lpObj->m_nMasterLevel >= 0 && iTokenNumber == 0))
			{
				LogAddTD("Use Test Command [%s][%s] /masterlevel %d -> Fail!!",lpObj->AccountID, lpObj->Name, iTokenNumber);
				return 0;
			}

			lpObj->m_nMasterLevel = iTokenNumber;

			lpObj->m_i64MasterLevelExp = g_MasterLevelSystem.GetMasterLevelExpTlbInfo(iTokenNumber);
			lpObj->m_i64NextMasterLevelExp = g_MasterLevelSystem.GetMasterLevelExpTlbInfo(iTokenNumber+1);

			g_MasterLevelSystem.GCMasterLevelInfo(lpObj);

			LogAddTD("Use Test Command [%s][%s] /masterlevel %d -> Success!!",lpObj->AccountID, lpObj->Name, iTokenNumber);		
		}
		break;
	case COMMAND_MLPOINT:
		{
			int iTokenNumber = this->GetTokenNumber();

			if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) == 0)
			{
				LogAddTD("Use Test Command [%s][%s] /mlpoint %d -> Fail!!",lpObj->AccountID, lpObj->Name, iTokenNumber);
				return 0;
			}

			lpObj->m_iMasterLevelPoint = iTokenNumber;
			g_MasterLevelSystem.GCMasterLevelInfo(lpObj);

			LogAddTD("Use Test Command [%s][%s] /mlpoint %d -> Success!!",lpObj->AccountID, lpObj->Name, iTokenNumber);
		}
		break;
	case COMMAND_MS_RESET:
		{
			for(int i = 0; i < MAX_MAGIC; i++)
			{
				if(lpObj->Magic[i].IsMagic() == TRUE)
				{
//					if(g_MasterSkillSystem.CheckMasterLevelSkill(lpObj->Magic[i].m_Skill) != 0)
//					{
//						lpObj->Magic[i].Clear();
//					}
				}
			}

			GCServerMsgStringSend("Master Skill Reset", lpObj->m_Index, 1);
			LogAddTD("Use Test Command [%s][%s] /ms_reset %d",lpObj->AccountID, lpObj->Name, lpObj->m_iMasterLevelPoint);
			gObjCloseSet(lpObj->m_Index, 1);
		}
		break;
	case COMMAND_MAKESOCKETOPTION:
		{
			int iItemPos = this->GetTokenNumber();
			BYTE EBP8FC = this->GetTokenNumber();
			BYTE EBP900 = this->GetTokenNumber();
			BYTE iSocketSlot = this->GetTokenNumber();
			gItemSocketOption.SetSocketSlot(&lpObj->pInventory[iItemPos],EBP8FC,EBP900-1,iSocketSlot-1);
			::GCInventoryItemOneSend(lpObj->m_Index, iItemPos);
		}
		break;
	case COMMAND_REMAKESOCKETOPTION:
		{
			int iItemPos= this->GetTokenNumber();
			BYTE iSocketSlot = this->GetTokenNumber();

			if(iSocketSlot == 0)
			{
				for(int i = 0;i < 5;i++)
				{
					gItemSocketOption.ClearSocketSlot(&lpObj->pInventory[iItemPos],i);
				}
			}
			else
			{
				gItemSocketOption.ClearSocketSlot(&lpObj->pInventory[iItemPos],iSocketSlot-1);
			}

			::GCInventoryItemOneSend(lpObj->m_Index, iItemPos);
		}
		break;
	case COMMAND_CLEARSOCKETOPTION:
		{
			int iItemPos= this->GetTokenNumber();
			BYTE iSocketSlot = this->GetTokenNumber();
			gItemSocketOption.MakeSocketSlot(&lpObj->pInventory[iItemPos],iSocketSlot);
			::GCInventoryItemOneSend(lpObj->m_Index, iItemPos);
		}
		break;
	case COMMAND_FIRECRACK:
		{
			LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "FIRECRACK.");
			int x = lpObj->X;
			int y = lpObj->Y;
			PMSG_SERVERCMD pMsg;

			PHeadSubSetB((LPBYTE)&pMsg,0xF3,0x40, sizeof(pMsg));
			pMsg.CmdType = 0;

			for ( int i=0;i<15;i++)
			{
				pMsg.X = x+(rand() % 5)*2 - 4;
				pMsg.Y = y+(rand() % 5)*2 - 4;
				MsgSendV2(lpObj,(LPBYTE)&pMsg, sizeof(pMsg));
				::DataSend(lpObj->m_Index ,(LPBYTE)&pMsg, sizeof(pMsg));
			}
		}
		break;
	case COMMAND_SETWATCHINDEX:
		{
			pId = this->GetTokenString();

			if ( pId == NULL )
			{
				return 0;
			}

			LPOBJ lpTargetObj = gObjFind(pId);

			if ( lpTargetObj == NULL )
			{
				return 0;
			}

			LogAddTD("Use GM Command -> [ %s ]	[ %s ]	[ %s ] / Target : [%s][%s] : %s ",
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
				lpTargetObj->Name, "User Watching");

			char szTemp[256];

			if ( this->WatchTargetIndex == lpTargetObj->m_Index )
			{
				this->WatchTargetIndex = -1;
				
				wsprintf(szTemp, "%s : 감시해제", lpTargetObj->Name);	// #translation
				GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
			}
			else
			{
				wsprintf(szTemp, "%s : 감시시작", lpTargetObj->Name);	// #translation
				GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				this->WatchTargetIndex = lpTargetObj->m_Index;
			}
		}
		break;
	case COMMAND_TRACK:
		{
			pId = this->GetTokenString();

			if ( pId == NULL )
			{
				return 0;
			}

			int map;
			int iX;
			int iY;
			LPOBJ lpTargetObj = gObjFind(pId);
			int iIndex;

			if ( lpTargetObj == NULL )
			{
				return 0;
			}

			LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
				lpTargetObj->Name, "User Tracking");
			map = lpTargetObj->MapNumber;
			iX = lpTargetObj->X;
			iY = lpTargetObj->Y;
			iIndex = lpObj->m_Index;

			if ( iIndex >= 0 )
			{
				gObjTeleport(iIndex, map, iX, iY);
			}
		}
		break;
	case COMMAND_USERSTATS:
		{
			LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, 
				"User Stat (connection)");

			int lc151 = 0;
			int lc152 = 400;
			int iTokenNumber = this->GetTokenNumber();

			if ( iTokenNumber > 0 )
			{
				lc151 = iTokenNumber;
			}

			int iTokenNumber2 = this->GetTokenNumber();

			if ( iTokenNumber2 > 0 )
			{
				lc152 = iTokenNumber2;
			}

			gObjSendUserStatistic(lpObj->m_Index, lc151, lc152);
		}
		break;
	case COMMAND_DISCONNECT:
		{
			pId = this->GetTokenString();

			if ( pId == NULL )
			{
				return 0;
			}

			int iTargetIndex = gObjGetIndex(pId);
			
			if( iTargetIndex >= 0 )
			{
				LPOBJ lpTargetObj = gObjFind(pId);

				if ( lpTargetObj == NULL )
				{
					return 0;
				}

#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
				g_ConnectEx.SendClose(lpTargetObj->m_Index);
#endif
				LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
					lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
					lpTargetObj->Name, "User Disconnect");
				LogAdd(lMsg.Get(MSGGET(1, 191)), pId);
				CloseClient(iTargetIndex);
			}
		}
		break;
	case COMMAND_ADDBUFF:
		{
			pId				= this->GetTokenString();
			int BuffIndex	= this->GetTokenNumber();
			int BuffEffect1 = this->GetTokenNumber();
			int Value1		= this->GetTokenNumber();
			int BuffEffect2	= this->GetTokenNumber();
			int Value2		= this->GetTokenNumber();
			int Duration	= this->GetTokenNumber();
			// ----
			if ( pId == NULL )
			{
				return 0;
			}
			// ----
			int iTargetIndex = gObjGetIndex(pId);
			// ----
			if ( iTargetIndex >= 0 )
			{
				LPOBJ lpTargetObj = gObjFind(pId);
				// ----
				if ( lpTargetObj == NULL )
				{
					return 0;
				}
				// ----
				LogAddTD("[AddBuffCmd] Buff [%d] added for [%s]", BuffIndex, lpTargetObj->Name);
				gObjAddBuffEffect(lpTargetObj, BuffIndex, BuffEffect1, Value1, BuffEffect2, Value2, Duration);
			}
		}
		break;
	case COMMAND_CLEARBUFF:
		{
			BYTE Packet[] = { 0xC1, 0x05, 0xFE, 0x00, 0x00 };
			::DataSend(lpObj->m_Index , (LPBYTE)&Packet, sizeof(Packet));

			pId				= this->GetTokenString();
			int BuffIndex	= this->GetTokenNumber();
			// ----
			if ( pId == NULL )
			{
				return 0;
			}
			// ----
			int iTargetIndex = gObjGetIndex(pId);
			// ----
			if ( iTargetIndex >= 0 )
			{
				LPOBJ lpTargetObj = gObjFind(pId);
				// ----
				if ( lpTargetObj == NULL )
				{
					return 0;
				}
				// ----
				LogAddTD("[ClearBuffCmd] Buff [%d] deleted for [%s]", BuffIndex, lpTargetObj->Name);
				gObjRemoveBuffEffect(lpTargetObj, BuffIndex);
			}
		}
		break;
	case COMMAND_GIFTBUFF:
		{
			pId				= this->GetTokenString();
			int BuffEffect1 = this->GetTokenNumber();
			int Value1		= this->GetTokenNumber();
			int BuffEffect2	= this->GetTokenNumber();
			int Value2		= this->GetTokenNumber();
			int Duration	= this->GetTokenNumber();
			// ----
			if ( pId == NULL )
			{
				return 0;
			}
			// ----
			int iTargetIndex = gObjGetIndex(pId);
			// ----
			if ( iTargetIndex >= 0 )
			{
				LPOBJ lpTargetObj = gObjFind(pId);
				// ----
				if ( lpTargetObj == NULL )
				{
					return 0;
				}
				// ----
				if ( Duration <= 0 )
				{
					Duration = 60 * 60;
				}
				// ----
				gObjAddBuffEffect(lpTargetObj, 116, BuffEffect1, Value1, BuffEffect2, Value2, Duration*60);
			}
		}
		break;
	case COMMAND_GDISCONNECT:
		{
			LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name,
				"Guild Disconnect");

			pId = this->GetTokenString();

			if ( pId == NULL )
			{
				return 0;
			}

			_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(pId);
			int iIndex;

			if ( lpGuild != NULL )
			{
				for ( int i=0;i<MAX_USER_GUILD ; i++ )
				{
					if ( lpGuild->Index[i] >= 0 )
					{
						iIndex = lpGuild->Index[i];

						if ( iIndex >= 0 )
						{
							LogAdd(lMsg.Get(MSGGET(1, 191)), pId);
							CloseClient(iIndex);
						}
					}
				}
			}
		}
		break;
	case COMMAND_TRANS:
		{
			//if(CheckAuthorityCondition(32,lpObj) == 1)//Season 4.5 addon
			//{
				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int iTokenNumber1 = this->GetTokenNumber();
				int iTokenNumber2 = this->GetTokenNumber();
				int iTokenNumber3 = this->GetTokenNumber();
				int iIndex = gObjGetIndex(pId);
				LPOBJ lpTargetObj;

				if ( iIndex >= 0 )
				{
					lpTargetObj = gObjFind(pId);

					if ( lpTargetObj == NULL )
					{
						return 0;
					}

					LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
						lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
						lpTargetObj->Name, "User SetPosition");

					gObjTeleport(iIndex, iTokenNumber1, iTokenNumber2, iTokenNumber3);
				}
			//}
		}
		break;
	case COMMAND_WARP:
	case COMMAND_MOVE:
		{
				pId = this->GetTokenString();

				if ( pId != NULL )
				{
					int lc165 = -1;
					int lc166 = 0;
					int lc167 = 0;

					if ( lpObj->Teleport != 0 )
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), lpObj->m_Index, 1);
						return 0;
					}

					if ( (lpObj->m_IfState.use) != 0 )
					{
						if ( lpObj->m_IfState.type  == 3 )
						{
							lpObj->TargetShopNumber = -1;
							lpObj->m_IfState.type = 0;
							lpObj->m_IfState.use = 0;
						}
					}

					if ( lpObj->m_IfState.use > 0 )
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), lpObj->m_Index, 1);
						return 0;
					}

					if ( gObj[aIndex].IsInBattleGround != false )
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), lpObj->m_Index, 1);
						return 0;
					}

					BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
#ifdef PARTYKILLPKSET
					if(gObj[aIndex].PartyNumber >= 0) //Season 2.5 add-on
					{
						if(gParty.GetPKPartyPenalty(gObj[aIndex].PartyNumber) >= 5)
						{
							bPlayerKiller = TRUE;
						}
					}
					else if(gObj[aIndex].m_PK_Level >= 5)
					{
						bPlayerKiller = TRUE;
					}
#else
					if( gObj[aIndex].m_PK_Level >= 5 )
					{
						bPlayerKiller = TRUE;
					}
#endif
					if ( bPlayerKiller == TRUE )
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(4, 101)), lpObj->m_Index, 1);
						return 0;
					}

					gMoveCommand.Move(lpObj, pId);
				}
#ifndef ZEONGAMESERVER
			}
#endif
		}
		break;
	case COMMAND_GMOVE:
		{
			LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, 
				"Guild SetPosition");

			pId = this->GetTokenString();

			if ( pId == NULL )
			{
				return 0;
			}

			int iTokenNumber1 = this->GetTokenNumber();
			int iTokenNumber2 = this->GetTokenNumber();
			int iTokenNumber3 = this->GetTokenNumber();
			_GUILD_INFO_STRUCT* lpGuild = Guild.SearchGuild(pId);
			int iIndex;

			if ( lpGuild != NULL )
			{
				for ( int i=0;i<MAX_USER_GUILD;i++)
				{
					if (lpGuild->Index[i] >= 0 )
					{
						iIndex = lpGuild->Index[i];
						gObjTeleport(iIndex, iTokenNumber1, iTokenNumber2++, iTokenNumber3);
					}
				}
			}
		}
		break;
	case COMMAND_WARSTART:
	case COMMAND_GUILDWARSTART:
		{
			if ( (lpObj->Authority &2)== 2 )
			{
				LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->Ip_addr, lpObj->AccountID, lpObj->Name,
					"Start BattleSoccer");

				BattleSoccerGoalStart(0);
			}
		}
		break;
	case COMMAND_WARSTOP:
	case COMMAND_GUILDWARSTOP:
		{
			if ( (lpObj->Authority &2) == 2 )
			{
				LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->Ip_addr, lpObj->AccountID, lpObj->Name,
					"Stop BattleSoccer");

				BattleSoccerGoalEnd(0);
			}
			else
			{
				if ( gObj[aIndex].lpGuild != NULL )
				{
					if (gObj[aIndex].lpGuild->WarType == 1 )
					{
						if( strcmp( gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] ) == 0 )
						{					
							if( gObj[aIndex].lpGuild->BattleGroundIndex >= 0 )
							{
								//BattleSoccerGoalEnd(gObj[aIndex].lpGuild->BattleGroundIndex);
								//GCServerMsgStringSend("GuildMaster 요청으로 3초 후 경기가 자동으로 종료됩니다.", aIndex, 0);
							}
						}
					}
				}
			}
		}
		break;
	case COMMAND_WAREND:
	case COMMAND_GUILDWAREND:
		{
			if ( (lpObj->Authority & 2) == 2 )
			{
				LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "End GuildWar");

				char * szGuild = this->GetTokenString();

				if ( szGuild != NULL )
				{
					GCManagerGuildWarEnd(szGuild);
				}
			}
			else
			{
				if ( gObj[aIndex].lpGuild != NULL && gObj[aIndex].lpGuild->lpTargetGuildNode != NULL)
				{
					if ( strcmp( gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] ) ==  0)
					{
						if ( gObj[aIndex].lpGuild->BattleGroundIndex >= 0 && gObj[aIndex].lpGuild->WarType == 1 )
						{
							::gObjAddMsgSendDelay(&gObj[aIndex], 7, aIndex, 10000, 0);

							char szTemp[100];

							wsprintf(szTemp, lMsg.Get(MSGGET(4, 129)), gObj[aIndex].lpGuild->Names[0] );
							::GCServerMsgStringSendGuild(gObj[aIndex].lpGuild, szTemp, 1);
							::GCServerMsgStringSendGuild(gObj[aIndex].lpGuild->lpTargetGuildNode, szTemp, 1);
						}
					}
				}
			}
		}
		break;
	case COMMAND_DISABLECHAT:
		{
			LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->Ip_addr, lpObj->AccountID,
				lpObj->Name, "Ban Chatting");

			pId = this->GetTokenString();

			if ( pId == NULL )
			{
				return FALSE;
			}

			int Index = ::gObjGetIndex(pId);

			if ( Index >= 0 )
			{
				gObj[Index].Penalty |= 2;
			}

		}
		break;
	case COMMAND_ENABLECHAT:
		{
			LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->Ip_addr, lpObj->AccountID,
				lpObj->Name, "Free Ban-Chatting");

			pId = this->GetTokenString();

			if ( pId == NULL )
			{
				return FALSE;
			}

			int Index = ::gObjGetIndex(pId);

			if ( Index >= 0 )
			{
				gObj[Index].Penalty &= ~2;
			}
		}
		break;
	case COMMAND_GUILDWAR:
		{
			pId = this->GetTokenString();

			if ( pId != NULL )
			{
				if ( strlen(pId) >= 1 )
				{
					::GCGuildWarRequestResult(pId, aIndex, 0);
				}
			}
		}
		break;
	case COMMAND_BATTLESOCCER:
		{
			if ( (lpObj->Authority & 2 ) == 2 )
			{
				LogAddTD("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Set GuildWar");
	
				pId = this->GetTokenString();

				if ( pId != NULL )
				{
					char * Rival = this->GetTokenString();

					if ( Rival != NULL )
					{
						if ( strlen(pId) >= 1 )
						{
							if ( strlen(Rival) >= 1 )
							{
								::GCManagerGuildWarSet(pId, Rival, 1);
							}
						}
					}
				}
			}
			else
			{
				if ( gEnableBattleSoccer != FALSE )
				{
					pId = this->GetTokenString();

					if ( pId != NULL )
					{
						if ( strlen(pId) >= 1 )
						{
							::GCGuildWarRequestResult(pId, aIndex, 1);
						}
					}
				}
			}
		}
		break;
	case COMMAND_RE:
	case COMMAND_REQUEST:
		{
			pId = this->GetTokenString();

			if ( pId != NULL )
			{
				BOOL bState;

				if ( strcmp(pId, "on" ) == 0 )
				{
					bState = TRUE;
				}
				else if ( strcmp(pId, "off") == 0 )
				{
					bState = FALSE;
				}

				if ( bState >= FALSE && bState <= TRUE )
				{
					::gObjSetTradeOption(aIndex, bState);
					::gObjSetDuelOption(aIndex, bState);
				}
			}
		}
		break;
	case COMMAND_SETDEFSTATS:
		{
			int iClass = lpObj->Class;

			lpObj->ChangeUP = 0;

			lpObj->Strength = DCInfo.DefClass[iClass].Strength;
			lpObj->Dexterity = DCInfo.DefClass[iClass].Dexterity;
			lpObj->Vitality = DCInfo.DefClass[iClass].Vitality;
			lpObj->Energy = DCInfo.DefClass[iClass].Energy;
			lpObj->Life = DCInfo.DefClass[iClass].Life;
			lpObj->MaxLife = DCInfo.DefClass[iClass].MaxLife;
			lpObj->Mana = DCInfo.DefClass[iClass].Mana;
			lpObj->MaxMana = DCInfo.DefClass[iClass].MaxMana;
			lpObj->Leadership = DCInfo.DefClass[iClass].Leadership;

			if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA)
			{
				lpObj->LevelUpPoint = 7 * (lpObj->Level -1 );
			}
			else
			{
				lpObj->LevelUpPoint = 5 * (lpObj->Level -1 );
			}

			GCServerMsgStringSend("Default char. Complete", lpObj->m_Index, 1);
			gObjCloseSet(lpObj->m_Index, 1);
		}
		break;
	case COMMAND_SETDSMODE:
		{
			int iTokenNumber = this->GetTokenNumber();
			int iPetItem_Mode = 0;

			if(iTokenNumber == 0)
			{
				iPetItem_Mode = CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal;
			}
			else if(iTokenNumber == 1)
			{
				iPetItem_Mode = CDarkSpirit::ePetItem_Mode::PetItem_Mode_Attack_Random;
			}
			else if(iTokenNumber == 2)
			{
				iPetItem_Mode = CDarkSpirit::ePetItem_Mode::PetItem_Mode_Attack_WithMaster;
			}
			else if(iTokenNumber == 3)
			{
				iPetItem_Mode = CDarkSpirit::ePetItem_Mode::PetItem_Mode_Attack_Target;
			}

			gDarkSpirit[lpObj->m_Index].SetMode((CDarkSpirit::ePetItem_Mode)iPetItem_Mode,-1);
		}
		break;
	case COMMAND_GETINFARROWMP:
		this->GetInfinityArrowMPConsumption(lpObj);
		break;
	case COMMAND_CONTROLINFARRORMP0:
		this->ControlInfinityArrowMPConsumption0(lpObj, this->GetTokenNumber());
		break;
	case COMMAND_CONTROLINFARRORMP1:
		this->ControlInfinityArrowMPConsumption1(lpObj, this->GetTokenNumber());
		break;
	case COMMAND_CONTROLINFARRORMP2:
		this->ControlInfinityArrowMPConsumption2(lpObj, this->GetTokenNumber());
		break;
	case COMMAND_CONTROLINFARRORMP3:
		this->ControlInfinityArrowMPConsumption3(lpObj, this->GetTokenNumber());
		break;
	case COMMAND_SETINFARROWTIME:
		this->SetInfinityArrowTime(lpObj, this->GetTokenNumber());
		break;
	case COMMAND_ADDEXP:
		{
			__int64 addexp = this->GetTokenNumber();
			gObjLevelUp(lpObj, addexp, 0, 0);
		}
		break;
		/*
	case 366:
		this->ControlFireScreamDoubleAttackDistance(lpObj, this->GetTokenNumber());
		break;*/
	case COMMAND_MAKECHAOSCARD:
		{
			CItem pItem;
			char szItemName[50];
			memset(szItemName, NULL, sizeof(szItemName));
			strcpy(szItemName, this->GetTokenString());

			g_ChaosCard.CheckChaosCardInfo(szItemName, &pItem);

			if(pItem.IsItem() == 0)
			{
				break;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, pItem.m_Type, pItem.m_Level, pItem.m_Durability, pItem.m_Option1, pItem.m_Option2, pItem.m_Option3, lpObj->m_Index, pItem.m_NewOption, 0); 
		}
		break;
	case COMMAND_ADDPCPOINT:
		{
#ifdef PCBANG
			int iAddPoint = this->GetTokenNumber();
			lpObj->m_PcBangPointSystem.m_dwPcBangPointTick = GetTickCount();
			lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint += iAddPoint;

			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(3392), iAddPoint);
			GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);

			g_PCBangPointSystem.EGReqUpdatePcBangResetPointInfo(lpObj->m_Index);

			LogAddTD("[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
					lpObj->AccountID, lpObj->m_PcBangPointSystem.m_sPcBangGainPoint, 
					lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint, lpObj->m_PcBangPointSystem.m_bPcBangCommonRule);
#endif
		}
		break;
	case COMMAND_SETPCPOINT:
		{
#ifdef PCBANG
			int iAddPoint = 0;
			lpObj->m_PcBangPointSystem.m_dwPcBangPointTick = GetTickCount();
			lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint = iAddPoint;

			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(3392), iAddPoint);
			GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);

			g_PCBangPointSystem.EGReqUpdatePcBangResetPointInfo(lpObj->m_Index);

			LogAddTD("[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
					lpObj->AccountID, lpObj->m_PcBangPointSystem.m_sPcBangGainPoint, 
					lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint, lpObj->m_PcBangPointSystem.m_bPcBangCommonRule);
#endif
		}
		break;
	case COMMAND_HIDEON:
			{
				gObjAddBuffEffect(lpObj, AT_INVISIBILITY, 0, 0, 0, 0, -10);
				gObjViewportListProtocolDestroy(lpObj);
			}
			break;
	case COMMAND_HIDEOFF:
			{
				gObjRemoveBuffEffect(lpObj, AT_INVISIBILITY);
				gObjViewportListProtocolCreate(lpObj);
			}
			break;
	case COMMAND_ABILITY:
		this->CmdAbility(lpObj,this->GetTokenString());
		break;
    case COMMAND_PARTY:
		this->CmdParty(lpObj,this->GetTokenString());
		break;
    case COMMAND_SUMMONMONSTER:
   		{
			char * szName = this->GetTokenString();
			int ct = this->GetTokenNumber();
			this->CmdSummonMonster(lpObj,szName,ct);
		}
		break;
    case COMMAND_CLEARMONSTER:
		this->CmdClearMonster(lpObj,this->GetTokenNumber());
		break;
    case COMMAND_CLEARITEM:
		this->CmdClearItem(lpObj,this->GetTokenNumber());
		break;
    case COMMAND_CLEARINVEN:
		this->CmdClearInven(lpObj);
		break;
    case COMMAND_SUMMONCHAR:
		this->CmdSummonChar(lpObj,this->GetTokenString());
		break;
	case COMMAND_SKIN:
		break;
#ifdef __CUSTOMS__
	case COMMAND_ADDSTR:
		{
			int iPoints = this->GetTokenNumber();
			// ----
			if( lpObj->Strength + iPoints > g_MaxStat )
			{
				GCServerMsgStringSend("You can't add more stats", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if( iPoints == 0 )
			{
				GCServerMsgStringSend("Error in syntax of command", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if( lpObj->LevelUpPoint < iPoints || iPoints < 1 )
			{
				GCServerMsgStringSend("Not enough points to add", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			lpObj->Strength		+= iPoints;
			lpObj->LevelUpPoint -= iPoints;
			// ----
			GCReFillSend(aIndex, (int)lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
			gObjSetBP(aIndex);
			GCManaSend(aIndex,(int)lpObj->MaxMana + lpObj->AddMana, 0xFE, 0, lpObj->MaxBP + lpObj->AddBP);
			gObjCalCharacter(aIndex);
			UpdateCharInfo(aIndex);
			// ----
			return TRUE;
		}
		break;
	case COMMAND_ADDAGI:
		{
			int iPoints = this->GetTokenNumber();
			// ----
			if( lpObj->Dexterity + iPoints > g_MaxStat )
			{
				GCServerMsgStringSend("You can't add more stats", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if( iPoints == NULL )
			{
				GCServerMsgStringSend("Error in syntax of command", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if(lpObj->LevelUpPoint < iPoints || iPoints < 1)
			{
				GCServerMsgStringSend("Not enough points to add", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			lpObj->Dexterity += iPoints;
			lpObj->LevelUpPoint -= iPoints;
			// ----
			GCReFillSend(aIndex, (int)lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
			gObjSetBP(aIndex);
			GCManaSend(aIndex,(int)lpObj->MaxMana + lpObj->AddMana, 0xFE, 0, lpObj->MaxBP + lpObj->AddBP);
			gObjCalCharacter(aIndex);
			UpdateCharInfo(aIndex);
			// ----
			return TRUE;
		}
		break;
	case COMMAND_ADDVIT:
		{
			int iPoints = this->GetTokenNumber();
			// ----
			if( iPoints == NULL )
			{
				GCServerMsgStringSend("Error in syntax of command", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if( lpObj->Vitality + iPoints > g_MaxStat )
			{
				GCServerMsgStringSend("You can't add more stats", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if(lpObj->LevelUpPoint < iPoints || iPoints < 1)
			{
				GCServerMsgStringSend("Not enough points to add", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			lpObj->Vitality += iPoints;
			lpObj->MaxLife += lpObj->VitalityToLife * iPoints;
			lpObj->LevelUpPoint -= iPoints;
			// ----
			GCReFillSend(aIndex, (int)lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
			gObjSetBP(aIndex);
			GCManaSend(aIndex,(int)lpObj->MaxMana + lpObj->AddMana, 0xFE, 0, lpObj->MaxBP + lpObj->AddBP);
			gObjCalCharacter(aIndex);
			UpdateCharInfo(aIndex);
			// ----
			return TRUE;
		}
		break;
	case COMMAND_ADDENE:
		{
			int iPoints = this->GetTokenNumber();
			// ----
			if( iPoints == NULL )
			{
				GCServerMsgStringSend("Error in syntax of command", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if( lpObj->Energy + iPoints > g_MaxStat )
			{
				GCServerMsgStringSend("You can't add more stats", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if(lpObj->LevelUpPoint < iPoints || iPoints < 1)
			{
				GCServerMsgStringSend("Not enough points to add", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			lpObj->Energy += iPoints;
			lpObj->MaxMana += lpObj->EnergyToMana * iPoints;
			lpObj->LevelUpPoint -= iPoints;
			// ----
			GCReFillSend(aIndex, (int)lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
			gObjSetBP(aIndex);
			GCManaSend(aIndex,(int)lpObj->MaxMana + lpObj->AddMana, 0xFE, 0, lpObj->MaxBP + lpObj->AddBP);
			gObjCalCharacter(aIndex);
			UpdateCharInfo(aIndex);
			// ----
			return TRUE;
		}
		break;
		// --
	case COMMAND_ADDCOM:
		{
			int iPoints = this->GetTokenNumber();
			// ----
			if( lpObj->Class !=  CLASS_DARKLORD )
			{
				return FALSE;
			}
			// ----
			if( lpObj->Energy + iPoints > g_MaxStat )
			{
				GCServerMsgStringSend("You can't add more stats", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if( iPoints == NULL )
			{
				GCServerMsgStringSend("Error in syntax of command", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			if( lpObj->LevelUpPoint < iPoints || iPoints < 1 )
			{
				GCServerMsgStringSend("Not enough points to add", lpObj->m_Index, 1);
				return FALSE;
			}
			// ----
			lpObj->Leadership += iPoints;
			lpObj->LevelUpPoint -= iPoints;
			// ----
			GCReFillSend(aIndex, (int)lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
			gObjSetBP(aIndex);
			GCManaSend(aIndex,(int)lpObj->MaxMana + lpObj->AddMana, 0xFE, 0, lpObj->MaxBP + lpObj->AddBP);
			gObjCalCharacter(aIndex);
			UpdateCharInfo(aIndex);
			// ----
			return TRUE;
		}
		break;
#endif
		// --
	case COMMAND_POST:
		{
			if ( (lpObj->Penalty&2) == 2 )
			{
				return FALSE;
			}

			if ( lpObj->ChatLitmitTime > 0 )
			{
				MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(4, 223)), lpObj->ChatLitmitTime);
				return FALSE;
			}

			if(lpObj->Level < g_iPostReqLevel)
			{
				GCServerMsgStringSend("Your level is small for command", lpObj->m_Index, 1);
				return FALSE;
			}

			if(lpObj->Money < g_iPostCost)
			{
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 102)), lpObj->m_Index, 1);
				return FALSE;
			}

			if(g_bPostFloodProtect == 1)
			{
				if(lpObj->m_dwPostTickCount != 0)
				{
					if ( 1000 * g_bPostFloodProtectTime >= GetTickCount() - lpObj->m_dwPostTickCount )
					{
						char szTemp[256];
						wsprintf(szTemp, "Please wait %d seconds before next post", g_bPostFloodProtectTime - ((GetTickCount() - lpObj->m_dwPostTickCount) / 1000));
						GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
						return FALSE;
					}
				}
			}

			lpObj->m_dwPostTickCount = GetTickCount();

			char * Token = &szCmd[strlen("/post")+1];
			int len = strlen(Token);

			if( len > 0 )
			{
#pragma message("!!!!!!!!!!!!!!!!! ADD PREFIX SETTING !!!!!!!!!!!!!!!!")
				//Need add prefix setting / reading from CommonServer.cfg
				//PostChatPrefix = [WORLD]
				g_PostLog.Output("[%s] [%s] %s", lpObj->AccountID, lpObj->Name, Token);
				MessageSendEx(g_PostChatColor, lpObj->Name, "[WORLD] %s", Token);
			}
			else
			{
				break;
			}

			lpObj->Money -= g_iPostCost;
			GCMoneySend(lpObj->m_Index,lpObj->Money);
		}
		break;
		// --
		case COMMAND_CHATBLOCK:
		{
			char * Account	= this->GetTokenString();
			int BlockTime	= this->GetTokenNumber();
			// ----
			if ( Account == NULL )
			{
				return FALSE;
			}
			// ----
			int aIndex = ::gObjGetIndex(Account);
			// ----
			if ( aIndex >= 0 )
			{
				gObj[aIndex].ChatLitmitTime = BlockTime;
				char Text[100];
				sprintf(Text, "%s get %d second(s) of chat block", gObj[aIndex].Name, BlockTime);
				GCServerMsgStringSend(Text, lpObj->m_Index, 1);
			}
			else
			{
				char Text[100];
				sprintf(Text, "%s not connected", gObj[aIndex].Name);
				GCServerMsgStringSend(Text, lpObj->m_Index, 1);
			}
		}
		break;
		// --
		case COMMAND_CHATUNBLOCK:
		{
			char * Account	= this->GetTokenString();
			// ----
			if ( Account == NULL )
			{
				return FALSE;
			}
			// ----
			int aIndex = ::gObjGetIndex(Account);
			// ----
			if ( aIndex >= 0 )
			{
				gObj[aIndex].ChatLitmitTime = 0;
				char Text[100];
				sprintf(Text, "%s chat block has been reseted");
				GCServerMsgStringSend(Text, lpObj->m_Index, 1);
			}
			else
			{
				char Text[100];
				sprintf(Text, "%s not connected", gObj[aIndex].Name);
				GCServerMsgStringSend(Text, lpObj->m_Index, 1);
			}
		}
		break;
		// --
	case COMMAND_OFFTRADE:
		{
#ifdef __ALIEN__
			char * OffTradeType = {0};

			OffTradeType = this->GetTokenString();

			if ( OffTradeType == NULL )
			{
				return FALSE;
			}

			if(strcmp(OffTradeType, "zen") == 0 && g_OfflineTrade.EnableZen == true)
			{
				g_OfflineTrade.Start(aIndex, 0);
			}
			else if(strcmp(OffTradeType, "wcoin") == 0 && g_OfflineTrade.EnableWCoin == true)
			{
				g_OfflineTrade.Start(aIndex, 1);
			}
			else if(strcmp(OffTradeType, "soul") == 0 && g_OfflineTrade.EnableSoul == true)
			{
				g_OfflineTrade.Start(aIndex, 2);
			}
			else
			{
				return FALSE;
			}
#else
			g_OfflineTrade.Start(aIndex);
#endif
		}
		break;
		// --
	case COMMAND_SETPARTYLEADER:
		{
			char * TargetName	= this->GetTokenString();
			int TargetIndex		= gObjGetIndex(TargetName);
			// ----
			if( !gParty.SetLeader(lpObj->m_Index, TargetIndex) )
			{
				return false;
			}
		}
		break;
#ifdef OFFEXP
	case COMMAND_OFFEXP:
		{
			OffExp.OffExpStart(aIndex);
			return TRUE;
		}
		break;
#endif
	case COMMAND_LOREN_MARKET:
		{
			gObjMoveGate(lpObj->m_Index, 333);
		}
		break;
	}
	return 0;
}


void CGMMng::GetInfinityArrowMPConsumption(LPOBJ lpObj)
{
	MsgOutput(lpObj->m_Index, "인피니티 애로우 MP 소모량[+0:%d] [+1:%d] [+2:%d]",
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus0(),
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus1(),
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus2());
}

void CGMMng::ControlInfinityArrowMPConsumption0(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus0(iValue);
	MsgOutput(lpObj->m_Index, "인피니티 애로우 MP 소모량 변경(+0) : %d",iValue);

}	

void CGMMng::ControlInfinityArrowMPConsumption1(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus1(iValue);
	MsgOutput(lpObj->m_Index, "인피니티 애로우 MP 소모량 변경(+1) : %d",iValue);

}

void CGMMng::ControlInfinityArrowMPConsumption2(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus2(iValue);
	MsgOutput(lpObj->m_Index, "인피니티 애로우 MP 소모량 변경(+2) : %d",iValue);

}

void CGMMng::ControlInfinityArrowMPConsumption3(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus3(iValue);
	MsgOutput(lpObj->m_Index, "인피니티 애로우 MP 소모량 변경(+3) : %d",iValue);

}

void CGMMng::SetInfinityArrowTime(LPOBJ lpObj, int iValue)
{
	if ( lpObj->Class == CLASS_ELF && lpObj->Type == OBJ_USER && lpObj->ChangeUP == 1 )
	{
		MsgOutput(lpObj->m_Index, "인피니티 애로우 시간 강제 설정 : %d초", iValue);
	}
	else
	{
		MsgOutput(lpObj->m_Index, "220레벨 이상 뮤즈엘프만 사용가능합니다.");
	}
}

void CGMMng::ControlFireScreamDoubleAttackDistance(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetFireScreamExplosionAttackDistance(iValue);
	MsgOutput(lpObj->m_Index, "파이어스크림 더블데미지 폭발거리 변경:%d", iValue);
}

// #Season 4.5 NEW FUNCS START
//00577CF0 
void CGMMng::CmdAbility(LPOBJ lpObj,char *szName)
{
	LPOBJ lpTargetObj = 0;
	int tIndex = -1;

	char szText[256];

	memset(szText,0,sizeof(szText));

	if(szName == NULL)
	{
		GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1);
		return;
	}
	
	tIndex = gObjGetIndex(szName);

	if(tIndex <= 0 )
	{
		GCServerMsgStringSend("Result-User Not Found", lpObj->m_Index, 1);
		return;
	}
	
	lpTargetObj = &gObj[tIndex];
	
	wsprintf(szText, "Result-Ability(%s)", szName);

	GCServerMsgStringSend(szText, lpObj->m_Index, 1);

	int loc68 = 0;

	memset(szText,0,sizeof(szText));

	if(lpTargetObj->Class >= 0 && lpTargetObj->Class <= 5)
	{
		char classname[6][20] = {
			"WIZARD",
			"KNIGHT",
			"ELF",
			"MAGUMSA",
			"DARKLORD",
			"SUMMONER"
		};
		
		wsprintf(szText, "Class:%s",classname[lpTargetObj->Class] );
        GCServerMsgStringSend(szText, lpObj->m_Index, 1);
	}

	memset(szText,0,sizeof(szText));
	 
	wsprintf(szText,"Strength[%d] Dexterity[%d] Vitality[%d] Energy[%d] Leadership[%d]",
	lpTargetObj->AddStrength+lpTargetObj->Strength,
	lpTargetObj->AddDexterity+lpTargetObj->Dexterity,
	lpTargetObj->AddVitality+lpTargetObj->Vitality,
	lpTargetObj->AddEnergy+lpTargetObj->Energy,
	lpTargetObj->AddLeadership+lpTargetObj->Leadership);

	GCServerMsgStringSend(szText, lpObj->m_Index, 1);
}

void CGMMng::CmdParty(LPOBJ lpObj,char *szName)
{
	LPOBJ lpTargetObj = 0;
	int tIndex = -1;

	char szText[256];

	memset(szText,0,sizeof(szText));

	if(szName == NULL)
	{
		GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1);
		return;
	}
	
	tIndex = gObjGetIndex(szName);

	if(tIndex <= 0 )
	{
		GCServerMsgStringSend("Result-User Not Found", lpObj->m_Index, 1);
		return;
	}

	if(gObj[tIndex].PartyNumber < 0)
	{
		GCServerMsgStringSend("Result-Party Not Found", lpObj->m_Index, 1);
		return;
	}

	PARTY_STRUCT * lpParty = &gParty.m_PartyS[gObj[tIndex].PartyNumber];//loc68

	if(lpParty == 0)
	{
		GCServerMsgStringSend("Result-Invalid Point", lpObj->m_Index, 1);
		return;
	}
	
	GCServerMsgStringSend("Result-Party", lpObj->m_Index, 1);

	int PartyCount = 0;
	int PartyNumber = -1;//loc70

	for(int i = 0; i < 5;i++)
	{
		PartyNumber = lpParty->Number[i];

		if(PartyNumber < 0)
		{
			continue;
		}

		LPOBJ lpPartyObj = &gObj[PartyNumber];

		if(lpPartyObj != NULL)
		{
			if(lpPartyObj->Connected >= PLAYER_PLAYING)
			{
				PartyCount++;
				
				strcat(szText, lpPartyObj->Name);

				if(i == 0)
				{
					strcat(szText, "(Leader)");
				}

				if( lpParty->Count > PartyCount)
				{
					strcat(szText, ", ");
				}
			}
		}
	}

	GCServerMsgStringSend(szText, lpObj->m_Index, 1);
}

void CGMMng::CmdSummonMonster(LPOBJ lpObj,char *szMonsterName,int MonsterCount)
{
	char szText[256];

	memset(szText,0,sizeof(szText));

	if(MonsterCount < 1)
	{
		MonsterCount = 1;
	}

	if(szMonsterName == NULL)
	{
		GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1);
		return;
	}

	if(MonsterCount > 20)
	{
		GCServerMsgStringSend("Result-Too Manay Count", lpObj->m_Index, 1);
		return;
	}

	BYTE MapAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

	if ( (MapAttr &1) == 1 )
	{
		GCServerMsgStringSend("Result-Is Safetyzone", lpObj->m_Index, 1);
		return;
	}
	
	int MonsterClass = atoi(szMonsterName);//loc67
	LPMONSTER_ATTRIBUTE lpMonsterAttr = NULL;//loc68

	if(MonsterClass != 0)
	{
		lpMonsterAttr = gMAttr.GetAttr(MonsterClass);
	}
	else
	{
		lpMonsterAttr = gMAttr.GetAttr(szMonsterName);
	}

	if(lpMonsterAttr == NULL)
	{
		GCServerMsgStringSend("Result-Monster Not Found", lpObj->m_Index, 1);
		return;
	}

	if(this->GetType(lpMonsterAttr->m_Index) != OBJ_MONSTER)
	{
		GCServerMsgStringSend("Result-Monster Not Found", lpObj->m_Index, 1);
		return;	
	}

	for(int i = 0; i < MonsterCount; i++)
	{
		BYTE X = lpObj->X;
		BYTE Y = lpObj->Y;

		int iMonsterIndex = gObjAddMonster(lpObj->MapNumber);

		if ( iMonsterIndex >= 0 )
		{
			gObj[iMonsterIndex].m_bGMSummon = 1;//i think its m_bGMSummon
			gObj[iMonsterIndex].m_PosNum = -1;
			gObj[iMonsterIndex].X = X;
			gObj[iMonsterIndex].Y = Y;
			gObj[iMonsterIndex].MapNumber = lpObj->MapNumber;
			gObj[iMonsterIndex].TX = gObj[iMonsterIndex].X;
			gObj[iMonsterIndex].TY = gObj[iMonsterIndex].Y ;
			gObj[iMonsterIndex].m_OldX = gObj[iMonsterIndex].X;
			gObj[iMonsterIndex].m_OldY = gObj[iMonsterIndex].Y;
			gObj[iMonsterIndex].StartX = gObj[iMonsterIndex].X;
			gObj[iMonsterIndex].StartY = gObj[iMonsterIndex].Y;
			gObj[iMonsterIndex].Level = lpMonsterAttr->m_Level;
			gObj[iMonsterIndex].m_Attribute = 60;
			gObj[iMonsterIndex].MaxRegenTime = 0;
			gObj[iMonsterIndex].Dir = rand() % 8;
			gObjSetMonster(iMonsterIndex,lpMonsterAttr->m_Index );
		}
	}

	wsprintf(szText, "Result-SummonMonster(%s:%d)", lpMonsterAttr->m_Name, MonsterCount);
	GCServerMsgStringSend(szText, lpObj->m_Index, 1);
}

void CGMMng::CmdClearMonster(LPOBJ lpObj,int Dis)
{
	char szText[256];

	memset(szText,0,sizeof(szText));
	
	if(Dis <= 0)
	{
		GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1);
		return;
	}

	int MapNumber = lpObj->MapNumber;//loc66
	int DeletedMonCount = 0;

	for(int i = 0;i<OBJ_MAXMONSTER;i++)
	{
		LPOBJ lpMonObj = &gObj[i];//loc69

		if( lpMonObj->MapNumber == MapNumber && 
			gObj[i].Connected == PLAYER_PLAYING &&
			gObj[i].Type == OBJ_MONSTER &&
			lpMonObj->Live != 0 &&
			lpMonObj->m_State == 2 )
		{
			if(gObjCalDistance(lpObj, lpMonObj) <= Dis)
			{
				if(lpMonObj->m_bGMSummon == 1)
				{
					gObjDel(lpMonObj->m_Index);
				}
				else
				{
					lpMonObj->Live = 0;
					lpMonObj->m_State = 4;
					lpMonObj->RegenTime = GetTickCount();
					lpMonObj->DieRegen = 1;
					lpMonObj->PathCount = 0;
					GCDiePlayerSend( lpMonObj, lpMonObj->m_Index, 0, lpObj->m_Index);
				}

				DeletedMonCount++;
			}
		}
	}
	
	wsprintf(szText, "Result-ClearMonster(%d)", DeletedMonCount);
	GCServerMsgStringSend(szText, lpObj->m_Index, 1);
}

void CGMMng::CmdClearItem(LPOBJ lpObj,int Dis)
{
	char szText[256];

	memset(szText,0,sizeof(szText));

	if(Dis <= 0)
	{
		GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1);
		return;
	}
	
	int MapNumber = lpObj->MapNumber;//loc66
	MapClass * lpMap = &MapC[MapNumber];//loc67

	int ItemCount = lpMap->GetVisibleItemCount(lpObj,Dis);

	wsprintf(szText, "Result-ClearItem(%d)", ItemCount);
	GCServerMsgStringSend(szText, lpObj->m_Index, 1);

}

void CGMMng::CmdClearInven(LPOBJ lpObj)
{
	char szText[256];
	memset(szText,0,sizeof(szText));

	int ItemCount = 0;

	for(int i = INVETORY_WEAR_SIZE;i < MAIN_INVENTORY_SIZE ; i++)
	{
		if(lpObj->pInventory[i].IsItem() == 1)
		{
			gObjInventoryDeleteItem(lpObj->m_Index, i);
			ItemCount++;
		}
	}

	GCItemListSend(lpObj->m_Index);

	wsprintf(szText, "Result-ClearInven(%d)", ItemCount);

	GCServerMsgStringSend(szText, lpObj->m_Index, 1);
}

void CGMMng::CmdSummonChar(LPOBJ lpObj,char * szName)
{
	LPOBJ lpTargetObj = 0;
	int tIndex = -1;

	char szText[256];
	memset(szText,0,sizeof(szText));

	if(szName == NULL)
	{
		GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1);
		return;
	}

	tIndex = gObjGetIndex(szName);

	if(tIndex <= 0)
	{
		GCServerMsgStringSend("Result-User Not Found", lpObj->m_Index, 1);
		return;
	}
	
	if(gObj[tIndex].Connected != PLAYER_PLAYING)
	{
		GCServerMsgStringSend("Result-Disconnect User", lpObj->m_Index, 1);
		return;
	}

	int MapNum = lpObj->MapNumber;//loc68
	int X = lpObj->X + 1;//loc69
	int Y = lpObj->Y + 1;//loc70

	BYTE MapAttr = MapC[MapNum].GetAttr(X, Y);
	
	if((MapAttr&4) == 4 || (MapAttr&8) == 8)
	{
		GCServerMsgStringSend("Result-Invalid Area", lpObj->m_Index, 1);
		return;
	}
	
	wsprintf(szText, "Result-SummonCharacter(%s)", szName);

	GCServerMsgStringSend(szText, lpObj->m_Index, 1);
	
	gObjTeleport(tIndex, MapNum,X, Y);

}

WORD CGMMng::GetType(WORD wClass)
{
	WORD Type = 0xFF;

	for(int i = 0; i< OBJ_MAXMONSTER; i++)
	{
		LPOBJ lpMonsterObj = &gObj[i];

		if(lpMonsterObj == NULL)
		{
			continue;
		}
		
		if(lpMonsterObj->Class == wClass)
		{
			Type = lpMonsterObj->Type;
			break;
		}
	}
	
	return Type;
}

void CGMMng::AddLevelUpPoints(int aIndex, unsigned long ulValue, short sType)
{
	LPOBJ lpObj = &gObj[aIndex];
	CRITICAL_SECTION ChatCrit;
	// ----
	switch(sType)
	{
	case 0:
		{
			if(lpObj->Strength+ulValue > g_iMaxCharStats)
			{
				return;
			}
			// --
			lpObj->Strength += ulValue;
		}
		break;
	case 1:
		{
			if(lpObj->Dexterity+ulValue > g_iMaxCharStats)
			{
				return;
			}
			// --
			lpObj->Dexterity += ulValue;
		}
		break;
	case 2:
		{
			if(lpObj->Vitality+ulValue > g_iMaxCharStats)
			{
				return;
			}
			// --
			lpObj->Vitality += ulValue;

		}
		break;
	case 3:
		{
			if(lpObj->Energy+ulValue > g_iMaxCharStats)
			{
				return;
			}
			// --
			lpObj->Energy += ulValue;
		}
		break;
	case 4:
		{
			if(lpObj->Leadership+ulValue > g_iMaxCharStats)
			{
				return;
			}
			// --
			lpObj->Leadership += ulValue;
		}
		break;
	}
	// ----
	InitializeCriticalSection(&ChatCrit);
	EnterCriticalSection(&ChatCrit);
	// ----
	lpObj->LevelUpPoint -= ulValue;
	// ----
	BYTE lpMsg[5] = {0xC1, 0x05, 0xF3, 0x06, sType};
	// ----
	for( int i=0; i < ulValue; i++ )
	CGLevelUpPointAdd((PMSG_LVPOINTADD*)lpMsg, aIndex);
	// ----
	LeaveCriticalSection(&ChatCrit);
	DeleteCriticalSection(&ChatCrit);
}