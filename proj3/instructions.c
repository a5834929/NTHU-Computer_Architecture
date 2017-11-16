#include<string.h>
typedef struct{
    int op;
    char type;
    int immi;
    int rs,rt,rd;
    int func;
    int shamt;
}IDisk;
extern IDisk idisk[1024];

void encapsulate(int x,int op,int rs,int rt,int rd,int shamt,int func,int immi){
    if(op==0x00){
        idisk[x].type = 'R';
        idisk[x].rs = rs;
        idisk[x].rt = rt;
        idisk[x].rd = rd;
        idisk[x].shamt = shamt;
        idisk[x].func = func;
    }
    else{
        if(op==0x3F){
            idisk[x].type = 'S';
            idisk[x].op = op;
        }
        else if(op==0x02 || op==0x03){
            idisk[x].op = op;
            idisk[x].type = 'J';
            idisk[x].immi = immi;
        }
        else{
            idisk[x].op = op;
            idisk[x].type = 'I';
            idisk[x].rs = rs;
            idisk[x].rt = rt;
            idisk[x].immi = immi;
        }
    }
}

