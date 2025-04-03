#define PERROR(x) {printf(x);printf("Press any key to restart...\n\n");getc(stdin);reset();return(1);}

int syntax(char *);
//// checks the syntax for errors
//// returns 1 for correct syntax
//// ARGUMENT : command entered from user

void reset(void);
//// resets the apollo to initial state

int execute(char *);
//// executes the command
//// returns 0 for successful execution
//// ARGUMENT : command entered from user

int victory(char*);
//// sees if the puzzle has been solved
//// gives the victory message
//// returns 1 for victory
//// ARGUMENT : Command issued by user

int digit(int ,unsigned int );
//// gives a single digit from a number
//// ARGUMENT : number and the digit place from left (1,2,3...)

