#ifndef LIST_H
#define LIST_H

#define POISON_VALUE -666
#define DLL_SIZE 10
#define FICTIVE_ELEM_POS -1

struct ListElem {
    int val;
    int next;
    int prev;
};

struct DLL {
    ListElem* data;
    size_t size;
    int head;
    int tail;
    int free;
    bool linearized;
};

void ListCtor(struct DLL* list);
void ListDtor(struct DLL* list);
int ListInsert(struct DLL* list, int value, int after_which, int* index);
int ListRemove(struct DLL* list, int index);
void PrintList(struct DLL* list);

#endif
