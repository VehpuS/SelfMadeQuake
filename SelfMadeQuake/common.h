#pragma once
#ifndef _STR_TOOLS
#define _STR_TOOLS

bool isNonWhitespace(char input);
bool isWhitespace(char input);
int safeStrCmp(char* s1, char* s2);
int stringToInt(char* str);
int findStringArrayIndex(char* parm, char* gameArgs[], int argsCount);

#endif  // _STR_TOOLS