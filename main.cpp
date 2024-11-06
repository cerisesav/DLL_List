#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    struct DLL list = {};
    ListCtor(&list);
    int index = 0;

    ListInsert(&list, 10, FICTIVE_ELEM_POS, &index);
    ListInsert(&list, 20, 1, NULL);
    ListInsert(&list, 30, 2, NULL);

    PrintList(&list);

	ListRemove(&list, 2);
	PrintList(&list);

	ListInsert(&list, 13, 1, NULL);
	PrintList(&list);

    ListDtor(&list);
    return 0;
}


