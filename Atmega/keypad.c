#define F_CPU 8000000
#include <avr/io.h>
#include "keypad.h"
#include <util/delay.h>

#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
    /* on ATmega64/128 PINF is on port 0x00 and not 0x60 */
    #define PIN(x) ( &PORTF==&(x) ? _SFR_IO8(0x00) : (*(&x - 2)) )
#else
	#define PIN(x) (*(&x - 2))    /* address of input register of port x          */
#endif

#define SCAN_COLUMN(x) DDR(KEYPAD_PORT)=_BV(keypad_find_pin(x+4))   
#define CHECK_ROW_CLEAR(x)    bit_is_clear( PIN(KEYPAD_PORT) , keypad_find_pin(x))
#define RELAX_COLUMN() DDR(KEYPAD_PORT)=0x00 

DEBOUNCE debounce = {0,0,KEY_CHAR_NO_KEY};	 
        
void keypad_init()
{
	RELAX_COLUMN();
	KEYPAD_PORT=( _BV(KEYPAD_PIN_R0) | _BV(KEYPAD_PIN_R1) | _BV(KEYPAD_PIN_R2) | _BV(KEYPAD_PIN_R3 ) );
}

char keypad_scan()
{
	for(int column=0;column<4;column++)
    {
        SCAN_COLUMN(column);
        _delay_us(KEYPAD_COLUMN_SCAN_DELAY); 
        for(int row=0;row<4;row++)
        {
            if( CHECK_ROW_CLEAR(row) )
            {
                RELAX_COLUMN();
                return keypad_map_to_key(row,column);
            }
        }
	}
	RELAX_COLUMN();
	return KEY_CHAR_NO_KEY;// if no key is pressed
}

int keypad_key_pressed(char ch)
{
	KEYMAP keymap ={0,0};
    keymap = keypad_key_to_map(ch);
	SCAN_COLUMN(keymap.column);
	_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
	if( CHECK_ROW_CLEAR(keymap.row) )
	{
        RELAX_COLUMN();
	    return 1;
	}
	RELAX_COLUMN();
	return 0;
}

void keypad_wait_till_key_pressed(char ch)
{
	while(!keypad_key_pressed(ch))
	{
	}
}

char keypad_loop()
{
 char buffer;
 buffer=keypad_scan();
 if(buffer==debounce.current_key)
 {
 	if(buffer==KEY_CHAR_NO_KEY)	return KEY_CHAR_NO_KEY;
	else if(!debounce.pressed)
	{
		debounce.confidence++;
 		if(debounce.confidence>=KEYPAD_DEBOUNCE_CYCLES)
		{
			debounce.pressed=1;
			return debounce.current_key;
		}	
	}
	else debounce.confidence=KEYPAD_DEBOUNCE_CYCLES;
 }
 else 
 {
	if(debounce.pressed)
	{
		debounce.confidence--;
 		if(debounce.confidence<=0)
		{
			debounce.pressed=0;
			debounce.current_key=buffer;
			return KEY_CHAR_BUTTON_RELEASE;
		}	
	}
	else 
	{
		debounce.confidence=0;
		debounce.current_key=buffer;
	}
 }
 return KEY_CHAR_NO_KEY;
}

KEYMAP keypad_key_to_map(char ch)
{
    KEYMAP keymap = { 0,0 };
    switch(ch)
	{
		case KEY_CHAR_C0_R0 :keymap.row=0;keymap.column=0;break;
		case KEY_CHAR_C0_R1 :keymap.row=1;keymap.column=0;break;
		case KEY_CHAR_C0_R2 :keymap.row=2;keymap.column=0;break;
		case KEY_CHAR_C0_R3 :keymap.row=3;keymap.column=0;break;
		case KEY_CHAR_C1_R0 :keymap.row=0;keymap.column=1;break;
		case KEY_CHAR_C1_R1 :keymap.row=1;keymap.column=1;break;
		case KEY_CHAR_C1_R2 :keymap.row=2;keymap.column=1;break;
		case KEY_CHAR_C1_R3 :keymap.row=3;keymap.column=1;break;
		case KEY_CHAR_C2_R0 :keymap.row=0;keymap.column=2;break;
		case KEY_CHAR_C2_R1 :keymap.row=1;keymap.column=2;break;
		case KEY_CHAR_C2_R2 :keymap.row=2;keymap.column=2;break;
		case KEY_CHAR_C2_R3 :keymap.row=3;keymap.column=2;break;
		case KEY_CHAR_C3_R0 :keymap.row=0;keymap.column=3;break;
		case KEY_CHAR_C3_R1 :keymap.row=1;keymap.column=3;break;
		case KEY_CHAR_C3_R2 :keymap.row=2;keymap.column=3;break;
		case KEY_CHAR_C3_R3 :keymap.row=3;keymap.column=3;break;
	}
    return keymap;
}

char keypad_map_to_key(int row,int column)
{
    char keymap[4][4] = {
        { KEY_CHAR_C0_R0,KEY_CHAR_C1_R0,KEY_CHAR_C2_R0,KEY_CHAR_C3_R0 } ,
        { KEY_CHAR_C0_R1,KEY_CHAR_C1_R1,KEY_CHAR_C2_R1,KEY_CHAR_C3_R1 } ,
        { KEY_CHAR_C0_R2,KEY_CHAR_C1_R2,KEY_CHAR_C2_R2,KEY_CHAR_C3_R2 } ,
        { KEY_CHAR_C0_R3,KEY_CHAR_C1_R3,KEY_CHAR_C2_R3,KEY_CHAR_C3_R3 } 
    };
    return keymap[row][column];
}

int keypad_find_pin(int number)
{
    int pinArray[8] = {
        KEYPAD_PIN_R0,
        KEYPAD_PIN_R1,
        KEYPAD_PIN_R2,
        KEYPAD_PIN_R3,
        KEYPAD_PIN_C0,
        KEYPAD_PIN_C1,
        KEYPAD_PIN_C2,
        KEYPAD_PIN_C3
    };
    return pinArray[number];
}






	
