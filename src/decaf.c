#include "symTab.h"
#include "genCode.h"
#include "genMips.h"

extern int yyparse();
extern int yydebug; // pour l'option -t (trace) de bison
extern FILE* yyin;
extern FILE* yyout;

void usage()
{
    fprintf(stderr, "usage : decaf infile [-version] [-tos] [-o outfile]\n");
    exit(1);
}

int main(int argc, char** argv)
{
    int afficher_table = 0;
    yydebug = 0; // 1: activer - 0: désactiver
    yyout = NULL;
    yyin = NULL;
    if (argc<2)
        usage();
    
    //Arguments
    for (int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"-version") == 0)
        {
            printf("Membres :\n");
            printf("Antoine DUMOULIN, Antoine PIERRE, Mickaël DA SILVA, Florent LECOULTRE\n");
        }
        else if(strcmp(argv[i],"-tos") == 0)
        {
            afficher_table = 1;
        }
        else if(strcmp(argv[i], "-o") == 0)
        {
            i++;
            if (argc <= i)
            {
                fprintf(stderr, "no output file\n");
                usage();
            }
            yyout=fopen(argv[i], "w");
        }
        else
        {
            if (yyin != NULL)
                usage();
            yyin = fopen(argv[i], "r");
        }
    }
    if (yyin == NULL)
        usage();
    if (yyout == NULL)
    {
        yyout=fopen("out.asm", "w");
    }
    
    //initialise table des symboles
    initGenCode();
    initST();

    int r = yyparse();

    if (r != 0)
        return 1;

    genMips(yyout);
    if (afficher_table)
        afficheAllST();
    printf("\n");
    afficheGenCode();
    depilerST();
    
    fclose(yyin);
    fclose(yyout);
    return 0;
}
