drl
===

Daytime running lamp controller. Use standard high beam lamps with reduced power to implemen daytime running lamps.
The controller is implemented using ATMEL ATTiny13A microcontroller.
It uses PWM to control lamp brightness. 


![screenshot](https://raw.github.com/sheinz/drl/master/doc/drl_4.jpg)

**Features:**
- Use PWM to control hi beam lamp power.
- Automatically turns on after engine is started. Detects voltage increase to determine if engine is running.
- Automatically turns off when low beam lamps is on.
- Provides DRL brightness configuration by clicking a switch. Each click slightly increases brightness.
- The brightness of the DRL is stored in EEPROM. 
- Can be completely disabled by configurin brightness that equal zero.
- If a device is malfunctioning it records errors and blinks hi beam lamps at start. 
- The error count is cleared after clicking a switch.


**Build**

In order to build the source code the following packages should be installed:

    sudo apt-get install gcc-avr
    sudo apt-get install avr-libc
    sudo apt-get install cmake
    
If you encounter errors like these:
    
    ld: crttn13a.o: No such file: No such file or directory
    undefined reference to `__eerd_byte_tn13a'
    
Copy libraries crttn13a.o and libc.a into tiny-stack subdirectory:

    sudo cp /usr/lib/avr/lib/avr25/crttn13a.o /usr/lib/avr/lib/avr25/tiny-stack/
    sudo cp /usr/lib/avr/lib/avr25/libc.a /usr/lib/avr/lib/avr25/tiny-stack/
    
To build and upload the firmware:
    
    mkdir drl-build
    cd drl-build
    cmake ../drl
    make
    make upload
    

--------------
![screenshot](https://raw.github.com/sheinz/drl/master/doc/drl_1.jpg)

![screenshot](https://raw.github.com/sheinz/drl/master/doc/drl_3.jpg)

![screenshot](https://raw.github.com/sheinz/drl/master/doc/schematic.png)

![screenshot](https://raw.github.com/sheinz/drl/master/doc/pcb.png)
