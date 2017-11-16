void LRU_Update(int* LRU, int VPN, int len){
    int i, j, tmp;
    for(i=0;i<len;i++)
        if(LRU[i]==VPN) break;

    tmp = LRU[i];
    for(j=i+1;j<len;j++)
        LRU[j-1] = LRU[j];
    LRU[len-1] = tmp;
}
