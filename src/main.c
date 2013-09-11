#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB |= 1<<0;

	while (1)
	{
		PORTB |= 1<<0;

		_delay_ms(5);

		PORTB &= ~(1<<0);

		_delay_ms(5);
	}

	return 0;
}
