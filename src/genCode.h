#ifndef __genCode__
#define __genCode__

#include "symTab.h"


enum Operation {load, loadimm, store, add, sub, mul, divi, mod,
                eq, inf, infeq, sup, supeq, goto_op};

struct code3add{
    enum Operation op;
    char *arg1, *arg2, *dst;
};

struct genCode
{
    struct code3add* tab;
    int size;
    int line;
}genCode;


//initialise le tableau gencode (dans decaf.c)
void initGenCode(void);

//creer un nouveau temporaire et renvoye son entrer dans la table des symboles
struct ID* newtemp(void);

//genere le code3add et le place dans genCode.
void gencode(enum Operation, char* s1, char* s2, char* dst);

//affiche tous le code generer
void afficheGenCode();
#endif

