#include "SkipList.h"

void pushBack(Column*& tmp, Column* prev, Column* next) {
    tmp->cells = (Cell*)realloc(tmp->cells, sizeof(Cell) * (tmp->size_Column + 1));
    tmp->cells[tmp->size_Column].next_column = next;
    tmp->cells[tmp->size_Column].previous_column = prev;
    tmp->size_Column++;
}

SkipList::SkipList() {
	this->head = (Column*) malloc(sizeof(Column));
    this->tail = (Column*) malloc(sizeof(Column));
	this->head->size_Column = 0;
	this->tail->size_Column = 0;
	this->head->str = NULL;
	this->tail->str = NULL;
    this->head->count = -1;
    this->tail->count = -1;
    this->head->cells = this->tail->cells = NULL;
	for (int i = 0; i < MAX_LEVEL; ++i) {
        pushBack(this->head, NULL, tail);
        pushBack(this->tail, head, NULL);
	}
}

bool SkipList::empty() {
	return this->head->cells[0].next_column == tail;
}

Column* SkipList::lowerBound(char* str) {
	Column* iter = this->head;
	for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        while (iter->cells[level].next_column != tail && /*change here*/ strcmp(iter->cells[level].next_column->str, str) == -1 /*change here*/) {
			iter = iter->cells[level].next_column;
		}
	}
	return iter->cells[0].next_column;
}


void SkipList::insert(char* str) {
    Column* temp = lowerBound(str);
    if (temp != tail && /*change here*/ strcmp(temp->str, str) == 0 /*change here*/) {
        temp->count++;
        return;
    }

    Column* inserted_column = (Column*) malloc(sizeof(Column));
    inserted_column->str = _strdup(str);
    inserted_column->size_Column = 0;
    inserted_column->count = 1;
    inserted_column->cells = NULL;
    pushBack(inserted_column, NULL, NULL);
   
    while (inserted_column->size_Column < MAX_LEVEL && rand() % 2 == 0) {
        pushBack(inserted_column, NULL, NULL);
    }
    Column* iter = head;
    for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        while (iter->cells[level].next_column != tail && /*change here*/ strcmp(iter->cells[level].next_column->str, str) == -1 /*here*/) {
            iter = iter->cells[level].next_column;
        }
        if (level < inserted_column->size_Column) {
            Column* next_iter = iter->cells[level].next_column;
            iter->cells[level].next_column = inserted_column;
            next_iter->cells[level].previous_column = inserted_column;
            inserted_column->cells[level].previous_column = iter;
            inserted_column->cells[level].next_column = next_iter;
        }
    }
}

void freeColumn(Column*& head) {
    free(head->str);
    free(head->cells);
    free(head);
}

void freeSkipList(SkipList& list) {
    while (list.head) {
        Column* tmp = list.head;
        list.head = list.head->cells[0].next_column;
        freeColumn(tmp);
    }
}
