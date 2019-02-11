#include "list.h"
#include <stddef.h>
#include <stdlib.h>

/* create an empty list*/
List *init_list() {
    List *list = (List *) malloc(sizeof(List));
    list->first = NULL;
    list->last = NULL;
    list->count = 0;
    return list;
}

/*add a value to list*/
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

/*run the "action" on each element in list.
 * For example, if list contains the element a,b,c
 * this function will call to:
 * action(a);
 * action(b);
 * action(c);
 * */
void poor_for_each(List *list, void (*action)(void *)) {
    Node *pos = list->first;
    while (pos != NULL) {
        action(pos->val);
        pos = pos->next;
    }
}


/*run the "action" on each element in list.when running action,
 * it gets pointer to element, and to "var_aside"
 * For example, if list contains the elements a,b,c
 * this function will call to:
 * action(a,var_aside);
 * action(b,var_aside);
 * action(c,var_aside);
 * */
void for_each(List *list, void (*action)(void *, void *), void *aside_var) {
    Node *pos = list->first;
    while (pos != NULL) {
        action(pos->val, aside_var);
        pos = pos->next;
    }
}


/*run the "action" on each element in list.when running action,
 * it gets pointer to element, to "var_aside", and a zero-based index
 * For example, if list contains the elements a,b,c
 * this function will call to:
 * action(a,var_aside,0);
 * action(b,var_aside,1);
 * action(c,var_aside,2);
 * */
void for_each_with_index(List *list, void (*action)(void *, void *, int), void *aside_var) {
    int i = 0;
    Node *pos = list->first;
    while (pos != NULL) {
        action(pos->val, aside_var, i);
        pos = pos->next;
        i++;
    }
}


/*run the "action" on each element  pair in 2 list.when running action,
 * it gets pointer to element in list1, to element in list2, and to "var_aside"
 * For example, if list1 contains the elements a1,a2 and list2 contains the elements b1,b2
 * this function will call to:
 * action(a1,b1,var_aside);
 * action(a1,b2,var_aside);
 * action(a2,b1,var_aside);
 * action(a2,b2,var_aside);
 * */
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

/* return a pointer the element that equal the val using the function "cmp" to compare*/
void *search(List *list, void *val, int (*cmp)(void *, void *)) {
    Node *pos = list->first;
    while (pos != NULL) {
        if (cmp(pos->val, val) == 0)
            return pos->val;
        pos = pos->next;
    }
    return NULL;
}

/* safe free to list and its elements.
 * in case that the element in list is complex structure that need a free to itself,
 * function call to function "free_val" on every element
 * "free_val" can be NULL
 */
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
