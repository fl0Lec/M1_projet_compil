#include "genMips.h"

int currentParam = 0; // le nombre de paramètres déjà empilés avant call

// --------------------------------------
// generation de code mips par instruction
// --------------------------------------

void genLoad(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# load\n");
    fprintf(out, "lw %s, %s\n", instr.dst->u.id, instr.arg1->u.id);
}

void genLoadimm(struct code3add instr, FILE* out)   // TODO gerer si > 16 bits
{
    fprintf(out, "\n# load immediate\n");
    fprintf(out, "li %s, %d\n", instr.dst->u.id, instr.arg1->u.val);
}

// load les arguments pour une opération basique (add, sub...)
void genLoadForOP(struct code3add instr, FILE* out)
{
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "li $t0 %d\n", instr.arg1->u.val);
    break;
    case TEMPO_TAB:
        if (instr.arg1->table->prev == NULL)
            fprintf(out, "lw $t0 %s\n", instr.arg1->u.id);
        else
            fprintf(out, "lw $t0 %d($sp)\n", instr.arg1->location);
        fprintf(out, "lw $t0 ($t0)\n");
    break;
    default:
        if (instr.arg1->table->prev == NULL)
            fprintf(out, "lw $t0 %s\n", instr.arg1->u.id);
        else
            fprintf(out, "lw $t0 %d($sp)\n", instr.arg1->location);
    break;
    }

    switch (instr.arg2->kind)
    {
    case CST_INT:
        fprintf(out, "li $t1 %d\n", instr.arg2->u.val);
    break;
    case TEMPO_TAB:
        if (instr.arg1->table->prev == NULL)
            fprintf(out, "lw $t1 %s\n", instr.arg2->u.id);
        else
            fprintf(out, "lw $t1 %d($sp)\n", instr.arg2->location);
        fprintf(out, "lw $t1 ($t1)\n");
    break;
    default:
        if (instr.arg2->table->prev == NULL)
            fprintf(out, "lw $t1 %s\n", instr.arg2->u.id);
        else
            fprintf(out, "lw $t1 %d($sp)\n", instr.arg2->location);
    break;
    }
}
void genStoreForOp(struct code3add instr, FILE* out)
{
    if (instr.dst->kind == TEMPO_TAB) {
        if (instr.dst->table->prev == NULL)
            fprintf(out, "lw $t2 %s\n", instr.dst->u.id);
        else
            fprintf(out, "lw $t2, %d($sp)\n", instr.dst->location);
        fprintf(out, "sw $t0 ($t2)\n");
    } else if (instr.dst->table->prev == NULL)
        fprintf(out, "sw $t0 %s\n", instr.dst->u.id);
    else
        fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
}

void genStore(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# store\n");
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "li $t0, %d\n", instr.arg1->u.val);
    break;
    default:
        if (instr.arg1->table->prev == NULL)
            fprintf(out, "lw $t0 %s\n", instr.arg1->u.id);
        else
            fprintf(out, "lw $t0, %d($sp)\n", instr.arg1->location);
    break;
    }
    
    genStoreForOp(instr, out);
}

void genLoadT(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# load tab\n");
    fprintf(out, "la $t1, %s\n", instr.arg1->u.id);
    switch (instr.arg2->kind)
    {
    case CST_INT:
        fprintf(out, "li $t0, %d\n", instr.arg2->u.val);
    break;
    default:
        if (instr.arg2->table->prev == NULL)
            fprintf(out, "lw $t0 %s\n", instr.arg2->u.id);
        else
            fprintf(out, "lw $t0, %d($sp)\n", instr.arg2->location);
    break;
    }
    
    fprintf(out, "li $t2, 4\n");
    fprintf(out, "mult $t0 $t2\n");
    fprintf(out, "mflo $t0\n");
    fprintf(out, "addu $t0, $t0, $t1\n");
    
    if (instr.dst->table->prev == NULL)
        fprintf(out, "sw $t0 %s\n", instr.dst->u.id);
    else
        fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
}

void genAdd(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# add\n");

    genLoadForOP(instr, out);

    fprintf(out, "add $t0 $t0 $t1\n");

    genStoreForOp(instr, out);
}

void genSub(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# sub\n");

    genLoadForOP(instr, out);

    fprintf(out, "sub $t0 $t0 $t1\n");

    genStoreForOp(instr, out);
}

void genMul(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# mult\n");

    genLoadForOP(instr, out);

    fprintf(out, "mult $t0 $t1\n");
    fprintf(out, "mflo $t0\n");

    genStoreForOp(instr, out);
}

void genDivi(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# div\n");

    genLoadForOP(instr, out);

    fprintf(out, "div $t0 $t1\n");
    fprintf(out, "mflo $t0\n");
    
    genStoreForOp(instr, out);
}

void genMod(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# mod\n");

    genLoadForOP(instr, out);

    fprintf(out, "div $t0 $t1\n");
    fprintf(out, "mfhi $t0\n");
    
    genStoreForOp(instr, out);
}

void genEq(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# Equal\n");
    genLoadForOP(instr, out);
    
    switch (instr.dst->kind)
    {
    case CST_INT:
        fprintf(out, "beq $t0 $t1 line.%d\n", instr.dst->u.val);
        break;
    default:
        fprintf(out, "beq $t0 $t1 %s\n", instr.dst->u.id);
        break;
    }
}

void genNoteq(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# Not equal\n");
    genLoadForOP(instr, out);
    
    switch (instr.dst->kind)
    {
    case CST_INT:
        fprintf(out, "bne $t0 $t1 line.%d\n", instr.dst->u.val);
        break;
    default:
        fprintf(out, "bne $t0 $t1 %s\n", instr.dst->u.id);
        break;
    }
}

void genInf(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# Inferior\n");
    genLoadForOP(instr, out);
    
    switch (instr.dst->kind)
    {
    case CST_INT:
        fprintf(out, "blt $t0 $t1 line.%d\n", instr.dst->u.val);
        break;
    default:
        fprintf(out, "blt $t0 $t1 %s\n", instr.dst->u.id);
        break;
    }
}

void genInfeq(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# Inferior or equal\n");
    genLoadForOP(instr, out);
    
    switch (instr.dst->kind)
    {
    case CST_INT:
        fprintf(out, "ble $t0 $t1 line.%d\n", instr.dst->u.val);
        break;
    default:
        fprintf(out, "ble $t0 $t1 %s\n", instr.dst->u.id);
        break;
    }
}

void genSup(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# Superior\n");
    genLoadForOP(instr, out);
    
    switch (instr.dst->kind)
    {
    case CST_INT:
        fprintf(out, "bgt $t0 $t1 line.%d\n", instr.dst->u.val);
        break;
    default:
        fprintf(out, "bgt $t0 $t1 %s\n", instr.dst->u.id);
        break;
    }
}

void genSupeq(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# Superior or equal\n");
    genLoadForOP(instr, out);
    
    switch (instr.dst->kind)
    {
    case CST_INT:
        fprintf(out, "bge $t0 $t1 line.%d\n", instr.dst->u.val);
        break;
    default:
        fprintf(out, "bge $t0 $t1 %s\n", instr.dst->u.id);
        break;
    }
}

void genGoto(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# goto\n");
    if (!instr.dst){
        fprintf(stderr, "goto null detect break hors for ?\n");
        exit(1);
    }
    switch (instr.dst->kind)
    {
    case CST_INT:
        fprintf(out, "j line.%d\n", instr.dst->u.val);
        break;
    default:
        fprintf(out, "j %s\n", instr.dst->u.id);
        break;
    }
}

void genLabel(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# label\n");
    fprintf(out, "%s:\n", instr.dst->u.id);
    fprintf(out, "addiu $sp, $sp, -4\nsw $ra, 0($sp)\n"); // save return pointer
}

void genReturn(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# return\n");
    if (instr.dst!=NULL) {
        switch (instr.dst->kind)
        {
        case TEMPO_TAB:
            if (instr.dst->table->prev == NULL)
                fprintf(out, "lw $t2 %s\n", instr.dst->u.id);
            else
                fprintf(out, "lw $t2, %d($sp)\n", instr.dst->location);
            fprintf(out, "lw $v0 ($t2)\n");
        break;
        case CST_INT:
            fprintf(out, "li $v0, %d\n", instr.dst->u.val);
        break;
        default:
            if (instr.dst->table->prev == NULL)
                fprintf(out, "lw $v0 %s\n", instr.dst->u.id);
            else
                fprintf(out, "lw $v0, %d($sp)\n", instr.dst->location);
        break;
        }
    }
    fprintf(out, "lw $ra, 0($sp)\n");    // load return pointer
    fprintf(out, "addiu $sp, $sp, 4\n");
    fprintf(out, "jr $ra\n\n");
}

void genParam(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# param\n");
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "li $t0 %d\n", instr.arg1->u.val);
    break;
    case CST_STR:
        fprintf(out, "la $t0 %s\n", instr.arg1->u.id);
    break;
    case TEMPO_TAB:
        if (instr.arg1->table->prev == NULL)
            fprintf(out, "lw $t0 %s\n", instr.arg1->u.id);
        else
            fprintf(out, "lw $t0 %d($sp)\n", instr.arg1->location);
        fprintf(out, "lw $t0 ($t0)\n");
    break;
    default:
        if (instr.arg1->table->prev == NULL)
            fprintf(out, "lw $t0 %s\n", instr.arg1->u.id);
        else
            fprintf(out, "lw $t0 %d($sp)\n", instr.arg1->location);
    break;
    }

    // alignement dans la pile $sp
    int i;
    for (i=genCode.current; genCode.tab[i].op != call; i++); // il y a toujours un call après des param

    if(genCode.tab[i].arg1->type.desc->context != NULL)
        fprintf(out, "sw $t0, -%d($sp)\n", genCode.tab[i].arg1->type.desc->context->maxloc-currentParam*4);
    else // fonctions prédéfinies
        fprintf(out, "sw $t0, -4($sp)\n");
    currentParam ++;
}

void genCall(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# call\n");

    if (instr.arg1->type.desc->nbArg > 0)
        currentParam -= instr.arg1->type.desc->nbArg;

    if(instr.arg1->type.desc->context != NULL)
        fprintf(out, "subu $sp, $sp, %d\n", instr.arg1->type.desc->context->maxloc);
    else // fonctions prédéfinies
        fprintf(out, "addiu $sp, $sp, -4\n");

    fprintf(out, "jal %s\n", instr.arg1->u.id);

    if(instr.arg1->type.desc->context != NULL)
        fprintf(out, "addiu $sp, $sp, %d\n", instr.arg1->type.desc->context->maxloc);
    else // fonctions prédéfinies
        fprintf(out, "addiu $sp, $sp, 4\n");

    if (instr.dst != NULL) {    // store valeur de retour
        fprintf(out, "move $t0, $v0\n");
        genStoreForOp(instr, out);
    }

    if (strcmp("ReadInt", instr.arg1->u.id) == 0) {
        struct code3add param = genCode.tab[genCode.current-2];
        if (param.arg1->kind == TEMPO_TAB) {
            if (param.arg1->table->prev == NULL)
                fprintf(out, "sw $t0 %s\n", param.arg1->u.id);
            else
                fprintf(out, "sw $t0, %d($sp)\n", param.arg1->location);
            fprintf(out, "move $v0 $t0\n");
        }
        else if (param.arg1->table->prev == NULL)
            fprintf(out, "sw $v0 %s\n", param.arg1->u.id);
        else
            fprintf(out, "sw $v0 %d($sp)\n", param.arg1->location);
    }

}

// génère les fonctions d'entrées et sorties
void genIOFunctions(FILE* out)
{
    // print_string : print string en $a0
    fprintf(out, "\nWriteString:\n  lw $a0 0($sp)\n  subu $sp $sp 4\n  sw $ra 0($sp)\n  li $v0 4\n  syscall\n  lw $ra 0($sp)\n  addiu $sp, $sp, 4\n  jr $ra\n");
    // print_int : print int en $a0
    fprintf(out, "\nWriteInt:\n  lw $a0 0($sp)\n  subu $sp $sp 4\n  sw $ra 0($sp)\n  li $v0 1\n  syscall\n  lw $ra 0($sp)\n  addiu $sp, $sp, 4\n  jr $ra\n");
    //read_int : read int vers $v0
    fprintf(out, "\nReadInt:\n  subu $sp $sp 4\n  sw $ra 0($sp)\n  li $v0 5\n  syscall\n  lw $ra 0($sp)\n  addiu $sp, $sp, 4\n  jr $ra\n");
}

// --------------------------------------
void genMips(FILE* out)
{
    genCode.current = 0;
    struct code3add instr;
    struct symbole s;

    fprintf(out, "#start program\n");

    fprintf(out, "\n.data\n");
    for (; symTab->prev!=NULL; symTab = symTab->prev);
    for (size_t i=0;i<symTab->size;i++){
        s=symTab->symb[i];
        if (s.kind==IDENT)
            fprintf(out, "%s:\t.word 0\n",s.u.id);
        else if (s.kind == CST_STR) 
            fprintf(out, "%s:\t.asciiz %s\n",s.u.id, s.u.str);
        else if (s.kind == TAB) {
            fprintf(out, "%s:\t.word 0",s.u.id);
            for (int i=1; i<s.u.val; i++) {
                fprintf(out, ", 0");
            }
            fprintf(out, "\n");
        }
    }

    fprintf(out, "\n.text\n.globl main\njal main\nj program.exit\n\n");
    
    genIOFunctions(out);
    
    fprintf(out, "\n\n");
    while(genCode.current < genCode.size)
    {
        fprintf(out, "\nline.%ld:", genCode.current);
        instr = genCode.tab[genCode.current++];
        //printf("line %d\n", genCode.current);
        switch (instr.op) {
            case load: 
                genLoad(instr, out);
            break;
            case loadimm: 
                genLoadimm(instr, out);
            break;
            case store: 
                genStore(instr, out);
            break;
            case add: 
                genAdd(instr, out);
            break;
            case sub: 
                genSub(instr, out);
            break;
            case mul: 
                genMul(instr, out);
            break;
            case divi: 
                genDivi(instr, out);
            break;
            case mod: 
                genMod(instr, out);
            break;
            case eq: 
                genEq(instr, out);
            break;
            case noteq: 
                genNoteq(instr, out);
            break;
            case inf: 
                genInf(instr, out);
            break;
            case infeq: 
                genInfeq(instr, out);
            break;
            case sup: 
                genSup(instr, out);
            break;
            case supeq: 
                genSupeq(instr, out);
            break;
            case goto_op: 
                genGoto(instr, out);
            break;
            case label: 
                genLabel(instr, out);
            break;
            case call: 
                genCall(instr, out);
            break;
            case ret: 
                genReturn(instr, out);
            break;
            case param: 
                genParam(instr, out);
            break;
            case loadT:
                genLoadT(instr, out);
            break;
            default:
                fprintf(stderr, "operation non reconnue\n");
            break;
        }
    }
    fprintf(out, "\nline.%ld:\nprogram.exit:", genCode.current);
    fprintf(out, "\n# Program exit\nli $v0 10\nsyscall\n");
}


