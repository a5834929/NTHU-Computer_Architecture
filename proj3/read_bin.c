#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LRU_Update.c"
#include"ICache.c"
#include"IPageTable.c"
#include"ITLB.c"
#include"DCache.c"
#include"DPageTable.c"
#include"DTLB.c"
#include"instructions.c"
#include"ddisk.c"
#include"I_Ins.c"
#include"R_Ins.c"
#include"J_Ins.c"
#include"run.c"

IDisk idisk[1024];
int reg[32]={0};
int pc;
int parameter[10];
char DDisk[1024];

int main(int argc, char* argv[]){
    char input[1000][4], op;
    int i,num,rs,rt,rd,shamt,func,immi;

    FILE *in = fopen("iimage.bin", "rb");
    if(argc!=1){
       for(i=1;i<11;i++){
            freopen(argv[i], "r", stdin);
            parameter[i-1]=atoi(argv[i]);
        }
    }
    else{ // default
        parameter[0]=64; //IMemory size
        parameter[1]=32; //DMemory size
        parameter[2]=8;  //IMemory page size
        parameter[3]=16; //DMemory page size
        parameter[4]=16; //ICache size
        parameter[5]=4;  //ICache block size
        parameter[6]=4;  //ICache associativity
        parameter[7]=16; //DCache size
        parameter[8]=4;  //DCache block size
        parameter[9]=1;  //DCache associativity
    }

    fread(&input, 4, 1, in);
    pc=(((input[0][0]<<24)|((input[0][1]&255)<<16))|((input[0][2]&255)<<8))|(input[0][3]&255);

    fread(&input, 4, 1, in);
    num=(((input[0][0]<<24)|((input[0][1]&255)<<16))|((input[0][2]&255)<<8))|(input[0][3]&255);
    fread(&input, 4, num, in);

    memset(idisk, 0, sizeof(idisk));
    for(i=0;i<num;i++){
        op=(input[i][0]>>2)&63;

        if(op==0x00){//R-type
            rs=((input[i][0]&3)<<3) | ((input[i][1]&224)>>5);
            rt=input[i][1]&31;
            rd=(input[i][2]>>3)&31;
            shamt=((input[i][2]&7)<<2) | ((input[i][3]>>6)&3);
            func=input[i][3]&63;
            encapsulate(pc+i*4,op,rs,rt,rd,shamt,func,0);
        }
        else if(op==0x3F){//S-type
            encapsulate(pc+i*4,op,0,0,0,0,0,0);
        }
        else if(op==0x02 || op==0x03){//J-type
            immi=((((input[i][0]&3)<<24)|((input[i][1]&255)<<16)) | ((input[i][2]&255)<<8))|(input[i][3]&255);
            encapsulate(pc+i*4,op,0,0,0,0,0,immi);
        }
        else{//I-type
            rs=((input[i][0]&3)<<3) | ((input[i][1]&224)>>5);
            rt=input[i][1]&31;
            if(op==0x09){
                immi=(input[i][2]<< 8) | (input[i][3]&255);
                immi=immi&0x0000FFFF;
            }
            else
                immi=(input[i][2] << 8) | (input[i][3]&255);
            encapsulate(pc + i*4,op,rs,rt,0,0,0,immi);
        }
    }

    read_D();
    run(num);
    return 0;
}
