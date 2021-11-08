%{
#include <stdio.h>
extern int yylex();
void yyerror(const char *msg);

/**
 *  fonctions à utiliser dans le code des grammaires
 */

%}

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
%token MOINS // unaire
%token PLUS
%token SUB
%token MULT
%token DIV
%token MOD
%token NOT
%token PAR_O
%token PAR_C

// TODO pas tout de suite
%token INF_EQ
%token SUP_EQ
%token INF
%token SUP
%token EQ
%token NOT_EQ
%token OR
%token AND

%start begin

%%

begin
:
| ID begin
;


%%

void yyerror(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}
