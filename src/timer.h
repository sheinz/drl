#ifndef __TIMER_H__
#define __TIMER_H__

#include <inttypes.h>

void init_timer(void);


uint32_t get_ticks(void);

void set_pwm(uint8_t duty);

void sleep_ms(uint32_t mseconds);

#endif // __TIMER_H__
