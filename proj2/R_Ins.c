void R_Ins(int func,int rs,int rt,int shamt){
    switch(func){
        case 0x20://add
            ID_EX.result = rs + rt;
            break;
        case 0x22://sub
            ID_EX.result = rs - rt;
            break;
        case 0x24://and
            ID_EX.result = rs & rt;
            break;
        case 0x25://or
            ID_EX.result = rs | rt;
            break;
        case 0x26://xor
            ID_EX.result = rs ^ rt;
            break;
        case 0x27://nor
            ID_EX.result = ~(rs | rt);
            break;
        case 0x28://nand
            ID_EX.result = ~(rs & rt);
            break;
        case 0x2A://slt
            ID_EX.result = (rs<rt)?1:0;
            break;
        case 0x00://sll
            ID_EX.result = rt<<shamt;
            break;
        case 0x02://srl
            ID_EX.result = (unsigned int)rt>>shamt;
            break;
        case 0x03://sra
            ID_EX.result = rt>>shamt;
            break;
        case 0x08://jr
            ID_EX.result = rs;
            break;
        case 0x21://addu
            ID_EX.result=(unsigned int)rs+(unsigned int)rt;
            break;
        case 0x23://subu
            ID_EX.result=(unsigned int)rs-(unsigned int)rt;
            break;
        case 0x2b://sltu
            ID_EX.result =((unsigned int)rs<(unsigned int)rt)?1:0;
            break;
    }
}
