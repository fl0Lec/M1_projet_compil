%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symTab.h"
#include "genCode.h"
#include "tabD.h"

#define WRITE(msg) fwrite(msg, strlen(msg),1, yyout);
extern int yylex();
extern FILE* yyout;

void yyerror(const char *msg);

/**
 *  fonctions à utiliser dans le code des grammaires
 */

%}

%define parse.error verbose

%code requires{
    #include "symTab.h"
    #include "genCode.h"
    #include "tabD.h"
}

%union {int val; char* mot; enum type type; struct tab* tabID; struct symbole* id; 
        enum Operation op;}

%token SEMICOLON

/* LITERALS */
%token ID
%token CLASS
%token INT_TYPE
%token BOOL_TYPE
%token <val> INT
%token <val> HEXA // ? si on traduit en base 10 en C ou pas
%token <val> CHAR
%token <mot> STRING
%token BOOL
%token <val> FALSE
%token <val> TRUE

/* ASSIGN */
%token ASSIGN
%token ASSIGN_PLUS
%token ASSIGN_SUB

/* OPERATIONS et EXPRESSIONS */
%nonassoc USUB // unaire
%left PLUS
%left SUB
%left MULT
%left DIV
%left MOD
%nonassoc NOT
%token PAR_O
%token PAR_C
%token ACO_O
%token ACO_C
%token COMA

// TODO pas tout de suite
%token INF_EQ
%token SUP_EQ
%token INF
%token SUP
%token EQ
%token NOT_EQ
%left OR
%left AND

%type <mot> ID
%type <val> int_literal
%type <val> char_literal
%type <val> bool_literal
%type <type> type

%type <tabID> liste_id

%type <id> location
%type <id> expr
%type <id> literal

%type <op> arith_op
%type <op> bin_op

%start program

%%

program : CLASS ID check_program ACO_O list_decl list_statement ACO_C

check_program : {
    WRITE("#start program\n");
    //verifie que l'on a bien class Program avant de l'effacer de la pile
    if (strcmp(yylval.mot, "Program")){
        fprintf(stderr, "erreur attend class Program recoit %s\n", yylval.mot);
        exit(-1);
    }
}
;

list_decl :
| field_decl list_decl
;

field_decl : type liste_id SEMICOLON {
    //ajout dans table des symboles la liste des identifiants dans liste_id
    for (int i=0; i<$2->current;i++){
        addST_id($2->s[i], $1);
    }
    freeTD($2);
}
; 

list_statement : statement
| statement list_statement
;

statement : 
|location assign_op expr SEMICOLON {
    //verifie que l'on a bien location et expr
    if (!$1)
        printf("location wrong\n");
    if (!$3)
        printf("expr wrong\n");
    else {
        //reste a faire different assign_op

        //store resultat expr dans location
        gencode(store, $3->id, NULL, $1->id);
    }
}

;

type 
: INT_TYPE {$$ = INT_T;}
| BOOL_TYPE {$$ = BOOL_T;}
;

liste_id
: ID {
    //initialise tableaux dynamique contenue dans liste_id 
    //ajoute l'identifiant dans le tableaux
    $$=initTD(); 
    addTD($$, $1, strlen($1));
    }
| liste_id COMA ID {
    //ajoute l'identifiant dans le tableaux
    addTD($$, $3, strlen($3));
    }
;

assign_op
: ASSIGN
| ASSIGN_PLUS
| ASSIGN_SUB
;

location
: ID {
    $$=lookupST($1);
    //verification si est dans la table des symboles
    if (!$$){
        fprintf(stderr, "no entry in table for %s\n", $1);
        exit(-1);
    }
}
// | ID '[' expr ']' TODO tableaux
;

expr
: location  {$$=$1;}
// | method_call TODO
| literal   {$$=$1;}
| expr bin_op expr {//shif reduce ici
    struct symbole* t=newtemp();
    //switch sur les differentes operations binaires
    switch($2){
        case add:
            t->type.type=INT_T;
            gencode($2, $1->id, $3->id, t->id);
            break;
        case sub:
            t->type.type=INT_T;
            gencode($2, $1->id, $3->id, t->id);
            break;
        case mul:
            t->type.type=INT_T;
            gencode($2, $1->id, $3->id, t->id);
            break;
        case divi:
            t->type.type=INT_T;
            gencode($2, $1->id, $3->id, t->id);
            break;
        case mod:
            t->type.type=INT_T;
            gencode($2, $1->id, $3->id, t->id);
            break;
        default :
            break;
    }
    $$=t;
}
| USUB expr {$$=NULL;}
| NOT expr  {$$=NULL;}
| PAR_O expr PAR_C  {$$=NULL;}
;

bin_op
: arith_op {$$=$1;}
//| rel_op
//| eq_op
//| cond_op
;

arith_op
: PLUS {$$=add;}
| SUB {$$=sub;}
| MULT {$$=mul;}
| DIV {$$=divi;}
| MOD {$$=mod;}
;

/*
rel_op
: INF
| INF_EQ
| SUP
| SUP_EQ
;

eq_op
: EQ
| NOT_EQ
;

cond_op // TODO courts-cicuits
: OR
| AND
;
*/

literal
: int_literal {
    struct symbole* temp=newtemp();
    temp->type.type=INT_T;
    char buf[10];
    sprintf(buf, "%d", $1);
    gencode(loadimm, buf, NULL, temp->id);
    $$ = temp;
    }
| char_literal {
    struct symbole* temp=newtemp();
    temp->type.type=INT_T;
    char buf[10];
    sprintf(buf, "%d", $1);
    gencode(loadimm, buf, NULL, temp->id);
    $$ = temp;
    }
| string_literal {$$=NULL;}
| bool_literal {
    $$=NULL;
    }
;

int_literal
: INT {$$ = $1;}
| HEXA {$$ = $1;}
;

bool_literal
: TRUE {$$ = $1;}
| FALSE {$$ = $1;}
;

char_literal
: CHAR {$$ = $1;}
;

string_literal
: STRING
;


%%

void yyerror(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}
