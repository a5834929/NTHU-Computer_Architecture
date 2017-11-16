extern char Dinput[1024];
extern int reg[32], pc, NumOverflow, DataMisaligned, AddrOverflow,WriteZero,cycle;
void I_Ins(int op, int rs, int rt, int immi){
    int tmp=0, i, sh=24;
    switch(op){
    case 0x08://addi
        if(rt==0)WriteZero=1;
        NumberOverflow(reg[rs],immi,'+');
        reg[rt]=reg[rs]+immi;
        pc=pc+4;
        break;
    case 0x23://lw
        if(rt==0)WriteZero=1;
        NumberOverflow(reg[rs],immi,'+');
        if((reg[rs]+immi)>1023 || (reg[rs]+immi)<0){
            AddrOverflow=1;
            break;
        }
        tmp=0;
        if((reg[rs]+immi)%4==0)
            for(i=0;i<=3;i++)
                tmp|=((Dinput[reg[rs]+immi+i]&255)<<(sh-8*i));
        else{
            DataMisaligned=1;
            break;
        }
        reg[rt]=tmp;
        pc=pc+4;
        break;
    case 0x21://lh
        if(rt==0)WriteZero=1;
        NumberOverflow(reg[rs],immi,'+');
        if((reg[rs]+immi)>1023 || (reg[rs]+immi)<0){
            AddrOverflow=1;
            break;
        }
        if((reg[rs]+immi)%2==0)
            reg[rt]=((Dinput[reg[rs]+immi])<<8)|(Dinput[reg[rs]+immi+1]&255);
        else{
            DataMisaligned=1;
            break;
        }
        pc=pc+4;
        break;
    case 0x25://lhu
        if(rt==0)WriteZero=1;
        NumberOverflow(reg[rs],immi,'+');
        if((reg[rs]+immi)>1023 || (reg[rs]+immi)<0){
            AddrOverflow=1;
            break;
        }
        if((reg[rs]+immi)%2==0)
            reg[rt]=((Dinput[reg[rs]+immi]&255)<<8)|(Dinput[reg[rs]+immi+1]&255);
        else{
            DataMisaligned=1;
            break;
        }
        pc=pc+4;
        break;
    case 0x20://lb
        if(rt==0)WriteZero=1;
        NumberOverflow(reg[rs],immi,'+');
        if((reg[rs]+immi)>1023 || (reg[rs]+immi)<0){
            AddrOverflow=1;
            break;
        }
        reg[rt]=Dinput[reg[rs]+immi];
        pc=pc+4;
        break;
    case 0x24://lbu
        if(rt==0)WriteZero=1;
        NumberOverflow(reg[rs],immi,'+');
        if((reg[rs]+immi)>1023 || (reg[rs]+immi)<0){
            AddrOverflow=1;
            break;
        }
        reg[rt]=Dinput[reg[rs]+immi]&255;
        pc=pc+4;
        break;
    case 0x2B://sw
        NumberOverflow(reg[rs],immi,'+');
        if((reg[rs]+immi)>1023 || (reg[rs]+immi)<0){
            AddrOverflow=1;
            break;
        }
        if((reg[rs]+immi)%4){DataMisaligned=1;break;}
        for(i=0;i<=3;i++)
            Dinput[reg[rs]+immi+i]=(reg[rt]>>(sh-8*i))&255;
        pc=pc+4;
        break;
    case 0x29://sh
        NumberOverflow(reg[rs],immi,'+');
        if((reg[rs]+immi)>1023 || (reg[rs]+immi)<0){
            AddrOverflow=1;
            break;
        }
        if((reg[rs]+immi)%2){DataMisaligned=1;break;}

        Dinput[reg[rs]+immi]=(reg[rt]>>8)&255;
        Dinput[reg[rs]+immi+1]=(reg[rt]&255);

        pc=pc+4;
        break;
    case 0x28://sb
        NumberOverflow(reg[rs],immi,'+');
        if((reg[rs]+immi)>1023 || (reg[rs]+immi)<0){
            AddrOverflow=1;
            break;
        }
        Dinput[reg[rs]+immi]=reg[rt]&255;
        pc=pc+4;
        break;
    case 0x0F://lui
        if(rt==0)WriteZero=1;
        reg[rt]=immi<<16;
        pc=pc+4;
        break;
    case 0x0C://andi
        if(rt==0)WriteZero=1;
        reg[rt]=reg[rs]&(immi&0xFFFF);
        pc=pc+4;
        break;
    case 0x0D://ori
        if(rt==0)WriteZero=1;
        reg[rt]=reg[rs]|(immi&0xFFFF);
        pc=pc+4;
        break;
    case 0x0E://nori
        if(rt==0)WriteZero=1;
        reg[rt]=~(reg[rs]|(immi&0xFFFF));
        pc=pc+4;
        break;
    case 0x0A://slti
        if(rt==0)WriteZero=1;
        reg[rt]=(reg[rs]<immi);
        pc=pc+4;
        break;
    case 0x04://beq
        pc=(reg[rt]==reg[rs])?pc+4+4*immi:pc+4;
        break;
    case 0x05://bne
        pc=(reg[rt]!=reg[rs])?pc+4+4*immi:pc+4;
        break;
    case 0x09://addiu
        immi = immi&0xFFFF;
        if(rt==0)WriteZero=1;
        NumberOverflow_unsigned(reg[rs],immi,'+');
        reg[rt]=reg[rs]+immi;
        pc=pc+4;
        break;
    }
}
