extern IDisk idisk[1024];
extern int reg[32], pc;

FILE *snapshot;
FILE* report;
int cycle=0;
int ICache_hit, ICache_miss, DCache_hit, DCache_miss,
    ITLB_hit, ITLB_miss, DTLB_hit, DTLB_miss,
    IPT_hit, IPT_miss, DPT_hit, DPT_miss;

void run(int num){
    int i;

    snapshot = fopen("snapshot.rpt","w");
    report = fopen("report.rpt","w");

    ICache_hit=ICache_miss=DCache_hit=DCache_miss=0;
    ITLB_hit=ITLB_miss=DTLB_hit=DTLB_miss=0;
    IPT_hit=IPT_miss=DPT_hit=DPT_miss=0;

    Initial_ICache();
    Initial_IPT();
    Initial_ITLB();

    Initial_DCache();
    Initial_DPT();
    Initial_DTLB();

    while(1){
        fprintf(snapshot,"cycle %d\n",cycle++);
        for(i=0;i<32;i++)
            fprintf(snapshot,"$%02d: 0x%08X\n", i, reg[i]);
        fprintf(snapshot,"PC: 0x%08X\n\n\n", pc);

        Check_ITLB(pc);
        Check_ICache(pc);

        if(idisk[pc].type=='R')
            R_Ins(idisk[pc].func,idisk[pc].rs,idisk[pc].rt,idisk[pc].rd,idisk[pc].shamt);

        else if(idisk[pc].type=='I')
            I_Ins(idisk[pc].op,idisk[pc].rs,idisk[pc].rt,idisk[pc].immi);

        else if(idisk[pc].type=='S')
            break;

        else if(idisk[pc].type=='J')
            J_Ins(idisk[pc].op, idisk[pc].immi);

        else//sll
            R_Ins(idisk[pc].func,idisk[pc].rs,idisk[pc].rt,idisk[pc].rd,idisk[pc].shamt);

        reg[0]=0;

    }

    fprintf(report,"ICache :\n# hits: %d\n# misses: %d\n\n", ICache_hit, ICache_miss);
    fprintf(report,"DCache :\n# hits: %d\n# misses: %d\n\n", DCache_hit, DCache_miss);
    fprintf(report,"ITLB :\n# hits: %d\n# misses: %d\n\n", ITLB_hit, ITLB_miss);
    fprintf(report,"DTLB :\n# hits: %d\n# misses: %d\n\n", DTLB_hit, DTLB_miss);
    fprintf(report,"IPageTable :\n# hits: %d\n# misses: %d\n\n", IPT_hit, IPT_miss);
    fprintf(report,"DPageTable :\n# hits: %d\n# misses: %d\n\n", DPT_hit, DPT_miss);

    fclose(snapshot);
    fclose(report);
}
