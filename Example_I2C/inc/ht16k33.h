/*
 * ht16k33.h
 *
 *  Created on: Oct 6, 2016
 *      Author: timppa
 */

#ifndef HT16K33_H_
#define HT16K33_H_

#define HT16K33_CMD_BLINK          0x80
#define HT16K33_CMD_BRIGHTNESS     0xE0
#define HT16K33_CMD_OSCILLATOR_OFF 0x20
#define HT16K33_CMD_OSCILLATOR_ON  0x21

#define SEGMENT_IDX_NUMBER1 0
#define SEGMENT_IDX_NUMBER2 1
#define SEGMENT_IDX_COLON   2
#define SEGMENT_IDX_NUMBER3 3
#define SEGMENT_IDX_NUMBER4 4

//
// number segments are displayed with combination
// of following values:
//
//   1 1 1
// 32     2
// 32     2
// 32     2
//   64 64
// 16     4
// 16     4
// 16     4
//   8 8 8
//
// 0 (zero) is 1+2+4+8+16+32 = 63
#define SEGMENTS_0 63
// 1 (one)  is 2+4 = 6
#define SEGMENTS_1 6
// 2 (two)  is 1+2+8+16+64 = 91
#define SEGMENTS_2 91
// 3 (three)  is 1+2+4+8+64 = 79
#define SEGMENTS_3 79
// 4 (four)  is 2+4+32+64 = 102
#define SEGMENTS_4 102
// 5 (five)  is 1+4+8+32+64 = 109
#define SEGMENTS_5 109
// 6 (six)  is 1+4+8+16+32+64 = 125
#define SEGMENTS_6 125
// 7 (seven)  is 1+2+4 = 7
#define SEGMENTS_7 7
// 8 (eight)  is 1+2+4+8+16+32+64 = 127
#define SEGMENTS_8 127
// 9 (nine)  is 1+2+4+8+32+64 = 111
#define SEGMENTS_9 111
//
// Letters may be needed later: some of them can be displayed correctly
//
// F is 1+32+64+16 = 113
#define SEGMENTS_F 133

#if 0
//
// not used but possible characters
//
#define SEGMENTS_A
#define SEGMENTS_B
#define SEGMENTS_C
#define SEGMENTS_D
#define SEGMENTS_E
#define SEGMENTS_F

#define SEGMENTS_A
#define SEGMENTS_d
#define SEGMENTS_E
#define SEGMENTS_m
#define SEGMENTS_n
#define SEGMENTS_o
#define SEGMENTS_P
#define SEGMENTS_S
#define SEGMENTS_u
#define SEGMENTS_w
#endif

#define SEGMENTS_ALL  (char)0xFF
#define SEGMENTS_NONE (char)0x00

#endif /* HT16K33_H_ */
