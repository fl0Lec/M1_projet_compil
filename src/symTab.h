#ifndef __symTab__
#define __symTab__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//les diffferents type
//TODO string 
enum type {BOOL_T, INT_T, STRING_T, VOID_T, TEMP};

//descripteur de fonction
struct fundesc {
    int nbArg;          //nombre d'argument
    int capacity;       //capcity = allocated
    enum type* args;    //tableau des argument
    enum type ret;      //valeur de retour
};

//liste chainée d'ID
struct symbole {
    enum  {TEMPO, IDENT, TAB, FUN, CST_INT, CST_STR} kind;
    union 
    {
        char* id;           //identifiant (variable, tableau, ou fonction)
        char* str;          //for string literal
        int val;
    } u;
    union {
        struct fundesc* desc;
        enum type type;
    } type;
    int location;           //emplacement memoire (comment faire pour fonction ? -1 ?)
};

struct symTab {
    struct symTab *prev;    //table des symboles de "niveau superieur"
    struct symTab **fils;    //table des symboles suivantes
    size_t size_fils, capacity_fils;
    size_t size;            //taille actuelle de la symtab
    size_t capacity;        //taille en mémoire
    size_t nbTemp;          //nombre de temporaire deja present dans symTab
    int lastloc;            //last location in memorie
    struct symbole* symb;   //tableau des symboles
};

struct symTab *symTab;


//empile une table sur la table global
void empilerST(void); 

//depile ! a la memoire dynamique
void depilerST(void);

//ajout une entrée dans la table des symbole
struct symbole* addST_id(char *id, enum type type);
struct symbole* addST_constInt(int val, enum type type);
struct symbole* addST_constStr(char* val);
struct symbole* addST_fun(char *id, enum type type, struct fundesc*);
struct symbole* addST_temp();

//cherche dans les tables
struct symbole* lookupST(char *id);

//afficheST et ID
void afficheSymb(struct symbole*);
void afficherST(void);
void afficheAllST(void);

struct fundesc* initfun();
void addtypefd(struct fundesc*, enum type);

static char * const kind_names[] = {
    [TEMPO] =   "temp",
	[IDENT] =   "variable",
	[TAB] =     "tableau",
	[FUN] =     "fonction",
    [CST_INT] = "const string",
    [CST_STR] = "const string"  
};

static char * const type_names[] = {
	[INT_T] =       "int",
	[BOOL_T] =      "boolean",
    [STRING_T] =    "string",
	[VOID_T] =      "void",
    [TEMP] =        "temp"
};
#endif