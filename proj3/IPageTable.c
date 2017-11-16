extern int parameter[10];
extern int IPT_hit, IPT_miss;

int Imemory_size, Ipage_size, Ipagetable_entry, IPPN_num;
extern int ITLB_PT_LRU[1024], ITLB_PT_LRU_tail;
int IPageTable[1024][2];
/*
    IPageTable[i][0] = valid;
    IPageTable[i][1] = PPN;
*/

void Initial_IPT(void){
    IPPN_num = 0;
    ITLB_PT_LRU_tail = 0;
    Ipage_size = parameter[2];
    Ipagetable_entry = 1024/Ipage_size;
    int i;
    for(i=0;i<Ipagetable_entry;i++){
        IPageTable[i][0] = 0;
        IPageTable[i][1] = -1;
        ITLB_PT_LRU[i] = -1;
    }
}

int Check_IPageTable(int pc, int VPN, int PPN_total){
    if(IPPN_num<PPN_total){
        if(!IPageTable[VPN][0]){
            IPT_miss++;
            IPageTable[VPN][0] = 1;
            IPageTable[VPN][1] = IPPN_num;
            ITLB_PT_LRU[ITLB_PT_LRU_tail++] = VPN;
            LRU_Update(ITLB_PT_LRU,VPN, ITLB_PT_LRU_tail);
        }
        else{
            IPT_hit++;
            LRU_Update(ITLB_PT_LRU,VPN, ITLB_PT_LRU_tail);
        }
        IPPN_num++;
    }
    else{//full
        if(!IPageTable[VPN][0]){//valid = 0
            IPT_miss++;
            IPageTable[VPN][0] = 1;
            IPageTable[VPN][1] = IPageTable[ITLB_PT_LRU[0]][1];
            IPageTable[ITLB_PT_LRU[0]][0] = 0;
            ICache_update(IPageTable[ITLB_PT_LRU[0]][1]);
            ITLB_PT_LRU[0] = VPN;
            LRU_Update(ITLB_PT_LRU,ITLB_PT_LRU[0], ITLB_PT_LRU_tail);
        }
        else{
            IPT_hit++;
            LRU_Update(ITLB_PT_LRU,VPN, ITLB_PT_LRU_tail);
        }
    }
    return IPageTable[VPN][1];
}


