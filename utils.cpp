#include "utils.h"

int wcharCmp(void* a, void* b) {
    wchar_t p1 = *(wchar_t*)a, p2 = *(wchar_t*)b;
    if (p1 == p2) return 0;
    return p1 > p2 ? 1 : -1;
}

int wcharCmpReverse(void* a, void* b) {
    return (-1) * wcharCmp(a, b);
}

int wstringCmp(void* a, void* b) {
    wchar_t* _a = *(wchar_t**)a;
    wchar_t* _b = (wchar_t*)b;
    return wcscmp(_a, _b);
}

bool isNormalChar(wchar_t c) {
    wchar_t normal[] = L"$%0123456789@abcdefghijklmnopqrstuvwxyz@%$";
    return bSearch(normal, 0, 42, &c, sizeof(wchar_t), wcharCmp) != -1;
}

bool lastCharOfParaph(wchar_t* s) {
    int len = wcslen(s);
    if (len == 0) return false;
    if (s[len - 1] == L'\n' || s[len - 1] == L'\r') {
        s[len - 1] = L' ';
        len--;
    }
    if (len >= 1 && s[len - 1] == L'\r') s[len - 1] = L' ';
    if (len == 0) s[0] = L'\0';
    return true;
}

bool lastCharOfParaph(char* s) {
    int len = strlen(s);
    if (len == 0) return false;
    if (s[len - 1] == '\n') {
        s[len - 1] = ' ';
        len--;
    }
    if (len >= 1 && s[len - 1] == '\r') s[len - 1] = ' ';
    if (len == 0) s[0] = '\0';
    return true;
}

bool lastCharOfFileName(char* s) {
    int len = strlen(s);
    if (len == 0) return false;
    if (s[len - 1] == '\n' || s[len - 1] == '\r') {
        s[len - 1] = '\0';
        len--;
    }
    if (len >= 1 && s[len - 1] == '\r') s[len - 1] = '\0';
    return true;
}

bool isNumber(wchar_t c) {
    return c >= L'0' && c <= L'9';
}

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

void toLower(wchar_t* letter, wchar_t* UniFull, wchar_t* UniLower) {
    wchar_t c;
    int len = wcslen(letter);

    for (int i = 0; i < len; i++) {
        c = letter[i];
        int pos = bSearch(UniFull, 0, 133, &c, sizeof(wchar_t), wcharCmp);
        if (pos == -1) {
            letter[i] = towlower(c);
        }
        else {
            letter[i] = UniLower[pos];
        }
    }
}

void addWstring(wchar_t*& dest, int& lenDest, wchar_t* source) {
    lenDest += wcslen(source);
    dest = (wchar_t*)realloc(dest, (lenDest + 1) * sizeof(wchar_t));
    wcscat(dest, source);
    dest[lenDest] = L'\0';
}

void UniToUtf8(char* u8Word, wchar_t* uniWord, wchar_t* UniMin, char* Utf8) {
    wchar_t c;
    int uniLen = wcslen(uniWord);
    int u8Len = 0;

    for (int i = 0; i < uniLen; i++) {
        c = uniWord[i];

        int pos = bSearch(UniMin, 0, 66, &c, sizeof(wchar_t), wcharCmp);

        if (pos == -1) {
            if (!isNormalChar(c)) continue;
            u8Word[u8Len++] = (char)c;

        }
        else {
            u8Word[u8Len++] = Utf8[pos];
        }
    }
    u8Word[u8Len] = '\0';
}

bool isStopWord(wchar_t* letter, wchar_t** stopWords, int nStopWord) {
    return bSearch(stopWords, 0, nStopWord - 1, letter, sizeof(wchar_t*), wstringCmp) != -1;
}

void skipLine(FILE* file) {
    char* buffer = (char*)malloc(2001 * sizeof(char));
    do {
        fgets(buffer, 2000, file);
        if (buffer[strlen(buffer) - 1] == '\n') break;
    } while (1);
    free(buffer);
}

void trimmed(char*& s) {
    int i = 0, j = strlen(s) - 1;
    while (s[i] == ' ') i++;
    while (s[j] == ' ') j--;
    char* p = (char*)malloc((j - i + 2) * sizeof(char));
    memcpy(p, s + i, (j - i + 1) * sizeof(char));
    p[j - i + 1] = '\0';
    free(s);
    s = p;
}

void trimmed(wchar_t*& s) {
    int i = 0, j = wcslen(s) - 1;
    while (s[i] == L' ') i++;
    while (s[j] == L' ') j--;
    wchar_t* p = (wchar_t*)malloc((j - i + 2) * sizeof(wchar_t));
    memcpy(p, s + i, (j - i + 1) * sizeof(wchar_t));
    p[j - i + 1] = L'\0';
    free(s);
    s = p;
}
