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


--------------
![screenshot](https://raw.github.com/sheinz/drl/master/doc/drl_1.jpg)

![screenshot](https://raw.github.com/sheinz/drl/master/doc/drl_3.jpg)
