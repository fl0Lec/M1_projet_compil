%{
#include <stdio.h>
extern int yylex();
void yyerror(const char *msg);

/**
 *  fonctions à utiliser dans le code des grammaires
 */

%}

%token ID

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
