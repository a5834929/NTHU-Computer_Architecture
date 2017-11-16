void LoadOper(int op);
void StoreOper(int op);

void MEM(){
    if(EX_MEM.signal.MemRead){//load
        LoadOper(EX_MEM.instr.op);
    }
    else if(EX_MEM.signal.MemWrite){//store
        StoreOper(EX_MEM.instr.op);
    }
}

void LoadOper(int op){
     int tmp,i, sh=24;
     switch(op){
        case 0x23://lw
            tmp=0;
            for(i=0;i<=3;i++){
                tmp|=((Dinput[EX_MEM.result+i]&255)<<(sh-8*i));
            }
            EX_MEM.result = tmp;
            break;
        case 0x21://lh
            EX_MEM.result=((Dinput[EX_MEM.result])<<8)|(Dinput[EX_MEM.result+1]&255);
            break;
        case 0x25://lhu
            EX_MEM.result=((Dinput[EX_MEM.result]&255)<<8)|(Dinput[EX_MEM.result+1]&255);
            break;
        case 0x20://lb
            EX_MEM.result=Dinput[EX_MEM.result];
            break;
        case 0x24://lbu
            EX_MEM.result=Dinput[EX_MEM.result]&255;
            break;
    }
}

void StoreOper(int op){
    int i, sh=24;
     switch(op){
        case 0x2B://sw
            for(i=0;i<=3;i++){
                Dinput[EX_MEM.result+i]=(EX_MEM.data2>>(sh-8*i))&255;
            }
            break;
        case 0x29://sh
            Dinput[EX_MEM.result]=(EX_MEM.data2>>8)&255;
            Dinput[EX_MEM.result+1]=(EX_MEM.data2&255);
            break;
        case 0x28://sb
            Dinput[EX_MEM.result]=EX_MEM.data2&255;
            break;
    }
}

