#include <stdio.h>
#include <string.h>
#include "MD5.h"
#include "apollo.h"

extern int mAmp,amp,PC,DR,TA,NC,CM;
extern char enabled[9];

int main()
{
    printf("###################################################################\n");
    printf("##                                                               ##\n");
    printf("##                      SITUATION APOLLO 13                      ##\n");
    printf("##                                                               ##\n");
    printf("###################################################################\n");

    char password[20];
    char hash[20];
    while(1)
    {
        printf("\nEnter the password : ");
        scanf("%s",password);
        encrypt(password,hash);
        //printf("HASH = %s\n",hash);
        if( strcmp(hash,"v?D633/ibm9") ==0 )  break;
        else printf("Password incorrect!\n");
    }

    char ch[100]="";
    reset();
	while(1)
	{
	    printf("PC: %d%d | AMPERE CONSUMED: %d%d | AMPERE LIMIT: %d%d | COMMAND: ",
            digit(PC,2),
            digit(PC,1),
            digit(amp,2),
            digit(amp,1),
            digit(mAmp,2),
            digit(mAmp,1));
        scanf("%s",ch);
        if(syntax(ch))   continue;
        if(execute(ch))  continue;
        if(victory(ch))  continue;
	}
}
