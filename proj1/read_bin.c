#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"instructions.c"
#include"dmemory.c"
#include"NumberOverflow.c"
#include"NumberOverflow_unsigned.c"
#include"I_Ins.c"
#include"R_Ins.c"
#include"J_Ins.c"
#include"run.c"

IMemory imemory[1024];
int reg[32]={0};
int pc;
char Dinput[1024];
int main(){
    FILE *in = fopen("iimage.bin", "rb");
    char input[1000][4], op;
    int i,num,rs,rt,rd,shamt,func,immi,j;

    fread(&input, 4, 1, in);
    pc=(((input[0][0]<<24)|((input[0][1]&255)<<16))|((input[0][2]&255)<<8))|(input[0][3]&255);

    fread(&input, 4, 1, in);
    num=(((input[0][0]<<24)|((input[0][1]&255)<<16))|((input[0][2]&255)<<8))|(input[0][3]&255);
    fread(&input, 4, num, in);

    memset(imemory, 0, sizeof(imemory));
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
                immi=(input[i][2]<< 8)| (input[i][3]&255);
                immi=immi&0x0000FFFF;
            }
            else
                immi=(input[i][2] << 8) | (input[i][3]&255);

            encapsulate(pc + i*4,op,rs,rt,0,0,0,immi);
        }
    }
    /*j=pc;
    for(i=1;i<=num;j=pc+i*4,i++)
        printf("%d: PC:%d op:%x rs:%d rt:%d rd:%d func:%x shamt:%d immi:%d\n",i,j,imemory[j].op,imemory[j].rs,imemory[j].rt,imemory[j].rd,imemory[j].func,imemory[j].shamt,imemory[j].immi);
    */
    read_D();
    run(num);
    return 0;
}


