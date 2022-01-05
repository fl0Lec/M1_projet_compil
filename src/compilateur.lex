%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "compilateur.tab.h"
%}

%option nounput
%option noyywrap

CHAR [[:alpha:][:digit:]]

%%

\/\/[^\n]*[\n] ;
[0-9]* {yylval.val = atoi(yytext); return INT;} //TODO : limiter les valeurs entre -2147483648 et 2147483648
[0][x][0-9a-fA-F]+ {yylval.val = strtol(yytext, NULL, 0); return HEXA;} //si ca ne marche pas, modifier les argument de strol
\'{CHAR}\' {yylval.val = (int) yytext[1]; return CHAR;}
\"([[:space:]]|\\\"|\\\'|\\\\|\\[n]|\\[t]|{CHAR})*\" {yylval.mot = yytext; return STRING;}
\'\\\"\' {yylval.val = (int) yytext[1]; return CHAR;}
\'\\\'\' {yylval.val = (int) yytext[1]; return CHAR;}
\'\\\\\' {yylval.val = (int) yytext[1]; return CHAR;}
\'\\[t]\' {yylval.val = (int) yytext[1]; return CHAR;}
\'\\[n]\' {yylval.val = (int) yytext[1]; return CHAR;}

int {return INT_TYPE;}
boolean {return BOOL_TYPE;}

false {return FALSE;}
true {return TRUE;}

[(] {return PAR_O;}
[)] {return PAR_C;}
[{] {return ACO_O;}
[}] {return ACO_C;} 
[,] {return COMA;}
[;] {return SEMICOLON;}

\= {return ASSIGN;}
\+\= {return ASSIGN_PLUS;}
\-= {return ASSIGN_SUB;}

[-] {return SUB;}
[+] {return PLUS;}
[*] {return MULT;}
[/] {return DIV;}
[%] {return MOD;}

[<] {return INF;}
[<][=] {return INF_EQ;}
[>] {return SUP;}
[>][=] {return SUP_EQ;}

[=][=] {return EQ;}
[!][=] {return NOT_EQ;}

[|][|] {return OR;}
[&][&] {return AND;}
[!] {return NOT;}


class {return CLASS;}


if {return IF;}
for {return FOR;}
return {return RETURN;}
break {return BREAK;}
continue {return CONTINUE;}
else {return ELSE;} 
void {return VOID_TYPE;}



[[:alpha:]]([[:alpha:]]|[0-9])* {yylval.mot = malloc(sizeof(char)*(strlen(yytext)+1)); strcpy(yylval.mot, yytext);return ID;}

[[:space:]] ;

. {
    fprintf(stderr, "Caractère illégal (%d %c)\n", yytext[0], yytext[0]);
}

%%
