#include "list.h"
#include <stddef.h>
#include <stdlib.h>

List* init_list(){
    List* list=(List*)malloc(sizeof(List));
    list->first=NULL;
    list->last=NULL;
    return list;
};

List* add(List* list,void* val){
    if(list->first==NULL){
        list->first=(Node*)malloc(sizeof(Node));
        list->first->val=val;
        list->first->next=NULL;
        list->last=list->first;
    } else{
        list->last->next=(Node*)malloc(sizeof(Node));
        list->last=list->last->next;
        list->last->val=val;
        list->last->next=NULL;
    }
}

void for_each(List* list, void (*action)(void *))
{
    Node* pos=list->first;
    while(pos!=NULL){
        action(pos->val);
        pos=pos->next;
    }

}