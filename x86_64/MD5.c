#include <ctype.h>

unsigned char shift [20] = {9,0,3,2,12,13,24,18,7,11,5,3,21,3,1,4,16,22,2,19};

//max length 20
void encrypt(unsigned char*input,unsigned char* output)
{
    int i =0;
    for(i=0;i<20;i++)
    {
        output[i]=input[i]+shift[i];
        if(input[i] == '\0')
        {
            output[i] = input[i];
            break;
        }
        else if(islower(input[i]))
            while(output[i] > 122)
                  output[i] -=26;
        else if(isupper(input[i]))
            while(output[i] >90)
                output[i] -=26;
        else if(48<=input[i] && 57>=input[i])
            while(output[i] >57)
                output[i] -=10;
        else output[i] = input[i];
    }
}

