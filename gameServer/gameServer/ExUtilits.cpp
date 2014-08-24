#include "StdAfx.h"

int ExcOpt(int amount)
{
	if (amount > 6) amount = 6;
	if (amount < 1) amount = 1;
	
	srand(time(NULL));

	amount = rand() % amount + 1;

	int opt_db[6]  = {1, 2, 4, 8, 16, 32};
	int exc = 0;
	
	int opt_done[6] = { 0 };

	for(int n=0; n < amount; n++)
	{
		int SearchExc = 0;

		srand(time(NULL));

		if( n == 0 )
		{
			SearchExc = opt_db[rand()%6];
			opt_done[n] = SearchExc;
		}
		else
		{
			while(true)
			{
				SearchExc = opt_db[rand()%6];
				bool bOpt = true;

				for(int i=0;i<amount;i++)
				{
					if(SearchExc == opt_done[i])
					{
						bOpt = false;
					}
				}

				if(bOpt == true)
				{
					opt_done[n] = SearchExc;
					break;
				}
			}
		}
		
		exc += SearchExc;
	}

	return exc;
}

bool Ex_IsBadFileLine(char *FileLine, int &Flag)
{
	if(Flag == 0)
	{
		if(isdigit(FileLine[0]))
		{
			Flag = FileLine[0] - 48;
			return true;
		}
	}
	else if(Flag < 0 || Flag > 9)
	{
		Flag = 0;
	}

	if(!strncmp(FileLine, "end", 3))
	{
		Flag = 0;
		return true;
	}

	if(FileLine[0] == '/' || FileLine[0] == '\n')
		return true;

	for(UINT i = 0; i < strlen(FileLine); i++)
	{
		if(isalnum(FileLine[i]))
			return false;
	}
	return true;
}