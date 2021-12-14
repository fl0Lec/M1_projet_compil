#include "symTab.h"
#include "genCode.h"
#include "genMips.h"

extern int yyparse();
extern int yydebug; // pour l'option -t (trace) de bison
extern FILE* yyin;
extern FILE* yyout;

int main(int argc, char** argv)
{
    yydebug = 1; // 1: activer - 0: désactiver
    if (argc<2){
        fprintf(stderr, "attend un nom de fichier\n");
    }
    yyin=fopen(argv[1], "r");
    if (argc<3){
        yyout=fopen("out.asm", "w");
    } else {
        yyout=fopen(argv[2], "w");
    }
    //initialise table des symboles
    empilerST();
    initGenCode();

    int r = yyparse();

    genMips(yyout);
    afficherST();
    printf("\n");
    afficheGenCode();
    depilerST();
    
    fclose(yyin);
    fclose(yyout);
    return r;
}
