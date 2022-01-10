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
    exit(1);
}

struct comb 
{
    struct list_addr* la;
    struct symbole* s;
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



/* OPERATIONS et EXPRESSIONS */

%token PAR_O
%token PAR_C
%token ACO_O
%token ACO_C
%token COMA
%token CRO_O
%token CRO_F


/* CONTROL STRUCTURES */
%token IF
%token FOR
%token RETURN
%token BREAK
%token CONTINUE
%token ELSE

/* PRIORITE */
%left AND
%left OR
%left EQ NOT_EQ
%left SUP SUP_EQ INF INF_EQ
%left PLUS SUB
%left MULT DIV MOD
%nonassoc NOT
%left PAR_O PAR_C COMA

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


%type <assign_op_type> assign_op
%type <val> next_ligne

%type <id> method_call
%type <fundesc> method_call_args

%type<fundesc> method_decl_param

%type <id> add_fun_imm
%type <list_addr> empile_fun

%type <comb> else_bloc
%type <comb> genGoto
%type <comb> assign_for

%type <list_addr> block statement list_statement
%type <id> add_tab_imm

%start program

%%

program : CLASS ID check_program ACO_O declaration list_method_decl ACO_C 

check_program : %empty {
    // si il y a quelque chose à faire du nom du programme (même nom que le fichier ...)
}
;

declaration : 
type ID add_id_imm SEMICOLON declaration 
| type ID add_id_imm COMA liste_id_init SEMICOLON declaration 
| type ID add_tab_imm CRO_O int_literal CRO_F SEMICOLON declaration {
    if ($5<1)
        error("tableau decalration minimum 1");
    $3->u.val=$5;
}
| type ID add_tab_imm CRO_O int_literal CRO_F COMA liste_id_init SEMICOLON declaration {
    if ($5<1)
        error("tableau declaration minimum 1");
    $3->u.val=$5;
}
| VOID_TYPE ID PAR_O add_fun_imm  empile_fun method_decl_param PAR_C empile block depile {
    $6->context=symTab;
    depilerST();
    $6->ret=VOID_T; 
    $4->type.desc=$6;
    completeLabel($5, $4);
    gencode(ret, 0, 0, 0);
    }
| type ID PAR_O add_fun_imm empile_fun method_decl_param PAR_C empile block depile {
    $6->context=symTab;
    depilerST(); 
    $6->ret=$1; 
    $4->type.desc=$6;
    completeLabel($5, $4);
    }
;

add_id_imm : %empty {addST_id(yylval.mot, last_type);}
;

add_tab_imm : %empty {
    $$=addST_tab(yylval.mot, last_type, 5);
    printf("tab : %s\n", yylval.mot);
}
;

add_fun_imm : %empty {$$=addST_fun(yylval.mot, 0);}
;
empile_fun : %empty {empilerST(); $$=creerlist(genCode.size); gencode(label, NULL, NULL, NULL);}
;

list_field_decl : %empty {}
| list_field_decl field_decl
;

liste_id_init : %empty 
| ID add_id_imm 
| ID add_id_imm COMA liste_id_init
| ID add_tab_imm CRO_O int_literal CRO_F{
    if ($4<1)
        error("tableau decalration minimum 1");
    $2->u.val=$4;   
}
| ID add_tab_imm CRO_O int_literal CRO_F COMA liste_id_init {
    if ($4<1)
        error("tableau decalration minimum 1");
    $2->u.val=$4;   
}
;

liste_id: %empty {}
| ID add_id_imm {gencode(store, addST_constInt(0, INT_T),0, lookupST($1));}
| ID add_id_imm COMA liste_id {gencode(store, addST_constInt(0, INT_T),0, lookupST($1));}
;

list_method_decl : %empty
| list_method_decl method_decl
;

field_decl : type liste_id SEMICOLON {
    if ($1 == VOID_T)
        yyerror("id de type VOID illégal");
}
; 

method_decl : VOID_TYPE ID add_fun_imm empile_fun PAR_O method_decl_param PAR_C empile block depile {
    $6->context=symTab;
    depilerST(); 
    $6->ret=VOID_T; 
    $3->type.desc=$6;
    completeLabel($4, $3);
    gencode(ret, 0, 0, 0);
    if ($9!=NULL){
        error("break hors boucle detecter");
    }
    }
| type ID add_fun_imm empile_fun PAR_O method_decl_param PAR_C empile block depile {
    $6->context=symTab;
    depilerST();
    $6->ret=$1;
    $3->type.desc=$6;
    completeLabel($4, $3);
    gencode(ret, 0, 0, 0);
}
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

block : ACO_O list_field_decl list_statement  ACO_C {$$=$3;}
;

empile : %empty {empilerST();}
;

depile : %empty {depilerST();}
;

list_statement : %empty    {$$=NULL;}
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
        error("assignement entre variable bool et expression bool necessaire");
    else if ($1->type.type!=BOOL_T && $1->type.type!=$3->type.type){
        afficheSymb($1);
        afficheSymb($3);
        error("assignement entre variable de different type");
    }
    else if ($1->type.type==BOOL_T && $2!=NORMAL_ASSIGN){
        error("seulement l'affectation est valide pour les boolean");
    }
    
    if ($1->type.type==BOOL_T && $3->kind!=CST_INT){
        complete($3->true, genCode.size);
        complete($3->false, genCode.size+2);
        gencode(store, addST_constInt(1, INT_T), NULL, $1);
        gencode(goto_op, 0, 0, addST_constInt(genCode.size+2, INT_T));
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
| method_call SEMICOLON {$$=0; gencode(call, $1, NULL, NULL);}
| IF PAR_O expr PAR_C next_ligne empile block depile else_bloc { //$6 -> $7 $7->$9
    if ($3->kind!=EXPR_B)
        error("expression de type incorrect dans if\n");
    $$=$7;
    complete($3->true, $5);
    if ($9){
        complete($3->false, $9->quad);
        completeFirst($9->la, genCode.size);
        $$=concat($$, $9->la);

    } else {
        complete($3->false, genCode.size);
    } 
}
| FOR assign_for empile block {
    gencode(add, addST_constInt(1, INT_T), $2->s, $2->s);
    gencode(goto_op, 0, 0, addST_constInt($2->quad, INT_T));
    complete($2->la, genCode.size);
    depilerST();
    complete($4, genCode.size); // TODO SEGFAULT ($4 = NULL?)
    $$=0;
}
| RETURN SEMICOLON      {$$=NULL;gencode(ret, NULL, NULL, NULL);}
| RETURN expr SEMICOLON {
    $$=NULL;
    if ($2->kind!=EXPR_B)
        gencode(ret, NULL, NULL, $2);
    else {
        complete($2->true, genCode.size);
        complete($2->false, genCode.size);
        gencode(ret, NULL, NULL, addST_constInt(1,INT_T));
        gencode(ret, NULL, NULL, addST_constInt(0,INT_T));
    }}
| BREAK SEMICOLON       {$$=creerlist(genCode.size); gencode(goto_op, 0,0,0);}
| CONTINUE SEMICOLON    {$$=NULL;}
| empile block depile   {$$=$2;}
;

else_bloc : %empty {$$=0;}
| ELSE genGoto next_ligne empile block depile {
    $$=$2;
    $$->la=concat($$->la, $5);
    $$->quad=$3;
    }
;

genGoto : %empty {$$=malloc(sizeof(struct comb)); $$->la=creerlist(genCode.size); gencode(goto_op, 0, 0, 0);}
;

assign_for : ID ASSIGN expr COMA expr {
    $$=malloc(sizeof(struct comb));
    empilerST(); 
    struct symbole* s=addST_id($1, INT_T);
    $$->s=s;
    if ($3->type.type!=INT_T)
        error("for attend valeur entiere");
    gencode(store, $3, 0, s);
    struct symbole *t=newtemp();
    gencode(store, $5, 0, t);
    $$->quad=genCode.size;
    $$->la=creerlist(genCode.size);
    gencode(sup, s, t, 0);
    }
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
        exit(1);
    }
    if ($$->kind==FUN){
        fprintf(stderr, "%s ",$1);
        error("location ne peut pas être un identificateur de fonction\n");
    }
}
| ID CRO_O expr CRO_F {
    struct symbole* s=lookupST($1);
    printf("location tab\n");
    if (!s){
        afficherST();
        fprintf(stderr, "no entry in table for %s\n", $1);
        exit(1);
    }
    if (s->kind!=TAB){
        fprintf(stderr, "%s", $1);
        error(" attend un tableau avant crochet");
    }
    if ($3->type.type!=INT_T){
        fprintf(stderr, "%s", $1);
        error(" attend entier dans crochet");
    }
    $$=newtemp();
    $$->type.type=s->type.type;
    $$->kind=TEMPO_TAB;
    gencode(loadT, s, $3, $$);
}
;

method_call
: ID PAR_O PAR_C {
    struct symbole* s=lookupST($1);
    if (!s){
        fprintf(stderr, "pour %s ",$1);
        error("identificateur de fonction non trouver");
    }
    if (s->kind!=FUN){
        fprintf(stderr, "%s :", $1);
        error("erreur attend une fonction");
    }
    if (s->type.desc->nbArg!=0){
        fprintf(stderr, "%s ", $1);
        error("n'as pas d'argument");
    }
    $$=s;
}   // procédure
| ID PAR_O method_call_args PAR_C {
    struct symbole* s=lookupST($1);
    if (!s){
        fprintf(stderr, "pour %s ",$1);
        error("identificateur de fonction non trouver");
    }
    if (s->type.desc->nbArg!=-1 && !compfundesc(s->type.desc, $3)){
        affichefundesc($3);
        (s?affichefundesc(s->type.desc):0);
        fprintf(stderr,"%s : ", $1);
        error("erreur argument different");
    }
    $$=s;
    
} // fonction
;

method_call_args
: method_call_args COMA expr {
    struct symbole* t;
    $$=$1;
    if ($3->kind==EXPR_B){
        t=newtemp();
        t->type.type=BOOL_T;
        complete($3->true, genCode.size);
        complete($3->false, genCode.size+2);
        gencode(store, addST_constInt(1, INT_T), NULL, t);
        gencode(goto_op, 0, 0, addST_constInt(genCode.size+2, INT_T));
        gencode(store, addST_constInt(0, INT_T), NULL, t);
        $3=t;

    }
    addtypefd($$, $3->type.type);
    gencode(param, $3, NULL, NULL);
    }
| expr {$$=initfun(); addtypefd($$, $1->type.type); gencode(param, $1, NULL, NULL);}
;

expr
: location  {
    if ($1->type.type==BOOL_T){
        $$=addST_exprbool();
        $$->true=creerlist(genCode.size);
        $$->false=creerlist(genCode.size+1);
        gencode(eq, $1, addST_constInt(1,INT_T), 0);
        gencode(goto_op, 0,0,0);
    }
    else 
        $$=$1;
        }
| method_call {
    if ($1->type.desc->ret==VOID_T)
        error("fonction sans retour dans expression");
    $$=newtemp();
    $$->type.type=$1->type.desc->ret;
    gencode(call, $1, NULL, $$);
}
| literal   {
    if ($1->type.type==BOOL_T){
        $$=addST_exprbool();
        $$->true=creerlist(genCode.size);
        $$->false=creerlist(genCode.size+1);
        gencode(eq, $1, addST_constInt(1,INT_T), 0);
        gencode(goto_op, 0,0,0);
    }
    else 
        $$=$1;
        }
| expr PLUS expr {
    struct symbole* t;
    t=newtemp();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->type.type=INT_T;
    gencode(add, $1, $3, t);
    $$=t;
}
| expr SUB expr {
    struct symbole* t;
    t=newtemp();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->type.type=INT_T;
    gencode(sub, $1, $3, t);
    $$=t;
}
| expr MULT expr {
    struct symbole* t;
    t=newtemp();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->type.type=INT_T;
    gencode(mul, $1, $3, t);
    $$=t;
}
| expr DIV expr {
    struct symbole* t;
    t=newtemp();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->type.type=INT_T;
    gencode(divi, $1, $3, t);
    $$=t;
}
| expr MOD expr {
    struct symbole* t;
    t=newtemp();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->type.type=INT_T;
    gencode(mod, $1, $3, t);
    $$=t;
    //////////////////////////rel_op
}
|expr INF expr {
    struct symbole* t;
    t=addST_exprbool();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->true=creerlist(genCode.size);
    t->false=creerlist(genCode.size+1);
    gencode(inf, $1, $3, NULL);
    gencode(goto_op, NULL, NULL, NULL);
    $$=t;
}
| expr INF_EQ expr {
    struct symbole* t;
    t=addST_exprbool();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->true=creerlist(genCode.size);
    t->false=creerlist(genCode.size+1);
    gencode(infeq, $1, $3, NULL);
    gencode(goto_op, NULL, NULL, NULL);
    $$=t;
}
| expr SUP expr {
    struct symbole* t;
    t=addST_exprbool();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->true=creerlist(genCode.size);
    t->false=creerlist(genCode.size+1);
    gencode(sup, $1, $3, NULL);
    gencode(goto_op, NULL, NULL, NULL);
    $$=t;
}
| expr SUP_EQ expr {
    struct symbole* t;
    t=addST_exprbool();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->true=creerlist(genCode.size);
    t->false=creerlist(genCode.size+1);
    gencode(supeq, $1, $3, NULL);
    gencode(goto_op, NULL, NULL, NULL);
    $$=t;
}
| expr EQ expr {
    struct symbole* t;
    t=addST_exprbool();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->true=creerlist(genCode.size);
    t->false=creerlist(genCode.size+1);
    gencode(eq, $1, $3, NULL);
    gencode(goto_op, NULL, NULL, NULL);
    $$=t;
}
| expr NOT_EQ expr {
    struct symbole* t;
    t=addST_exprbool();
    if (!$1 || $1->type.type!=INT_T || !$3 || $3->type.type!=INT_T)
        error("erreur de type doit être de type int");
    t->true=creerlist(genCode.size);
    t->false=creerlist(genCode.size+1);
    gencode(noteq, $1, $3, NULL);
    gencode(goto_op, NULL, NULL, NULL);
    $$=t;
    /////////////////and-or
}

|expr AND next_ligne expr {
    if (($1->kind!=FUN) && ($1->type.type=BOOL_T)){
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
                error("erreur de type doit etre de type expression bool ou boolean");
            }
            complete($1->true, $3);
            $$=$4;
            $$->false=concat($4->false, $1->false);

}
|expr OR next_ligne expr {
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
}
| SUB expr {
    $$=newtemp();
    $$->type.type=INT_T;
    if (!$2 || $2->type.type!=INT_T)
        error("erreur de type doit être int");
    gencode(sub, addST_constInt(0, INT_T), $2, $$);}
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

literal
: int_literal {$$ = addST_constInt($1, INT_T);}
| char_literal { $$ = addST_constInt($1, INT_T);}
| string_literal {$$=addST_constStr($1);}
| bool_literal {
    $$=addST_constInt($1, BOOL_T);
    }
;

int_literal
: INT {$$ = $1;}
| HEXA {$$ = $1;}
;

bool_literal
: TRUE {$$ = 1;}
| FALSE {$$ = 0;}
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
