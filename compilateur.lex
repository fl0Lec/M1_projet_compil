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
    return ID;
}

[[:space:]] ;

. {
    fprintf(stderr, "Caractère illégal (%d)\n", yytext[0]);
}

%%
