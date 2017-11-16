#include<string.h>
typedef struct I{
    int op;
    char type;
    int immi;
    int rs,rt,rd;
    int func;
    int shamt;
    int binform;
    char name[10];
}IMemory;
IMemory imemory[1024];

typedef struct C{
    int MemRead, MemWrite;
    int RegWrite, Branch;
}Con;
Con signal;

typedef struct R{
    IMemory instr;
    Con signal;
    int input_s, input_t;
    int data1,data2;
    int output, result;
    int shamt, immi;
    int En, pc;
    int preResult,verResult;
}REG;
REG IF_ID,ID_EX,EX_MEM,MEM_WB,afterWB;

void findInstr(char type,int op,int func,char *name,int binform);

void encapsulate(int x,int op,int rs,int rt,int rd,int shamt,int func,int immi,int binform){
    if(op==0x00){
        imemory[x].type = 'R';
        imemory[x].rs = rs;
        imemory[x].rt = rt;
        imemory[x].rd = rd;
        imemory[x].shamt = shamt;
        imemory[x].func = func;
        imemory[x].binform=binform;
    }
    else{
        if(op==0x3F){
            imemory[x].type = 'S';
            imemory[x].op = op;
            imemory[x].binform=binform;
        }
        else if(op==0x02 || op==0x03){
            imemory[x].op = op;
            imemory[x].type = 'J';
            imemory[x].immi = immi;
            imemory[x].binform=binform;
        }
        else{
            imemory[x].op = op;
            imemory[x].type = 'I';
            imemory[x].rs = rs;
            imemory[x].rt = rt;
            imemory[x].immi = immi;
            imemory[x].binform=binform;
        }
    }
    findInstr(imemory[x].type,imemory[x].op,imemory[x].func,imemory[x].name,imemory[x].binform);
}
void findInstr(char type,int op,int func,char *name,int binform){
    if(type=='R'){
        switch(func){
            case 0x20://add
                strcpy(name,"ADD");
                break;
            case 0x22://sub
                strcpy(name,"SUB");
                break;
            case 0x24://and
                strcpy(name,"AND");
                break;
            case 0x25://or
                strcpy(name,"OR");
                break;
            case 0x26://xor
                strcpy(name,"XOR");
                break;
            case 0x27://nor
                strcpy(name,"NOR");
                break;
            case 0x28://nand
                strcpy(name,"NAND");
                break;
            case 0x2A://slt
                strcpy(name,"SLT");
                break;
            case 0x00://sll
                if(binform==0x00)strcpy(name,"NOP");
                else strcpy(name,"SLL");
                break;
            case 0x02://srl
                strcpy(name,"SRL");
                break;
            case 0x03://sra
                strcpy(name,"SRA");
                break;
            case 0x08://jr
                strcpy(name,"JR");
               // pc = reg[instr.rs];
                break;
            case 0x21://addu
                strcpy(name,"ADDU");
                break;
            case 0x23://subu
                strcpy(name,"SUBU");
                break;
            case 0x2b://sltu
                strcpy(name,"SLTU");
                break;
        }
    }
    else if(type=='I'){
        switch(op){
            case 0x08://addi
                strcpy(name,"ADDI");
                break;
            case 0x23://lw
                strcpy(name,"LW");
                break;
            case 0x21://lh
                strcpy(name,"LH");
                break;
            case 0x25://lhu
                strcpy(name,"LHU");
                break;
            case 0x20://lb
                strcpy(name,"LB");
                break;
            case 0x24://lbu
                strcpy(name,"LBU");
                break;
            case 0x2B://sw
                strcpy(name,"SW");
                break;
            case 0x29://sh
                strcpy(name,"SH");
                break;
            case 0x28://sb
                strcpy(name,"SB");
                break;
            case 0x0F://lui
                strcpy(name,"LUI");
                break;
            case 0x0C://andi
                strcpy(name,"ANDI");
                break;
            case 0x0D://ori
                strcpy(name,"ORI");
                break;
            case 0x0E://nori
                strcpy(name,"NORI");
                break;
            case 0x0A://slti
                strcpy(name,"SLTI");
                break;
            case 0x04://beq
                strcpy(name,"BEQ");
                break;
            case 0x05://bne
                strcpy(name,"BNE");
                break;
            case 0x09://addiu
                strcpy(name,"ADDIU");
                break;
        }
    }
    else if(type=='J'){
        switch(op){
            case 0x02://j
                strcpy(name, "J");
                break;
            case 0x03://jal
                strcpy(name, "JAL");
                break;
        }
    }

    else if(type=='S'){
        strcpy(name,"HALT");
    }
}

