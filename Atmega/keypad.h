#define KEYPAD_PORT		PORTB
#define KEYPAD_PIN_R0	7
#define KEYPAD_PIN_R1	6	
#define KEYPAD_PIN_R2	5
#define KEYPAD_PIN_R3	4
#define KEYPAD_PIN_C0	3
#define KEYPAD_PIN_C1	2
#define KEYPAD_PIN_C2	1
#define KEYPAD_PIN_C3	0

#define KEY_CHAR_C0_R0                  '7'    //these values must not conflict
#define KEY_CHAR_C1_R0                  '8'
#define KEY_CHAR_C2_R0                  '9' 
#define KEY_CHAR_C3_R0                  'L'
#define KEY_CHAR_C0_R1                  '4'
#define KEY_CHAR_C1_R1                  '5'
#define KEY_CHAR_C2_R1                  '6'
#define KEY_CHAR_C3_R1                  'C'
#define KEY_CHAR_C0_R2                  '1'
#define KEY_CHAR_C1_R2                  '2'
#define KEY_CHAR_C2_R2                  '3'
#define KEY_CHAR_C3_R2                  'D'
#define KEY_CHAR_C0_R3                  '*'
#define KEY_CHAR_C1_R3                  '0'
#define KEY_CHAR_C2_R3                  '#'
#define KEY_CHAR_C3_R3                  'E'
#define KEY_CHAR_NO_KEY                 '$'    //no key pressed or no event msg
#define KEY_CHAR_BUTTON_RELEASE         ' '    //button release msg
 
#define KEYPAD_DEBOUNCE_CYCLES     100
//// no of loop itteration 
//// for successful confirmation of event(button press or release)


#define KEYPAD_COLUMN_SCAN_DELAY  30 
//// delay in micro seconds for each column scan
//// mandatory for transcient adjustment

typedef struct KEYMAP  // structure is used to map a key to row and column
{
    int row;
    int column; 
}KEYMAP;

typedef struct DEBOUNCE  // structure is used to software debounce the key
{
	int pressed;
	int confidence;
	char current_key;
}DEBOUNCE;

void keypad_init();
//// initialises the keypad port

char keypad_loop();	
//// debounced funcion that is used in fast loops
//// KEY_CHAR_C(x)_R(x)//////////returns key value msg for button press 
//// KEY_CHAR_NO_KEY/////////////returns no key msg for no event
//// KEY_CHAR_BUTTON_RELEASE/////returns button release msg for button release

char keypad_scan();		
//// not recommended to be used in fast loop(not debounced)
//// function assumes that button was pressed a long time before calling
//// returns the status of keypad a the moment of function call
//// KEY_CHAR_C(x)_R(x) i.e. returns key value for currently pressed button 
//// KEY_CHAR_NO_KEY i.e. returns no key for null status

int  keypad_key_pressed(char);	
//// not recommended to be used in fast loop(not debounced)
//// returns 1 if the button is pressed at the moment of funtion call
//// ARGUMENT: KEY_CHAR_C(x)_R(x) i.e. key value for button

void keypad_wait_till_key_pressed(char);
//// function does not return until the key is pressed
//// ARGUMENT: KEY_CHAR_C(x)_R(x)///key value for button

KEYMAP keypad_key_to_map(char);
//// function returns the row and column values of a key
//// ARGUMENT: KEY_CHAR_C(x)_R(x)///key value for button

char keypad_map_to_key(int row,int column);
//// function returns the key char for the row and column
//// ARGUMENT: row and column in the keypad matrix

int keypad_find_pin(int);
//// function takes a row(0-3) or column(0-3 mapped to 4-7) 
//// and gives the pin which is connected to it
//// ARGUMENT : row(0-3) or column(0-3 mapped to 4-7)