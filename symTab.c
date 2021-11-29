#include "symTab.h"
#include <stdlib.h>

void initSymTab(){
    symTab.prev=NULL;
    symTab.head = symTab.tail = NULL;
}

void addTab(char *id, enum type type, int location){
    if (symTab.tail){
        struct ID* tail = symTab.tail, *new;
        new = malloc(sizeof(struct ID*));
        new->next = NULL;
        new->id = id;
        new->type = type;
        new->location = location;
        tail->next=new;
        symTab.tail=new;
    }
}