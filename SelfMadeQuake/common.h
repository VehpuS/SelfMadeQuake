#pragma once
#ifndef _COMMON
#define _COMMON

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_NUM_ARGS (50)
#define BITS_PER_BYTE (8)

#define RGB_BYTES (4)
#define PALETTE_BYTES (1)
#define NUM_OF_COLORS_IN_PALETTE (256)

typedef unsigned char byte;

typedef enum parserRC {
	PARSER_SUCCESS,
	PARSER_INVALID_ARG,
	PARSER_TOO_MANY_ARGS,
	PARSER_FAILURE
} ParserRC;

extern int gameArgsCount;  // A counter for the number of command Line parameters passed
extern char* gameArgs[MAX_NUM_ARGS]; // An array that will store char* pointers to each string's array.

bool isNonWhitespace(const char input);
bool isWhitespace(const char input);
int safeStrCmp(const char* s1, const char* s2);
void safeStrCpy(const char* s1, char* s2);
void safeStrNCpy(const char* source, char* destination, int length);
int safeStrLen(const char* string);
bool isSubstring(const char* substring, const char* string);
int stringToInt(const char* string);

int findStringArrayIndex(const char* parm, const char* gameArgs[], const int argsCount);
int findSubstringArrayIndex(const char* substring, const char* stringArray[], const int argsCount);

ParserRC parseCmdLine(char* commandLine, const int maxNumArgs, char* parsedArray[], int* const argsCounter);

byte randomRGBIntensity(void);
unsigned char randomPaletteColor(void);

#endif  // _COMMON