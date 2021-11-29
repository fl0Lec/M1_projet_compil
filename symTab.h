#ifndef __symTab__
#define __symTab__

//les diffferents type - plus tard string/tableau
enum type {BOOL_T, INT_T};

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
};

struct symTab symTab;

//init symTab
void initSymTab(void);
//empile un table sur la table global
void empilerTab(void); 

//depile ! a la memoire dynamique
void depilerTab();

//ajout une entrer dans la table des symbole
void addTab(char *id, enum type type, int location);

//cherche dans les tables
void lookup(char *id);
#endif