#include<string.h>
extern int reg[32];
extern char DDisk[1024];
void read_D(){
    FILE *in = fopen("dimage.bin", "rb");
    int sp;
    unsigned int num;

    memset(DDisk,0,sizeof(DDisk));

    fread(&DDisk, 4, 1, in);
    sp=(((DDisk[0]<<24)|((DDisk[1]&255)<<16))|((DDisk[2]&255)<<8))|(DDisk[3]&255);
    reg[29]=sp;

    fread(&DDisk, 4, 1, in);
    num=(((DDisk[0]<<24)|((DDisk[1]&255)<<16))|((DDisk[2]&255)<<8))|(DDisk[3]&255);
    fread(&DDisk, 4, num, in);
}
