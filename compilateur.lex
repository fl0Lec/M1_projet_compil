%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "compilateur.tab.h"
%}

%option nounput
%option noyywrap

CHAR \'[ -!#-&(-[]-~]|"\""|"\'"|"\\"|"\n"|"\t"\' 

%%

[[:alpha:]]([[:alpha:]]|[0-9])* {yylval.mot = yytext; return ID;}

[0-9]* {yylval.val = atoi(yytext); return INT;} //TODO : limiter les valeurs entre -2147483648 et 2147483648
[0][x][0-9a-fA-F]* {yylval.val = strtol(yytext, NULL, 0); return HEXA;} //si ca ne marche pas, modifier les argument de strol
{CHAR}* {yylval.mot = yytext; return CHAR;}

'f''a''l''s''e' {yylval.val = 0; return FALSE;}
't''r''u''e' {yylval.val = 1; return TRUE;}

\= {return ASSIGN;}
\+\= {return ASSIGN_PLUS;}
\-= {return ASSIGN_SUB;}

[-] {return USUB;} //TODO : a en parler de la distinction du moins et unaire + on peut le faire sur en une instruction si on modifie les tokens
[+] {return PLUS;}
[*] {return MULT;}
[/] {return DIV;}
[%] {return MOD;}
[!] {return NOT;}
[(] {return PAR_O;}
[)] {return PAR_C;}

[<] {return INF;}
[<][=] {return INF_EQ;}
[>] {return SUP;}
[>][=] {return SUP_EQ;}

[=][=] {return EQ;}
[!][=] {return NOT_EQ;}

[|][|] {return OR;}
[&][&] {return AND;}


'i''f' ;
'f''o''r' ;
'r''e''t''u''r''n' ;
'b''r''e''a''k' ;
'c''o''n''t''i''n''u''e' ;

'i''n''t'
'b''o''o''l''e''a''n' ;



[[:space:]] ;

. {
    fprintf(stderr, "Caractère illégal (%d)\n", yytext[0]);
}

%%
