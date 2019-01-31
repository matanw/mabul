#define STREET_NAME_LENGTH 256
struct yard_node {
    int id;
    char street_name[STREET_NAME_LENGTH];
    char parking;/*'y' or 'n'*/
    float area;
    struct yard_node *next;
};
typedef struct yard_node YardNode;
struct yards_list {
    struct yard_node *head;
};
typedef struct yards_list YardsList;
/*
ב. בנוסף לקוד בסעיף א, נכתוב
*/
#include <stdio.h>
#include <stdlib.h>

YardsList *init_list();

YardNode *get_node();

void add_to_list(YardsList *list, YardNode *node);

void opposite_list(YardsList *list);

void free_list(YardsList *list);

void print_list(YardsList *list);


int main() {
    char stop_get;
    YardsList *list;
    YardNode *node;
    stop_get = 'n';
    list = init_list();
    while (stop_get == 'n') {
        node = get_node();
        add_to_list(list, node);
        printf("Stop get yards?'y' –yes , stop, 'n'-no,continue");
        scanf(" %c", &stop_get);
    }
    print_list(list);
    opposite_list(list);
    print_list(list);
    free_list(list);
    return 0;
}

int main1() {
    YardNode *pos;
    pos = get_node();
    printf("id %d, street %s, area %f park %c\n", pos->id, pos->street_name, pos->area, pos->parking);

    return 0;
}

YardsList *init_list() {
    YardsList *list = (YardsList *) malloc(sizeof(YardsList));
    list->head = NULL;
    return list;
}

YardNode *get_node() {
    YardNode *node = (YardNode *) malloc(sizeof(YardNode));
    printf("Enter id:\n");
    scanf("%d", &(node->id));
    printf("Enter street name:\n");
    scanf("%s", node->street_name);
    printf("Enter parking –y/n:\n");
    scanf(" %c", &(node->parking));
    printf("Enter area:\n");
    scanf("%f", &(node->area));
    node->next = NULL;
    return node;
}

void add_to_list(YardsList *list, YardNode *node) {
/*head is always point to last element that added and if we loop from head->next until head we get all elements by order*/
    if (list->head == NULL) {
        list->head = node;
        node->next = node;
    } else {
        node->next = list->head->next;
        list->head->next = node;
        list->head = node;
    }
}

void opposite_list(YardsList *list) {
    YardNode *pos1 = list->head;
    YardNode *pos2 = pos1->next;
    YardNode *new_head = pos2;
    while (pos2 != list->head) {
        YardNode *temp;
        temp = pos2->next;
        pos2->next = pos1;
        pos1 = pos2;
        pos2 = temp;
    }
    pos2->next = pos1;
    list->head = new_head;
}


void free_list(YardsList *list) {
    YardNode *pos1 = list->head;
    YardNode *pos2 = pos1->next;
    while (pos2 != list->head) {
        free(pos1);
        pos1 = pos2;
        pos2 = pos2->next;
    }
    free(list);
}

void print_list(YardsList *list) {
    YardNode *pos = list->head->next;
    printf("list is:\n");

    while (pos != list->head) {
        printf("id %d, street %s, area %f park %c\n", pos->id, pos->street_name, pos->area, pos->parking);
        pos = pos->next;

    }
    printf("id %d, street %s, area %f park %c\n", pos->id, pos->street_name, pos->area, pos->parking);

}
