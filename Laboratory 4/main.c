/*
 * Exercise 4 - main file
 */

#include <config/AT91SAM7S256.h>			// Hardware description header

#define LED_DATA_LINE AT91C_PIO_PA25		// Data line (Output)
#define LED_CLOCK_LINE AT91C_PIO_PA24		// Clock line (Output)

#define SWITCH_SELECT_LINE AT91C_PIO_PA25	// Select line (Output)
#define SWITCH_BUTTON_4_8 AT91C_PIO_PA26	// Button 4 or 8 (Input)
#define SWITCH_BUTTON_3_7 AT91C_PIO_PA27	// Button 3 or 7 (Input)
#define SWITCH_BUTTON_2_6 AT91C_PIO_PA28	// Button 2 or 6 (Input)
#define SWITCH_BUTTON_1_5 AT91C_PIO_PA29	// Button 1 or 5 (Input)

int main()
{
	unsigned long SwitchValues;		// 32 bit variable to store parallel data value
	short SwitchOne, led;

	/*
	 * Setup the PIO pins to the correct direction.
	 *
	 * 1. All pins become PIO pins.
	 * 2. Select Output pins.
	 * 3. Select Input pins (disable as outputs).
	 * 4. Enable Input pin Glitch filters (stops switch bounce).
	 */
	AT91C_BASE_PIOA->PIO_PER = (AT91C_PIO_PA24 | AT91C_PIO_PA25 | AT91C_PIO_PA26 | AT91C_PIO_PA27 | AT91C_PIO_PA28 | AT91C_PIO_PA29);
	AT91C_BASE_PIOA->PIO_OER = (AT91C_PIO_PA24 | AT91C_PIO_PA25);
	AT91C_BASE_PIOA->PIO_ODR = (AT91C_PIO_PA26 | AT91C_PIO_PA27 | AT91C_PIO_PA28 | AT91C_PIO_PA29);
	AT91C_BASE_PIOA->PIO_IFER = (AT91C_PIO_PA26 | AT91C_PIO_PA27 | AT91C_PIO_PA28 | AT91C_PIO_PA29);

	/*
	 * Clear all the LEDs
	 */
	AT91C_BASE_PIOA->PIO_CODR = LED_CLOCK_LINE;
	AT91C_BASE_PIOA->PIO_SODR = LED_DATA_LINE;
	for (led = 0; led < 16; led++)
	{
		AT91C_BASE_PIOA->PIO_SODR = LED_CLOCK_LINE;
		AT91C_BASE_PIOA->PIO_CODR = LED_CLOCK_LINE;
	}


	short i;
	short leds[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0};
	//ensure the clock line is low
	AT91C_BASE_PIOA->PIO_CODR = LED_CLOCK_LINE;
	for (i = 0; i < 16; i++)
	{
	if (leds[i] == 1)
	AT91C_BASE_PIOA->PIO_SODR = LED_DATA_LINE;
	else
	AT91C_BASE_PIOA->PIO_CODR = LED_DATA_LINE;
	//low value sets the LED on
	AT91C_BASE_PIOA->PIO_SODR = LED_CLOCK_LINE;
	AT91C_BASE_PIOA->PIO_CODR = LED_CLOCK_LINE;
	}

	return 0;
}

