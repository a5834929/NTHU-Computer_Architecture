extern char Dinput[1024];
extern int reg[32], pc;
void J_Ins(int op,int immi){
    switch(op){
        case 0x02:
            pc= (pc & 0xF0000000)|(unsigned int)(immi*4);
            break;
        case 0x03:
            reg[31]= pc + 4;
            pc = (pc & 0xF0000000)|(unsigned int)(immi*4);
            break;
    }
}
