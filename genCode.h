#ifndef __genCode__
#define __gencode__
#include "symTab.h"

enum operation {load, loadimm, store, add, sub, mul, divi};

struct code3add{
    enum operation op;
    char *arg1, *arg2, *dst;
};

struct genCode
{
    struct code3add* tab;
    int size;
    int line;
}genCode;

int nbTemp;

void initGenCode(void);
struct ID* newtemp(void);
void gencode(enum operation, char* s1, char* s2, char* dst);

void afficheGenCode();
#endif