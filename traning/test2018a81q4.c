//
// Created by Matan Wiesner on 21/12/2018.
//

struct supplier {
    int id;
    int months;
    double price;
    struct supplier *next;
};
typedef struct supplier Supplier;
struct suppliers_list {
    struct supplier *head;
};
typedef struct suppliers_list SuppliersList;

#include <stdlib.h>
#include <stdio.h>

SuppliersList *get_suppliers_list() {
    SuppliersList *list = (SuppliersList *) malloc(sizeof(SuppliersList));
    Supplier *pos;
    int is_first_created = 0;
    list->head = NULL;
    while (1) {
        char choice;
        printf("Prees a to add another supplier, or e for end the list");
        scanf("%c", &choice);
        if (choice == 'e')
            return list;
        if (!is_first_created) {
            pos = (Supplier *) malloc(sizeof(Supplier));
            list->head = pos;
            is_first_created = 1;
        } else {
            pos->next = (Supplier *) malloc(sizeof(Supplier));
            pos = pos->next;
            pos->next = NULL;
        }
        printf("enter id , month,s, price");
        scanf("%d %d %lf", &(pos->id), &(pos->months), &(pos->price));
    }
}

enum compare_result {
    SAME = 1, DIFFERENT = 2, COMMON = 3
};
typedef enum compare_result CompareResult;

CompareResult compare_lists(SuppliersList *list1, SuppliersList *list2) {
    int same = 1;
    int different = 1;/*האם יתכן שהרשימות שונות לחלוטין  */
    Supplier *pos1 = list1->head;
    Supplier *pos2 = list2->head;
    while (1) {
        if (pos1 == NULL && pos2 == NULL)
            break;
        if (pos1 == NULL || pos2 == NULL) {
            same = 0;
            break;
        }
        if (pos1->id == pos2->id) {
            if (pos1->months == pos2->months && pos1->price == pos2->price)
                different = 0;
            else
                same = 0;
            pos1 = pos1->next;
            pos2 = pos2->next;
        } else if (pos1->id > pos2->id) {
            same = 0;
            pos2 = pos2->next;
        } else {  /*pos1->id < pos2->id*/
            same = 0;
            pos1 = pos1->next;
        }
    }
    if (same)
        return SAME;
    if (different)
        return DIFFERENT;
    return COMMON;
}

int main() {
    SuppliersList *list1, *list2;
    CompareResult compare_result;
    printf("enter list1");
    list1 = get_suppliers_list();
    printf("enter list2");
    list2 = get_suppliers_list();
    compare_result = compare_lists(list1, list2);
    if (compare_result == SAME)
        printf("lists are same");
    if (compare_result == DIFFERENT)
        printf("lists are totally diffrent");
    if (compare_result == COMMON)
        printf("lists are different, but have common objects");
}
