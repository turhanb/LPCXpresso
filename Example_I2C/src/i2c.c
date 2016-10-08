/*
 * i2c.c
 *
 *  Created on: Mar 19, 2016
 *      Author: timppa
 */

#include "chip.h"

#include "i2c.h"

int i2c_reset(LPC_SYSCTL_T* pLPC_SYSCON)
{
	if ( pLPC_SYSCON ){
		pLPC_SYSCON -> SYSAHBCLKCTRL |=  (1 << 5);
		pLPC_SYSCON -> PRESETCTRL &= ~(1 << 6);		// I2C reset
		pLPC_SYSCON -> PRESETCTRL |= (1 << 6);
		// I2C clock division
		uint32_t* pclk = (uint32_t*)0x40050014;		// page 222
		*pclk = 15;	// sysclk divided by 120

		return 0;
	}
	return -1;
}

#define I2C_DEBUG_READ 0

//
// I2C read protocol implementation
//
int i2c_read_p( LPC_I2C_T* pLPC_I2C,
		i2c_data_t* data )
{
	pLPC_I2C->CFG = I2C_CFG_MSTEN;
	// wait until pending
	while( 0==i2c_pending_status(pLPC_I2C) );
	if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
		return I2C_ERR_READ_IDLE;
	}

	if ( data->reglen && data->reg ){
		pLPC_I2C->MSTDAT = (data->address << 1) | 0;
		pLPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
		// wait until pending
		while( 0==i2c_pending_status(pLPC_I2C) );
		if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){
			return I2C_ERR_WRITE_TX;
		}
		pLPC_I2C->MSTDAT = data->reg[0];
		pLPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;
		// wait until pending
		while( 0==i2c_pending_status(pLPC_I2C) );
		pLPC_I2C->MSTDAT = (data->address << 1) | 1;
		pLPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
	} else {
		pLPC_I2C->MSTDAT = (data->address << 1) | 1;
		pLPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
	}
	// wait until pending
	while( 0==i2c_pending_status(pLPC_I2C) );
	if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_RX ){
		return I2C_ERR_READ_RX;
	}
	int count = 0;
	while ( count<data->length ){
		data->buffer[count] = pLPC_I2C->MSTDAT;
		count++;
		if ( count<data->length ){
			pLPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;
			// wait until pending
			while( 0==i2c_pending_status(pLPC_I2C) );
			if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_RX ){
				return I2C_ERR_READ_RX;
			}
		} else {
			pLPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP;
			// wait until pending
			while( 0==i2c_pending_status(pLPC_I2C) );
			if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
				return I2C_ERR_READ_STOP;
			}
		}
	}
	return count;
}

#ifndef UNIT_TESTING
// otherwise a module separation is needed
int i2c_read(int address,
		char* reg,int reglen,
		char* buffer,int length)
{
	i2c_data_t i2c_data;
	i2c_data.address=address;
	i2c_data.reg=reg;
	i2c_data.reglen =reglen;
	i2c_data.buffer = buffer;
	i2c_data.length = length;
	return i2c_read_p( LPC_I2C, &i2c_data );
}
#endif

//
// I2C write protocol implementation
//
int i2c_write_p( LPC_I2C_T* pLPC_I2C,
		i2c_data_t* data )
{
	pLPC_I2C->CFG = I2C_CFG_MSTEN;
	// wait until pending
	while( 0==i2c_pending_status(pLPC_I2C) );
	if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
		return I2C_ERR_WRITE_IDLE;
	}

	pLPC_I2C->MSTDAT = (data->address << 1) | 0;
	pLPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
	// wait until pending
	while( 0==i2c_pending_status(pLPC_I2C) );
	if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){
		return I2C_ERR_WRITE_TX;
	}
	int count=0;
	while ( count<data->length ){
		pLPC_I2C->MSTDAT = data->buffer[count];
		count++;
		pLPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;
		// wait until pending
		while( 0==i2c_pending_status(pLPC_I2C) );
		if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){
			return I2C_ERR_WRITE_TX;
		}
	}
	pLPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP;
	// wait until pending
	while( 0==i2c_pending_status(pLPC_I2C) );
	if ( (pLPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
		return I2C_ERR_WRITE_STOP;
	}
	// should return "count" here
	return 1;
}

#ifndef UNIT_TESTING
// otherwise a module separation is needed
int i2c_write(int address,const char* buffer,int length)
{
	i2c_data_t i2c_data;
	i2c_data.address = address;
	i2c_data.reg = 0;
	i2c_data.reglen = 0;
	i2c_data.buffer = (char*)buffer;
	i2c_data.length = length;
	return i2c_write_p( LPC_I2C, &i2c_data );
}
#endif
