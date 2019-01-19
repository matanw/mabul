#ifndef INCLUDE_LIST_H
#define INCLUDE_LIST_H

struct node {
    void *val;
    struct node *next;
};
typedef struct node Node;

struct list {
    Node *first;
    Node *last;
    int count;
};
typedef struct list List;

List *init_list();

void add(List *list, void *val);

void for_each(List *list, void (*action)(void *));

void for_each_with_aside_var(List *list, void (*action)(void *, void *), void *aside_var);

void for_each_with_aside_var_B(List *list, void (*action)(void *, void *, int), void *aside_var);

void for_each_on_cartesian_product(List *list1, List *list2,
                                   void (*action)(void *, void *, void *), void *aside_var);

void free_list(List *list, void (*free_val)(void *));

void *search(List *list, void *val, int (*cmp)(void *, void *));

#endif
