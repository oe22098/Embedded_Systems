/*
 * Exercise 3 - main file
 */

#include <config/AT91SAM7S256.h>

#define PULSE_LINE 0x0080

/*
 * Function example using simple pointers to access device registers.
 */
void AccessPIOusingSimplePointers(short loopCount, unsigned short delayTime)
{
	unsigned short delay;

	// PIOA base address 0xfffff400
	volatile unsigned int *ClearOutputDataReg = (unsigned int *)0xfffff434;	// Offset 0x34
	volatile unsigned int *SetOutputDataReg = (unsigned int *)0xfffff430;	// Offset 0x30

	*ClearOutputDataReg = PULSE_LINE;

	while (loopCount-- > 0)
	{
		for (delay = 0; delay < delayTime; delay++);
		*ClearOutputDataReg = PULSE_LINE;

		for (delay = 0; delay < delayTime; delay++);
		*SetOutputDataReg = PULSE_LINE;
	}
}

/*
 * Structure definition of (most) of the PIO registers.
 */
struct PIOregisters {
	unsigned int PIOenable;
	unsigned int PIOdisable;
	unsigned int PIOstatus;
	unsigned int Unused1;
	unsigned int OutputEnable;
	unsigned int OutputDisable;
	unsigned int OutputStatus;
	unsigned int Unused2;
	unsigned int GlitchEnable;
	unsigned int GlitchDisable;
	unsigned int GlitchStatus;
	unsigned int Unused3;
	unsigned int SetOutputData;
	unsigned int ClearOutputData;
	unsigned int OutputDataStatus;
	unsigned int PinDataStatus;
	unsigned int InterruptEnable;
	unsigned int InterruptDisable;
	unsigned int InterruptMask;
	unsigned int InterruptStatus;
	unsigned int MultiDriveEnable;
	unsigned int MultiDriveDisable;
	unsigned int MultiDriveStatus;
	unsigned int Unused4;
	unsigned int PullUpDisable;
	unsigned int PullUpEnable;
	unsigned int PullUpStatus;
	unsigned int Unused5;
	unsigned int PeripheralAselect;
	unsigned int PeripheralBselect;
	unsigned int PeripheralABstatus;
};

/*
 * Function example using struct pointers to access device registers.
 */
void AccessPIOusingStructPointers(short loopCount, unsigned long delayTime)
{
	struct PIOregisters *PIObase = (struct PIOregisters *)0xfffff400;
	unsigned long delay;

	PIObase->ClearOutputData = PULSE_LINE;

	do {
		for (delay = 0; delay < delayTime; delay++);
		PIObase->ClearOutputData = PULSE_LINE;

		for (delay = 0; delay < delayTime; delay++);
		PIObase->SetOutputData = PULSE_LINE;
	} while (--loopCount > 0);
}

/*
 * Function example using header file defined pointers to access device registers.
 */
void AccessPIOusingHeaderDefinitions(short loopCount, unsigned long delayTime)
{
	unsigned long delay, i;

	AT91C_BASE_PIOA->PIO_CODR = AT91C_PIO_PA7;

	for (i = 0; i < loopCount; i++)
	{
		for (delay = 0; delay < delayTime; delay++);
		AT91C_BASE_PIOA->PIO_CODR = AT91C_PIO_PA7;

		for (delay = 0; delay < delayTime; delay++);
		AT91C_BASE_PIOA->PIO_SODR = AT91C_PIO_PA7;
	}
}

int main()
{
	while (1)
	{
		AccessPIOusingSimplePointers(200, 64000);
		AccessPIOusingStructPointers(100, 128000);
		AccessPIOusingHeaderDefinitions(50, 256000);
		AccessPIOusingSimplePointers(32000, 64);
		AccessPIOusingSimplePointers(32000, 64);
	}

	return 0;
}

