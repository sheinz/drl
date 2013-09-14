#include <inttypes.h>

#include <avr/io.h>
#include <avr/eeprom.h>

#include "timer.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


#define SHOW_ERRORS


#define IS_LOW_BEAM_ON (bit_is_clear(PINB, PB3))

static uint8_t pwm;
static uint32_t last_low_beam_off = 0;
static uint8_t low_beam_on = 0;
static uint8_t clicks = 0;

EEMEM uint8_t eeprom_pwm = 100;

#ifdef SHOW_ERRORS
   EEMEM uint8_t eeprom_error_count = 0;
#endif

#define PWM_INCREMENT_STEP    20
#define ERROR_RESET_COUNT     0



int main(void)
{
   wdt_enable(WDTO_2S);

#ifdef SHOW_ERRORS
   uint8_t errors = eeprom_read_byte(&eeprom_error_count);
   if (bit_is_set(MCUSR, WDRF) || bit_is_set(MCUSR, EXTRF))
   {
      errors++;
      eeprom_update_byte(&eeprom_error_count, errors);
   }
#endif

   init_timer();

   pwm = eeprom_read_byte(&eeprom_pwm);

   if (IS_LOW_BEAM_ON)     // to trigger first init
   {
      low_beam_on = 0;
   }
   else
   {
      low_beam_on = 1;
   }

#ifdef SHOW_ERRORS
   // indicate errors
   for (uint8_t i = 0; i < errors; i++)
   {
      set_pwm(20);
      sleep_ms(500);
      set_pwm(0);
      sleep_ms(500);
      wdt_reset();
   }
#endif

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

#ifdef SHOW_ERRORS
            if (clicks == 1)
            {
               eeprom_update_byte(&eeprom_error_count, 0);
            }
#endif
            if (clicks == 2)
            {
               if (0xFF - pwm < PWM_INCREMENT_STEP)
               {
                  pwm = 0;
               }
               else
               {
                  pwm += PWM_INCREMENT_STEP;
               }
            }
            if (clicks == 3)
            {
               if (pwm >= PWM_INCREMENT_STEP)
               {
                  pwm -= (PWM_INCREMENT_STEP + PWM_INCREMENT_STEP);
               }
            }
            if (clicks > 1)
            {
               eeprom_update_byte(&eeprom_pwm, pwm);
            }
	         set_pwm(pwm);
	      }
	   }
	   sleep_ms(100);
	   wdt_reset();
	}


	return 0;
}

