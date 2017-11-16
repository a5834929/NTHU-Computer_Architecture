extern int parameter[10];
extern int DTLB_hit, DTLB_miss, Dpagetable_entry;
int DTLB_PT_LRU[1024], Dmemory_size,DTLB_PT_LRU_tail;
int DTLB_entry, Dpage_size, Dpass_PPN;
int DTLB[1024][3];
/*
    DTLB[i][0] = tag
    DTLB[i][1] = valid
    DTLB[i][2] = PPN
*/

void Initial_DTLB(void){
    DTLB_entry = Dpagetable_entry/4;
    DTLB_PT_LRU_tail = 0;
    int i;
    for(i=0;i<DTLB_entry;i++){
        DTLB[i][0] = -1;
        DTLB[i][1] = 0;
        DTLB[i][2] = -1;
        DTLB_PT_LRU[i] = -1;
    }
}

void Check_DTLB(int pc){
    int i, VPN, flag_hit;
    int isFull=1;
    Dmemory_size = parameter[1];
    Dpage_size = parameter[3];
    int PPN_num = Dmemory_size/Dpage_size;
    int limit = (PPN_num < DTLB_entry)?PPN_num:DTLB_entry;

    VPN = pc/Dpage_size;
    flag_hit = 0;
    if(DTLB_PT_LRU_tail < limit)isFull = 0;

    for(i=0;i<DTLB_entry;i++){
        if(DTLB[i][1] && DTLB[i][0]==VPN){//hit
            DTLB_hit++;
            flag_hit = 1;
            Dpass_PPN = DTLB[i][2];
            LRU_Update(DTLB_PT_LRU, VPN, DTLB_PT_LRU_tail);
            break;
        }
    }
    if(!flag_hit){//miss: full or not-full
        DTLB_miss++;
        int tmp=DTLB_PT_LRU_tail;
        if(!isFull){//ITLB_LRU_tail is the first empty place
            DTLB[tmp][0] = VPN;
            DTLB[tmp][1] = 1;
            DTLB[tmp][2] = Check_DPageTable(VPN, PPN_num);
            Dpass_PPN = DTLB[tmp][2];
        }
        else{
            for(i=0;i<DTLB_entry;i++){
                if(DTLB[i][0]==DTLB_PT_LRU[0]){//overlap the LRU VPN
                    DTLB[i][0] = VPN;
                    DTLB[i][1] = 1;
                    DTLB[i][2] = Check_DPageTable(VPN, PPN_num);
                    Dpass_PPN = DTLB[i][2];
                    break;
                }
            }
        }
    }
}

