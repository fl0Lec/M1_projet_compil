#include "genCode.h"
#include<stdio.h> 
#include<stdlib.h>
#include<string.h>

struct ID* newtemp(void){
    char f[10]="t", t[9];
    do {
        sprintf(t, "%d", symTab->nbTemp++);
        strcat(f, t);
    } while (lookupST(f));
    return addST(f, TEMP);

}
void initGenCode(void){
    genCode.line=0;
    genCode.size=10;
    genCode.tab=malloc(genCode.size*sizeof(struct code3add));
}

void gencode(enum operation op, char* s1, char* s2, char* dst){
    if (genCode.line>=genCode.size-1){
        genCode.size*=2;
        genCode.tab=realloc(genCode.tab, sizeof(struct code3add)*genCode.size);
    }
    struct code3add *line=genCode.tab+genCode.line++;
    line->op = op;
    line->arg1=line->arg2=line->dst=NULL;
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
        printf("dst %s\n", dst);
    }
}

void affiche3add(struct code3add *line){
    printf("op : %s\t arg1 : %s\t arg2 : %s\t dst : %s\n",
    line->op==load?"load":line->op==loadimm?"loadimm":line->op==store?"store":line->op==add?"add":"autre",
    (line->arg1?line->arg1:"NULL"),
    (line->arg2?line->arg2:"NULL"),
    (line->dst?line->dst:"NULL")
    );
}

void afficheGenCode(){
    printf("gencode nb line : %d\n", genCode.line);
    struct code3add* line=genCode.tab;
    for (; line<genCode.tab+genCode.line; line++){
        affiche3add(line);
    }
}