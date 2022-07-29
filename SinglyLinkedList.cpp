#include "SinglyLinkedList.h"


void init(List& list, int dataSize) {
	list.head = list.tail = NULL;
	list.size = 0;
	list.dataSize = dataSize;
}

bool isEmpty(List& list) {
	return list.head == NULL;
}

Node* createNode(void* data, int dataSize) {
	Node* p = (Node*)malloc(1 * sizeof(Node));
	p->data = malloc(dataSize);
	if (!p->data) return NULL;

	memcpy(p->data, data, dataSize);
	p->next = NULL;

	return p;
}

void removeNode(Node* p) {
	free(p->data);
	p->data = NULL;
	free(p);
	p = NULL;
}

Node* addHead(List& list, void* data) {
	Node* temp = createNode(data, list.dataSize);
	if (!temp) return NULL;

	list.size++;
	if (list.head == NULL) {
		list.head = list.tail = temp;
	}
	else {
		temp->next = list.head;
		list.head = temp;
	}

	return temp;
}

Node* addTail(List& list, void* data) {
	Node* temp = createNode(data, list.dataSize);
	if (!temp) return NULL;

	list.size++;
	if (list.tail == NULL) {
		list.head = list.tail = temp;
	}
	else {
		list.tail->next = temp;
		list.tail = temp;
	}

	return temp;
}

void removeHead(List& list) {
	if (list.head == NULL) return;
	
	Node* p = list.head;
	list.head = p->next;
	removeNode(p);
	list.size--;

	if (list.size == 0) list.tail = NULL;
}

void removeTail(List& list) {
	if (list.head == NULL) return;

	if (list.size == 1) {
		removeHead(list);
		return;
	}
	else {
		Node* p = list.head;
		while (p->next != list.tail) {
			p = p->next;
		}
		removeNode(list.tail);
		p->next = NULL;
		list.tail = p;
		list.size--;
	}
}

void freeList(List& list) {
	while (list.head) {
		removeHead(list);
	}
}

