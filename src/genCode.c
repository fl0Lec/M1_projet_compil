#include "genCode.h"

struct symbole* newtemp(void)
{
    return addST_temp();
}

void initGenCode(void)
{
    genCode.line=0;
    genCode.size=10;
    genCode.tab=malloc(genCode.size*sizeof(struct code3add));
}

void gencode(enum Operation op, char* s1, char* s2, char* dst)
{
    //augmente taille genCode si taille max atteinte
    if (genCode.line>=genCode.size-1){
        genCode.size*=2;
        genCode.tab=realloc(genCode.tab, sizeof(struct code3add)*genCode.size);
    }
    //line contient la dernière ligne non initialiser
    struct code3add *line=genCode.tab+genCode.line++;
    line->op = op;
    line->arg1=line->arg2=line->dst=NULL;
    //ajout argument seulement si present
    if (s1){
        line->arg1 = malloc(sizeof(char)*strlen(s1));
        strcpy(line->arg1, s1);
    }
    if (s2){
        line->arg2 = malloc(sizeof(char)*strlen(s2));
        strcpy(line->arg2, s2);
    }
    if (dst){
        line->dst = malloc(sizeof(char)*strlen(dst));
        strcpy(line->dst, dst);
    }
}

void affiche3add(struct code3add *line)
{
    printf("op : %s\t arg1 : %s\t arg2 : %s\t dst : %s\n",
    line->op==load?"load":line->op==loadimm?"loadimm":line->op==store?"store":
    line->op==add?"add":line->op==sub?"sub":line->op==mod?"mod":"autre",
    (line->arg1?line->arg1:"NULL"),
    (line->arg2?line->arg2:"NULL"),
    (line->dst?line->dst:"NULL")
    );
}

void afficheGenCode()
{
    printf("gencode nb line : %d\n", genCode.line);
    struct code3add* line=genCode.tab;
    for (; line<genCode.tab+genCode.line; line++){
        affiche3add(line);
    }
}
