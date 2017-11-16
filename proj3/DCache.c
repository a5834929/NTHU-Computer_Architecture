extern int parameter[10];
extern int DCache_hit, DCache_miss, Dpass_PPN;

int Dblock_size, Dblock_num, Dpage_size;
int Dcache_size, Dcache_set, Dcache_way;
int DC_LRU[1024][1024];
int DC_LRU_tail[1024];
int DCache[1024][3];
/*
    DCache[i][0] = tag
    DCache[i][1] = valid
    DCache[i][2] = PPN
*/

void Initial_DCache(void){
    Dcache_size = parameter[7];
    Dblock_size = parameter[8];
    Dblock_num = Dcache_size/Dblock_size;
    Dcache_way = parameter[9];
    Dcache_set = Dblock_num/Dcache_way;

    int i, j;
    for(i=0;i<Dcache_size;i++){
        DCache[i][0] = -1;
        DCache[i][1] = 0;
        DCache[i][2] = -1;
        DC_LRU_tail[i] = 0;
    }
    for(i=0;i<Dcache_set;i++)
        for(j=0;j<Dcache_way;j++)
            DC_LRU[i][j] = -1;
}

void Check_DCache(int DDisk_addr){
    int PA, index, tag ,flag_hit;
    int now_set, i, isFull;
    Dpage_size = parameter[3];
    PA = Dpass_PPN*Dpage_size + DDisk_addr%Dpage_size;
    index = (PA/Dblock_size)%Dcache_set;
    tag = (PA/Dblock_size)/Dcache_set;
    now_set = index*Dcache_way;
    flag_hit = 0;

    for(i=now_set;i<now_set+Dcache_way;i++){
        if(DCache[i][1] && DCache[i][0]==tag){
            DCache_hit++;
            flag_hit = 1;
            LRU_Update(DC_LRU[index], i-now_set, DC_LRU_tail[index]);
            break;
        }
    }
    if(!flag_hit){
        DCache_miss++;
        isFull = 1;
        for(i=now_set;i<now_set+Dcache_way;i++){//check invalid
            if(!DCache[i][1]){
                DCache[i][0] = tag;
                DCache[i][1] = 1;
                DCache[i][2] = Dpass_PPN;
                DC_LRU[index][DC_LRU_tail[index]++] = i-now_set;
                isFull = 0;
                break;
            }
        }
        if(isFull){
            DCache[now_set + DC_LRU[index][0] ][0] = tag;
            DCache[now_set + DC_LRU[index][0] ][1] = 1;
            DCache[now_set + DC_LRU[index][0] ][2] = Dpass_PPN;
            LRU_Update(DC_LRU[index], DC_LRU[index][0], DC_LRU_tail[index]);
        }
    }
}
void DCache_update(int PPN){
    int i;
    for(i=0;i<Dblock_num;i++){
        if(DCache[i][2]==PPN && DCache[i][1]){
            DCache[i][1] = 0;
            LRU_Update(DC_LRU[i/Dcache_way],i%Dcache_way,Dcache_way);
            DC_LRU_tail[i/Dcache_way]--;
        }
    }
}
