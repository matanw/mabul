
struct node {
    void* val;
    struct node* next;
};
typedef struct node Node;

struct list{
    Node* first;
    Node* last;
};
typedef struct list List;

List* init_list();
List* add(List* list,void* val);
void for_each(List* list, void (*action)(void *));

