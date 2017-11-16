int isBranch(int op);
int isJump(char type,int func);
int isStore(int op);
int isLoad(int op);

int isBranch(int op){
    if(op==0x04 || op==0x05)return 1;
    else return 0;
}

int isJump(char type,int func){
    if(type=='J')return 1;//j jal
    else if(type=='R' && func==0x08)return 1;//jr
    else return 0;
}
int isStore(int op){
    if(op>=0x28 && op<=0x2B)return 1;
    else return 0;
}
int isLoad(int op){
    if(op>=0x20 && op<=0x25)return 1;
    else return 0;
}
