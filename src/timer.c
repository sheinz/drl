#include "timer.h"

#ifdef __ECLIPSE_INDEXER_BUG__
   #include <avr/iotn13.h>
#else
   #include <avr/io.h>
#endif

#include <avr/interrupt.h>

static volatile uint32_t sys_ticks;


ISR(TIM0_OVF_vect)
{
   sys_ticks += 8;   // timer overflows every 8 ms
}

void init_timer(void)
{
   // Set OC0A pin to output
   PORTB |= _BV(PB0);    // pull it high before switching to output
   DDRB |= _BV(PB0);

   // Clear OC0A on Compare Match, set OC0A at TOP (inverted mode)
   // Inverted mode is used in order to remove narrow spikes in OFF mode.
   // Fast PWM mode
   TCCR0A = _BV(COM0A1) | _BV(WGM00) | _BV(WGM01);

   OCR0A = 0xFF;     // initial state OFF (inverted)

   // Prescaler CLK/256
   TCCR0B = _BV(CS02);

   sys_ticks = 0;

   TIMSK0 = _BV(TOIE0);

   sei();
}

void set_pwm(uint8_t duty)
{
   OCR0A = 255 - duty;
}

uint32_t get_ticks(void)
{
   uint32_t mseconds;
   uint8_t sreg = SREG;

   cli();

   mseconds = sys_ticks;

   SREG = sreg;

   return mseconds;
}

void sleep_ms(uint32_t mseconds)
{
   uint32_t startTime = get_ticks();

   while ( (startTime + mseconds) > get_ticks() );
}

