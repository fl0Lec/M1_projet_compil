#include "tabD.h"

#define INIT_SIZE 10

struct tab* initTD(void)
{
    struct tab* t=malloc(sizeof(struct tab));
    t->s=malloc(INIT_SIZE*sizeof(char*));
    t->current=0;
    t->max=INIT_SIZE;
    return t;
}

void addTD(struct tab* t, char* s, int size)
{
    if (t->current==t->max){
        t->s=realloc(t->s, t->max*INIT_SIZE*sizeof(char));
        t->max*=INIT_SIZE;
    }
    
    t->s[t->current]=malloc((size+1)*sizeof(char));
    strcpy(t->s[t->current++], s);
    //printf("add %s %s\n", s, t->s[t->current-1]);

}

char* get(struct tab* t, int i)
{
    if (i>t->current)
        return NULL;
    return t->s[i];
}

void afficheTD(struct tab* t)
{
    for (int i=0;i<t->current;i++){
        printf("%s\n", t->s[i]);
    }
}

void freeTD(struct tab* t){
    for (int i=0;i<t->current;i++){
        free(t->s[i]);
    }
    free(t->s);
}
