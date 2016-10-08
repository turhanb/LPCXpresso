/*
 * swm.c
 *
 *  Created on: Oct 3, 2016
 *      Author: timppa
 */

#include "chip.h"

#include "swm.h"

//
// creates SWM configuration for LEDs
// - the GPIOs are enabled by default
// - so we ensure that RESET and debugging work
//
// For SWM configuration reading/writing the clocking of SWM
// must be enabled. After the read/write the clocking can be
// disabled, saves power...
//
void swm_config_LED()
{
	//
	// LPC_SYSCON is at address: 0x40000000 + 0x48000 = 0x40048000
	LPC_SYSCTL_T* pLPC_SYSCON = (LPC_SYSCTL_T*)LPC_SYSCON;
	// enable system clock for SWM block: toggle bit 7 to one
	pLPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
	//
	// LPC_SWM is at address: 0x40000000 + 0x0C000 = 0x4000C000
	LPC_SWM_T* pLPC_SWM = (LPC_SWM_T*)LPC_SWM;
	pLPC_SWM->PINENABLE0 = 0xffffffb3UL;
	// disable system clock of SWM block: toggle bit 7 to zero
	pLPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<7);
}

void swm_config_I2C()
{
	// LPC_SWM is at address: 0x40000000 + 0x0C000 = 0x4000C000
	LPC_SWM_T* pLPC_SWM = (LPC_SWM_T*)LPC_SWM;
	// U0_TXD on PIO0_4, pin 5
	// U0_RXD on PIO0_0, pin 19
//	pLPC_SWM->PINASSIGN0 = 0xffff0004UL;
	// I2C0_SDA on PIO0_10, pin 9
	pLPC_SWM->PINASSIGN[7] = 0x0affffffUL;
	// I2C0_SCL on PIO0_11, pin 8
	pLPC_SWM->PINASSIGN[8] = 0xffffff0bUL;
	// RESET on PIO0_5, pin 4
	// SWCLK on PIO0_3, pin 6
	// SWDIO on PIO0_2, pin 7
	pLPC_SWM->PINENABLE0 = 0xffffffb3UL;
}
