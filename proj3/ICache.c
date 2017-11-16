extern int parameter[10];
extern int ICache_hit, ICache_miss, Ipass_PPN;

int Iblock_size, Iblock_num, Ipage_size;
int Icache_size, Icache_set, Icache_way;
int IC_LRU[1024][1024];
int IC_LRU_tail[1024];
int ICache[1024][3];
/*
    ICache[i][0] = tag
    ICache[i][1] = valid
    ICache[i][2] = PPN
*/

void Initial_ICache(void){
    Icache_size = parameter[4];
    Iblock_size = parameter[5];
    Iblock_num = Icache_size/Iblock_size;
    Icache_way = parameter[6];
    Icache_set = Iblock_num/Icache_way;
    int i, j;
    for(i=0;i<Icache_size;i++){
        ICache[i][0] = -1;
        ICache[i][1] = 0;
        ICache[i][2] = -1;
        IC_LRU_tail[i] = 0;
    }
    for(i=0;i<Icache_set;i++)
        for(j=0;j<Icache_way;j++)
            IC_LRU[i][j] = -1;
}

void Check_ICache(int pc){
    int PA, index, tag ,flag_hit,isFull;
    int now_set, i;
    Ipage_size = parameter[2];
    PA = Ipass_PPN*Ipage_size + pc%Ipage_size;
    index = (PA/Iblock_size)%Icache_set;
    tag = (PA/Iblock_size)/Icache_set;
    now_set = index*Icache_way;
    flag_hit = 0;
    for(i=now_set;i<now_set+Icache_way;i++){
        if(ICache[i][1] && ICache[i][0]==tag){
            ICache_hit++;
            flag_hit = 1;
            LRU_Update(IC_LRU[index], i-now_set, IC_LRU_tail[index]);
            break;
        }
    }
    if(!flag_hit){
        ICache_miss++;
        isFull = 1;
        for(i=now_set;i<now_set+Icache_way;i++){//check invalid
            if(!ICache[i][1]){
                ICache[i][0] = tag;
                ICache[i][1] = 1;
                ICache[i][2] = Ipass_PPN;
                IC_LRU[index][IC_LRU_tail[index]++] = i-now_set;
                isFull = 0;
                break;
            }
        }
        if(isFull){
            ICache[now_set + IC_LRU[index][0] ][0] = tag;
            ICache[now_set + IC_LRU[index][0] ][1] = 1;
            ICache[now_set + IC_LRU[index][0] ][2] = Ipass_PPN;
            LRU_Update(IC_LRU[index], IC_LRU[index][0], IC_LRU_tail[index]);
        }
    }
}

void ICache_update(int PPN){
    int i;

    for(i=0;i<Icache_size;i++){
        if(ICache[i][2]==PPN && ICache[i][1]){
            ICache[i][1] = 0;
            LRU_Update(IC_LRU[i/Icache_way],ICache[i][0],Icache_way);
            IC_LRU_tail[i/Icache_way]--;
        }
    }
}
