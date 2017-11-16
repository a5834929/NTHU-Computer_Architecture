void J_Ins(int op,int rs){
    switch(op){
        case 0x02://j
            break;
        case 0x03://jal
            ID_EX.result=rs;
            break;
    }
}
