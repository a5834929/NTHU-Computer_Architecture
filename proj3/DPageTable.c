extern int parameter[10];
extern int DPT_hit, DPT_miss;

int Dmemory_size, Dpage_size, Dpagetable_entry, DPPN_num;
extern int DTLB_PT_LRU[1024], DTLB_PT_LRU_tail;
int DPageTable[1024][2];
/*
    DPageTable[i][0] = valid;
    DPageTable[i][1] = PPN;
*/

void Initial_DPT(void){
    DPPN_num = 0;
    DTLB_PT_LRU_tail = 0;
    Dpage_size = parameter[3];
    Dpagetable_entry = 1024/Dpage_size;

    int i;
    for(i=0;i<Dpagetable_entry;i++){
        DPageTable[i][0] = 0;
        DPageTable[i][1] = -1;
        DTLB_PT_LRU[i] = -1;
    }
}

int Check_DPageTable(int VPN, int PPN_total){
    if(DPPN_num<PPN_total){
        if(!DPageTable[VPN][0]){
            DPT_miss++;
            DPageTable[VPN][0] = 1;
            DPageTable[VPN][1] = DPPN_num;
            DTLB_PT_LRU[DTLB_PT_LRU_tail++] = VPN;
            LRU_Update(DTLB_PT_LRU,VPN, DTLB_PT_LRU_tail);
        }
        else{
            DPT_hit++;
            LRU_Update(DTLB_PT_LRU,VPN, DTLB_PT_LRU_tail);
        }
        DPPN_num++;
    }
    else{
        if(!DPageTable[VPN][0]){
            DPT_miss++;
            DPageTable[VPN][0] = 1;
            DPageTable[VPN][1] = DPageTable[DTLB_PT_LRU[0]][1];
            DPageTable[DTLB_PT_LRU[0]][0] = 0;
            DCache_update(DPageTable[DTLB_PT_LRU[0]][1]);
            DTLB_PT_LRU[0] = VPN;
            LRU_Update(DTLB_PT_LRU, VPN, DTLB_PT_LRU_tail);
        }
        else{
            DPT_hit++;
            LRU_Update(DTLB_PT_LRU, VPN, DTLB_PT_LRU_tail);
        }
    }
    return DPageTable[VPN][1];
}
