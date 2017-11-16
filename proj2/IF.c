#include<string.h>
#include<stdio.h>
extern FILE *snapshot;
extern char printafter[10][100];
extern int BHT[16];
extern int PCWrite;
extern int flush;
extern int fwd_rs,fwd_rt,from_EX,from_MEM;
extern int tarAddr;
int BHTindex;
int predict(int ind);
void updateBHT(int ind,int verResult);

void IF(){
    //fetch instr
    if(IF_ID.En){
        IF_ID.instr = imemory[pc];
        IF_ID.pc = pc;
    }
    fprintf(snapshot,"IF: 0x%08X",imemory[pc].binform);

    //branch prediction
    int tmp=0;
    if(isBranch(imemory[pc].op)){
        BHTindex=(pc>>2)&15;
        int preResult=predict(BHTindex);
        if(IF_ID.En)IF_ID.preResult=preResult;
        if(preResult){
            tmp=4*imemory[pc].immi;
            strcpy(printafter[5],"taken");
        }
        else{
            tmp=0;
            strcpy(printafter[5],"not_taken");
        }
    }

    //stall or flush
    if(PCWrite){
        if(isBranch(ID_EX.instr.op)){
            updateBHT((ID_EX.pc>>2)&15,ID_EX.verResult);
        }
        if(flush){
            pc=tarAddr;
            memset(&IF_ID,0,sizeof(REG));
            IF_ID.En=1;
            strcpy(IF_ID.instr.name,"NOP");
            strcat(printafter[0]," to_be_flushed");
        }
        else pc=pc+4+tmp;
    }
    else{
        strcat(printafter[0]," to_be_stalled");
    }
}

int predict(int ind){
    if(BHT[ind]<=1)return 0;//not taken
    else return 1;//taken
}

void updateBHT(int ind, int verResult){
    if(verResult){
        if(BHT[ind]<3) BHT[ind]++;
    }
    else{
        if(BHT[ind]>0) BHT[ind]--;
    }
}
