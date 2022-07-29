#pragma once
#include "preprocess.h"
#include "./utils/utils.h"
#include "./algo/algo.h"

int Max(int a, int b);

// Tính trọng số của 1 từ
int calWeight(char* s);

// Tính index trong quá trình chạy thuật toán radix sort
int calIndex(void* p, int pow);

// Sắp xếp kết quả tìm kiếm theo score
void sortResult(List& result, int maxScore);

// Xóa kết quả tìm kiếm
void removeResult(List& list);

// Trả về kết quả tìm kiếm cho chuỗi input
List searchResult(wchar_t*& input, AppData& data);
