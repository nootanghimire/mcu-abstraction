#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "mcu-abstraction.h"

void* logger_function

int mcu_int_power(int base, int exp){
	int answer = 1;
	int (i = 0; i<exp; i++){
		answer *= answer
	}
	return answer;
}
int mcu_logger(char* message){
	logger_function(message);
}

/* Public */

uint8_t[] makeInitPort(int a, int b, int c, int d, int e, int f, int g, int h){
	return {a,b,c,d,e,f,g,h};
}

void mcu_init(initPort p){
	int a, b, counter;
	a = 0;
	b = 0;
	for(counter = 0; counter<8; counter++){
		a += p.portAmask[counter] * mcu_int_power(2, counter)
	}
	for(counter = 0; counter<8; counter++){
		b += p.portBmask[counter] * mcu_int_power(2, counter)
	}
	DDRA = a;
	DDRB = b;
}

void  mcu_register_logger(void (*logfunc)(*char)){
	logger_function = logfunc;
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
		mcu_logger(ERROR, "Invalid length of pin (>7) in makeOn()");
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
		mcu_logger(ERROR, "Invalid Port Information on port in makeOn()");
		return 0;
		break;
	}	
	return 1;
}

uint8_t makeOff(portMaps portInfo){
	uint8_t pin = portInfo.pin;
	uint8_t port =portInfo.port;
	if(pin > 7) { 
		mcu_logger(ERROR, "Invalid length of pin (>7) in makeOff()");
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
		mcu_logger(ERROR, "Invalid Port Information on port in makeOff()");
		return 0;
		break;
	}	
	return 1;
}

uint8_t initSensor(Sensor s, portMaps p){
	static int count = 0;
	count++;
	s.id = count;
	s.inputPort.port = p.port;
	s.inputPort.pin = p.pin;
	return 1;
}
uint8_t isSensorOn(Sensor s){
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
			mcu_logger(ERROR, "Invalid Port Infromation specified on isOn()");
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
	 	mcu_logger(ERROR, "Invalid duty cycle supplied. Must be between 0 and 100");
	 	return 0; 
	 }
