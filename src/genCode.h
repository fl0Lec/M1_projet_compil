#ifndef __genCode__
#define __genCode__

#include "symTab.h"


enum Operation {load, loadimm, store, add, sub, mul, divi, mod,
                eq, inf, infeq, sup, supeq, goto_op};

static char * const op_names[] = {
	[load] =    "load",
	[loadimm] = "loadimm",
	[store] =   "store",
    [add] =     "add",
    [mul] =     "mult",
    [divi] =     "divi",
    [mod] =     "mod",
    [eq] =      "eq",
    [inf] =     "inf",
    [infeq] =   "infeq",
    [sup] =     "sup", 
    [supeq] =   "supeq",
    [goto_op] = "goto_op"
};

struct code3add{
    enum Operation op;
    struct symbole *arg1, *arg2, *dst;
};

struct genCode
{
    struct code3add* tab;
    int size;
    int capacity;
}genCode;


//initialise le tableau gencode (dans decaf.c)
void initGenCode(void);

//creer un nouveau temporaire et renvoye son entrer dans la table des symboles
struct symbole* newtemp(void);

//genere le code3add et le place dans genCode.
void gencode(enum Operation, struct symbole* s1, struct symbole* s2, struct symbole* dst);

//affiche tous le code generer
void afficheGenCode();
#endif

