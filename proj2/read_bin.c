#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"determine.c"
#include"instructions.c"
#include"dmemory.c"
#include"I_Ins.c"
#include"R_Ins.c"
#include"J_Ins.c"
#include"Control.c"
#include"IF.c"
#include"ID.c"
#include"EX.c"
#include"MEM.c"
#include"WB.c"
#include"run.c"

int reg[32]={0};
int BHT[16];
int preResult;
int pc, PCWrite;
int flush;
int fwd_EX_rs,fwd_EX_rt,fwd_MEM_rs,fwd_MEM_rt;
int from_EX,from_MEM;
char Dinput[1024];

int main(){
    FILE *in = fopen("iimage.bin", "rb");
    char input[1024][4], op;
    int i,num,rs,rt,rd,shamt,func,immi,binform,j;

    fread(&input, 4, 1, in);
    pc=(((input[0][0]<<24)|((input[0][1]&255)<<16))|((input[0][2]&255)<<8))|(input[0][3]&255);

    fread(&input, 4, 1, in);
    num=(((input[0][0]<<24)|((input[0][1]&255)<<16))|((input[0][2]&255)<<8))|(input[0][3]&255);
    fread(&input, 4, num, in);

    memset(imemory, 0, sizeof(imemory));

    for(i=0;i<1024;i+=4)
        strcpy(imemory[i].name, "NOP");

    for(i=0;i<num;i++){
        op=(input[i][0]>>2)&63;

        if(op==0x00){//R-type
            binform=(((input[i][0]<<24)|((input[i][1]&255)<<16))|((input[i][2]&255)<<8))|(input[i][3]&255);
            rs=((input[i][0]&3)<<3) | ((input[i][1]&224)>>5);
            rt=input[i][1]&31;
            rd=(input[i][2]>>3)&31;
            shamt=((input[i][2]&7)<<2) | ((input[i][3]>>6)&3);
            func=input[i][3]&63;
            encapsulate(pc+i*4,op,rs,rt,rd,shamt,func,0,binform);
        }
        else if(op==0x3F){//S-type
            binform=(((input[i][0]<<24)|((input[i][1]&255)<<16))|((input[i][2]&255)<<8))|(input[i][3]&255);
            encapsulate(pc+i*4,op,0,0,0,0,0,0,binform);
        }
        else if(op==0x02 || op==0x03){//J-type
            binform=(((input[i][0]<<24)|((input[i][1]&255)<<16))|((input[i][2]&255)<<8))|(input[i][3]&255);
            immi=((((input[i][0]&3)<<24)|((input[i][1]&255)<<16)) | ((input[i][2]&255)<<8))|(input[i][3]&255);
            encapsulate(pc+i*4,op,0,0,0,0,0,immi,binform);
        }
        else{//I-type
            binform=(((input[i][0]<<24)|((input[i][1]&255)<<16))|((input[i][2]&255)<<8))|(input[i][3]&255);
            rs=((input[i][0]&3)<<3) | ((input[i][1]&224)>>5);
            rt=input[i][1]&31;
            if(op==0x09){
                immi=(input[i][2]<< 8)| (input[i][3]&255);
                immi=immi&0x0000FFFF;
            }
            else{
                immi=(input[i][2] << 8) | (input[i][3]&255);
            }
            encapsulate(pc + i*4,op,rs,rt,0,0,0,immi,binform);
        }
    }

    read_D();

    for(i=0;i<16;i++)//init BHT state to 01
        BHT[i]=1;

    run();
    fclose(in);
    return 0;
}
