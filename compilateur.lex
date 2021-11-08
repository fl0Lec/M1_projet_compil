%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "compilateur.tab.h"
%}

%option nounput
%option noyywrap

%%

[[:alpha:]]([[:alpha:]]|[0-9])* {return ID;}

[0-9]* {yylval = atoi(yytext); return INT;} //TODO : limiter les valeurs entre -2147483648 et 2147483648
[0][x][0-9a-fA-F]* {yylval = strtol(yytext, NULL, 0); return HEXA;} //si ca ne marche pas, modifier les argument de strol
'[ -!#-&(-[]-~]|"\""|"\'"|"\\"|"\n"|"\t"' {yylval = yytext[0]; return CHAR;}
'f''a''l''s''e' {yylval = 0; return FALSE;}
't''r''u''e' {yylval = 1; return TRUE;}

\= {return ASSIGN;}
\+\= {return ASSIGN_PLUS;}
\-= {return ASSIGN_SUB;}

[-] {return MOINS;} //TODO : a en parler de la distinction du moins et unaire + on peut le faire sur en une instruction si on modifie les tokens
[+] {return PLUS;}
[*] {return MULT;}
[/] {return DIV;}
[%] {return MOD;}
[!] {return NOT;}
[(] {return PAR_O;}
[)] {return PAR_C;}



[[:space:]] ;

. {
    fprintf(stderr, "Caractère illégal (%d)\n", yytext[0]);
}

%%
