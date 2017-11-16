
void Control(void){
    IF_ID.signal.Branch=IF_ID.signal.RegWrite=0;
    IF_ID.signal.MemRead=IF_ID.signal.MemWrite=0;

    if(IF_ID.instr.type=='R'){
        if(IF_ID.instr.func!=0x08)//except jr
            IF_ID.signal.RegWrite=1;
    }

    else if(IF_ID.instr.type=='I'){
        if(0x20<=IF_ID.instr.op && IF_ID.instr.op<=0x25) //load
            IF_ID.signal.MemRead=IF_ID.signal.RegWrite=1;
        else if(0x28<=IF_ID.instr.op && IF_ID.instr.op<=0x2B) //store
            IF_ID.signal.MemWrite=1;
        else if(IF_ID.instr.op==0x04 || IF_ID.instr.op==0x05) //branch
            IF_ID.signal.Branch=1;
        else IF_ID.signal.RegWrite=1; //addi, slti......
    }

    else if(IF_ID.instr.type=='J'){
        if(IF_ID.instr.op==0x03)//jal
            IF_ID.signal.RegWrite=1;
    }
    return;
}
