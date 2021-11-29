#include "symTab.h"
#include <stdlib.h>
#include <stdio.h>
void initSymTab(){
    symTab.prev=NULL;
    symTab.head = symTab.tail = NULL;
}


//il reste a verifier si déjà present dans la table
void addST(char *id, enum type type, int location)
{
    struct ID* tail = symTab.tail, *new;
        new = malloc(sizeof(struct ID*));
        new->next = NULL;
        new->id = id;
        new->type = type;
        new->location = location;
    if (symTab.tail){
        tail->next=new;
        symTab.tail=new;
    } else{
        symTab.head=symTab.tail=new;
    }
}

void afficherST()
{
    struct ID *i=symTab.head;
    printf("symb tab\n");
    while (i){
        printf("id : %s  type : %s location : %d\n", i->id, (i->type==INT_T?"int":"boolean"), i->location);
        i=i->next;
    }
}
