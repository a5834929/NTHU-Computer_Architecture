extern int pc;
void I_Ins(int op,int rs, int rt, int immi){
    switch(op){
        case 0x08://addi
            ID_EX.result = rs + immi;
            break;
        case 0x23://lw
            ID_EX.result = rs + immi;
            break;
        case 0x21://lh
            ID_EX.result = rs + immi;
            break;
        case 0x25://lhu
            ID_EX.result = rs + immi;
            break;
        case 0x20://lb
            ID_EX.result = rs + immi;
            break;
        case 0x24://lbu
            ID_EX.result = rs + immi;
            break;
        case 0x2B://sw
            ID_EX.result = rs + immi;
            break;
        case 0x29://sh
            ID_EX.result = rs + immi;
            break;
        case 0x28://sb
            ID_EX.result = rs + immi;
            break;
        case 0x0F://lui
            ID_EX.result = immi<<16;
            break;
        case 0x0C://andi
            ID_EX.result = rs & (immi&0xFFFF);
            break;
        case 0x0D://ori
            ID_EX.result = rs | (immi&0xFFFF);
            break;
        case 0x0E://nori
            ID_EX.result = ~(rs | (immi&0xFFFF));
            break;
        case 0x0A://slti
            ID_EX.result = (rs<immi);
            break;
        case 0x04://beq
            break;
        case 0x05://bne
            break;
        case 0x09://addiu
            ID_EX.result = rs + (immi&0xFFFF);
            break;
    }
}
