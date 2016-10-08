/*
 * disp.c
 *
 *  Created on: Oct 6, 2016
 *      Author: timppa
 */

#include "i2c.h"
#include "hardware.h"
#include "ht16k33.h"

#include "disp.h"

int disp_off()
{
	const int addr = HW_I2C_ADDR_HT16K33;
	char data[10];
	int rc=0;

	data[0] = HT16K33_CMD_OSCILLATOR_OFF;
	data[1]=0;

	rc = i2c_write( addr,data,1 );
	return rc;
}

static char display_data[10] = {
		SEGMENT_IDX_NUMBER1,
		0,
		SEGMENT_IDX_NUMBER2,
		0,
		SEGMENT_IDX_COLON,
		0,
		SEGMENT_IDX_NUMBER3,
		0,
		SEGMENT_IDX_NUMBER4,
		0
};

//
// initialize the display device
// - configure its clocking
// - configure brightness and blinking
// - set initial value: all on or all off
//
int disp_init(int showall)
{
	const int addr = HW_I2C_ADDR_HT16K33;
	char cmd_data[2];
	int rc=0;

	cmd_data[0] = HT16K33_CMD_OSCILLATOR_ON;
	cmd_data[1]=0;

	rc = i2c_write( addr,cmd_data,1 );
	if ( 0 <= rc ){
		cmd_data[0] = HT16K33_CMD_BLINK | 0x01;
		rc = i2c_write( addr,cmd_data,1 );
	} else {
		// failed in i2c_write
		return rc;
	}
	if ( 0 <= rc ){
		cmd_data[0] = HT16K33_CMD_BRIGHTNESS | 0x08;
		rc = i2c_write( addr,cmd_data,1 );
	} else {
		// failed in i2c_write
		return rc;
	}
	if ( rc<1 ){
		// failed in i2c_write
		return rc;
	}
	if ( showall ){
		display_data[1] = SEGMENTS_ALL;
		display_data[3] = SEGMENTS_ALL;
		display_data[5] = SEGMENTS_ALL;
		display_data[7] = SEGMENTS_ALL;
		display_data[9] = SEGMENTS_ALL;
	} else {
		display_data[1] = SEGMENTS_NONE;
		display_data[3] = SEGMENTS_NONE;
		display_data[5] = SEGMENTS_NONE;
		display_data[7] = SEGMENTS_NONE;
		display_data[9] = SEGMENTS_NONE;
	}
	return i2c_write( addr,display_data,10 );
}

//
// indexing this array with the required decimal
// digit will provide the correct segment combination
// when all SEGMENT_n definitions have been made.
//
// for example
//   digit_segments[0] gives value 63
//   => value 63 lights up segments for "0"
//
// since the array size is 10 one SHOULD NOT index it
// with value larger than 9, nor with value smaller than 0
// (bad things may happen)
//
const int digit_segments[10]={
		SEGMENTS_0,
		SEGMENTS_1,
		SEGMENTS_2,
		SEGMENTS_3,
		SEGMENTS_4,
		SEGMENTS_5,
		SEGMENTS_6,
		SEGMENTS_7,
		SEGMENTS_8,
		SEGMENTS_9,
};

int disp_decimal(int value)
{
	const int addr = HW_I2C_ADDR_HT16K33;
	if ( value > 9999 ){
		return DISP_ERR_OVERFLOW;
	} else if ( value < 0 ){
		return DISP_ERR_UNDERFLOW;
	}
	display_data[1] = digit_segments[ disp_digit_of( value, 3 ) ];
	display_data[3] = digit_segments[ disp_digit_of( value, 2 ) ];
	display_data[5] = SEGMENTS_NONE;
	display_data[7] = digit_segments[ disp_digit_of( value, 1 ) ];
	display_data[9] = digit_segments[ disp_digit_of( value, 0 ) ];
	return i2c_write( addr,display_data,10 );
}

int disp_007()
{
	const int addr = HW_I2C_ADDR_HT16K33;

	display_data[1] = SEGMENTS_NONE;
	display_data[3] = SEGMENTS_0 ;
	display_data[5] = SEGMENTS_NONE;
	display_data[7] = SEGMENTS_0;
	display_data[9] = SEGMENTS_7 ;
	return i2c_write( addr,display_data,10 );
}

//
// return the Nth rightmost digit from value
//   value | n | result
//   ------+---+-------
//   417   | 0 | 7
//   417   | 1 | 1
//   417   | 2 | 4
//   417   | 3 | 0
//
int disp_digit_of(int value,int n)
{
	while( n && value ){
		n--;
		value /= 10;
	}
	return value % 10;
}
