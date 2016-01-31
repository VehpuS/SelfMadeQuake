#include <stdbool.h>
#include <string.h>

/********************
 * String utilities *
 ********************/
// Recognize non whitespace characters
bool isNonWhitespace(char input) {
	// See ASCII table for reference: http://www.asciitable.com/
	return (input > ' ' && input <= '~');
}

// Recognize whitespace characters
bool isWhitespace(char input) {
	// See ASCII table for reference: http://www.asciitable.com/
	return (input == ' ' || input == '\t');
}

// Compare two strings, handle NULL arguments safely, unlike strcmp
int safeStrCmp(char* s1, char* s2) {
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

/***********
 * Parsers *
 ***********/
// Convert strings containing a number value into an integer of the corresponding value
// "-54", "12", "0x9", "-0x8d"
int stringToInt(char* str) {
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

/***************
 * Array tools *
 ***************/
// Given an array of whitespace separated strings, find the index which contains the given array
int findStringArrayIndex(char* parm, char* gameArgs[], int argsCount) {
	for (int i = 0; i < argsCount; i += 1) {
		if (0 == safeStrCmp(parm, gameArgs[i])) {
			return (i);
		}
	}

	return (-1);  // Parameter not found
}
