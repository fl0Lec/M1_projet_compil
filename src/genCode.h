#ifndef __genCode__
#define __genCode__

#include "symTab.h"


enum Operation {load, loadT, loadimm, store, add, sub, mul, divi, mod,
                eq, noteq, inf, infeq, sup, supeq, goto_op, and, or, //and et or ne se retrouve pas dans genCode
                param, call, label, ret};

enum Assign_op_type {NORMAL_ASSIGN, ADD_ASSIGN, SUB_ASSIGN};

static char * const op_names[] = {
	[load] =    "load",
    [loadT] =   "loadTab",
	[loadimm] = "loadimm",
	[store] =   "store",
    [add] =     "add",
    [sub] =     "sub",
    [mul] =     "mult",
    [divi] =    "divi",
    [mod] =     "mod",
    [eq] =      "equal",
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

struct code3add
{
    enum Operation op;
    struct symbole *arg1, *arg2, *dst;
};

struct genCode
{
    struct code3add* tab;
    size_t size;
    size_t capacity;
    size_t current;    // used to iterate
}genCode;

struct addr 
{
    size_t val;
    struct addr* next;
};

struct list_addr 
{
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

//complete les destination dans liste et libère la mémoire commpleteFirst ne complete que le premier et e retire de la liste
void complete(struct list_addr*, int);
void completeFirst(struct list_addr* l, int addr);

//compele la fonction quand label
void completeLabel(struct list_addr*, struct symbole*);

//affiche list addr
void afficheLA(struct list_addr* l);

#endif

