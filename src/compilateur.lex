%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "compilateur.tab.h"
%}

%option nounput
%option noyywrap

CHAR [[:alpha:][:digit:]#_]

%%

\/\/[^\n]*[\n] ;
[0-9]* {
    yylval.val = atoi(yytext);
    char tmp [20];
    snprintf(tmp, 20, "%d", yylval.val);
    if (strcmp(tmp, yytext) != 0) {
        fprintf(stderr, "DecimalDepassement (%s %s)\n", yytext, tmp);
        exit(1);
    }
    return INT;
}
[0][x][0-9a-fA-F]+ {
    yylval.val = strtol(yytext, NULL, 0);
    char tmp [20];
    snprintf(tmp, 20, "0x%x", yylval.val);
    
    if (strcmp(tmp, yytext) != 0) {
        fprintf(stderr, "HexadecimalDepassement (%s, %s)\n", yytext, tmp);
        exit(1);
    }
    return HEXA;
} //si ca ne marche pas, modifier les argument de strol

\'{CHAR}\' {yylval.val = (int) yytext[1]; return CHAR;}
\"([[:space:]]|\\\"|\\\'|\\\\|\\[n]|\\[t]|{CHAR})*\" {
    yylval.mot = yytext;
    return STRING;
}
\'\\(\"|[t]|[n]|\\|\')\' {
    char tmp [5];
    snprintf(tmp, 5, "%s", yytext);
    yylval.val = (int) tmp[1];
    return CHAR;
}

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



[[:alpha:]_]([[:alpha:]0-9_])* {
    yylval.mot = malloc(sizeof(char)*(strlen(yytext)+1));
    strcpy(yylval.mot, yytext);
    return ID;
}

[[:space:]] ;

. {
    fprintf(stderr, "Caractere illegal (%d %c)\n", yytext[0], yytext[0]);
    exit(1);
}

%%
