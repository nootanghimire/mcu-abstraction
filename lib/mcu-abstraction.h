#ifndef MCU_ABSTRACTION_H
	#define MCU_ABSTRACTION_H

	#ifndef F_CPU 
		#define F_CPU 12000000
	#endif

	#define MCU_INIT_INPUT  0
	#define MCU_INIT_OUTPUT 1

	#include <avr/io.h>
	#include <util/delay.h>
	#include <stdint.h>

	/* Global Declarations */
	typedef struct {
		char port; /* 'A', 'B', 'C', 'D' */
		uint8_t pin;  /* 0 to 7 */
	} portMaps;

	typedef struct {
		int id;
		portMaps inputPort;
		int type; //no need
	} Sensor ;

	typedef struct {
		uint8_t portAmask[8];
		uint8_t portBmask[8];
	} initPort;



	/* Initialization Functions */
	uint8_t[] makeInitPort(int, int, int, int, int, int, int, int);
	void  mcu_register_logger(void (*f)(*char));
	void  mcu_init(initPort);
	/* Pin and Port functions */
	portMaps makePort(char, uint8_t);
	uint8_t makeOn(portMaps);
	uint8_t makeOff(portMaps);

	/* Sensor Functions */
	uint8_t initSensor(Sensor, portMaps);
	uint8_t isSensorOn(Sensor);
	

	/* PWM functions (not implemented) */
	uint8_t makeOnPWM(portMaps, int);
	
#endif