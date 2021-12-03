%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "compilateur.tab.h"
%}

%option nounput
%option noyywrap

CHAR \'.\' 

%%

\/\/[^\n]*[\n] ;
[0-9]* {yylval.val = atoi(yytext); return INT;} //TODO : limiter les valeurs entre -2147483648 et 2147483648
[0][x][0-9a-fA-F]+ {yylval.val = strtol(yytext, NULL, 0); return HEXA;} //si ca ne marche pas, modifier les argument de strol
{CHAR} {yylval.val = yytext[1]; return CHAR;}

int {return INT_TYPE;}
boolean {return BOOL_TYPE;}

false {yylval.val = 0; printf("false\n"); return FALSE;}
true {yylval.val = 1; printf("true\n"); return TRUE;}

[(] {return PAR_O;}
[)] {return PAR_C;}
[{] {return ACO_O;}
[}] {return ACO_C;} 
[,] {return COMA;}
[;] {return SEMICOLON;}

\= {return ASSIGN;}
\+\= {return ASSIGN_PLUS;}
\-= {return ASSIGN_SUB;}

[-] {return SUB;} //TODO : a en parler de la distinction du moins et unaire + on peut le faire sur en une instruction si on modifie les tokens
[+] {return PLUS;}
[*] {return MULT;}
[/] {return DIV;}
[%] {return MOD;}
[!] {return NOT;}

[<] {return INF;}
[<][=] {return INF_EQ;}
[>] {return SUP;}
[>][=] {return SUP_EQ;}

[=][=] {return EQ;}
[!][=] {return NOT_EQ;}

[|][|] {return OR;}
[&][&] {return AND;}


class {return CLASS;}

if {printf("if\n");}
for {printf("for\n");}
return {printf("return\n");}
break {printf("break\n");}
continue {printf("continue\n");}


[[:alpha:]]([[:alpha:]]|[0-9])* {yylval.mot = yytext; return ID;}

[[:space:]] ;

. {
    fprintf(stderr, "Caractère illégal (%d %c)\n", yytext[0], yytext[0]);
}

%%
