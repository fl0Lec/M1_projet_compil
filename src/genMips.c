#include "genMips.h"

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
void genLoadForOP(struct code3add instr, FILE* out) {
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "li $t0 %d\n", instr.arg1->u.val);
    break;
    default:
        fprintf(out, "lw $t0 %d($sp)\n", instr.arg1->location);
    break;
    }

    switch (instr.arg2->kind)
    {
    case CST_INT:
        fprintf(out, "li $t1 %d\n", instr.arg2->u.val);
    break;
    default:
        fprintf(out, "lw $t1 %d($sp)\n", instr.arg2->location);
    break;
    }
}
void genStore(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# store\n");
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "li $t0, %d\n", instr.arg1->u.val);
        fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
        break;
    default:
        fprintf(out, "lw $t0, %d($sp)\n", instr.arg1->location);
        fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
        break;
    }
}

void genAdd(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# add\n");

    genLoadForOP(instr, out);

    fprintf(out, "add $t0 $t0 $t1\n");
    fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
}

void genSub(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# sub\n");

    genLoadForOP(instr, out);

    fprintf(out, "sub $t0 $t0 $t1\n");
    fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
}

void genMul(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# mult\n");

    genLoadForOP(instr, out);

    fprintf(out, "mult $t0 $t1\n");
    fprintf(out, "mflo $t0\n");
    fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
}

void genDivi(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# div\n");

    genLoadForOP(instr, out);

    fprintf(out, "div $t0 $t1\n");
    fprintf(out, "mflo $t0\n");
    fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
}

void genMod(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# mod\n");

    genLoadForOP(instr, out);

    fprintf(out, "div $t0 $t1\n");
    fprintf(out, "mfhi $t0\n");
    fprintf(out, "sw $t0, %d($sp)\n", instr.dst->location);
}

void genEq(struct code3add instr, FILE* out)
{
    fprintf(out, "beq %s %s %s\n", instr.arg1->u.id, instr.arg2->u.id, instr.dst->u.id);
}

void genNoteq(struct code3add instr, FILE* out)
{
    fprintf(out, "bne %s %s %s\n", instr.arg1->u.id, instr.arg2->u.id, instr.dst->u.id);
}

void genInf(struct code3add instr, FILE* out)
{
    fprintf(out, "blt %s %s %s\n", instr.arg1->u.id, instr.arg2->u.id, instr.dst->u.id);
}

void genInfeq(struct code3add instr, FILE* out)
{
    fprintf(out, "ble %s %s %s\n", instr.arg1->u.id, instr.arg2->u.id, instr.dst->u.id);
}

void genSup(struct code3add instr, FILE* out)
{
    fprintf(out, "bgt %s %s %s\n", instr.arg1->u.id, instr.arg2->u.id, instr.dst->u.id);
}

void genSupeq(struct code3add instr, FILE* out)
{
    fprintf(out, "bge %s %s %s\n", instr.arg1->u.id, instr.arg2->u.id, instr.dst->u.id);
}

void genGoto(struct code3add instr, FILE* out)
{
    fprintf(out, "j %s\n", instr.dst->u.id);
}

void genLabel(struct code3add instr, FILE* out)
{
    fprintf(out, "\n%s:\n", instr.dst->u.id);
    fprintf(out, "addiu $sp, $sp, -%d\n", instr.dst->table->lastloc);
    fprintf(out, "addiu $sp, $sp, -4\nsw $ra, 0($sp)\n"); // save return pointer
}

void genReturn(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# return\n");
    fprintf(out, "lw $v0, %d($sp)\n", instr.dst->location); // return value in v0
    fprintf(out, "lw $ra, 0($sp)\n");    // load return pointer
    fprintf(out, "jr $ra\n\n");
}

void genParam(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# param\n");
    fprintf(out, "addiu $sp, $sp, -4\n"); // save parameter
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "li $t0 %d\n", instr.arg1->u.val);
    break;
    default:
        fprintf(out, "lw $t0 %d($sp)\n", instr.arg1->location);
    break;
    }
    fprintf(out, "sw $t0, 0($sp)\n");
}

void genCall(struct code3add instr, FILE* out)
{
    fprintf(out, "\n# call\n");
    fprintf(out, "jal %s\n", instr.arg1->u.id);
    fprintf(out, "addiu $sp, $sp, %d\n", instr.arg1->table->lastloc+4);
    if (instr.dst != NULL) {
        fprintf(out, "lw $v0, %d($sp)\n", instr.dst->location);
    }
}

// génère les fonctions d'entrées et sorties
void genIOFunctions(FILE* out)
{
    // print_string : print string en $a0
    fprintf(out, "\nWriteString:\n  lw $a0 0($sp)\n  subu $sp $sp 4\n  sw $ra 0($sp)\n  li $v0 4\n  syscall\n  print_string.exit:\n    lw $ra 0($sp)\n   jr $ra\n");
    // print_int : print int en $a0
    fprintf(out, "\nWriteInt:\n  lw $a0 0($sp)\n  subu $sp $sp 4\n  sw $ra 0($sp)\n  li $v0 1\n  syscall\n  print_int.exit:\n    lw $ra 0($sp)\n    jr $ra\n");
    //read_int : read int vers $v0
    fprintf(out, "\nReadInt:\n  subu $sp $sp 4\n  li $v0 5\n  syscall\n  sw $v0 4($sp)\n  jr $ra\n");
}

// --------------------------------------
void genMips(FILE* out)
{
    int i = 0;
    struct code3add instr;
    struct symbole s;

    fprintf(out, "#start program\n");

    fprintf(out, "\n.data\n");
    for (size_t i=0;i<symTab->size;i++){
        s=symTab->symb[i];
        if (s.kind==IDENT)
            fprintf(out, "%s:\t.word 0\n",s.u.id);
    }
    
    fprintf(out, "\n.text\n.globl main\nj main\n\n");
    
    genIOFunctions(out);
    
    fprintf(out, "\n\n");
    while(i < genCode.size)
    {
        instr = genCode.tab[i++];
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
            default:
                fprintf(stderr, "operation non reconnue\n");
            break;
        }
    }
    fprintf(out, "\n# Program exit\nli $v0 10\nsyscall\n");
}


