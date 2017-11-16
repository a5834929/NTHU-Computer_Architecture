extern IMemory imemory[1024];
extern int reg[32], pc;
int NumOverflow=0, DataMisaligned=0, AddrOverflow=0,WriteZero=0;
FILE *snapshot;
FILE *error;
int cycle=0;
void run(int num){
    int i;

    snapshot = fopen("snapshot.rpt","w");
    error = fopen("Error_dump.rpt","w");

    while(1){
        fprintf(snapshot,"cycle %d\n",cycle++);
        for(i=0;i<32;i++)
            fprintf(snapshot,"$%02d: 0x%08X\n", i,reg[i]);
        fprintf(snapshot,"PC: 0x%08X\n\n\n", pc);

        if(pc>1023 || pc<0){
            fprintf(error, "Address overflow in cycle: %d\n", cycle-1);
            break;
        }

        if(pc%4){
            fprintf(error, "Misalignment error in cycle: %d\n", cycle-1);
            break;
        }

        if(imemory[pc].type=='R'){
            if(imemory[pc].rd==0 && imemory[pc].func!=0x08)WriteZero=1;
            R_Ins(imemory[pc].func,imemory[pc].rs,imemory[pc].rt,imemory[pc].rd,imemory[pc].shamt);
        }

        else if(imemory[pc].type=='I'){
            I_Ins(imemory[pc].op,imemory[pc].rs,imemory[pc].rt,imemory[pc].immi);
        }

        else if(imemory[pc].type=='S')
            break;


        else if(imemory[pc].type=='J')
            J_Ins(imemory[pc].op, imemory[pc].immi);

        else{//sll
            if(imemory[pc].rd==0)WriteZero=1;
            R_Ins(imemory[pc].func,imemory[pc].rs,imemory[pc].rt,imemory[pc].rd,imemory[pc].shamt);
        }


        if(NumOverflow){
            fprintf(error, "Number overflow in cycle: %d\n", cycle-1);
            NumOverflow=0;
        }

        if(AddrOverflow){
            fprintf(error, "Address overflow in cycle: %d\n", cycle-1);
            break;
        }

        if(DataMisaligned){
            fprintf(error, "Misalignment error in cycle: %d\n", cycle-1);
            break;
        }

        if(WriteZero){//after instruction
            fprintf(error, "Write $0 error in cycle: %d\n", cycle-1);
            reg[0]=0;
            WriteZero=0;
        }
    }

    fclose(snapshot);
    fclose(error);
}
