#include "genCode.h"
#define SIZE_INIT 64
struct symbole* newtemp(void)
{
    return addST_temp();
}

void initGenCode(void)
{
    genCode.size=0;
    genCode.capacity=SIZE_INIT;
    genCode.tab=malloc(genCode.capacity*sizeof(struct code3add));
}

void gencode(enum Operation op, struct symbole* s1, struct symbole* s2, struct symbole* dst)
{
    //augmente taille genCode si taille max atteinte
    if (genCode.size==genCode.capacity){
        genCode.capacity*=2;
        genCode.tab=realloc(genCode.tab, sizeof(struct code3add)*genCode.capacity);
    }
    //line contient la dernière ligne non initialiser
    struct code3add *line=&(genCode.tab[genCode.size++]);
    line->op = op;
    line->arg1=line->arg2=line->dst=NULL;
    //ajout argument seulement si present
    line->arg1=s1;
    line->arg2=s2;
    line->dst=dst;
}

void resumeSYmbole(struct symbole* s)
{
    if (!s){
        printf("(null)");
        return;
    }
    switch (s->kind)
    {
    case CST_INT:
        printf("%d", s->u.val);
        break;
    case CST_STR:
        printf("str:%s",s->u.str);
    default:
        printf("%s", s->u.id);
        break;
    }
}

void affiche3add(struct code3add *line)
{
    printf("op : %s\targ1 : ",op_names[line->op]);
    resumeSYmbole(line->arg1);
    printf("\targ2 : ");
    resumeSYmbole(line->arg2);
    printf("\tdst : ");
    resumeSYmbole(line->dst);
    printf("\n");
}

void afficheGenCode()
{
    printf("gencode nb line : %d\n", genCode.size);
    for (size_t i=0; i<genCode.size; i++){
        affiche3add(&(genCode.tab[i]));
    }
}
