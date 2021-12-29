#include "genMips.h"

// --------------------------------------
// generation de codde mips par instruction
// --------------------------------------

void genLabel(int line, FILE* out)
{
    fprintf(out, "line.%d: ", line);
}

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
    fprintf(out, "add %s %s %s\n", instr.dst->u.id, instr.arg1->u.id, instr.arg2->u.id);
}

void genSub(struct code3add instr, FILE* out)
{
    fprintf(out, "sub %s %s %s\n", instr.dst->u.id, instr.arg1->u.id, instr.arg2->u.id);
}

void genMul(struct code3add instr, FILE* out)
{
    fprintf(out, "mult %s %s\n", instr.arg1->u.id, instr.arg2->u.id);
    fprintf(out, "mflo %s\n", instr.dst->u.id);
}

void genDivi(struct code3add instr, FILE* out)
{
    fprintf(out, "div %s %s\n", instr.arg1->u.id, instr.arg2->u.id);
    fprintf(out, "mflo %s\n", instr.dst->u.id);
}

void genMod(struct code3add instr, FILE* out)
{
    fprintf(out, "div %s %s\n", instr.arg1->u.id, instr.arg2->u.id);
    fprintf(out, "mfhi %s\n", instr.dst->u.id);
}

void genEq(struct code3add instr, FILE* out)
{
    
}

void genInf(struct code3add instr, FILE* out)
{
    
}

void genInfeq(struct code3add instr, FILE* out)
{
    
}

void genSup(struct code3add instr, FILE* out)
{
    
}

void genSupeq(struct code3add instr, FILE* out)
{
    
}

void genGoto(struct code3add instr, FILE* out)
{
    
}

// --------------------------------------
void genMips(FILE* out)
{
    int i = 0;
    struct code3add instr;
    struct symbole s;

    fprintf(out, "\t.globl main\n\n\t.text\nmain:\n");
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
            default:
                fprintf(stderr, "operation non reconnue\n");
            break;
        }
    }
    fprintf(out, "\n\n\t.data\n");
    for (size_t i=0;i<symTab->size;i++){
        s=symTab->symb[i];
        if (s.kind==IDENT)
            fprintf(out, "%s:\t.word 0\n",s.u.id);
    }
}


