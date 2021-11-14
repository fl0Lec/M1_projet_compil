%{
#include <stdio.h>
extern int yylex();
void yyerror(const char *msg);

/**
 *  fonctions à utiliser dans le code des grammaires
 */

%}

%define parse.error verbose

%union {int val; char* mot;}

%token SEMICOLON

/* LITERALS */
%token ID
%token INT
%token HEXA // ? si on traduit en base 10 en C ou pas
%token CHAR
%token STRING
%token BOOL
%token FALSE
%token TRUE

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

%start statement

%%

statement
: 
| statement location assign_op expr SEMICOLON // TODO statement temporaire
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
| expr bin_op expr
| USUB expr
| NOT expr
| PAR_O expr PAR_C
;

bin_op
: arith_op
| rel_op
| eq_op
| cond_op
;

arith_op
: PLUS
| SUB
| MULT
| DIV
| MOD
;

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

literal
: int_literal
| char_literal
| string_literal
| bool_literal
;

int_literal
: INT
| HEXA
;

bool_literal
: TRUE
| FALSE
;

char_literal
: CHAR
;

string_literal
: STRING
;


%%

void yyerror(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}
