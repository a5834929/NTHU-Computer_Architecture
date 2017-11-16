#include<string.h>

extern int fwd_EX_rs,fwd_EX_rt,fwd_MEM_rs,fwd_MEM_rt;
extern REG IF_ID,ID_EX,EX_MEM,MEM_WB,afterWB;
extern char fwd[100];
extern int BHT[16];

void clear(void);
void decode(IMemory instr);
int verify(int op, int rs, int rt);
int EXHazard(void);
int MEMHazard(void);
int tarAddr;

void ID(){
    //decode
    IMemory instr=IF_ID.instr;
    clear();
    decode(instr);
    Control();
    strcpy(printafter[1],IF_ID.instr.name);

    fwd_EX_rs=fwd_EX_rt=fwd_MEM_rs=fwd_MEM_rt=from_EX=from_MEM=0;
    //branch verify
    if(isBranch(IF_ID.instr.op)){
        //data hazard
        from_EX=EXHazard();
        from_MEM=MEMHazard();
        if(from_EX){//stall
            PCWrite=0;
            IF_ID.En=0;
            from_EX=0;
            from_MEM=0;
            //can't return since it also needs to verify the result
        }
        else if(from_MEM){
            if(isLoad(MEM_WB.instr.op)){//stall
                PCWrite=0;
                IF_ID.En=0;
                from_MEM=0;
                from_EX=0;
                //can't return since it also needs to verify the result
            }
            else{//forwarding
                if(fwd_MEM_rs){
                    IF_ID.data1=MEM_WB.result;
                    sprintf(fwd," fwd_EX-DM_rs_$%d",IF_ID.input_s);
                    strcat(printafter[1],fwd);
                }
                if(fwd_MEM_rt){
                    IF_ID.data2=MEM_WB.result;
                    sprintf(fwd," fwd_EX-DM_rt_$%d",IF_ID.input_t);
                    strcat(printafter[1],fwd);
                }
            }
        }
        //verification
        IF_ID.verResult=verify(instr.op, IF_ID.data1, IF_ID.data2);
        //int index = (IF_ID.pc>>2)&15;
        if(IF_ID.preResult^IF_ID.verResult){//prediction incorrect
            flush=1;
            if(IF_ID.preResult) tarAddr=pc-IF_ID.instr.immi*4;
            else tarAddr=pc+IF_ID.instr.immi*4;
            strcpy(printafter[6], "incorrect");
        }
        else strcpy(printafter[6], "correct");//prediction correct

        //if(PCWrite)updateBHT(index, verResult);//will not update when stalled
    }
    //jump
    else if(isJump(IF_ID.instr.type,IF_ID.instr.func)){//jr,j,jal
        //data hazard
        from_EX=EXHazard();
        from_MEM=MEMHazard();
        if(from_EX){//stall
            PCWrite=0;
            IF_ID.En=0;
            from_EX=0;
            from_MEM=0;
        }
        else if(from_MEM){
            if(isLoad(MEM_WB.instr.op)){//stall
                PCWrite=0;
                IF_ID.En=0;
                from_MEM=0;
                from_EX=0;
            }
            else{//forwarding for jr
                IF_ID.data1=MEM_WB.result;
                sprintf(fwd," fwd_EX-DM_rs_$%d",IF_ID.input_s);
                strcat(printafter[1],fwd);
            }
        }
        //flush, tarAddr
        if(IF_ID.instr.type=='R'){//jr
            tarAddr=IF_ID.data1;
        }
        else if(IF_ID.instr.op==0x02){//j
            tarAddr=(pc & 0xF0000000)|(unsigned int)(IF_ID.immi*4);
        }
        else if(IF_ID.instr.op==0x03){//jal
            tarAddr=(pc & 0xF0000000)|(unsigned int)(IF_ID.immi*4);
        }
        flush=1;
    }
    //stall detection of all instr
    else{
        from_EX=EXHazard();
        from_MEM=MEMHazard();
        if(from_EX){
            if(isLoad(EX_MEM.instr.op)){
                PCWrite=0;
                IF_ID.En=0;
                from_EX=0;
                from_MEM=0;
            }
        }

    }
    //whether ID should be stalled?
    if(IF_ID.En)ID_EX=IF_ID;
    else{//ID stall
        memset(&ID_EX,0,sizeof(REG));
        strcpy(ID_EX.instr.name,"NOP");
        strcat(printafter[1]," to_be_stalled");
    }
}
void clear(){
    IF_ID.input_s=0;
    IF_ID.input_t=0;
    IF_ID.output=0;
    IF_ID.shamt=0;
    IF_ID.data1=0;
    IF_ID.data2=0;
}

void decode(IMemory instr){
    if(instr.type=='R'){
        if(instr.func>=0x20 && instr.func<=0x2B){//add,sub...
            IF_ID.input_s=instr.rs;
            IF_ID.input_t=instr.rt;
            IF_ID.output=instr.rd;
            IF_ID.data1=reg[instr.rs];
            IF_ID.data2=reg[instr.rt];
        }
        else if(instr.func<=0x03){//sll srl sra
            IF_ID.input_t=instr.rt;
            IF_ID.output=instr.rd;
            IF_ID.shamt=instr.shamt;
            IF_ID.data2=reg[instr.rt];
        }
        else if(instr.func==0x08){//jr
            IF_ID.input_s=instr.rs;
            IF_ID.data1=reg[instr.rs];
        }
    }
    else if(instr.type=='I'){
        if(isStore(instr.op) || isBranch(instr.op)){
            IF_ID.input_s=instr.rs;
            IF_ID.input_t=instr.rt;
            IF_ID.immi=instr.immi;
            IF_ID.data1=reg[instr.rs];
            IF_ID.data2=reg[instr.rt];
        }
        else if(isLoad(instr.op)){
            IF_ID.input_s=instr.rs;
            IF_ID.output=instr.rt;
            IF_ID.immi=instr.immi;
            IF_ID.data1=reg[instr.rs];
        }
        else if(instr.op==0x0F){//lui
            IF_ID.output=instr.rt;
            IF_ID.immi=instr.immi;
        }
        else{//addi,ori...
            IF_ID.input_s=instr.rs;
            IF_ID.output=instr.rt;
            IF_ID.immi=instr.immi;
            IF_ID.data1=reg[instr.rs];
        }
    }
    else if(instr.type=='J'){
        if(instr.op==0x03){//jal
            IF_ID.output=31;
            IF_ID.data1=pc;
            IF_ID.immi=instr.immi;
        }
        else if(instr.op==0x02){//j
            IF_ID.immi=instr.immi;
        }
    }
}

int verify(int op, int rs, int rt){
    if(op==0x04)//beq
        return rs==rt;
    else if(op==0x05)//bne
        return rs!=rt;
    return 0;
}

int EXHazard(){
    int hazard=0;
    if(EX_MEM.output==0)return 0;
    if(IF_ID.instr.type=='R'){
        if(EX_MEM.output==IF_ID.input_s){
            hazard=1;
            fwd_EX_rs=1;
        }
        if(EX_MEM.output==IF_ID.input_t){
            hazard=1;
            fwd_EX_rt=1;
        }
        if(hazard)return 1;
    }
    else if(IF_ID.instr.type=='I'){
        if(isStore(IF_ID.instr.op)||isBranch(IF_ID.instr.op)){//2 input
            if(EX_MEM.output==IF_ID.input_s){
                hazard=1;
                fwd_EX_rs=1;
            }
            if(EX_MEM.output==IF_ID.input_t){
                hazard=1;
                fwd_EX_rt=1;
            }
            if(hazard)return 1;
        }
        else{//1 input
            if(EX_MEM.output==IF_ID.input_s){
                hazard=1;
                fwd_EX_rs=1;
            }
            if(hazard)return 1;
        }
    }
    return 0;
    //j type has no input
}
int MEMHazard(){
    int hazard=0;
    if(MEM_WB.output==0)return 0;

    if(IF_ID.instr.type=='R'){
        if(MEM_WB.output==IF_ID.input_s){
            hazard=1;
            fwd_MEM_rs=1;
        }
        if(MEM_WB.output==IF_ID.input_t){
            hazard=1;
            fwd_MEM_rt=1;
        }
        if(hazard)return 1;
    }
    else if(IF_ID.instr.type=='I'){
        if(isStore(IF_ID.instr.op)||isBranch(IF_ID.instr.op)){//2 input
            if(MEM_WB.output==IF_ID.input_s){
                hazard=1;
                fwd_MEM_rs=1;
            }
            if(MEM_WB.output==IF_ID.input_t){
                hazard=1;
                fwd_MEM_rt=1;
            }
            if(hazard)return 1;
        }
        else{//1 input
            if(MEM_WB.output==IF_ID.input_s){
                hazard=1;
                fwd_MEM_rs=1;
            }
            if(hazard)return 1;
        }
    }
    return 0;
    //j type has no input
}
