/*
 * display.h
 *
 *  Created on: 15.09.2016
 *      Author: meiert
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include <Adafruit_SSD1306.h>
#include <tools.h>

//tool -> http://javl.github.io/image2cpp/
//Brightness =50
//Canvas 64*32
//inver colors
const unsigned char bmw_logo [] PROGMEM = {
0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1f, 0xf8, 0x18, 0x00, 0x00,
0x00, 0x01, 0x8f, 0xf3, 0xc7, 0xf1, 0x80, 0x00, 0x00, 0x0c, 0xff, 0xf1, 0x87, 0xff, 0x30, 0x00,
0x00, 0x23, 0xff, 0xf4, 0x27, 0xff, 0xc4, 0x00, 0x00, 0x9f, 0xff, 0xf6, 0x67, 0xff, 0x99, 0x00,
0x03, 0x60, 0xff, 0xff, 0xff, 0xfe, 0x3e, 0xc0, 0x04, 0x9c, 0x1f, 0x1f, 0x00, 0xfc, 0x03, 0x20,
0x09, 0x13, 0x31, 0xff, 0x00, 0x08, 0x47, 0x90, 0x17, 0xc2, 0x4f, 0xff, 0x00, 0x03, 0x80, 0xe8,
0x27, 0xf9, 0xbf, 0xff, 0x00, 0x01, 0x8f, 0xe4, 0x4f, 0xfe, 0x7f, 0xff, 0x00, 0x00, 0x7f, 0xf2,
0x5f, 0xfc, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xfa, 0x9f, 0xfd, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xf9,
0x9f, 0xf9, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xf9, 0xbf, 0xfb, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xfd,
0xbf, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xdf, 0xfd, 0x9f, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x9f, 0xf9,
0x9f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xbf, 0xf9, 0x5f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0x3f, 0xfa,
0x4f, 0xfe, 0x00, 0x00, 0xff, 0xfe, 0x7f, 0xf2, 0x27, 0xff, 0x00, 0x00, 0xff, 0xfc, 0xff, 0xe4,
0x17, 0xff, 0xc0, 0x00, 0xff, 0xf3, 0xff, 0xe8, 0x09, 0xff, 0xf0, 0x00, 0xff, 0x8f, 0xff, 0x90,
0x04, 0xff, 0xfe, 0x00, 0xf8, 0x7f, 0xff, 0x20, 0x03, 0x7f, 0xff, 0xfe, 0x7f, 0xff, 0xfe, 0xc0,
0x00, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x00, 0x00, 0x27, 0xff, 0xff, 0xff, 0xff, 0xe4, 0x00,
0x00, 0x0c, 0xff, 0xff, 0xff, 0xff, 0x30, 0x00, 0x00, 0x01, 0x8f, 0xff, 0xff, 0xf1, 0x80, 0x00,
0x00, 0x00, 0x18, 0x3f, 0xfc, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
};

const byte size_int_32=4;


void write_to_display(String);
void update_display(byte[],byte[]);

void display_setup(void);



#endif /* SRC_DISPLAY_H_ */
