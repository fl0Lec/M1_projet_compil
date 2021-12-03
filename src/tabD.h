#ifndef __tabDynamique__
#define __tabDynamique__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct tab{
    char** s;
    int current, max; //current size and max
};

struct tab* initTD(void);
void addTD(struct tab*, char* , int );
char* get(struct tab*, int );
void afficheTD(struct tab*);

void freeTD(struct tab*);
#endif
