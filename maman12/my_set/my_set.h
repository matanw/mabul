struct NumbersSet;
void print_set(struct NumbersSet *);
void push_to_set(struct NumbersSet *, int );
int is_in_set(struct NumbersSet *, int );
void  push_if_not_exists(struct NumbersSet *, int );
struct NumbersSet* init_set();
struct NumbersSet * get_set();