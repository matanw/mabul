#include<stdio.h>
#include <stdlib.h>
#include "my_set.h"

struct NumbersSet {/*todo: convension*/
    int array[64];
    int size;
};


void print_set(struct NumbersSet *set)
{
    printf("set is :");
    int i;
    for(i=0;i<set->size;i++)
    {
        printf("%d ",set->array[i]);
    }
}
void push_to_set(struct NumbersSet *set, int num)
{
    set->array[set->size]=num;
    set->size++;
}

int is_in_set(struct NumbersSet *set, int num){
    int i;
    for(i=0;i<set->size;i++)
    {
        if(set->array[i]==num){
            return 1;
        }
    }
    return 0;
}
void  push_if_not_exists(struct NumbersSet *set, int num)
{
    if(!is_in_set(set,num)){
        push_to_set(set,num);
    }
}
struct NumbersSet * init_set()
{
    struct NumbersSet *set = (struct NumbersSet*)malloc(sizeof(struct NumbersSet));
    set->size=0;
    return set;
}
struct NumbersSet * get_set(){
    struct NumbersSet *set=init_set();
    int num;
    while(scanf("%d",&num)==1)
    {
        push_if_not_exists(set,num);
    }
    return set;
}
int main(){
    struct NumbersSet *set=get_set();
    print_set(set);
    free(set);
    return 0;

}