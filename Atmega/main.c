#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "keypad.h"
#define C(x) (char)(48+x)

void check(void); 
void reset(void);
void victory(void);
void error(char *);
void load(void);
int enable(int);//1-4 lunar------5-8 command---------return 1 if error occurs 
int disable(int);//1-4 lunar------5-8 command---------return 1 if error occurs
int calc_amp();

char ch[4]={'\0','\0','\0','\0'};
int pos=0;
int max_amp=13;
int amp=5;
int pc=1;

int flag_airlock=0;
int flag_calibrated=0;
int flag_aligned=0;
int flag_com=1;

char enabled[9]={0,1,0,0,0,0,0,0,0};

int main(void)
{	
	
	char buffer;
		
    lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	keypad_init();
	load();
    lcd_clrscr();
	lcd_gotoxy(0,1);
	lcd_puts("PC:");
	lcd_putc(C(digit(pc,2)));
	lcd_putc(C(digit(pc,1)));
	lcd_gotoxy(9,1);
	lcd_puts("AMPS:");
	lcd_putc(C(digit(amp,2)));
	lcd_putc(C(digit(amp,1)));
	lcd_gotoxy(0,0);

	while(1)
	{
		buffer=keypad_loop();
		if(buffer!=KEY_CHAR_NO_KEY&&buffer!=KEY_CHAR_BUTTON_RELEASE)
		{
			if(buffer == '#')	check();
			else if(buffer == '*')
			{	
				if(pos)
				{
					pos--;
					ch[pos]='\0';
					lcd_gotoxy(pos,0);
					lcd_putc(' ');
					lcd_gotoxy(pos,0);
				}		
			}
			else if(pos<3)
			{
				ch[pos]=buffer;
				lcd_putc(buffer);
				pos++;
			}
		}
	}
}


void check(void)
{

////////////////////////////////////syntax checking //////////////////////////////////////
	if(pos!=3)	
	{
		error("SYNTAX ERROR\nCode incomplete!");
		return;
	}
	if( 	(!(ch[0]=='E'||ch[0]=='D'))
	     || (!(ch[1]=='L'||ch[1]=='C'))
		 || (!(ch[2]=='1'||ch[2]=='2'||ch[2]=='3'||ch[2]=='4'))
	  )
	{
		error("SYNTAX ERROR\nUnknown command!");
		return;
	}


////////////////////////////////syntax checking complete///////////////////////////////// 
	if(ch[0]=='E')//enable
	{
		if(ch[1]=='L')//lunar module
		{
			if(enable(ch[2]-48))//error
			return;
		}
		else if(enable(ch[2]-44))//command module
		return;
	
	}
	else //disable
	{
		if(ch[1]=='L')//lunar module
		{
			if(disable(ch[2]-48))//error
			return;
		}
		else if(disable(ch[2]-44))//command module
		return;
	
	}

////////////////////////////no error for enabling or disabling////////////////////////////////////
  amp=calc_amp();
  if(amp>max_amp)
  {
  	error("OVERLOAD! \nBattery failed!");
	return;
  }
  if(flag_airlock)	enabled[4]=0;  //automatically disable airlock
  amp=calc_amp();

  if( ch[0]=='E' && ch[1]=='C' && ch[2]=='3' )
  {
  	victory();
	return;
  }

  pc++;
  if(pc>14)
  {
  	error("Procedure too\nlong to transmit");
	return;
  }
	
	lcd_clrscr();
	lcd_gotoxy(0,1);
	lcd_puts("PC:");
	lcd_putc(C(digit(pc,2)));
	lcd_putc(C(digit(pc,1)));
	lcd_gotoxy(9,1);
	lcd_puts("AMPS:");
	lcd_putc(C(digit(amp,2)));
	lcd_putc(C(digit(amp,1)));
	lcd_gotoxy(0,0);
	pos=0;
	ch[0]='\0';
	ch[1]='\0';
	ch[2]='\0';
	ch[3]='\0';
}

void victory(void)
{
	lcd_clrscr();
	lcd_command(LCD_DISP_ON);
	for(int i=0;i<5;i++)
	{
		lcd_puts("-LEVEL CLEARED!-");
		_delay_ms(200);
		lcd_clrscr();
		_delay_ms(200);
	}
	lcd_puts("Good job.\nCongratulations");
	_delay_ms(100);
	keypad_wait_till_key_pressed(KEY_CHAR_C2_R3);
	lcd_command(LCD_DISP_ON_CURSOR_BLINK);
	reset();
}

void error(char *p)
{
	lcd_clrscr();
	lcd_command(LCD_DISP_ON);
	for(int i=0;i<5;i++)
	{
		lcd_puts("-----ERROR------");
		_delay_ms(200);
		lcd_clrscr();
		_delay_ms(200);
	}
	lcd_puts(p);
	_delay_ms(100);
	keypad_wait_till_key_pressed(KEY_CHAR_C2_R3);
	lcd_command(LCD_DISP_ON_CURSOR_BLINK);
	reset();
}

void reset(void)
{
	max_amp=13;
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
	flag_airlock=0;
	flag_calibrated=0;
	flag_aligned=0;
	flag_com=1;
	pc=1;
	pos=0;
	ch[0]='\0';
	ch[1]='\0';
	ch[2]='\0';
	ch[3]='\0';
	load();
	lcd_clrscr();
	lcd_gotoxy(0,1);
	lcd_puts("PC:");
	lcd_putc(C(digit(pc,2)));
	lcd_putc(C(digit(pc,1)));
	lcd_gotoxy(9,1);
	lcd_puts("AMPS:");
	lcd_putc(C(digit(amp,2)));
	lcd_putc(C(digit(amp,1)));
	lcd_gotoxy(0,0);
}
	
void load(void)
{
	lcd_clrscr();
	lcd_command(LCD_DISP_ON);
	for(int i=0;i<4;i++)
	{
		lcd_gotoxy(0,0);
		lcd_puts("   /LOADING/    ");
		lcd_gotoxy(0,1);
		lcd_puts("      .         ");
		_delay_ms(100);
		lcd_gotoxy(0,0);
		lcd_puts("   -LOADING-    ");
		lcd_gotoxy(0,1);
		lcd_puts("      ..        ");
		_delay_ms(100);
		lcd_gotoxy(0,0);
		lcd_puts("   ");lcd_putc(0xCD);lcd_puts("LOADING");lcd_putc(0xCD);
		lcd_gotoxy(0,1);
		lcd_puts("      ...       ");
		_delay_ms(100);
		lcd_gotoxy(0,0);
		lcd_puts("   |LOADING|    ");
		lcd_gotoxy(0,1);
		lcd_puts("      ....      ");
		_delay_ms(100);
		lcd_gotoxy(0,0);
		lcd_puts("   /LOADING/    ");
		lcd_gotoxy(0,1);
		lcd_puts("       ...      ");
		_delay_ms(100);
		lcd_gotoxy(0,0);
		lcd_puts("   -LOADING-    ");
		lcd_gotoxy(0,1);
		lcd_puts("        ..      ");
		_delay_ms(100);
		lcd_gotoxy(0,0);
		lcd_puts("   ");lcd_putc(0xCD);lcd_puts("LOADING");lcd_putc(0xCD);
		lcd_gotoxy(0,1);
		lcd_puts("         .      ");
		_delay_ms(100);
		lcd_gotoxy(0,0);
		lcd_puts("   |LOADING|    ");
		lcd_gotoxy(0,1);
		lcd_puts("                ");
		_delay_ms(100);
	}
	lcd_command(LCD_DISP_ON_CURSOR_BLINK);
}

int enable(int a)
{
	if(enabled[a])
	{
		error("Cannot enable a \nenabled device!");
		return 1;
	}

	switch (a)
	{
		case 1:	//lunar communication
			   if(flag_airlock)
			   {
			   	error("Lunar Module is \nnot avaliable!");
				return 1;
			   }
			   flag_com++;
			   break;
		case 2: //lunar thrusters
			   if(flag_airlock)
			   {
			   	error("Lunar Module is \nnot avaliable!  ");
				return 1;
			   }
			   if(!flag_calibrated)
			   {
			   	error("Feedback loop is\nnot calibrated! ");
				return 1;
			   }
			   if(!enabled[8])
			   {
			   	error("Navigation comp.\nunresponsive!   ");
				return 1;
			   }
			   flag_aligned=1;
			   break;
		case 3: // lunar gyroscope
			   if(!enabled[8])
			   {
			   	error("Navigation comp.\nunresponsive!   ");
				return 1;
			   }
			   if(flag_airlock)
			   {
			   	error("Lunar Module is \nnot avaliable!");
				return 1;
			   }
			   flag_calibrated=1;
			   break;
		case 4: // lunar airlock
			   if(flag_airlock)
			   {
			   	error("Lunar Module is \nnot avaliable!");
				return 1;
			   }
			   if(enabled[1]||enabled[2]||enabled[3]||enabled[4])
			   {
			   	error("Lunar Module is \ncurrently in use");
				return 1;
			   }
			   flag_airlock=1;
			   break;
		case 5: //command communication
			   flag_com++;
			   break;
		case 6: //command thrusters 
		       if(!flag_calibrated)
			   {
			   	error("Feedback loop is\nnot calibrated! ");
				return 1;
			   }
			   if(!enabled[8])
			   {
			   	error("Navigation comp.\nunresponsive!   ");
				return 1;
			   }
			   flag_aligned=1;
			   break;
		case 7:
		       if(!flag_aligned)
			   {
			   	error("Re-entry pitch \nnot aligned!");
				return 1;
			   }
			   if(!flag_airlock)
			   {
			   	error("Lunar Module is \nstill attached!");
				return 1;
			   } 
			   //command parachute
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
	{
		error("Cannot disable a\ndisabled device!");
		return 1;
	}
	switch (a)
	{
		case 1://lunar communication
			   flag_com--;
			   if(!flag_com)
			   {
			   	error("FATAL ERROR.\nUplink failed!");
				return 1;
			   }	
			   break;
		case 2://lunar thrusters
			   if(!enabled[8])
			   {
			   	error("Navigation comp.\nunresponsive!   ");
				return 1;
			   }
			   max_amp=10;
			   break;
		case 3: // lunar gyroscope
		       if(!enabled[8])
			   {
			   	error("Navigation comp.\nunresponsive!   ");
				return 1;
			   }
			   break;
		case 4:// lunar airlock
			   break;
		case 5://command communication
		       flag_com--;
			   if(!flag_com)
			   {
			   	error("FATAL ERROR.\nUplink failed!");
				return 1;
			   }
			   break;
		case 6://command thrusters
			   if(!enabled[8])
			   {
			   	error("Navigation comp.\nunresponsive!   ");
				return 1;
			   }
			   max_amp=10;
			   break;
		case 7://command parachute
			   break;
		case 8://command navigation
		           if(enabled[2] || enabled[3] || enabled[6])
			   {
			   	error("Navigation comp.\nunresponsive!   ");
				return 1;
			   }
			   break;
	}
	enabled[a]=0;
	return 0;
}	

int calc_amp()
{
	return enabled[1]*5+enabled[2]*3+
		   enabled[3]*4+enabled[4]*4+
		   enabled[5]*6+enabled[6]*4+
		   enabled[7]*4+enabled[8]*4;
}
