#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <random>
#include <stdio.h>
#include <string.h>

struct Column {
    char* str;
    int count;
    struct Cell* cells;
    int size_Column;
};

struct Cell {
    struct Column* previous_column, * next_column;
};

struct SkipList {
    static const int MAX_LEVEL = 10; // Giới hạn số tầng
    struct Column* head, * tail;
    SkipList();
    bool empty();
    Column* lowerBound(char*);
    void insert(char*);
};

void pushBack(Column*&, Column*, Column*);
void freeColumn(Column*&);
void freeSkipList(SkipList&);






