#include "symTab.h"
#include "genCode.h"

#define TAILLE_INIT 64
#define MAX(a, b) (a>b?a:b)
//sur quelle valeur un type doit être aligné en mémoire
size_t allignement(enum type t){
    switch (t)
    {
    case INT_T:
        return 4;
        break;
    case BOOL_T:
        return 4;
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
        return 4;
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
    s->lastloc=(symTab==NULL||symTab->prev==NULL?0:symTab->lastloc);
    s->maxloc=(symTab==NULL||symTab->prev==NULL?0:symTab->maxloc);
    s->size_fils = 0;
    s-> capacity_fils = TAILLE_INIT;
    s->fils=malloc(sizeof(struct symTab *)*TAILLE_INIT);
    if (symTab){
        if (symTab->size_fils==s->capacity_fils){
            symTab->capacity*=2;
            symTab->fils=realloc(symTab->fils, sizeof(struct symTab *)*symTab->capacity_fils);
        }
        symTab->fils[symTab->size_fils++]=s;
    }

    symTab=s;
}

//TODO peut être ne pas liberer la memoire mais garder la table plus tard pour la generation de code ?
void depilerST(void)
{
    if (symTab->prev != NULL)
        symTab->prev->maxloc=MAX (MAX(symTab->prev->maxloc, symTab->lastloc), symTab->maxloc);
    symTab=symTab->prev;
}

void initST(void)
{
    empilerST();
    struct fundesc* desc=malloc(sizeof(struct fundesc));
    char* s;
    desc->nbArg=1;
    desc->args=malloc(sizeof(enum type)*desc->nbArg);
    desc->args[0]=INT_T;
    desc->ret=VOID_T;
    s=malloc(sizeof(char)*(strlen("WriteInt")+1));
    strcpy(s, "WriteInt");
    addST_fun(s, desc);

    desc=malloc(sizeof(struct fundesc));
    desc->nbArg=1;
    desc->ret=VOID_T;
    desc->args=malloc(sizeof(enum type)*desc->nbArg);
    desc->args[0]=INT_T;
    s=malloc(sizeof(char)*(strlen("ReadInt")+1));
    strcpy(s, "ReadInt");
    addST_fun(s, desc);

    desc=malloc(sizeof(struct fundesc));
    desc->nbArg=1;
    desc->ret=VOID_T;
    desc->args=malloc(sizeof(enum type)*desc->nbArg);
    desc->args[0]=STRING_T;
    //addST_fun("WriteString", desc);
    s=malloc(sizeof(char)*(strlen("WriteString")+1));
    strcpy(s, "WriteString");
    addST_fun(s, desc);
    return;
    
}

void check_idST(char* str){
    for (size_t i=0; i<symTab->size; i++){
        struct symbole* s=&symTab->symb[i];
        if ((s->kind==IDENT || s->kind==TAB || s->kind==FUN) && strcmp(str, s->u.id)==0){
            fprintf(stderr, "impossible identifiant : %s deja present\n", str);
            afficherST();
            afficheGenCode();
            exit(1);
        }

    }
}
////////////////////////////////////////////////////////////////////
//TODO il faudrait a verifier si déjà present dans la table
struct symbole* addST_id(char *id, enum type type)
{
    checksize(symTab);
    check_idST(id);
    struct symbole* s= &(symTab->symb[symTab->size++]);
    s->kind=IDENT;
    s->u.id=id;
    s->type.type=type;
    symTab->lastloc+=allignement(TEMP);
    s->location=symTab->lastloc;
    s->table = symTab;
    return s; 
}

struct symbole* addST_temp()
{
    checksize(symTab);
    struct symbole* s= &(symTab->symb[symTab->size++]);
    s->kind=TEMPO;
    s->u.id=malloc(sizeof(char)*(1+sizeof(size_t)));
    s->u.id[0]='t';
    sprintf(s->u.id+1, "%ld", symTab->nbTemp++);
    symTab->lastloc+=allignement(TEMP);
    s->location=symTab->lastloc;
    s->table = symTab;
    return s; 
}

struct symbole* addST_exprbool()
{
    checksize(symTab);
    struct symbole* s= &(symTab->symb[symTab->size++]);
    s->kind=EXPR_B;
    s->type.type=VOID_T;
    s->table = symTab;
    return s;
}

struct symbole* addST_constInt(int val, enum type type)
{
    checksize(symTab);
    struct symbole* s= &(symTab->symb[symTab->size++]);
    s->kind=CST_INT;
    s->u.val=val;
    s->type.type=type;
    s->table = symTab;
    return s;
}

struct symbole* addST_constStr(char* val)
{
    struct symTab* old = symTab;
    for (;symTab->prev != NULL; symTab = symTab->prev);
    checksize(symTab);
    struct symbole* s= &(symTab->symb[symTab->size++]);
    s->kind=CST_STR;
    s->u.str=malloc(sizeof(char)*((strlen(val)+2)));
    strcpy(s->u.str, val);

    s->u.id=malloc(30); // number of digit of a long < 25
    sprintf(s->u.id, "str.%ld", nbStr++);
    s->type.type=STRING_T;
    s->table = symTab;
    symTab = old;
    return s;
}

struct symbole* addST_fun(char *id, struct fundesc* fundesc)
{
    checksize(symTab);
    check_idST(id);
    struct symbole* s= &(symTab->symb[symTab->size++]);
    s->kind=FUN;
    s->u.id=id;
    /*
    s->u.id=malloc(sizeof(char)*(strlen(id)));
    strncpy(s->u.id, id, strlen(id));
    s->u.id[strlen(id)]='\0';
    //printf("|new fun : %s %s\n", s->u.id, id);*/
    s->type.desc=fundesc;
    s->table = symTab;
    //s->type.desc->ret=ret;
    return s;
}


struct fundesc* initfun()
{
    struct fundesc *fd=malloc(sizeof(struct fundesc));
    fd->nbArg=0;
    fd->capacity=4;
    fd->args = malloc(sizeof(enum type)*fd->capacity);
    return fd;
}

void addtypefd(struct fundesc* fd, enum type t)
{
    if (fd->nbArg==fd->capacity){
        fd->capacity*=2;
        fd->args=realloc(fd->args, sizeof(enum type)*fd->capacity);
    }
    fd->args[fd->nbArg++]=t;
}

int compfundesc(struct fundesc* f1, struct fundesc* f2)
{
    if (f1->nbArg!=f2->nbArg)
        return 0;
    for (int i=0; i<f1->nbArg; i++){
        if (f1->args[i]!=f2->args[i])
            return 0;
    }
    return 1;
}
/////////////////////////////////////////////////////
struct symbole* lookupST(char *id)
{
    struct symTab *s=symTab;
    size_t i=0;
    while (s){
        for (i=0; i<s->size ; i++) {
            if ((s->symb[i].kind==FUN || s->symb[i].kind==IDENT || s->symb[i].kind==TAB) && strcmp(s->symb[i].u.id, id)==0)
                break;
        }
        if (i<s->size)
            return &(s->symb[i]);
        else 
            s=s->prev;
    }
    //aucun identifiant trouvé
    return NULL;
}

void afficheSymb(struct symbole* s)
{
    if (s->kind!=CST_INT && s->kind!=CST_STR)
        printf("id : %s | kind : %s | type :", s->u.id, kind_names[s->kind]);
    switch (s->kind)
    {
    case TEMPO:
    case IDENT:
    case TAB:
        printf("%s | location : %d", type_names[s->type.type], s->location);
        break;
    case CST_INT:
        printf("const int | val : %d", s->u.val);
        break;
    case CST_STR:
        printf("const str | id : %s | str : %s", s->u.id, s->u.str);
        break;
    case FUN:
        if (!s->type.desc)
            break;
        printf("retour : %s | args :[", type_names[s->type.desc->ret]);
        for (int i=0;i<s->type.desc->nbArg;i++)
            printf("%s, ", type_names[s->type.desc->args[i]]);
        printf("]");
        break;
    default:
        ;
        break;
    }
    printf("\n");
}

void afficherST(void)
{
    struct symTab *s=symTab;
    while (s){
        printf("tab \n");
        for (size_t i=0; i<s->size; i++)
            afficheSymb(&(s->symb[i]));
        s=s->prev;
    }

}


void affichRecST(struct symTab* s, int lvl){
    printf("%s : lastlocatio=%d maxlocation=%d\n", (lvl?"symtab suivant":"symtab global"), s->lastloc, s->maxloc);
    for (size_t i=0; i<s->size; i++){
        for (int j=0; j<lvl; j++) printf("\t");
        afficheSymb(&(s->symb[i]));
    }
    for (size_t i=0; i<s->size_fils;i++){
        affichRecST((s->fils[i]), lvl+1);
    }
}

void afficheAllST(void){
    struct symTab* s=symTab;
    while (s->prev)
        s=s->prev;
    affichRecST(s, 0);
    
}

void affichefundesc(struct fundesc* fd)
{
    for (int i=0; i<fd->nbArg; i++){
        printf("%s ",type_names[fd->args[i]]);
    }
    printf("| ret : %s\n", type_names[fd->ret]);
}
