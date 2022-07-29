#pragma once
#include <memory.h>
#include <malloc.h>

struct Node {
	void* data;
	Node* next;
};

struct List {
	Node* head;
	Node* tail;
	int size;
	int dataSize;
};

void init(List& list, int dataSize);

bool isEmpty(List& list);

Node* createNode(void* data, int dataSize);

Node* addHead(List& list, void* data);

Node* addTail(List& list, void* data);

void removeHead(List& list);

void removeTail(List& list);

void freeList(List& list);
