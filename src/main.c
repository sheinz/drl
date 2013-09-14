#include <inttypes.h>

#ifdef __ECLIPSE_INDEXER_BUG__
   #include <avr/iotn13.h>
#else
   #include <avr/io.h>
#endif

#include "timer.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define IS_LOW_BEAM_ON (bit_is_clear(PINB, PB3))

static uint8_t pwm;
static uint32_t last_low_beam_off = 0;
static uint8_t low_beam_on = 0;
static uint8_t clicks = 0;


int main(void)
{
//   DDRB |= _BV(PB0);
//   PORTB |= _BV(PB0);
//
//   if (MCUSR & _BV(BORF))
//   {
//      _delay_ms(1000);
//   }
//   else if (MCUSR & _BV(EXTRF))
//   {
//      //_delay_ms(2000);
//   }
//   else if (MCUSR & _BV(PORF))
//   {
//      _delay_ms(3000);
//   }

   init_timer();

   pwm = 80;

   if (IS_LOW_BEAM_ON)     // to trigger first init
   {
      low_beam_on = 0;
   }
   else
   {
      low_beam_on = 1;
   }

	while (1)
	{
	   if (IS_LOW_BEAM_ON)
	   {
	      if (!low_beam_on)
	      {
	         low_beam_on = 1;
	         set_pwm(0);
	      }
	   }
	   else
	   {
	      if (low_beam_on)
	      {
	         low_beam_on = 0;
            uint32_t ticks = get_ticks();
            if (ticks - last_low_beam_off < 1000)
            {
               clicks++;
            }
            else
            {
               clicks = 0;
            }
            last_low_beam_off = get_ticks();

            if (clicks == 2)
            {
               pwm += 50;
            }
	         set_pwm(pwm);
	      }
	   }
	   sleep_ms(100);
	}


	return 0;
}

