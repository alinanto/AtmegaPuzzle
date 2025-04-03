#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "apollo.h"

int mAmp,amp,PC,DR,TA,NC,CM;
char enabled[9];

int enable(int);
//// enables the module
//// ARGUMENT : 1-4 for lunar
//// ARGUMENT : 5-8 for command

int disable(int);
//// enables the module
//// ARGUMENT : 1-4 for lunar
//// ARGUMENT : 5-8 for command

int calc_amp(void);
//// calculates the total AMPERE used

int syntax(char* ch)
{
	if(strlen(ch) != 3)
        PERROR("SYNTAX ERROR!\nCode incomplete!\n")
   	else if( 	(!(ch[0]=='E'||ch[0]=='D'))
	     || (!(ch[1]=='L'||ch[1]=='C'))
		 || (!(ch[2]=='1'||ch[2]=='2'||ch[2]=='3'||ch[2]=='4')))
        PERROR("SYNTAX ERROR!\nUnknown command!\n")
    else return 0;
}

int execute(char* ch)
{
    int error=0;
    if(ch[0]=='E')//enable
	{
		if(ch[1]=='L')
             error = enable( ch[2]-48 );//lunar module
		else error = enable( ch[2]-44 );//command module
	}
	else if(ch[1]=='L')
         error = disable( ch[2]-48 );//lunar module
	else error = disable( ch[2]-44 );//command module

	if(error) return 1;
    amp=calc_amp();
    if(amp>mAmp)
        PERROR("CRITICAL ERROR!\nBattery overloaded.\n")
    if(DR) enabled[4]=0;  //automatically disable airlock
    amp=calc_amp();
    PC++;
    if(PC>13)
        PERROR("UPLINK-ERROR!\nProcedure too long to transmit.\n")
    else return 0;
}

int victory(char * ch)
{
    if( ch[0]=='E' && ch[1]=='C' && ch[2]=='3' )
        PERROR("Congratulations....\nYou have cracked Situation Apollo 13.\n")
    else return 0;
}

void reset(void)
{
	mAmp=13;
	amp=5;
	enabled[0]=0;
	enabled[1]=1;
	enabled[2]=0;
	enabled[3]=0;
	enabled[4]=0;
	enabled[5]=0;
	enabled[6]=0;
	enabled[7]=0;
	enabled[8]=0;
	DR=0;
	NC=0;
	TA=0;
	CM=1;
	PC=1;
	printf("Up-link activated.\n\n");
}

int enable(int a)
{
	if(enabled[a])
		PERROR("LOGICAL ERROR!\nCannot enable a enabled device!\n")
	switch (a)
	{
		case 1:	//lunar communication
			   if(DR)
                   PERROR("FATAL ERROR!\nLunar Module is not available!\n")
			   else CM++;
			   break;
		case 2: //lunar thrusters
			   if(DR)
                   PERROR("FATAL ERROR!\nLunar Module is not available!\n")
			   else if(!enabled[8])
                   PERROR("FATAL ERROR!\nNavigation computer unresponsive!\n")
			   else if(!NC)
                   PERROR("FEEDBACK ERROR!\nNavigation computer is not calibrated!\n")
			   else TA=1;
			   break;
		case 3: // lunar gyroscope
		       if(DR)
                   PERROR("FATAL ERROR!\nLunar Module is not available!\n")
			   else if(!enabled[8])
                   PERROR("FATAL ERROR!\nNavigation computer unresponsive!\n")
               else NC=1;
			   break;
		case 4: // lunar airlock
			   if(DR)
                   PERROR("FATAL ERROR!\nLunar Module is not available!\n")
			   else if(enabled[1]||enabled[2]||enabled[3]||enabled[4])
                   PERROR("ACCESS DENIED!\nCannot un-dock while lunar Module is still in use!\n")
			   else DR=1;
			   break;
		case 5: //command communication
			   CM++;
			   break;
		case 6: //command thrusters
               if(!enabled[8])
                   PERROR("FATAL ERROR!\nNavigation computer unresponsive!\n")
               else if(!NC)
                    PERROR("FEEDBACK ERROR!\nNavigation computer is not calibrated!\n")
               else TA=1;
			   break;
		case 7: //command parachutes
		       if(!DR)
			       PERROR("ACCESS DENIED!\nCannot deploy while lunar Module is still attached!\n")
               else if(!TA)
                   PERROR("SYSTEM ERROR!\nRe-entry pitch not aligned!\n")
			   break;
		case 8: //command navigation
               break;
	}
	enabled[a]=1;
	return 0;
}
int disable(int a)
{
	if(!enabled[a])
        PERROR("LOGICAL ERROR!\nCannot disable a disabled device!\n")
    switch (a)
	{
		case 1://lunar communication
			   CM--;
			   if(!CM)
                   PERROR("COMMS ERROR!\nUp-link failed!\n")
			   break;
		case 2://lunar thrusters
			   if(!enabled[8])
                   PERROR("FATAL ERROR!\nNavigation computer unresponsive!\n")
			   else mAmp=10;
			   break;
		case 3: // lunar gyroscope
		       if(!enabled[8])
			       PERROR("FATAL ERROR!\nNavigation computer unresponsive!\n")
			   break;
		case 4:// lunar airlock
			   break;
		case 5://command communication
		       CM--;
			   if(!CM)
			        PERROR("COMMS ERROR!\nUp-link failed!\n")
			   break;
		case 6://command thrusters
			   if(!enabled[8])
			        PERROR("FATAL ERROR!\nNavigation computer unresponsive!\n")
               else mAmp=10;
			   break;
		case 7://command parachute
			   break;
		case 8://command navigation
		       if(enabled[2] || enabled[3] || enabled[6])
                   PERROR("ACCESS DENIED!\nCannot disable while thrusters or gyroscope is in use!\n")
               break;
	}
	enabled[a]=0;
	return 0;
}

int digit(int num,unsigned int dig)
{
    num=abs(num);
	int temp=0;
	do{
        temp=num%10;
		num=num/10;
		dig--;
	  }while(dig);
	return	temp;
}

int calc_amp()
{
	return enabled[1]*5+enabled[2]*3+
		   enabled[3]*4+enabled[4]*4+
		   enabled[5]*6+enabled[6]*4+
		   enabled[7]*4+enabled[8]*4;
}
