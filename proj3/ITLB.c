extern int parameter[10];
extern int ITLB_hit, ITLB_miss, Ipagetable_entry;
int ITLB_PT_LRU[1024], Imemory_size,ITLB_PT_LRU_tail;
int ITLB_entry, Ipage_size, Ipass_PPN;
int ITLB[1024][3];
/*
    ITLB[i][0] = tag
    ITLB[i][1] = valid
    ITLB[i][2] = PPN
*/

void Initial_ITLB(void){
    ITLB_entry = Ipagetable_entry/4;
    ITLB_PT_LRU_tail = 0;
    int i;
    for(i=0;i<ITLB_entry;i++){
        ITLB[i][0] = -1;
        ITLB[i][1] = 0;
        ITLB[i][2] = -1;
        ITLB_PT_LRU[i] = -1;
    }
}

void Check_ITLB(int pc){
    int i, VPN, flag_hit;
    int isFull=1;
    Imemory_size = parameter[0];
    Ipage_size = parameter[2];
    int PPN_num = Imemory_size/Ipage_size;
    int limit = (PPN_num < ITLB_entry)?PPN_num:ITLB_entry;

    VPN = pc/Ipage_size;
    flag_hit = 0;

    if(ITLB_PT_LRU_tail < limit)isFull = 0;

    for(i=0;i<ITLB_entry;i++){
        if(ITLB[i][1] && ITLB[i][0]==VPN){//hit
            ITLB_hit++;
            flag_hit = 1;
            Ipass_PPN = ITLB[i][2];
            LRU_Update(ITLB_PT_LRU, VPN, ITLB_PT_LRU_tail);
            break;
        }
    }
    if(!flag_hit){//miss: full or not-full
        ITLB_miss++;
        int tmp = ITLB_PT_LRU_tail;
        if(!isFull){//ITLB_PT_LRU_tail is the first empty place
            ITLB[tmp][0] = VPN;
            ITLB[tmp][1] = 1;
            ITLB[tmp][2] = Check_IPageTable(pc, VPN, PPN_num);
            Ipass_PPN = ITLB[tmp][2];
            //ITLB_PT_LRU[ITLB_PT_LRU_tail++] = VPN;
        }
        else{
            for(i=0;i<ITLB_entry;i++){
                if(ITLB[i][0]==ITLB_PT_LRU[0]){//overlap the LRU VPN
                    ITLB[i][0] = VPN;
                    ITLB[i][1] = 1;
                    ITLB[i][2] = Check_IPageTable(pc, VPN, PPN_num);
                    Ipass_PPN = ITLB[i][2];
                    break;
                }
            }
        }
    }
}

