#include<string.h>
extern int reg[32];
extern char Dinput[1024];

void read_D(){
    FILE *in = fopen("dimage.bin", "rb");
    int sp;
    int num;

    memset(Dinput,0,sizeof(Dinput));

    fread(&Dinput, 4, 1, in);
    sp=(((Dinput[0]<<24)|((Dinput[1]&255)<<16))|((Dinput[2]&255)<<8))|(Dinput[3]&255);
    reg[29]=sp;

    fread(&Dinput, 4, 1, in);
    num=(((Dinput[0]<<24)|((Dinput[1]&255)<<16))|((Dinput[2]&255)<<8))|(Dinput[3]&255);
    fread(&Dinput, 4, num, in);
}
