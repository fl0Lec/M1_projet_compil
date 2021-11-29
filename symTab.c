#include "symTab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//sur quelle valeur un type doit être aligner en mémoire
size_t allignement(enum type t){
    switch (t)
    {
    case INT_T:
        return 4;
        break;
    case BOOL_T:
        return 1;
        break;
    }
}
//taille en memoire d'un identifiant 
size_t taille(struct ID* id){
    switch (id->type)
    {
    case INT_T:
        return 4;
        break;
    case BOOL_T:
        return 1;
        break;
    }
}

void initSymTab(){
    symTab.prev=NULL;
    symTab.head = symTab.tail = NULL;
}


//il reste a verifier si déjà present dans la table
void addST(char *id, enum type type)
{
    struct ID *i=symTab.head, *new;
    //verifie si déjà dans table de symbole
    while (i){
        if (strcmp(id, i->id)==0){
            afficherST();
            fprintf(stderr,"erreur identifiant déjà utliser dans se contexte %s\n", id);
            exit(-1);
        }
        i=i->next;
    }
    //preparer le nouveau id
    new = malloc(sizeof(struct ID*));
    new->next = NULL;
    new->id = malloc(strlen(id)*sizeof(char));
    strcpy(new->id, id);
    new->type = type;
    new->location = 0;
    //la liste existe déjà
    if (symTab.tail){
        //position par rapport a eip
        struct ID* tail = symTab.tail;
        size_t a = allignement(type);
        new->location=(((tail->location+taille(tail)+a-1)/a)*a);
        symTab.tail->next=new;
        symTab.tail=new;
    //premier element
    } else{
        symTab.head=symTab.tail=new;
    }
}

void afficherST(void)
{
    struct ID *i=symTab.head;
    printf("symb tab\n");
    while (i){
        printf("id : %s  \ttype : %s \tlocation : %d\n", i->id, (i->type==INT_T?"int":"boolean"), i->location);
        i=i->next;
    }
}
