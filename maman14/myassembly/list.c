#include "list.h"
#include <stddef.h>
#include <stdlib.h>

List *init_list() {
    List *list = (List *) malloc(sizeof(List));
    list->first = NULL;
    list->last = NULL;
    list->count = 0;
    return list;
}

void add(List *list, void *val) {
    if (list->first == NULL) {
        list->first = (Node *) malloc(sizeof(Node));
        list->first->val = val;
        list->first->next = NULL;
        list->last = list->first;
    } else {
        list->last->next = (Node *) malloc(sizeof(Node));
        list->last = list->last->next;
        list->last->val = val;
        list->last->next = NULL;
    }
    list->count++;
}

void poor_for_each(List *list, void (*action)(void *)) {
    Node *pos = list->first;
    while (pos != NULL) {
        action(pos->val);
        pos = pos->next;
    }
}

void for_each(List *list, void (*action)(void *, void *), void *aside_var) {
    Node *pos = list->first;
    while (pos != NULL) {
        action(pos->val, aside_var);
        pos = pos->next;
    }
}

void for_each_with_index(List *list, void (*action)(void *, void *, int), void *aside_var) {
    int i = 0;
    Node *pos = list->first;
    while (pos != NULL) {
        action(pos->val, aside_var, i);
        pos = pos->next;
        i++;
    }
}

/*action: list1, list2, var_aside*/
void for_each_on_cartesian_product(List *list1, List *list2, void (*action)(void *, void *, void *), void *aside_var) {
    Node *pos1 = list1->first;
    while (pos1 != NULL) {
        Node *pos2 = list2->first;
        while (pos2 != NULL) {
            action(pos1->val, pos2->val, aside_var);
            pos2 = pos2->next;
        }
        pos1 = pos1->next;
    }
}

void *search(List *list, void *val, int (*cmp)(void *, void *)) {
    Node *pos = list->first;
    while (pos != NULL) {
        if (cmp(pos->val, val) == 0)
            return pos->val;
        pos = pos->next;
    }
    return NULL;
}

void free_list(List *list, void (*free_val)(void *)) {
    Node *pos = list->first;
    Node *next;
    while (pos != NULL) {
        if (free_val != NULL)
            free_val(pos->val);
        next = pos->next;
        free(pos->val);
        free(pos);
        pos = next;
    }
    free(list);
}
