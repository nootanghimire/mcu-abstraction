#include "lib/mcu-abstraction.h"

/* Logger Function: Required */
void logger(char* c){
	printf("%s\n", c);
}

/* The main function */
uint8_t main(){
	initPort p; //to hold the initial port configuration 
	portMaps LED1_pLeg, LED1_nLeg, LED2_pLeg, LED2_nLeg; //to hold the Light Emitting Diodes' pin
	p.portAmask = makeInitPort(1,1,1,1,1,1,1,1); // to initialize Port A, 1 means output, 0 means input
	p.portBmask = makeInitPort(0,0,0,0,0,0,0,0); // ^^ Same for Port B
	mcu_register_logger(logger);				 // Register the logger function
	mcu_init(p);								 // Initialize the Ports according to 'p'

	portMaps LED1_pLeg = makePort('A', 1);		 // Assign Port A - Pin 1 to LED1 positive leg
	portMaps LED1_nLeg = makePort('A', 2);		 // Assign Port A - Pin 2 to LED1 negative leg

	portMaps LED2_pLeg = makePort('A', 3);		 // Assign Port A - Pin 3 to LED2 positive leg
	portMaps LED2_nLeg = makePort('A', 4);		 // Assign Port A - Pin 4 to LED2 negative leg

	/* Main Loop */
	while(1){
		makeOff(LED1_nLeg);						 // Negative LED1 gets logic low
		makeOn(LED1_pLeg);						 // Positive LED1 gets logic high
		_delay_ms(300);							 // Delay 300 ms
		makeOff(LED1_pLeg);						 // Positive LED1 gets logic low
		makeOff(LED2_nLeg);						 // Negative LED2 gets logic low
		makeOn(LED2_pLeg);						 // Positive LED2 gets logic high
		_delay_ms(300);							 // Delay 300 ms
		makeOff(LED2_pLeg);						 // Positive LED2 gets logic low
	}

	return 1;
}
