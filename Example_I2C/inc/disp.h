/*
 * disp.h
 *
 *  Created on: Oct 6, 2016
 *      Author: timppa
 */

#ifndef DISP_H_
#define DISP_H_

#define DISP_ERR_OVERFLOW  -1
#define DISP_ERR_UNDERFLOW -2

#ifdef __cplusplus
extern "C" {
#endif

#define DISP_SHOW_ALL   1
#define DISP_SHOW_NONE  0
//
//
//
int disp_init(int showall);
int disp_off();
int disp_007();
//
// display decimal value of 0...9999
//
int disp_decimal(int value);

//
// return the Nth rightmost digit from value
//   value | n | result
//   ------+---+-------
//   417   | 0 | 7
//   417   | 1 | 1
//   417   | 2 | 4
//   417   | 3 | 0
//
// note the "n" is from right to left
//
int disp_digit_of(int value,int n);

#ifdef __cplusplus
}
#endif

#endif /* DISP_H_ */
