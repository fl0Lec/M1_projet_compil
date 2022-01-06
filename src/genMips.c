#include "genMips.h"

// --------------------------------------
// generation de code mips par instruction
// --------------------------------------

void genLoad(struct code3add instr, FILE* out)
{
    fprintf(out, "la %s, %s\n", instr.dst->u.id, instr.arg1->u.id);
}

void genLoadimm(struct code3add instr, FILE* out)   // TODO gerer si > 16 bits
{
    fprintf(out, "li %s, %d\n", instr.dst->u.id, instr.arg1->u.val);
}

void genStore(struct code3add instr, FILE* out)
{
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "\tli $t7, %d\n", instr.arg1->u.val);
        fprintf(out, "\tsw $t7, %s \n", instr.dst->u.id);
        break;
    default:
        fprintf(out, "sw %s, %s\n", instr.dst->u.id, instr.arg1->u.id);
        break;
    }
}

void genAdd(struct code3add instr, FILE* out)
{
    fprintf(out, "add %s ", instr.dst->u.id);
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "%d ", instr.arg1->u.val);
    break;
    case IDENT: case TEMPO:
        fprintf(out, "%s ", instr.arg1->u.id);
    break;
    default:
    break;
    }

    switch (instr.arg2->kind)
    {
    case CST_INT:
        fprintf(out, "%d", instr.arg2->u.val);
    break;
    case IDENT: case TEMPO:
        fprintf(out, "%s", instr.arg2->u.id);
    break;
    default:
    break;
    }

    fprintf(out, "\n");
}

void genSub(struct code3add instr, FILE* out)
{
    fprintf(out, "sub %s ", instr.dst->u.id);
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "%d ", instr.arg1->u.val);
    break;
    case IDENT: case TEMPO: 
        fprintf(out, "%s ", instr.arg1->u.id);
    break;
    default:
    break;
    }

    switch (instr.arg2->kind)
    {
    case CST_INT:
        fprintf(out, "%d", instr.arg2->u.val);
    break;
    case IDENT: case TEMPO:
        fprintf(out, "%s", instr.arg2->u.id);
    break;
    default:
    break;
    }

    fprintf(out, "\n");
}

void genMul(struct code3add instr, FILE* out)
{
    fprintf(out, "mult ");
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "%d ", instr.arg1->u.val);
    break;
    case IDENT: case TEMPO:
        fprintf(out, "%s ", instr.arg1->u.id);
    break;
    default:
    break;
    }

    switch (instr.arg2->kind)
    {
    case CST_INT:
        fprintf(out, "%d", instr.arg2->u.val);
    break;
    case IDENT:
        fprintf(out, "%s", instr.arg2->u.id);
    break;
    default:
    break;
    }

    fprintf(out, "\n");
    fprintf(out, "mflo %s\n", instr.dst->u.id);
}

void genDivi(struct code3add instr, FILE* out)
{
    fprintf(out, "div ");
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "%d ", instr.arg1->u.val);
    break;
    case IDENT: case TEMPO:
        fprintf(out, "%s ", instr.arg1->u.id);
    break;
    default:
    break;
    }

    switch (instr.arg2->kind)
    {
    case CST_INT:
        fprintf(out, "%d", instr.arg2->u.val);
    break;
    case IDENT: case TEMPO:
        fprintf(out, "%s", instr.arg2->u.id);
    break;
    default:
    break;
    }

    fprintf(out, "\n");
    fprintf(out, "mflo %s\n", instr.dst->u.id);
}

void genMod(struct code3add instr, FILE* out)
{
    fprintf(out, "div ");
    switch (instr.arg1->kind)
    {
    case CST_INT:
        fprintf(out, "%d ", instr.arg1->u.val);
    break;
    case IDENT: case TEMPO:
        fprintf(out, "%s ", instr.arg1->u.id);
    break;
    default:
    break;
    }

    switch (instr.arg2->kind)
    {
    case CST_INT:
        fprintf(out, "%d", instr.arg2->u.val);
    break;
    case IDENT: case TEMPO:
        fprintf(out, "%s", instr.arg2->u.id);
    break;
    default:
    break;
    }

    fprintf(out, "\n");
    fprintf(out, "mfhi %s\n", instr.dst->u.id);
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
    fprintf(out, "goto %s %s\n", instr.dst->u.id, "");
}

// un label par ligne de code à trois adresse
void genLabel(struct code3add instr, FILE* out)
{
    fprintf(out, "%s: ", instr.dst->u.id);
}

// génère les fonctions d'entrées et sorties
void genIOFunctions(FILE* out)
{
    // print_string : print string en $a0
    fprintf(out, "\nWriteString:\n  subu $sp $sp 8\n  sw $ra 0($sp)\n  sw $a0 4($sp)\n  li $v0 4\n  syscall\n  print_string.exit:\n    lw $ra 0($sp)\n    lw $a0 4($sp)\n    addu $sp $sp 8\n    jr $ra\n");
    // print_int : print int en $a0
    fprintf(out, "\nWriteInt:\n  subu $sp $sp 8\n  sw $ra 0($sp)\n  sw $a0 4($sp)\n  li $v0 1\n  syscall\n  print_int.exit:\n    lw $ra 0($sp)\n    lw $a0 4($sp)\n    addu $sp $sp 8\n    jr $ra\n");
    //read_int : read int vers $v0
    fprintf(out, "\nReadInt:\n  li $v0 5\n  syscall\n  jr $ra\n");
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
    
    fprintf(out, "\n.text\n.globl main\n\n");
    
    genIOFunctions(out);
    
    fprintf(out, "\n\n");
    while(i < genCode.size)
    {
        instr = genCode.tab[i++];
        //genLabel(i, out);
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
            default:
                fprintf(stderr, "operation non reconnue\n");
            break;
        }
    }
    fprintf(out, "\n# Program exit\nli $v0 10\nsyscall\n");
}


