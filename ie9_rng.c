#include "stdio.h"
#include "math.h"

void printBits(unsigned long long num, int nbits)
{
   for(int bit=0;bit<nbits; bit++)
   {
      printf("%llu ", num & 0x01);
      num = num >> 1;
   }
   printf("\n");
}

unsigned long long a = 0x5DEECE66D;
unsigned long long b = 0xB;
unsigned long long NextState(unsigned long long x)
{
    return (a*x+b) & ((1ull<<48)-1); 
}

unsigned long long state = 0;
int pos[32]={17,19,21,23,25,27,29,31,1,3,5,7,9,11,13,15, 16,18,20,22,24,26,28,30,0,2,4,6,8,10,12,14};
//Epoch time in milliseconds
void SeedState(unsigned long long time)
{
    unsigned int t_low_32 = time;
    unsigned short t_high_16 = time>>32;

    unsigned short s_low_16 = t_high_16^t_low_32;
    unsigned int s_high_32_tmp = t_low_32^0xDEECE66Du;
    unsigned int s_high_32 = 0u;

    for (int i=0;i<32;i++)
    {
        s_high_32 |= ((s_high_32_tmp>>i)&1)<<pos[i];
    }

    state = (unsigned long long)s_high_32 << 16 | s_low_16;

}

double RandomNext()
{
    state = NextState(state);
    unsigned long long s1 = state >> 21;
    state = NextState(state);
    unsigned long long s2 = state >> 21;

    unsigned long long rndInt = (unsigned long long)(s1 << 27) | s2;

    //rounding to nearest even number
    if ((rndInt & (1ull<<53)) && (rndInt & 1))
    {
        rndInt--;
        if (rndInt & 2)
        rndInt+=2;
    }

    double result = (double)rndInt/(double)(1ull<<54);

    return result;
}

void PrintNext256HexRandByte()
{
    int bytes[32];
    for (int i = 0; i < 32; i++)
    {
        bytes[i] = floor(RandomNext()*256);
        printf("%02x", bytes[i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{   
    for (unsigned long long i = 0; i < 86400000ull*30*atoi(argv[1]); i++)
    {
        SeedState(1262300400000ull+i);
        PrintNext256HexRandByte();
    }
    
    return 0;
}
