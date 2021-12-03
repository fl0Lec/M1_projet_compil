#ifndef __symTab__
#define __symTab__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//les diffferents type - plus tard string/tableau
enum type {BOOL_T, INT_T, TEMP,};

struct ID {
    char* id; 
    enum type type;
    int location; //emplacement memoire
    struct ID* next;
};

struct symTab {
    struct symTab *prev;
    struct ID *head;
    struct ID *tail;
    int nbTemp;
};

struct symTab *symTab;


//empile une table sur la table global
void empilerST(void); 

//depile ! a la memoire dynamique
void depilerST(void);

//ajout une entrée dans la table des symbole
struct ID* addST(char *id, enum type type);

//cherche dans les tables
struct ID* lookupST(char *id);

//afficheST et ID
void afficheID(struct ID*);
void afficherST(void);
#endif