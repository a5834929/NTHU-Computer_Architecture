extern char DDisk[1024];
extern int reg[32], pc;
void I_Ins(int op, int rs, int rt, int immi){
    int tmp=0, i, sh=24;
    switch(op){
    case 0x08://addi
        reg[rt]=reg[rs]+immi;
        pc=pc+4;
        break;
    case 0x23://lw
        tmp=0;
        Check_DTLB(reg[rs]+immi);
        Check_DCache(reg[rs]+immi);
        for(i=0;i<=3;i++){
            tmp|=((DDisk[reg[rs]+immi+i]&255)<<(sh-8*i));
        }
        reg[rt]=tmp;
        pc=pc+4;
        break;
    case 0x21://lh
        Check_DTLB(reg[rs]+immi);
        Check_DCache(reg[rs]+immi);
        reg[rt]=((DDisk[reg[rs]+immi])<<8)|(DDisk[reg[rs]+immi+1]&255);
        pc=pc+4;
        break;
    case 0x25://lhu
        Check_DTLB(reg[rs]+immi);
        Check_DCache(reg[rs]+immi);
        reg[rt]=((DDisk[reg[rs]+immi]&255)<<8)|(DDisk[reg[rs]+immi+1]&255);
        pc=pc+4;
        break;
    case 0x20://lb
        Check_DTLB(reg[rs]+immi);
        Check_DCache(reg[rs]+immi);
        reg[rt]=DDisk[reg[rs]+immi];
        pc=pc+4;
        break;
    case 0x24://lbu
        Check_DTLB(reg[rs]+immi);
        Check_DCache(reg[rs]+immi);
        reg[rt]=DDisk[reg[rs]+immi]&255;
        pc=pc+4;
        break;
    case 0x2B://sw
        Check_DTLB(reg[rs]+immi);
        Check_DCache(reg[rs]+immi);
        for(i=0;i<=3;i++)
            DDisk[reg[rs]+immi+i]=(reg[rt]>>(sh-8*i))&255;
        pc=pc+4;
        break;
    case 0x29://sh
        Check_DTLB(reg[rs]+immi);
        Check_DCache(reg[rs]+immi);
        DDisk[reg[rs]+immi]=(reg[rt]>>8)&255;
        DDisk[reg[rs]+immi+1]=(reg[rt]&255);
        pc=pc+4;
        break;
    case 0x28://sb
        Check_DTLB(reg[rs]+immi);
        Check_DCache(reg[rs]+immi);
        DDisk[reg[rs]+immi]=reg[rt]&255;
        pc=pc+4;
        break;
    case 0x0F://lui
        reg[rt]=immi<<16;
        pc=pc+4;
        break;
    case 0x0C://andi
        reg[rt]=reg[rs]&(immi&0xFFFF);
        pc=pc+4;
        break;
    case 0x0D://ori
        reg[rt]=reg[rs]|(immi&0xFFFF);
        pc=pc+4;
        break;
    case 0x0E://nori
        reg[rt]=~(reg[rs]|(immi&0xFFFF));
        pc=pc+4;
        break;
    case 0x0A://slti
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
        reg[rt]=reg[rs]+immi;
        pc=pc+4;
        break;
    }
}
