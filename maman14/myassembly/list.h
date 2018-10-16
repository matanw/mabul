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
};
typedef struct list List;

List *init_list();

void add(List *list, void *val);

void for_each(List *list, void (*action)(void *));

void *search(List *list, void *val, int (*cmp)(void *, void *));

#endif
