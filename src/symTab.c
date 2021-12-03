#include "symTab.h"

//sur quelle valeur un type doit être aligné en mémoire
size_t allignement(enum type t){
    switch (t)
    {
    case INT_T:
        return 4;
        break;
    case BOOL_T:
        return 1;
        break;
    case TEMP :
        return 4;
        break;
    default :
        return -1;
        break;
    }

}
//taille en memoire d'un identifiant 
size_t taille(struct ID* id)
{
    switch (id->type)
    {
    case INT_T:
        return 4;
        break;
    case BOOL_T:
        return 1;
        break;
    case TEMP :
        return 4;
        break;
    default :
        return -1;
        break;
    }
}

void empilerST(void)
{
    struct symTab* s=malloc(sizeof(struct symTab));
    s->prev=symTab;
    s->head=s->tail=NULL;
    s->nbTemp=0;
    symTab=s;
}

void depilerST(void){
    struct ID* i=symTab->head, *prev;
    while (i){
        prev=i;
        i=i->next;
        free(prev->id);
        free(prev);
    }
    struct symTab *s=symTab;
    symTab=symTab->prev;
    free(s);
}

//il reste a verifier si déjà present dans la table
struct ID* addST(char *id, enum type type)
{
    struct ID *i=symTab->head, *new;
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
    new = malloc(sizeof(struct ID));
    new->next = NULL;
    new->id = malloc((strlen(id)+1)*sizeof(char));
    strcpy(new->id, id);
    new->type = type;
    new->location = 0;
    //la liste existe déjà
    
    if (symTab->tail){
        //position par rapport a eip
        struct ID* tail = symTab->tail;
        size_t a = allignement(type);
        new->location=(((tail->location+taille(tail)+a-1)/a)*a);
        symTab->tail->next=new;
        symTab->tail=new;
    //premier element
    } else{
        symTab->head=symTab->tail=new;
    }
    return symTab->tail;
}

struct ID* lookupST(char *id)
{
    struct symTab *s=symTab;
    struct ID* i;
    while (s){
        i=s->head;
        while (i){
            if (strcmp(i->id, id)==0)
                return i;
            i=i->next;
        }
        s=s->prev;
    }
    //aucun identifiant trouvé
    return 0;
}
void afficheID(struct ID* id)
{
    if (id)
        printf("id : %s  \ttype : %s \tlocation : %d\n", 
        id->id,
         (id->type==INT_T?"int":id->type==BOOL_T?"boolean":"temp"),
          id->location);
    else 
        printf("NULL");
}
void afficherST(void)
{
    
    printf("symb tab\n");
    struct ID *i=symTab->head;
    while (i){
        afficheID(i);
        i=i->next;
    }
}
