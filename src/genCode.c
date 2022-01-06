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

struct list_addr* creerlist(size_t val)
{
    struct list_addr* l=malloc(sizeof(struct list_addr));
    l->head=l->tail=malloc(sizeof(struct addr));
    l->head->val=val;
    l->head->next=0;
    return l;
}

void addlist(struct list_addr* l, size_t val)
{
    struct addr* e=malloc(sizeof(struct addr));
    e->val=val;
    l->tail->next=e;
    l->tail=e;
}

struct list_addr* concat(struct list_addr* l1, struct list_addr* l2)
{
    l1->tail->next=l2->head;
    return l1;
}

void complete(struct list_addr* l, int addr)
{
    struct addr* next, *e=l->head;
    while (e){
        genCode.tab[e->val].dst=addST_constInt(addr, ADDR);
        next=e->next;
        free(e);
        e=next;
    }
}

void completeLabel(struct list_addr* l, struct symbole* s)
{
    struct addr* next, *e=l->head;
    while (e){
        genCode.tab[e->val].dst=s;
        next=e->next;
        free(e);
        e=next;
    }

}