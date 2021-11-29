%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symTab.h"
#include "tabD.h"
extern int yylex();
void yyerror(const char *msg);

/**
 *  fonctions à utiliser dans le code des grammaires
 */

%}

%define parse.error verbose

%union {int val; char* mot; enum type *type; struct tab* tabID;}

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

%start program

%%

program : CLASS ID check_program ACO_O list_decl statement ACO_C

check_program : {
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
        addST($2->s[i], *$1);
    }
    freeTD($2);
}
; 

statement : 
|location assign_op expr SEMICOLON

;

type 
: INT_TYPE {*$$ = INT_T;}
| BOOL_TYPE {*$$ = BOOL_T;}
;

liste_id
: ID {
    $$=initTD(); 
    addTD($$, $1, strlen($1));
    }
| liste_id COMA ID {
    addTD($$, $3, strlen($3));
    }
;

assign_op
: ASSIGN
| ASSIGN_PLUS
| ASSIGN_SUB
;

location
: ID
// | ID '[' expr ']' TODO tableaux
;

expr
: location
// | method_call TODO
| literal
| expr bin_op expr //shif reduce ici
| USUB expr
| NOT expr
| PAR_O expr PAR_C
;

bin_op
: arith_op
//| rel_op
//| eq_op
//| cond_op
;

arith_op
: PLUS
| SUB
/*
| MULT
| DIV
| MOD
*/
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
: int_literal
| char_literal
| string_literal
| bool_literal {printf("BOOL %d\n", $1);}
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
: CHAR {$$ = $1; printf("CHAR %d %c\n", $$, $$);}
;

string_literal
: STRING
;


%%

void yyerror(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}
