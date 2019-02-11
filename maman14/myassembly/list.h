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

/* create an empty list*/
List *init_list();

/*add a value to list*/
void add(List *list, void *val);

/*run the "action" on each element in list.
 * For example, if list contains the element a,b,c
 * this function will call to:
 * action(a);
 * action(b);
 * action(c);
 * */
void poor_for_each(List *list, void (*action)(void *));

/*run the "action" on each element in list.when running action,
 * it gets pointer to element, and to "var_aside"
 * For example, if list contains the elements a,b,c
 * this function will call to:
 * action(a,var_aside);
 * action(b,var_aside);
 * action(c,var_aside);
 * */
void for_each(List *list, void (*action)(void *, void *), void *aside_var);


/*run the "action" on each element in list.when running action,
 * it gets pointer to element, to "var_aside", and a zero-based index
 * For example, if list contains the elements a,b,c
 * this function will call to:
 * action(a,var_aside,0);
 * action(b,var_aside,1);
 * action(c,var_aside,2);
 * */
void for_each_with_index(List *list, void (*action)(void *, void *, int), void *aside_var);

/*run the "action" on each element  pair in 2 list.when running action,
 * it gets pointer to element in list1, to element in list2, and to "var_aside"
 * For example, if list1 contains the elements a1,a2 and list2 contains the elements b1,b2
 * this function will call to:
 * action(a1,b1,var_aside);
 * action(a1,b2,var_aside);
 * action(a2,b1,var_aside);
 * action(a2,b2,var_aside);
 * */
void for_each_on_cartesian_product(List *list1, List *list2,
                                   void (*action)(void *, void *, void *), void *aside_var);


/* return a pointer the element that equal the val using the function "cmp" to compare*/
void *search(List *list, void *val, int (*cmp)(void *, void *));

/* safe free to list and its elements.
 * in case that the element in list is complex structure that need a free to itself,
 * function call to function "free_val" on every element
 * "free_val" can be NULL
 */
void free_list(List *list, void (*free_val)(void *));

#endif
