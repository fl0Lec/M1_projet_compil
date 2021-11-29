#include <stdio.h>
#include "symTab.h"
extern int yyparse();
extern int yydebug; // pour l'option -t (trace) de bison
extern FILE* yyin;
extern FILE* yyout;

int main(int argc, char** argv)
{
    yydebug = 0; // 1: activer - 0: désactiver
    if (argc<2){
        fprintf(stderr, "attend un nom de fichier\n");
    }
    yyin=fopen(argv[1], "r");
    if (argc<3){
        yyout=fopen("out.mips", "w");
    } else {
        yyout=fopen(argv[2], "w");
    }
    empilerST();

    int r = yyparse();


    afficherST();
    depilerST();
    return r;
}
