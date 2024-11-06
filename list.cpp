#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "list.h"

static int FindFree(struct DLL* list);

void ListCtor(struct DLL* list) {
    assert(list != NULL);

    list->size = 0;
    list->data = (ListElem*)calloc(DLL_SIZE, sizeof(ListElem));
    assert(list->data != NULL);

    list->data[0].val = POISON_VALUE;
    list->data[0].next = 0;
    list->data[0].prev = 0;

    for (int i = 1; i < DLL_SIZE; i++) {
        list->data[i].val = POISON_VALUE;
        list->data[i].next = (i + 1 < DLL_SIZE) ? i + 1 : FICTIVE_ELEM_POS;
        list->data[i].prev = FICTIVE_ELEM_POS;
    }

	list->data[1].prev = 0;

    list->head = FICTIVE_ELEM_POS;
    list->tail = FICTIVE_ELEM_POS;
    list->free = 1;
    list->linearized = true;
}

static int FindFree(struct DLL* list) {
    if (list->free == FICTIVE_ELEM_POS) {
        return FICTIVE_ELEM_POS;
    }

    int free_index = list->free;
    list->free = list->data[free_index].next;
    return free_index;
}

int ListInsert(struct DLL* list, int value, int after_which, int* index) {
    assert(list != NULL);

    if (list->size >= DLL_SIZE) {
        return -1;
    }

    int free_elem_index = FindFree(list);
    if (free_elem_index == FICTIVE_ELEM_POS) {
        return -1;
    }

    if (index != NULL) {
        *index = free_elem_index;
    }

    ListElem* new_elem = &list->data[free_elem_index];
    new_elem->val = value;

    if (after_which == FICTIVE_ELEM_POS || list->head == FICTIVE_ELEM_POS) {
        // в начало или в пустой
        new_elem->next = list->head;
        new_elem->prev = 0;

        if (list->head != FICTIVE_ELEM_POS) {
            list->data[list->head].prev = free_elem_index;
        }

        list->head = free_elem_index;

        if (list->tail == FICTIVE_ELEM_POS) {
            list->tail = free_elem_index;
        }
    } else {
        // после конкретного элемента
        new_elem->next = list->data[after_which].next;
        new_elem->prev = after_which;

        if (list->data[after_which].next != FICTIVE_ELEM_POS) {
            list->data[list->data[after_which].next].prev = free_elem_index;
        } else {
            list->tail = free_elem_index;
        }

        list->data[after_which].next = free_elem_index;
    }

    list->size++;
    list->linearized = false;
    return 0;
}

int ListRemove(struct DLL* list, int index) {
    assert(list != NULL);

    if (index <= 0 || index >= DLL_SIZE || list->data[index].val == POISON_VALUE) {
        return -1;
    }

    // голова
    if (index == list->head) {
        list->head = list->data[index].next;
        if (list->head != FICTIVE_ELEM_POS) {
            list->data[list->head].prev = 0;
        }
    }

    // хвостик
    if (index == list->tail) {
        list->tail = list->data[index].prev;
        if (list->tail != FICTIVE_ELEM_POS) {
            list->data[list->tail].next = FICTIVE_ELEM_POS;
        }
    }

    // между
    if (index != list->head && list->data[index].prev != 0) {
        list->data[list->data[index].prev].next = list->data[index].next;
    }
    if (index != list->tail && list->data[index].next != FICTIVE_ELEM_POS) {
        list->data[list->data[index].next].prev = list->data[index].prev;
    }

    // в пустой
    list->data[index].val = POISON_VALUE;
    list->data[index].next = list->free;
    list->data[index].prev = FICTIVE_ELEM_POS;
    list->free = index;

    list->size--;
    list->linearized = false;
    return 0;
}

void PrintList(struct DLL* list) {
    assert(list != NULL);

    printf("\nPhysical layout:");
    printf("\nIndex: ");
    for (int i = 0; i < DLL_SIZE; i++) {
        printf("%-4d ", i);
    }
    printf("\nData: ");
    for (int i = 0; i < DLL_SIZE; i++) {
        if (list->data[i].val == POISON_VALUE) {
            printf("P    ");
        } else {
            printf("%-4d ", list->data[i].val);
        }
    }
    printf("\nNext:  ");
    for (int i = 0; i < DLL_SIZE; i++) {
        printf("%-4d ", list->data[i].next);
    }
    printf("\nPrev:  ");
    for (int i = 0; i < DLL_SIZE; i++) {
        printf("%-4d ", list->data[i].prev);
    }
    printf("\nHead: %d, Tail: %d, Free: %d, Size: %zu\n",
           list->head, list->tail, list->free, list->size);
}


void ListDtor(struct DLL* list) {
    assert(list != NULL);

    free(list->data);
    list->data = NULL;
    list->head = FICTIVE_ELEM_POS;
    list->tail = FICTIVE_ELEM_POS;
    list->free = FICTIVE_ELEM_POS;
    list->size = 0;
    list->linearized = true;
}
