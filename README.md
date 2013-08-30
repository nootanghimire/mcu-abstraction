
*Note:I didn't test it in Proteus or things!*


MCU Abstraction
============

This is supposed to be a library, or a simple piece of code that helps writing for AVR  microcontrollers easier! This program is written taking ATMEGA16 in mind. This contains different functions that ease the process of writing a program for ATMEGA16 (or similar variants)


Features
===

I'll upload the docs soon! Till then I'll write some useful features of the code.

 - **Easier Pin/Port assignment** : Now you can assign individual pins ON or OFF  without affecting the state of other pins (in a same port).
 - **Get values of sensors** : Again, getting value from sensors or pin/ports now does not need any masking and things, Specify the pin/port and use isOn() function to decide whether the pin is on or off! (Examples below)



Issues
===

I have not tested this code in proteus or things (because i am lazy), but they should run fine! (I checked them for errors by inspection a lot of times). So, if you are interested (or a electronics guy), please do check that thing and file a issue! Or just email me at nootan.ghimire@gmail.com



Example
===


Here's a sneak peek at `int main()` . You can open up main.c and know completely


```C
int main(){

    /* making A as input port and B as output port */
    DDRA = 0x00; //You should be careful while assigning ports though
    DDRB = 0xff; 

    while(1){                            /* Our infinite loop  */
        if(isOn(makePort('A',4))){       /* i.e., if PA4 is on */
            makeOn(makePort('B',1));
            makeOn(makePort('B',2));
            makeOn(makePort('B',3));
            _delay_ms(100);
            makeOff(makePort('B',3));
            _delay_ms(100);
        }
    }
    return 1;
}
 
```
