extern char fwd[100];
extern int fwd_EX_rs,fwd_EX_rt,fwd_MEM_rs,fwd_MEM_rt;
void EX(){
    //branch and jump instr have done in ID stage
    if(isBranch(ID_EX.instr.op) || isJump(ID_EX.instr.type,ID_EX.instr.func)){
        EX_MEM=ID_EX;
    }
    else{
        //forwarding
        if(fwd_EX_rs && from_EX){
            ID_EX.data1=EX_MEM.result;
            sprintf(fwd," fwd_EX-DM_rs_$%d",ID_EX.input_s);
            strcat(printafter[2],fwd);
        }
        if(fwd_MEM_rs && from_MEM && !fwd_EX_rs){
            ID_EX.data1=afterWB.result;
            sprintf(fwd," fwd_DM-WB_rs_$%d",ID_EX.input_s);
            strcat(printafter[2],fwd);
        }
        if(fwd_EX_rt && from_EX){
            ID_EX.data2=EX_MEM.result;
            sprintf(fwd," fwd_EX-DM_rt_$%d",ID_EX.input_t);
            strcat(printafter[2],fwd);
        }
        if(fwd_MEM_rt && from_MEM  && !fwd_EX_rt){
            ID_EX.data2=afterWB.result;
            sprintf(fwd," fwd_DM-WB_rt_$%d",ID_EX.input_t);
            strcat(printafter[2],fwd);
        }
    }

    //ALU calculation
    if(ID_EX.instr.type=='R'){
        R_Ins(ID_EX.instr.func,ID_EX.data1,ID_EX.data2,ID_EX.shamt);
    }
    else if(ID_EX.instr.type=='I'){
        I_Ins(ID_EX.instr.op,ID_EX.data1,ID_EX.data2,ID_EX.immi);
    }
    else if(ID_EX.instr.type=='J'){
        J_Ins(ID_EX.instr.op,ID_EX.data1);
    }
    // j type has done in ID stage
}
