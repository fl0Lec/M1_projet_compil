#ifndef __genCode__
#define __genCode__

#include "symTab.h"


enum Operation {load, loadimm, store, add, sub, mul, divi, mod, subun,
                eq, noteq, inf, infeq, sup, supeq, goto_op, and, or, //and et or ne se retrouve pas dans genCode
                param, call, label, ret};

enum Assign_op_type {NORMAL_ASSIGN, ADD_ASSIGN, SUB_ASSIGN};

static char * const op_names[] = {
	[load] =    "load",
	[loadimm] = "loadimm",
	[store] =   "store",
    [add] =     "add",
    [mul] =     "mult",
    [divi] =    "divi",
    [mod] =     "mod",
    [subun] =   "moins un",
    [eq] =      "eq",
    [noteq] =   "noteq",
    [inf] =     "inf",
    [infeq] =   "infeq",
    [sup] =     "sup", 
    [supeq] =   "supeq",
    [goto_op] = "goto_op",
    [and] =     "and",
    [or]  =     "or",
    [param] =   "parametre",
    [call] =    "call",
    [label] =   "label",
    [ret] =     "return"
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

struct addr {
    size_t val;
    struct addr* next;
};

struct list_addr {
    struct addr* head;
    struct addr* tail;
};

//initialise le tableau gencode (dans decaf.c)
void initGenCode(void);

//creer un nouveau temporaire et renvoye son entrer dans la table des symboles
struct symbole* newtemp(void);

//genere le code3add et le place dans genCode.
void gencode(enum Operation, struct symbole* s1, struct symbole* s2, struct symbole* dst);

//affiche tous le code generer
void afficheGenCode();

//renvoye liste avec une addresse 
struct list_addr* creerlist(size_t val);

//ajout un element a la liste
void addlist(struct list_addr*, size_t);

//concat deux liste renvoye le pointeur sur la première
struct list_addr* concat(struct list_addr*, struct list_addr*);

//complete les destination dans liste et libère la mémoire
void complete(struct list_addr*, int);

//compele la fonction quand label
void completeLabel(struct list_addr*, struct symbole*);
#endif

