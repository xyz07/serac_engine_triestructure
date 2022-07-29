#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_WCSTOK
#define MAXLEN_DIRECT 300
#include <stdio.h>
#include <windows.h>
#include <memory.h>
#include <wchar.h>
#include <time.h>
#include "./algo/algo.h"
#include "./utils/utils.h"
#include "./datastructure/SkipList.h"
#include "./datastructure/SinglyLinkedList.h"

struct AppData {
    wchar_t* UniFull, * UniLower, * UniMin;
    char* Utf8;

    wchar_t** Stopwords;
    int nStopword = 0;
};

struct StringCount {
    char* str;
    int count;
};


// Tạo file index chứa tên tất cả các file txt trong database
void createIndexs();

// Đọc những dữ liệu cần thiết để xử lý chữ tiếng Việt
void readCodeTable(AppData& data);

// Đọc file chứa các stopword
void readStopword(AppData& data);

// Tách chuỗi đầu vào thành các token
void getTokens(wchar_t**& tokens, int& nToken, wchar_t*& rawData, AppData& data);

// Loại bỏ stopword từ mảng các token
void deleteStopword(wchar_t**& tokens, int& nToken, AppData& data);

// Chuyển các token có dấu thành token không dấu
void getUtf8Tokens(char**& u8Tokens, int& nU8Token, wchar_t**& uniTokens, int& nUniToken, AppData& data);

// Rút nội dung chính của 1 mảng các token không dấu
List getMainContent(char**& u8Tokens, int& nU8Token);

// Xử lý 1 chuỗi văn bản có dấu, trả về list chứa nội dung chính
List preprocessString(wchar_t*& rawData, AppData& data);

// Xử lý 1 file txt, ghi ra file meta data
void preprocessFile(char*& directory, AppData& data, FILE*& metaData);

// Tạo file meta data cho app
void preprocessData(AppData& data);

// Giải phóng bộ nhớ
void freeData(AppData& data);
