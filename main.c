/**********************************************************
 * The Line follower Robot Code
 * Simplified Code for easy use
 * 
 * @author Nootan Ghimire <nootan.ghimire@gmail.com>
 * 
 * Built under HCOE Developers Circle
 * Not tested!
 *
 *
 * 
 * TODO: We have not checked or pin<0 in the functions:
 *                       -> makeOn(portMaps);
 *						 -> makeOff(portMaps);
 **********************************************************/


/*F_CPU Defination (For Delays) . 12MHz*/
#ifndef F_CPU 
	#define F_CPU 12000000
#endif

/*Direction contstants, For easyness*/
#define DIRECTION_FWD 21332
#define DIRECTION_REV 21331

/*Error Constants*/
#define ERROR 1115
#define NOTICE 1114


#include<avr/io.h>
#include<util/delay.h>
#include<stdint.h>

/* Other Declarations */
#define LCD_MAX_CHAR 100
#define MOTOR_ALIAS_MAX 100

/* Global Declarations */
typedef struct {
	char port; /* 'A', 'B', 'C', 'D' */
	uint8_t pin;  /* 0 to 7 */
} portMaps;

typedef struct {
	int id;
	portMaps drivingPort1;
	portMaps drivingPort2;
} Motor;

typedef struct {
	int id;
	portMaps inputPort;
	int type; //no need
} oneInputSensor ;

uint8_t currentPortA= 0x00, currentPortB = 0x00, currentPortC = 0x00 , currentPortD = 0x00;


Motor m1, m2;


/* Function Prototypes */ 	

/* Motor Functions */
	uint8_t initMotor(Motor);
	/* Differential Drive Implemented. Assumes two motors m1 and m2 */
		uint8_t driveStraight();
		uint8_t driveRight();
		uint8_t driveLeft();

	uint8_t driveMotor(Motor, int, int);
	uint8_t stopMotor(Motor);

/* Sensor Functions */
		uint8_t initSensor(oneInputSensor, oneInputSensor, oneInputSensor);
		uint8_t isOneInputSensorOn(oneInputSensor);


/* Debugging Function */
	uint8_t displayinLCD(char[LCD_MAX_CHAR]);
	uint8_t xlog(int, char[LCD_MAX_CHAR]);

/* Pin and Port functions */
	portMaps makePort(char, uint8_t);
	uint8_t makeOn(portMaps);
	uint8_t makeOff(portMaps);
	

/* PWM functions (not implemented) */
	uint8_t makeOnPWM(portMaps, int);

/* Other Robot Functions */
	uint8_t lineFollow();
	uint8_t wallFollow();
	uint8_t followExceptObstacle();

/* Main Start */
uint8_t main(){
	DDRA = 0xff;   /*everything is output on port A*/
	DDRB = 0x00;   /*everything is input on port B*/

	/*Initializing Motor with driving ports PA1 and PA2*/
	/* Assume that this motor is left one! */
	initMotor(m1, makePort('A', 1), makePort('A', 2)); 

	/*Initializing Motor with driving ports PA3 and PA4*/
	/* Assume that this motor is right one! */
	initMotor(m2, makePort('A',3), makePort('A', 4));

	oneInputSensor top_sensor, right_sensor, left_sensor;
	initSensor(top_sensor, makePort('B', 0));
	initSensor(right_sensor, makePort('B',1));
	initSensor(left_sensor, makePort('B',2));

	 while(1){
	 		if(!followExceptObstacle()) {
	 			if(!wallFollow()){
	 				if(!lineFollow(top_sensor, right_sensor, left_sensor)){
	 					exit();
	 				}
	 			}
	 		}
	}

	/*End Test Case*/
	
	return 1; /*Ahoy, Success!*/
}

uint8_t lineFollow(oneInputSensor top, oneInputSensor right, oneInputSensor left){
	
	/* Test case => Line Follower Robot
	 * PB0 -> Top Middle Sensor
	 * PB1 -> Right Sensor & PB2 -> Left Sensor
	 * Both sensors are inside the white line
	 */
	if(isOneInputSensorOn(top)) {
		if(isOneInputSensorOn(right)) {
			//i.e, if PB1 is on:
			if(isOneInputSensorOn(left)) {
				//i.e if PB2 is also on!
				//There's no good line :)
				//For now, driveStraight();
			} else {
				//i.e, is PB1 is off (left one off)
				driveRight();
			} //End PORTB0x02			
		} else {
			//i.e if PB1 is off:
			if(isOneInputSensorOn(left)){
				//i.e if PB2 is on
				driveLeft();
			} else {
				driveStraight();
			} //End PORTB0x02
		} //END PORTB0x01
	} else {
		return 0; //There's no good line :)
	}
	return 1;
}




uint8_t wallFollow(){
	return 0; //return zero (false) if there is no line!
}

uint8_t followExceptObstacle(){
	return 0; //return false, if there is no any obstacle
}


uint8_t initSensor(Sensor s, portMaps p){
	static int count = 0;
	count++;
	s.id = count;
	s.inputPort.port = p.port;
	s.inputPort.pin = p.pin;
	return 1;
}
/* Locomotives */
uint8_t driveStraight(){
	uint8_t errorFlag = 1;
	if(!driveMotor(m1, DIRECTION_FWD, 100)) { xlog(NOTICE, "Could not drive m1 forward!"); errorFlag=0; }
	if(!driveMotor(m2, DIRECTION_FWD, 100)){  xlog(NOTICE, "Could not drive m2 forward!"); errorFlag=0; }
	return errorFlag; /*This thingo is  `universal`, aye?*/
}

uint8_t driveRight(){

	/* Assumption that m1 is left motor, and 
	 * m2 is right motor
	 * to Drive to right, (or turn right)
	 * we have to  stop right motor, and 
	 * move left motor forward
	 */


	/*Error flag also could be implemeted here*/
	uint8_t errorFlag = 1;
	if(!stopMotor(m2)){ xlog(NOTICE, "Could not stop motor!"); errorFlag = 0;}
	if(!driveMotor(m1, DIRECTION_FWD, 100)){ xlog(NOTICE, "Could not drive motor in driveRight()"); errorFlag = 0;}
	return errorFlag;
}

uint8_t driveLeft(){
	/*exact opposite of driveRight*/
	uint8_t errorFlag = 1;
	if(!stopMotor(m1)) { xlog(NOTICE, "Could to stop motor! on driveLeft()"); errorFlag = 0;}
	if(!driveMotor(m2, DIRECTION_FWD, 100)){ xlog(NOTICE, "Could not drive motor in driveLeft()"); errorFlag = 0; }
	return errorFlag;
}

/* Helper Functions */
uint8_t driveMotor(Motor whichMotor, int direction, int power){
	/*Extract PORT information first:*/
	portMaps p1 = whichMotor.drivingPort1;
	portMaps p2 = whichMotor.drivingPort2;

	switch (direction){
		case DIRECTION_FWD:
		makeOn(p1);
		makeOff(p2);
		break;

		case DIRECTION_REV:
		makeOn(p2);
		makeOff(p1);
		break;

		default:
		xlog(ERROR, "Unrecognizd Direction on driveMotor()");
		return 0; 
	}

	return 1; /*Ahoy, Success!*/
}

uint8_t stopMotor(Motor whichMotor){
	uint8_t errorFlag = 1; 
	if(!makeOn(whichMotor.drivingPort1)){ xlog(NOTICE, "Could not make On!! on stopMotor()"); errorFlag = 0; }
	if(!makeOn(whichMotor.drivingPort2)){ xlog(NOTICE, "Could not make On!! on stopMotor()"); errorFlag = 0; }
	return errorFlag;
}

/*Debugger Function*/
uint8_t displayinLCD(char textToDisplay[LCD_MAX_CHAR]){
	/*Scallywag, ye be initializing the LCD thingo! */
	return 1; /*Aye, Cap'n*/
}

uint8_t xlog(int logType, char charToLog[LCD_MAX_CHAR]){
	/*check logType and do things. */
	displayinLCD(charToLog); /*For now only, we'll make it opportunistic later.*/
}


uint8_t initMotor(Motor m, portMaps portInfo1, portMaps portInfo2){
    static uint8_t counter = 0;
 	m.id = counter++; 
 	m.drivingPort1 = portInfo1;
 	m.drivingPort2  = portInfo2;
 	return 1;  /*Ahoy, Succcess!*/
}

portMaps makePort(char portChar, uint8_t pin){
	portMaps port;
	port.port = portChar;
	port.pin = pin;
	return port;
}

uint8_t makeOn(portMaps portInfo){
	/*make the specified pin On */
	uint8_t pin = portInfo.pin;
	uint8_t port =portInfo.port;
	if(pin > 7) { 
		xlog(ERROR, "Invalid length of pin (>7) in makeOn()");
		return 0 ;
	}
	uint8_t init = 0x01<<pin;
	switch(port){
		case 'A':
		currentPortA |= init ;
		PORTA = currentPortA;
		break;

		case 'B':
		currentPortB |= init;
		PORTB  = currentPortB;
		break;

		case 'C':
		currentPortC |= init;
		PORTC = currentPortC;
		break;

		case 'D':
		currentPortD |= init;
		PORTD = currentPortD;
		break;

		default:
		xlog(ERROR, "Invalid Port Information on port in makeOn()");
		return 0;
		break;
	}	
	return 1;
}

uint8_t makeOff(portMaps portInfo){
	uint8_t pin = portInfo.pin;
	uint8_t port =portInfo.port;
	if(pin > 7) { 
		xlog(ERROR, "Invalid length of pin (>7) in makeOff()");
		return 0 ;
	}
	uint8_t init = 0x01<<pin;
	init = ~init;   /* Remembered a joke! how can `init` equal to `(not) init` :p */
	switch(port){
		case 'A':
		currentPortA &= init ;
		PORTA = currentPortA;
		break;

		case 'B':
		currentPortB &= init;
		PORTB = currentPortB;
		break;

		case 'C':
		currentPortC &= init;
		PORTC = currentPortC;
		break;

		case 'D':
		currentPortD &= init;
		PORTD = currentPortD;
		break;

		default:
		xlog(ERROR, "Invalid Port Information on port in makeOff()");
		return 0;
		break;
	}	
	return 1;
}

uint8_t isOn(Sensor s){
	int portInfo = s.inputPort;
	int pin =  portInfo.pin;
	char port = portInfo.port;
	int masker = 0x01<<pin;
	switch(port){
		case 'A':
			return ((PINA & masker) == masker);
			break;
		case 'B':
			return ((PINB & masker) == masker);
			break;
		case 'C':
			return ((PINC & masker) == masker);
			break;
		case 'D':
			return ((PIND & masker) == masker);
			break;
		default:
			xlog(ERROR, "Invalid Port Infromation specified on isOn()");
			return 0; //if port information is wrong, then it's not on! :)
			break;
	}
}

/* Area under research*/
uint8_t makeOnPWM(portMaps portInfo, int duty_cycle){
	/**********************8
	 * 
	 * PWM's the specified PIN-PORT Combination
	 * according to the duty cycle
	 * 
	 * Actually this is a PWM hack! (and not real PWM)
	 * Give duty cycle in percentage
	 *
	 */
	 
	 uint8_t pin = portInfo.pin;
	 uint8_t port = portInfo.port;

	if((duty_cycle > 100) || (duty_cycle < 0) ) {
	 	xlog(ERROR, "Invalid duty cycle supplied. Must be between 0 and 100");
	 	return 0; 
	 }

	 /********************************************************************************
	  *
	  * EXPERIMENTAL :)
	  * TOO DANGEROUS :P
	  */
	 /* 
	 if( (pin > 7) || (pin < 0) ){
	 	xlog(ERROR, "Invalid length of pin ("((pin > 7) ? ">7" : "<0")") ");
	 	return 0;
	 	// i don't know, does this work?
	 } */
	 /********************************************************************************/

	 //use CASE things and determine the duty cycle and apply appropriate thing

	 
}
/***********************************************************************
 *
 * BOTTOMLINE
 * I am lazy does not mean that you should stop teaching me!
 *
 ************************************************************************/
