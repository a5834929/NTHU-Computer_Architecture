extern char DDisk[1024];
extern int reg[32], pc;
void R_Ins(int func,int rs,int rt,int rd,int shamt){
    switch(func){
        case 0x20://add
            reg[rd] = reg[rs] + reg[rt];
            pc=pc+4;
            break;
        case 0x22://sub
            reg[rd] = reg[rs] - reg[rt];
            pc=pc+4;
            break;
        case 0x24://and
            reg[rd] = reg[rs] & reg[rt];
            pc=pc+4;
            break;
        case 0x25://or
            reg[rd] = reg[rs] | reg[rt];
            pc=pc+4;
            break;
        case 0x26://xor
            reg[rd] = reg[rs] ^ reg[rt];
            pc=pc+4;
            break;
        case 0x27://nor
            reg[rd] = ~(reg[rs] | reg[rt]);
            pc=pc+4;
            break;
        case 0x28://nand
            reg[rd] = ~(reg[rs] & reg[rt]);
            pc=pc+4;
            break;
        case 0x2A://slt
            reg[rd] = (reg[rs]<reg[rt])?1:0;
            pc=pc+4;
            break;
        case 0x00://sll
            reg[rd] = reg[rt]<<shamt;
            pc=pc+4;
            break;
        case 0x02://srl
            reg[rd] = (unsigned int)reg[rt]>>shamt;
            pc=pc+4;
            break;
        case 0x03://sra
            reg[rd] = reg[rt]>>shamt;
            pc=pc+4;
            break;
        case 0x08://jr
            pc = reg[rs];
            break;
        case 0x21://addu
            reg[rd]=(unsigned int)reg[rs]+(unsigned int)reg[rt];
            pc=pc+4;
            break;
        case 0x23://subu
            reg[rd]=(unsigned int)reg[rs]-(unsigned int)reg[rt];
            pc=pc+4;
            break;
        case 0x2b://sltu
            reg[rd] =((unsigned int)reg[rs]<(unsigned int)reg[rt])?1:0;
            pc=pc+4;
            break;
    }
}
