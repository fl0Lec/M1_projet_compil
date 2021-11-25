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

%start statement

%%

statement
: 
| statement location assign_op expr SEMICOLON // TODO statement temporaire
| statement literal
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
| bool_literal {printf("BOOL %d\n", $1);}
;

int_literal
: INT {$$ = $1; printf("INT %d %x\n", $$, $$);}
| HEXA {$$ = $1; printf("HEXA %d %x\n", $$, $$);}
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
