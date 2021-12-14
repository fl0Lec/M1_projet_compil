#ifndef __symTab__
#define __symTab__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//les diffferents type - plus tard string/tableau
enum type {BOOL_T, INT_T, TEMP};

//liste chainer d'ID
struct ID {
    char* id;               //identifiant 
    enum type type;         //type
    int location;           //emplacement memoire
    struct ID* next;        //ID suivant
};

struct symTab {
    struct symTab *prev;    //table des symboles de "niveau superieur"
    struct ID *head;
    struct ID *tail;        //pour ajouter ID (toujours à la fin -> O(1))
    int nbTemp;             //nombre de temporaire deja present dans symTab
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