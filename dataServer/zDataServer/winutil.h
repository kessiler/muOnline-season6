#ifndef __WINUTIL_H__
#define __WINUTIL_H__



// 값을 한도를 검사한다. max값을 넘으면 0 넘지 않으면 1이다.
#define CHECK_LIMIT(C_VALUE, C_MAX) (C_VALUE<0?FALSE:C_VALUE>(C_MAX-1)?FALSE:TRUE)



extern void BuxConvert(char *buf, int size);
extern BOOL SQLSyntexCheck(char *SQLString);
extern BOOL SQLSyntexCheckConvert(char *SQLString);
extern BOOL SpaceSyntexCheck(char *string);
extern BOOL StrHangulCheck(char * str);
extern void FileSaveString(char *filename, char * string);
extern BOOL IsFile(char *fileName);

void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub,  int size);
#endif

