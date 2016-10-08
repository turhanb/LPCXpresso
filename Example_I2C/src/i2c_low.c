/*
 * i2c_low.c
 *
 *  Created on: Apr 14, 2016
 *      Author: timppa
 */

#include "i2c.h"

int i2c_pending_status(LPC_I2C_T* pLPC_I2C)
{
	return pLPC_I2C->STAT & I2C_STAT_MSTPENDING;
}
