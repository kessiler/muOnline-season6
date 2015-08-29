#include "stdafx.h"
#include "winutil.h"
#include <stdio.h>

#define MAX_BUXCCODE				3

static BYTE	bBuxCode[MAX_BUXCCODE] = {0xFC, 0xCF, 0xAB};

void BuxConvert(char *buf, int size)
{
    for( int n=0; n<size; n++) buf[n] ^= (char)bBuxCode[n%MAX_BUXCCODE];
}

BOOL SQLSyntexCheck(char *SQLString)
{
    char *temp;
    temp = strchr( SQLString, '\'');
    if( temp == NULL )
    {
        temp = strchr( SQLString, 0x20);
        if( temp == NULL ) return TRUE;
        return FALSE;
    }
    return FALSE;
}

BOOL SpaceSyntexCheck(char *string)
{
    char *temp;
    temp = strchr( string, 0x20);
    if( temp == NULL ) return TRUE;
    return FALSE;
}

// 한글인지 체크해서 한글일때는 완성된 글자만 넣는다.
BOOL StrHangulCheck(char * str)
{
    BYTE code1, code2, code;
    int i=0, n=0;
    char sztemp[256]="";
    BOOL result = TRUE;
    int  count=255;

    while( count-- )
    {
        code1 = *(str+i);
        i++;
        if( code1 == '\0' ) break;
        if( code1 < 128 ) {
            sztemp[n] = code1;
            n++;
            continue;
        }
        code2 = *(str+i);
        i++;
        code = (code2)|(code1)<<8;
        if( code > 0 ) {
            sztemp[n] = code1;
            n++;
            sztemp[n] = code2;
            n++;
        }
        else result = FALSE;
        if( code2 == '\0' ) break;
    }
    strcpy( str, sztemp);
    return TRUE;
}

void FileSaveString(char *filename, char * string)
{
    FILE * fp;

    fp = fopen(filename, "at");
    if( fp == NULL ) return;
    fprintf(fp,"%s",string);
    fprintf(fp,"\n");
    fclose(fp);
}
