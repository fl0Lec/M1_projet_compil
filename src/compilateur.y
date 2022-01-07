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

enum type last_type;

void error(char* msg)
{
    fprintf(stderr, "%s\n", msg);
    afficheGenCode();
    afficheAllST();
    exit(-1);
}

struct comb 
{
    struct list_addr* la;
    int quad;
};

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
        enum Operation op; struct fundesc* fundesc; struct list_addr* list_addr; enum Assign_op_type assign_op_type;
        struct comb* comb;}
        
%token SEMICOLON

/* LITERALS */
%token ID
%token CLASS
%token INT_TYPE
%token BOOL_TYPE
%token STRING_TYPE
%token VOID_TYPE

%token <val> INT
%token <val> HEXA
%token <val> CHAR
%token <mot> STRING
%token BOOL
%token <val> FALSE
%token <val> TRUE

/* ASSIGN */
%token ASSIGN
%token ASSIGN_PLUS
%token ASSIGN_SUB

/* BOOLEAN OPERATIONS */
%token INF_EQ
%token SUP_EQ
%token INF
%token SUP
%token EQ
%token NOT_EQ
%left OR
%left AND

/* OPERATIONS et EXPRESSIONS */

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


/* CONTROL STRUCTURES */
%token IF
%token FOR
%token RETURN
%token BREAK
%token CONTINUE
%token ELSE

/* TYPES */
%type <mot> ID
%type <val> int_literal
%type <val> char_literal
%type <val> bool_literal
%type <mot> string_literal
%type <type> type


%type <id> location
%type <id> expr
%type <id> literal

%type <op> rel_op
%type <op> cond_op
%type <op> eq_op
%type <op> arith_op
%type <op> bin_op
%type <assign_op_type> assign_op
%type <val> next_ligne

%type <id> method_call
%type <fundesc> method_call_args

%type<fundesc> method_decl_param

%type <list_addr> empile_fun

%type <comb> else_bloc
%type <comb> genGoto

%type <list_addr> block statement list_statement

%start program

%%

program : CLASS ID check_program ACO_O declaration list_method_decl ACO_C

check_program : %empty {
    // si il y a quelque chose à faire du nom du programme (même nom que le fichier ...)
}
;

declaration : 
type ID add_id_imm SEMICOLON declaration 
| type ID add_id_imm COMA liste_id SEMICOLON declaration 
| VOID_TYPE ID PAR_O  empile_fun method_decl_param PAR_C block {
    $5->context=symTab;
    depilerST();
    $5->ret=VOID_T; 
    struct symbole*s=addST_fun($2, $5); 
    completeLabel($4, s);}
| type ID PAR_O empile_fun method_decl_param PAR_C block {
    $5->context=symTab;
    depilerST(); 
    $5->ret=$1; 
    struct symbole *s=addST_fun($2, $5); 
    completeLabel($4, s);}
;

add_id_imm : %empty {addST_id(yylval.mot, last_type);}
;

empile_fun : %empty {empilerST(); $$=creerlist(genCode.size); gencode(label, NULL, NULL, NULL);}
;

list_field_decl : %empty {}
| list_field_decl field_decl
;

liste_id: %empty {}
| ID add_id_imm 
| ID add_id_imm COMA liste_id
;

list_method_decl : %empty
| list_method_decl method_decl
;

field_decl : type liste_id SEMICOLON {
    if ($1 == VOID_T)
        yyerror("id de type VOID illégal");
}
; 

method_decl : VOID_TYPE ID empile_fun PAR_O method_decl_param PAR_C block {
    $5->context=symTab;
    depilerST(); 
    $5->ret=VOID_T; 
    struct symbole* s=addST_fun($2, $5); 
    completeLabel($3, s);
    }
| type ID empile_fun PAR_O method_decl_param PAR_C block  {
    $5->context=symTab;
    depilerST();
    $5->ret=$1;
    struct symbole* s=addST_fun($2, $5); 
    completeLabel($3, s);}
;

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

block : ACO_O empile list_field_decl list_statement  depile ACO_C {$$=$4;}
;

empile : %empty {empilerST();}
;

depile : %empty {depilerST();}
;

list_statement : %empty {$$=NULL;}
| statement list_statement {$$=concat($1, $2);}
;

statement : 
location assign_op expr SEMICOLON {
    $$=NULL;
    //verifie que l'on a bien location et expr
    if (!$1)
        error("location NULL\n");
    if (!$3)
        error("expr NULL\n");
    if ($1->type.type==BOOL_T && $3->kind!=EXPR_B)
        error("assignement entre variable bool et expression bool");
    else if ($1->type.type!=BOOL_T && $1->type.type!=$3->type.type){
        afficheSymb($1);
        afficheSymb($3);
        error("assignement entre variable de different type");
    }
    
    if ($1->type.type==BOOL_T){
        complete($3->true, genCode.size);
        complete($3->false, genCode.size+1);
        gencode(store, addST_constInt(1, INT_T), NULL, $1);
        gencode(store, addST_constInt(0, INT_T), NULL, $1);
    }
    else {
        //reste a faire different assign_op
        switch ($2) {
            case ADD_ASSIGN:
                gencode(add, $1, $3, $1);
            break;
            case SUB_ASSIGN:
                gencode(sub, $1, $3, $1);
            break;
            default:
                gencode(store, $3, NULL, $1);
            break;
        }
    }
}
| method_call SEMICOLON {gencode(call, $1, NULL, NULL);}
| IF PAR_O expr PAR_C next_ligne block else_bloc {
    $$=$6;
    afficheLA($3->true);
    afficheLA($3->false);
    complete($3->true, $5);
    //printf("%d\n", $7);
    if ($7){
        complete($3->false, $7->quad);
        complete($7->la, genCode.size);

    } else {
        complete($3->false, genCode.size);
    } 

}
| FOR ID ASSIGN expr COMA expr block {$$=NULL;}
| RETURN SEMICOLON      {$$=NULL;gencode(ret, NULL, NULL, NULL);}
| RETURN expr SEMICOLON {$$=NULL;gencode(ret, NULL, NULL, $2);}
| BREAK SEMICOLON       {$$=creerlist(genCode.size); gencode(goto_op, 0,0,0);}
| CONTINUE SEMICOLON    {$$=NULL;}
| block
;

else_bloc : %empty {$$=0;}
| ELSE genGoto next_ligne block {
    $$=$2;
    $$->la=concat($$->la, $4);
    $$->quad=genCode.size;
    }
;

genGoto : %empty {$$=malloc(sizeof(struct comb)); $$->la=creerlist(genCode.size); gencode(goto_op, 0, 0, 0);}
;
type 
: INT_TYPE {$$ = INT_T; last_type = $$;}
| BOOL_TYPE {$$ = BOOL_T; last_type = $$;}
;

assign_op
: ASSIGN {$$=NORMAL_ASSIGN;}
| ASSIGN_PLUS {$$=ADD_ASSIGN;}
| ASSIGN_SUB {$$=SUB_ASSIGN;}
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
    struct symbole* s=lookupST($1);
    if (!s || s->kind!=FUN || s->type.desc->nbArg!=0)
        error("erreur appel fonction nom ou pas d'argument");
    $$=s;
}   // procédure
| ID PAR_O method_call_args PAR_C {
    struct symbole* s=lookupST($1);
    if (!compfundesc(s->type.desc, $3))
        error("erreur argument different");
    $$=s;
    
} // fonction
;

method_call_args
: method_call_args COMA expr {$$=$1; addtypefd($$, $3->type.type); gencode(param, $3, NULL, NULL);}
| expr {$$=initfun(); addtypefd($$, $1->type.type); gencode(param, $1, NULL, NULL);}
;

expr
: location  {$$=$1;}
| method_call {
    if ($1->type.desc->ret==VOID_T)
        error("fonction sans retour dans expression");
    $$=newtemp();
    $$->type.type=$1->type.desc->ret;
    gencode(call, $1, NULL, $$);
}
| literal   {$$=$1;}
| expr bin_op next_ligne expr {//shif reduce ici
    struct symbole* t;
    //switch sur les differentes operations binaires
    switch($2){
        //arith_op
        case add:
        case sub:
        case mul:
        case divi:
        case mod: ;
            t=newtemp();
            if (!$1 || $1->type.type!=INT_T || !$4 || $4->type.type!=INT_T)
                error("erreur de type doit être de type int");
            t->type.type=INT_T;
            gencode($2, $1, $4, t);
            $$=t;
            break;
        //rel_op et eq_op
        case inf:
        case infeq:
        case sup:
        case supeq:
        case eq:
        case noteq: ;
            t=addST_exprbool();
            if (!$1 || $1->type.type!=INT_T || !$4 || $4->type.type!=INT_T)
                error("erreur de type doit être de type int");
            t->true=creerlist(genCode.size);
            t->false=creerlist(genCode.size+1);
            gencode($2, $1, $4, NULL);
            gencode(goto_op, NULL, NULL, NULL);
            $$=t;
            break;
        case and:
            if (($1->kind==IDENT) && ($1->type.type=BOOL_T)){
                struct symbole* s=addST_exprbool();
                s->true=creerlist(genCode.size);
                s->false=creerlist(genCode.size+1);
                gencode(eq, $1, addST_constInt(1, INT_T),0);
                gencode(goto_op, 0, 0, 0);
                $1=s;
            }
            if (($4->kind==IDENT) && ($4->type.type=BOOL_T)){
                struct symbole* s=addST_exprbool();
                s->true=creerlist(genCode.size);
                s->false=creerlist(genCode.size+1);
                gencode(eq, $4, addST_constInt(1, INT_T),0);
                gencode(goto_op, 0, 0, 0);
                $4=s;
            }
            if (!$1 || $1->kind!=EXPR_B || !$4 || $4->kind!=EXPR_B)
                error("erreur de type doit etre de type expression bool ou boolean");
            complete($1->true, $3);
            $$=$4;
            $$->false=concat($4->false, $1->false);
        case or :
            if (($1->kind==IDENT) && ($1->type.type=BOOL_T)){
                struct symbole* s=addST_exprbool();
                s->true=creerlist(genCode.size);
                s->false=creerlist(genCode.size+1);
                gencode(eq, $1, addST_constInt(1, INT_T),0);
                gencode(goto_op, 0, 0, 0);
                $1=s;
            }
            if (($4->kind!=FUN) && ($4->type.type==BOOL_T)){
                struct symbole* s=addST_exprbool();
                s->true=creerlist(genCode.size);
                s->false=creerlist(genCode.size+1);
                gencode(eq, $4, addST_constInt(1, INT_T),0);
                gencode(goto_op, 0, 0, 0);
                $4=s;
            }
            if (!$1 || $1->kind!=EXPR_B || !$4 || $4->kind!=EXPR_B){
                afficheSymb($1);
                afficheSymb($4);
                error("erreur de type doit etre de type expression bool");
            }
            complete($1->false, $3);
            $$=$4;
            $$->true=concat($$->true, $1->true);

        default :
            break;
    }
}
| SUB expr {
    $$=newtemp();
    $$->type.type=INT_T;
    if (!$2 || $2->type.type!=INT_T)
        error("erreur de type doit être int");
    gencode(subun, $2, 0, $$);}
| NOT expr  {
    struct symbole *t;
    if ($2->kind==EXPR_B){
    struct list_addr *la=$2->true;
    $$=$2;
    $$->true=$$->false;
    $$->false=la;
    }
    else if ($2->kind!=IDENT && $2->type.type!=BOOL_T){
        t=addST_exprbool();
        t->true=creerlist(genCode.size+1);
        t->false=creerlist(genCode.size);
        gencode(eq, $2, addST_constInt(1, INT_T), NULL);
        gencode(goto_op, NULL, NULL, NULL);
    }
    else 
        error("\"!\" attend une expression boolean");
}
| PAR_O expr PAR_C  {$$=$2;}
;

next_ligne : %empty {$$=genCode.size;}
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
: OR {$$=or;}
| AND {$$=and;}
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
