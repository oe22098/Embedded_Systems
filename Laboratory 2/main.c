/*
 * Exercise for Script 2.
 */

/*
 * Header files giving access to addition ARM main board functionality
 */
#include <board/OLED-lib.h>
#include <board/Button-lib.h>

void SetOLEDdisplayMode(DisplayMode);

/*
 * External variables defined in the loader configuration file
 */
extern unsigned long entry, _text, _etext, _data, _edata, _bss, _ebss;

/*
 * Main function - Program execution begins here!
 */

int main()
{
	ConfigureOLEDdisplay();
	SetOLEDdisplayMode(TextDisplay);
	ClearOLEDdisplay();

	/*
	 * Display the values of the global loader variables
	 *
	 * These are pointer variables, so we need to display their address
	 * rather than the contents of the location.
	 */
	printf("_text = 0x%x\n", (unsigned int)&_text);
	printf("_etext = 0x%x\n\n", (unsigned int)&_etext);
	printf("_data = 0x%x\n", (unsigned int)&_data);
	printf("_edata = 0x%x\n\n", (unsigned int)&_edata);
	printf("_bss = 0x%x\n", (unsigned int)&_bss);
	printf("_ebss = 0x%x\n\n", (unsigned int)&_ebss);
	printf("entry = 0x%x\n", (unsigned int)&entry);

	/*
	 * Pause until button is pressed
	 */
	while (IsButtonReleased(BUTTON8));

	return 0;
}

