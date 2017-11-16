extern REG IF_ID,ID_EX,EX_MEM,MEM_WB,afterWB;
char printafter[10][100];
char fwd[100];
extern int pc;
FILE *snapshot;
int cycle=0;
int isHalt;
void run(){
    snapshot = fopen("snapshot.rpt","w");
    isHalt=0;
    int i;

    strcpy(IF_ID.instr.name,"NOP");
    strcpy(ID_EX.instr.name,"NOP");
    strcpy(EX_MEM.instr.name,"NOP");
    strcpy(MEM_WB.instr.name,"NOP");
    strcpy(afterWB.instr.name,"NOP");
    while(1){
        fprintf(snapshot,"cycle %d\n",cycle++);
        for(i=0;i<32;i++)
            fprintf(snapshot,"$%02d: 0x%08X\n", i,reg[i]);
        fprintf(snapshot,"PC: 0x%08X\n", pc);
        memset(printafter,'\0',sizeof(printafter));
        PCWrite=1;
        flush=0;

        strcpy(printafter[5], "n/a");
        strcpy(printafter[6], "n/a");

        IF_ID.En=ID_EX.En=EX_MEM.En=MEM_WB.En=1;
        //WB
        strcpy(printafter[4],MEM_WB.instr.name);
        WB();
        afterWB=MEM_WB;
        //MEM
        strcpy(printafter[3],EX_MEM.instr.name);
        MEM();
        MEM_WB=EX_MEM;

        //EX
        strcpy(printafter[2],ID_EX.instr.name);
        EX();
        EX_MEM=ID_EX;

        //ID
        strcpy(printafter[1],IF_ID.instr.name);
        ID();

        //IF
        IF();

        fprintf(snapshot,"%s\n",printafter[0]);
        fprintf(snapshot,"ID: %s\n",printafter[1]);
        fprintf(snapshot,"EX: %s\n",printafter[2]);
        fprintf(snapshot,"DM: %s\n",printafter[3]);
        fprintf(snapshot,"WB: %s\n",printafter[4]);

        fprintf(snapshot,"PREDICTION: %s\n",printafter[5]);
        fprintf(snapshot,"CORRECTNESS: %s\n",printafter[6]);

        fprintf(snapshot,"\n\n");

        if(isHalt) break;
    }
    fclose(snapshot);
}

