%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symTab.h"
#include "genCode.h"
#include "tabD.h"

#define WRITE(msg) fwrite(msg, strlen(msg),1, yyout);
extern int yylex();
extern FILE* yyout;

void yyerror(const char *msg);

/**
 *  fonctions à utiliser dans le code des grammaires
 */

%}

%define parse.error verbose

%code requires{
    #include "symTab.h"
    #include "genCode.h"
    #include "tabD.h"
}

%union {int val; char* mot; enum type type; struct tab* tabID; struct symbole* id; 
        enum Operation op; struct fundesc* fundesc;}

%token SEMICOLON

/* LITERALS */
%token ID
%token CLASS
%token INT_TYPE
%token BOOL_TYPE
%token VOID_TYPE

%token <val> INT
%token <val> HEXA // ? si on traduit en base 10 en C ou pas
%token <val> CHAR
%token <mot> STRING
%token BOOL
%token <val> FALSE
%token <val> TRUE

/* ASSIGN */
%token ASSIGN
%token ASSIGN_PLUS
%token ASSIGN_SUB

/* OPERATIONS et EXPRESSIONS */
%nonassoc USUB // unaire
%left PLUS
%left SUB
%left MULT
%left DIV
%left MOD
%nonassoc NOT
%token PAR_O
%token PAR_C
%token ACO_O
%token ACO_C
%token COMA

// TODO pas tout de suite
%token INF_EQ
%token SUP_EQ
%token INF
%token SUP
%token EQ
%token NOT_EQ
%left OR
%left AND

%token IF
%token FOR
%token RETURN
%token BREAK
%token CONTINUE
%token ELSE

%type <mot> ID
%type <val> int_literal
%type <val> char_literal
%type <val> bool_literal
%type <mot> string_literal
%type <type> type

%type <tabID> liste_id

%type <id> location
%type <id> expr
%type <id> literal

%type <op> rel_op
%type <op> cond_op
%type <op> eq_op
%type <op> arith_op
%type <op> bin_op

%type <id> method_call

%type<fundesc> method_decl_param

%start program

%%

program : CLASS ID check_program ACO_O declaration list_method_decl ACO_C

check_program : %empty {
    // si il y a quelque chose à faire du nom du programme (même nom que le fichier ...)
}
;

declaration : 
type ID SEMICOLON declaration {addST_id($2, $1);}
| type ID COMA liste_id SEMICOLON declaration {
    addST_id($2, $1);
    for (int i=0; i<$4->current;i++){
        addST_id($4->s[i], $1);
    }
    freeTD($4);}
| VOID_TYPE ID PAR_O empile method_decl_param PAR_C block {depilerST(); $5->ret=VOID_T; addST_fun($2, VOID_T, $5);}
| type ID PAR_O empile method_decl_param PAR_C block {depilerST(); $5->ret=$1; addST_fun($2, VOID_T, $5);}
;

empile : %empty {empilerST();}
;

list_field_decl : %empty {}
| list_field_decl field_decl
;

liste_id: %empty {}
| ID {
    //initialise tableaux dynamique contenue dans liste_id 
    //ajoute l'identifiant dans le tableaux
    $$=initTD(); 
    addTD($$, $1, strlen($1));
    }
| liste_id COMA ID {
    //ajoute l'identifiant dans le tableaux
    addTD($$, $3, strlen($3));
    }
;

list_method_decl : %empty
| list_method_decl method_decl
;

field_decl : type liste_id SEMICOLON {
    if ($1 == VOID_T)
        yyerror("id de type VOID illégal");
    //ajout dans table des symboles la liste des identifiants dans liste_id
    for (int i=0; i<$2->current;i++){
        addST_id($2->s[i], $1);
    }
    freeTD($2);
}
; 

method_decl : VOID_TYPE empile ID PAR_O method_decl_param PAR_C block {depilerST(); $5->ret=VOID_T; addST_fun($3, VOID_T, $5);}
| type empile ID ACO_O method_decl_param ACO_C block  {depilerST();$5->ret=$1; addST_fun($3, VOID_T,$5);}

method_decl_param : %empty {$$=malloc(sizeof(struct fundesc)); $$->nbArg=$$->capacity=0;$$->args=0;}
|method_decl_param COMA type ID {
    if ($1->nbArg==$1->capacity){
        $1->capacity*=2;
        $1->args=realloc($1->args, sizeof(enum type)*$1->capacity);
    }
    $1->args[$1->nbArg++]=$3;
    $$=$1;
    addST_id($4, $3);
}
| type ID {
    $$=malloc(sizeof(struct fundesc));
    $$->capacity=10;
    $$->nbArg=0;
    $$->args=malloc(sizeof(enum type)*10);
    $$->args[$$->nbArg++]=$1;
    addST_id($2, $1);
}
;

block : ACO_O list_field_decl list_statement ACO_C


list_statement : %empty
| statement list_statement
;

statement : 
location assign_op expr SEMICOLON {
    //verifie que l'on a bien location et expr
    if (!$1)
        printf("location wrong\n");
    if (!$3)
        printf("expr wrong\n");
    else {
        //reste a faire different assign_op

        //store resultat expr dans location
        printf("genCode\n");
        gencode(store, $3, NULL, $1);
    }
}
| method_call SEMICOLON
| IF ACO_O expr ACO_C block ELSE block
| IF ACO_O expr ACO_C block
| FOR ID ASSIGN expr COMA expr block
| RETURN SEMICOLON
| RETURN expr SEMICOLON
| BREAK SEMICOLON
| CONTINUE SEMICOLON
| block
;

type 
: INT_TYPE {$$ = INT_T;}
| BOOL_TYPE {$$ = BOOL_T;}
;

assign_op
: ASSIGN
| ASSIGN_PLUS
| ASSIGN_SUB
;

location
: ID {
    //$1[strlen($1)-2]='\0';
    $$=lookupST($1);
    //verification si est dans la table des symboles
    if (!$$){
        afficherST();
        fprintf(stderr, "no entry in table for %s\n", $1);
        exit(-1);
    }
}
;

method_call
: ID PAR_O PAR_C {
    struct symbole* t=newtemp();
    t->type.desc=initfun();
    $$=t;
}   // procédure
| ID PAR_O method_call_args PAR_C {
    struct symbole* t=newtemp();
    t->type.desc=initfun();
    $$=t;
} // fonction
;

method_call_args
: method_call_args COMA
| expr
;

expr
: location  {$$=$1;}
| method_call {$$=$1;}
| literal   {$$=$1;}
| expr bin_op expr {//shif reduce ici
    struct symbole* t=newtemp();
    //switch sur les differentes operations binaires
    printf("expr\n");
    switch($2){
        case add:
            t->type.type=INT_T;
            gencode($2, $1, $3, t);
            break;
        case sub:
            t->type.type=INT_T;
            gencode($2, $1, $3, t);
            break;
        case mul:
            t->type.type=INT_T;
            gencode($2, $1, $3, t);
            break;
        case divi:
            t->type.type=INT_T;
            gencode($2, $1, $3, t);
            break;
        case mod:
            t->type.type=INT_T;
            gencode($2, $1, $3, t);
            break;
        default :
            break;
    }
    $$=t;
}
| SUB expr {$$=newtemp(); gencode(subun, $2, 0, $$);}
| NOT expr  {$$=NULL;}
| PAR_O expr PAR_C  {$$=$2;}
;

bin_op
: arith_op {$$=$1;}
| rel_op {$$=$1;}
| eq_op {$$=$1;}
| cond_op { {$$=$1;}}
;

arith_op
: PLUS {$$=add;}
| SUB {$$=sub;}
| MULT {$$=mul;}
| DIV {$$=divi;}
| MOD {$$=mod;}
;

rel_op
: INF {$$=inf;}
| INF_EQ {$$=infeq;}
| SUP {$$=sup;}
| SUP_EQ {$$=supeq;}
;

eq_op
: EQ {$$=eq;}
| NOT_EQ {$$=noteq;}
;

cond_op
: OR {}
| AND {}
;

literal
: int_literal {$$ = addST_constInt($1, INT_T);}
| char_literal { $$ = addST_constInt($1, INT_T);}
| string_literal {$$=addST_constStr($1);}
| bool_literal {
    $$=NULL;
    }
;

int_literal
: INT {$$ = $1;}
| HEXA {$$ = $1;}
;

bool_literal
: TRUE {$$ = $1;}
| FALSE {$$ = $1;}
;

char_literal
: CHAR {$$ = $1;}
;

string_literal
: STRING {$$=$1;}
;


%%

void yyerror(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}
