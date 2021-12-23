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
        //Arguments
        for (int i = 2; i < argc; i++) //pas sûr de faire commencer i à 2
        {
            if(strcmp(argv[i],"-version") == 0)
            {
                printf("Menbres :\n");
                printf("Antoine DUMOULIN, Antoine PIERRE, Mickaël DA SILVA Florent LECOULTRE\n");
            }
            if(strcmp(argv[i],"-tos") == 0)
            {
                printf("BOUCHON : afficher la table des symboles\n");
            }

            if(strcmp(argv[i], "-o") == 0)
            {
                printf("Ecriture du code dans les fichier %s\n", argv[i+1]);
                yyout=fopen(argv[i+1], "w");
            }
        }
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
