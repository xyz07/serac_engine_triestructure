#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include "../algo/algo.h"

int wcharCmp(void* a, void* b);

int wstringCmp(void* a, void* b);

int wcharCmpReverse(void* a, void* b);

bool isNormalChar(wchar_t c);

bool lastCharOfParaph(wchar_t* s);

bool lastCharOfParaph(char* s);

bool lastCharOfFileName(char* s);

bool isNumber(wchar_t c);

bool isNumber(char c);

void toLower(wchar_t* letter, wchar_t* UniFull, wchar_t* UniLower);

void addWstring(wchar_t*& dest, int& lenDest, wchar_t* source);

void UniToUtf8(char* u8Word, wchar_t* uniWord, wchar_t* UniMin, char* Utf8);

bool isStopWord(wchar_t* letter, wchar_t** stopWords, int nStopWord);

void skipLine(FILE* file);

void trimmed(char*& s);

void trimmed(wchar_t*& s);
