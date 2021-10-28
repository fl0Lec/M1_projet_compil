%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "compilateur.tab.h" 
%}

%option nounput
%option noyywrap

%%

[[:alpha:]]+ {
    yylval = toupper(yytext[0]);
    return ID;
}

[[:space:]] ;

. {
    fprintf(stderr, "Caractère illégal (%d)\n", yytext[0]);
}

%%
