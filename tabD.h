#ifndef __tabDynamique__
#define __tabDynamique__

struct tab{
    char** s;
    int current, max; //current size and max
};

struct tab* initTD();
void addTD(struct tab*, char* , int );
char* get(struct tab*, int );
void afficheTD(struct tab*);

#endif