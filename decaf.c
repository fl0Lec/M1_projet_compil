#include "symTab.h"
extern int yyparse();
extern int yydebug; // pour l'option -t (trace) de bison

int main()
{
    yydebug = 0; // 1: activer - 0: désactiver
    int r = yyparse();
    return r;
}
