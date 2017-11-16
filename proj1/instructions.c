#include<string.h>
typedef struct{
    int op;
    char type;
    int immi;
    int rs,rt,rd;
    int func;
    int shamt;
}IMemory;
extern IMemory imemory[1024];

void encapsulate(int x,int op,int rs,int rt,int rd,int shamt,int func,int immi){
    if(op==0x00){
        imemory[x].type = 'R';
        imemory[x].rs = rs;
        imemory[x].rt = rt;
        imemory[x].rd = rd;
        imemory[x].shamt = shamt;
        imemory[x].func = func;
    }
    else{
        if(op==0x3F){
            imemory[x].type = 'S';
            imemory[x].op = op;
        }
        else if(op==0x02 || op==0x03){
            imemory[x].op = op;
            imemory[x].type = 'J';
            imemory[x].immi = immi;
        }
        else{
            imemory[x].op = op;
            imemory[x].type = 'I';
            imemory[x].rs = rs;
            imemory[x].rt = rt;
            imemory[x].immi = immi;
        }
    }
}

