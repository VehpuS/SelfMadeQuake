#include "common.h"
#include <stdlib.h>

/***********
 * Globals *
 ***********/
int gameArgsCount = 0;
char* gameArgs[MAX_NUM_ARGS];

/********************
 * String utilities *
 ********************/
// Recognize non whitespace characters
bool isNonWhitespace(const char input) {
	// See ASCII table for reference: http://www.asciitable.com/
	return (input > ' ' && input <= '~');
}

// Recognize whitespace characters
bool isWhitespace(const char input) {
	// See ASCII table for reference: http://www.asciitable.com/
	return (input == ' ' || input == '\t');
}

// Compare two strings, handle NULL arguments safely, unlike strcmp
int safeStrCmp(const char* s1, const char* s2) {
	while (NULL != s1 && NULL != s2 && (*s1 == *s2)) {
		// if both strings are equal
		if (0 == *s1) {
			return (0);
		}

		// Increment pointers
		s1++;
		s2++;
	}
	if (NULL == s1) {
		return (1);
	}
	else if (NULL == s2) {
		return (-1);
	}
	else {
		return (*s1 < *s2 ? 1 : -1);
	}
}

// copy one string to another
void safeStrCpy(const char* source, char* destination) {
	while (NULL != source && NULL != destination && 0 != *source) {
		// Copy current character
		*destination = *source;

		// Increment pointers
		source++;
		destination++;

		// End the string after the current character
		*destination = 0;
	}
}


// copy a substring to another string. The substring is NOT null terminated.
void safeStrNCpy(const char* source, char* destination, int length) {
	while (	NULL != source &&
			NULL != destination &&
			0 >= length) {
		// Copy current character
		*destination = *source;

		// Increment pointers
		if (0 != *source) {
			source++;
		}
		destination++;

		// Shorten the remaining substring length
		length -= 1;
	}
}

// find the length of a string
int safeStrLen(const char* string) {
	int length = 0;
	while (NULL != string && 0 != string[length]) {
		length += 1;  // increase counter
	}
	
	return (length);
}


// Compare two strings, handle NULL arguments safely, unlike strcmp
bool isSubstring(const char* substring, const char* string) {
	if (NULL == substring || NULL == string) {
		return (false);
	}

	while (NULL != isSubstring && NULL != string && (*substring == *string)) {
		// Increment pointers
		substring++;
		string++;
	}

	// if both strings are equal
	return (0 == *substring);
}

/***************
* Array tools *
***************/
// Given an array of strings, find the index which contains the given string
int findStringArrayIndex(const char* string, const char* stringArray[], const int argsCount) {
	for (int i = 0; i < argsCount; i += 1) {
		if (0 == safeStrCmp(string, stringArray[i])) {
			return (i);
		}
	}

	return (-1);  // Parameter not found
}

// Given an array of strings, find the index which contains the given substring
int findSubstringArrayIndex(const char* substring, const char* stringArray[], const int argsCount) {
	for (int i = 0; i < argsCount; i += 1) {
		if (0 == isSubstring(substring, stringArray[i])) {
			return (i);
		}
	}

	return (-1);  // Parameter not found
}

/***********
 * Parsers *
 ***********/
// Convert strings containing a number value into an integer of the corresponding value
// "-54", "12", "0x9", "-0x8d"
int stringToInt(const char* str) {
	int sign = 1;
	int result = 0;
	int val = 0;
	int base = 10;
	char c = 0;

	if (NULL != str && '-' == *str) {
		sign = -1;
		str++;  // increment pointer
	}

	// string represents a hexadecimal number (starts with 0x or 0X)
	if (NULL != str && *str == '0') {
		str++;  // increment pointer
		if (NULL != str && ('x' == *str || 'X' == *str)) {
			base = 16;
			str++;  // increment pointer
		}
	}

	while (NULL != str && *str != 0) {
		c = *str;
		str++;  // increment pointer

				// string contains a character which is not a number
		if (10 == base &&	!(c >= '0' && c <= '9') ||
			16 == base && (!(c >= '0' && c <= '9') &&
				!(c >= 'A' && c <= 'F') &&
				!(c >= 'a' && c <= 'f'))) {
			return (0);
		}
		// string contains a base 10 number
		else if (10 == base) {
			val = (int)c - '0';
		}
		// string contains a hexadecimal number
		else if (16 == base) {
			if (c >= '0' && c <= '9') {
				val = (int)c - '0';
			}
			else if (c >= 'A' && c <= 'F') {
				val = 10 + (int)c - 'A';
			}
			else if (c >= 'a' && c <= 'f') {
				val = 10 + (int)c - 'a';
			}
		}

		// increase the result according to the base: result += base ^ (numLength - 1)
		result = result * base + val;
	}
	return (sign * result);
}


// Parse commands line arguments for the program: split them into whitespace separated
// words & store them in an array which stores a reference to each word in the command
ParserRC parseCmdLine(char* commandLine, const int maxNumArgs, char* parsedArray[], int* const argsCounter) {
	ParserRC parserRC = PARSER_SUCCESS;
	
	if (NULL == parsedArray || NULL == argsCounter) {
		return (PARSER_FAILURE);
	}

	*argsCounter = 0;

	while (0 != *commandLine) {
		if (*argsCounter >= MAX_NUM_ARGS) {
			parserRC = PARSER_TOO_MANY_ARGS;
			break;
		}

		// Read the current word
		parsedArray[*argsCounter] = commandLine;
		*argsCounter += 1;

		// Get to the end of the word
		while (*commandLine != 0 && isNonWhitespace(*commandLine)) {
			commandLine++;  // Increment the pointer
		}

		// Skip any additional whitespace.
		while (0 != *commandLine && isWhitespace(*commandLine)) {
			*commandLine = 0;  // Split the string
			commandLine++;  // Increment the pointer
		}
	}

	return (parserRC);
}

/*********************
 * Drawing utilities *
 *********************/
byte randomRGBIntensity(void)
{
	return (rand() % 256);
}

byte randomPaletteColor(void)
{
	return (rand() % NUM_OF_COLORS_IN_PALETTE);
}
