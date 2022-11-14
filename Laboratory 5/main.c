/*
 * Laboratory Program 5
 *
 * Interrupts
 */

/*
 * Header files giving access to addition ARM main board functionality
 */

#include <board/SevenSegmentDisplay.h> /* Provide access to 7 segment display functions */

#include <atmel/pit.h>
#include <atmel/aic.h>


#include <config/AT91SAM7S256.h>
#include <board/Button-lib.h>

/*
 * The digits on the Seven Segment LED Display are numbered right to left. DISPLAY1 is the right most digit.
 * The defines below give more sensible (useful) names to the display digits for use in this program.
 */

#define SECONDS				DISPLAY1
#define RIGHT_DISPLAY		DISPLAY2
#define MIDDLE_DISPLAY		DISPLAY3
#define LEFT_DISPLAY		DISPLAY4

/*
 * The array Digit contains the binary patterns to display the digits 0 - 9 to the Seven Segment LED Display.
 * Digit[0] is the pattern for zero, Digit[1] is the pattern for one, etc.
 *
 * See the SevenSegmentDisplay library manual entry for more details.
 */
short Digit[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67 };

/*
 * The array Bars contains the binary patterns to display a single bar on a display. There are seven entries,
 * one for each bar in the display.
 */
short Bars[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40 };

/*
 * Global variables.
 */
short seconds, tenthSeconds,hundredSeconds,thousandSeconds, left_bar_display, middle_bar_display, right_bar_display, hertz;

/*
 * Interrupt Routine
 *
 * This routine is called for every PIT interrupt
 */
void ISR_System(void)
{
	/*
	 * Read the PIT status register to clear the pending interrupt.
	 * We are not interested in the value so the result does not have to be assigned to anything.
	 */
    PIT_GetPIVR();

    /*
     * Now Process the interrupt.
     *
     * 50 interrupts equals one second
     */
    if (++hertz == 50)
    {
    	hertz = 0;

		if (++seconds == 10){
			seconds = 0;
			tenthSeconds++;
		}

		if(tenthSeconds == 10){
			tenthSeconds = 0;
			hundredSeconds++;
		}

		if(hundredSeconds == 10){
			hundredSeconds = 0;
			thousandSeconds++;
		}

		if(thousandSeconds == 10){
			thousandSeconds = 0;
		}

    }

    if ((hertz % 5) == 0)
    {
    	/*
    	 * The bars move at a rate 10 times faster than seconds digits increments.
    	 */
		left_bar_display = ((left_bar_display - 1) < 0) ? 5 : left_bar_display - 1;
		middle_bar_display = ((middle_bar_display + 3) > 6) ? 0 : middle_bar_display + 3;
		right_bar_display = (right_bar_display + 1) % 6;
    }
}

#define PIT_PERIOD          500 // interrupt at 20 milliseconds (50Hz)

/*
 * Main function - Program execution begins here!
 */

int main()
{
	/*
	 * First we need to configure the additional functionality required.
	 */
	Configure7SegmentDisplay();

	/*
	 * Reset all the values to zero
	 */

	hertz = 0;
	seconds = 0;
	tenthSeconds = 0; //Set it to 0 as well
	left_bar_display = 0;
	middle_bar_display = 6;
	right_bar_display = 0;

	/*
	 * Reset the Seven Segment Display.
	 */

	Set7SegmentDisplayValue(SECONDS, Digit[0]); // Displays the digit 0
	Set7SegmentDisplayValue(RIGHT_DISPLAY, 0);  // Sets the display to blank
	Set7SegmentDisplayValue(MIDDLE_DISPLAY, 0);
	Set7SegmentDisplayValue(LEFT_DISPLAY, 0);

	/*
	 * Initialize and enable the PIT
	 *
	 * First argument (PIT_PERIOD) is number of milliseconds between timer interrupts (maximum 20 bit number)
	 * Second argument is clock rate in MHz (BOARD_MCK is in Hz so divide by 1000000)
	 *
	 */

	PIT_Init(PIT_PERIOD, BOARD_MCK / 1000000);

	/*
	 * Disable the interrupt on the interrupt controller
	 */

	AIC_DisableIT(AT91C_ID_SYS);

	/*
	 * Configure the AIC for PIT interrupts
	 *
	 * The PIT is a System interrupt, so we need to associate the ID of the System interrupt with the interrupt
	 * routine ISR_System.
	 */

	AIC_ConfigureIT(AT91C_ID_SYS, 0, ISR_System);

	/*
	 * Enable the interrupt on the interrupt controller
	 */

	AIC_EnableIT(AT91C_ID_SYS);

	/*
	 * Enable the PIT interrupt and start the PIT
	 *
	 * The interrupt handler is always assigned before the interrupt is enabled.
	 * Using the library function the PIT interrupt is enabled separately from the PIT itself. the interrupt
	 * is enabled first so none are missed when the timer is enabled.
	 */
    PIT_EnableIT();
    PIT_Enable();

	/*
	 * Now start the display update running.
	 */

	while (1)
	{
		Set7SegmentDisplayValue(SECONDS, Digit[seconds]);
    	Set7SegmentDisplayValue(RIGHT_DISPLAY, Digit[tenthSeconds]);
		Set7SegmentDisplayValue(MIDDLE_DISPLAY, Digit[hundredSeconds]);
		Set7SegmentDisplayValue(LEFT_DISPLAY, Digit[thousandSeconds]);
	}

	return 0;
}
