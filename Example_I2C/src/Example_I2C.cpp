/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: include i2c.h
#include "i2c.h"

// TODO: include swm.h
#include "swm.h"

// TODO: include disp.h
#include "disp.h"

// TODO: include delay.h
#include "delay.h"

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: call swm_config_I2C();
    swm_config_I2C();
    // TODO: call i2c_reset();
    i2c_reset( LPC_SYSCON );
    // TODO: call disp_off();
    disp_off();
    // TODO: wait 1s
    delay_1s();
    // TODO: call disp_on();
    disp_init( DISP_SHOW_ALL );
    // Force the counter to be placed into memory
    volatile static int i = 30 ;
    // Enter an infinite loop, just incrementing a counter
    while( i > 7 ) {
        disp_decimal( i );
        delay_1s();
        i-- ;
    }
    while (1) {
        disp_007( );
        delay_1s();
    	disp_off();
    	delay_1s();
    	disp_init( DISP_SHOW_NONE );
    }
    return 0 ;
}
