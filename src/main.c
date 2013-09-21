#include <inttypes.h>

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


#include "timer.h"
#include "adc.h"


#define SHOW_ERRORS


#define IS_LOW_BEAM_ON (bit_is_clear(PINB, PB3))

#define IS_KEY_PRESSED (bit_is_clear(PINB, PB1))

static uint8_t pwm;
static uint8_t low_beam_on = 0;
static uint8_t key_is_pressed = 0;

EEMEM uint8_t eeprom_pwm = 100;

#ifdef SHOW_ERRORS
   EEMEM uint8_t eeprom_error_count = 0;
#endif

#define PWM_INCREMENT_STEP    20
#define ERROR_RESET_COUNT     0

#define VOLTAGE_THRESHOLD     200      // the DRL should turn on at 13.2 V

int main(void)
{
   wdt_enable(WDTO_2S);

   INIT_ADC();

   // init key
   // PB1 is already an input
   PORTB |= _BV(PB1);      // pull up


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
      if (GET_VOLTAGE() >= VOLTAGE_THRESHOLD)
      {  // Engine is on
         break;
      }
      sleep_ms(100);
      wdt_reset();
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
	         set_pwm(pwm);
	      }
	   }

	   // key processing
	   if (IS_KEY_PRESSED)
	   {
	      if (!key_is_pressed)
	      {
	         key_is_pressed = 1;
	      }
	   }
	   else
	   {
	      if (key_is_pressed)
	      {
	         key_is_pressed = 0;
	         if (0xFF - pwm < PWM_INCREMENT_STEP)
	         {
	            pwm = 0;
	         }
	         else
	         {
	            pwm += PWM_INCREMENT_STEP;
	         }
	         eeprom_update_byte(&eeprom_pwm, pwm);
	         eeprom_update_byte(&eeprom_error_count, 0);
	         set_pwm(pwm);
	      }
	   }

	   sleep_ms(100);
	   wdt_reset();
	}


	return 0;
}

