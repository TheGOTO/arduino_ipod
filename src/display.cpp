/*
 * display.cpp
 *
 *  Created on: 15.09.2016
 *      Author: meiert
 */

#include <display.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
byte linecount=0;

void write_to_display(String value)
{
	#ifdef DEBUG2
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setTextWrap(true);
	if(linecount>=4)
	{
	  display.setCursor(0, 0);
	  display.clearDisplay();
	  linecount=0;
	}

	display.println(value);
	display.display();
	#endif

  //  display.startscrollright(0x00, 0x0F);
  //  delay(2000);
  //  display.stopscroll();
  //  delay(1000);
  //  display.startscrollleft(0x00, 0x0F);
  //  delay(2000);
  //  display.stopscroll();
  linecount++;
}


void update_display(byte playlist_pos[],byte playlist_total[])
{

	#ifndef DEBUG2
	display.clearDisplay();
	display.setTextSize(0);
	display.setTextColor(WHITE);
	display.setTextWrap(true);


	display.setCursor(0,0);
	display.print(result_toString(playlist_pos,0,size_int_32));
	//display.setCursor(size_int_32, 0);
	display.print("/");
	//display.setCursor(size_int_32+1, 0);
	display.print(result_toString(playlist_total,0,size_int_32));



	display.display();
	#endif

  //  delay(1);

  //  display.startscrollright(0x00, 0x0F);
  //  delay(2000);
  //  display.stopscroll();
  //  delay(1000);
  //  display.startscrollleft(0x00, 0x0F);
  //  delay(2000);
  //  display.stopscroll();

}

void display_setup(void)
{
	  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	  display.clearDisplay();
	  display.drawBitmap(30, 0,  bmw_logo, 64, 32, 1);
	  display.display();
	  delay(2000);
	  display.clearDisplay();
}
