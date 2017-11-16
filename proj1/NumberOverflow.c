extern int NumOverflow;
void NumberOverflow(int a,int b,char oper){
    if(oper=='+'){
        if(a>=0 && b>=0){
            if(a+b<a)NumOverflow=1;
        }
        else if(a<=0 && b<=0){
            if(a+b>a)NumOverflow=1;
        }
    }
    else{
        if(a<=0 && b>=0){
            if(a-b>a)NumOverflow=1;
        }
        else if(a>=0 && b<=0){
            if(a-b<a)NumOverflow=1;
        }
    }
}
