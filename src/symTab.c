#include "symTab.h"
#define TAILLE_INIT 64
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
//taille en memoire d'un identifiant ! au tableau plus tard
size_t taille(struct symbole* id)
{
    switch (id->type.type)
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


void checksize(struct symTab* s){
    if (s->size==s->capacity){
        s->capacity*=2;
        s->symb=realloc(s->symb, sizeof(struct symbole)*s->capacity);
    }
}
/////////////////////////////////////////////////////////////
void empilerST(void)
{
    struct symTab* s=malloc(sizeof(struct symTab));
    s->prev=symTab;
    s->size=0;
    s->capacity=TAILLE_INIT;
    s->symb=malloc(sizeof(struct symbole)*TAILLE_INIT);
    s->nbTemp=0;
    symTab=s;
}

//TODO peut être ne pas liberer la memoire mais garder la table plus tard pour la generation de code ?
void depilerST(void){
    struct symTab *s=symTab;
    //TODO liberer memoire quand fonction
    symTab=symTab->prev;
    free(s->symb);
    free(s);
}

//il reste a verifier si déjà present dans la table
struct symbole* addST_id(char *id, enum type type)
{
    checksize(symTab);
    struct symbole* s= &(symTab->symb[symTab->size++]);
    s->kind=IDENT;
    s->id=malloc(sizeof(char)*strlen(id));
    strcpy(s->id, id);
    s->type.type=type;
    return s; 
}

struct symbole* addST_temp()
{
    checksize(symTab);
    struct symbole* s= &(symTab->symb[symTab->size++]);
    s->kind=TEMPO;
    s->id=malloc(sizeof(char)*(1+sizeof(size_t)));
    s->id[0]='t';
    sprintf(s->id+1, "%ld", symTab->nbTemp++);
    return s; 
}

struct symbole* lookupST(char *id)
{
    struct symTab *s=symTab;
    size_t i=0;
    while (s){
        for (i=0; i<s->size && strcmp(s->symb[i].id, id)!=0; i++) ;
        if (i<s->size)
            return &(symTab->symb[i]);
        else 
            s=s->prev;
    }
    //aucun identifiant trouvé
    return NULL;
}

void afficheSymb(struct symbole* s)
{
    printf("id : %s | kind : %s | type :", s->id, kind_names[s->kind]);
    switch (s->kind)
    {
    case TEMPO:
    case IDENT:
    case TAB:
        printf("%s", type_names[s->type.type]);
        break;
    case FUN:
        //TODO
        printf("TODO");
    break;
    }
    printf("\n");
}

void afficherST(void)
{
    for (size_t i=0; i<symTab->size; i++)
        afficheSymb(&(symTab->symb[i]));
}
