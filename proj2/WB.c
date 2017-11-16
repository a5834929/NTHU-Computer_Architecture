extern int isHalt;
void WB(){
    if(MEM_WB.instr.type=='S')isHalt=1;
    if(MEM_WB.signal.RegWrite && MEM_WB.output!=0){
        reg[MEM_WB.output]=MEM_WB.result;
    }
}
