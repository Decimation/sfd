/*
 Name:		sfd.ino
 Created:	12/17/2022 4:53:29 PM
 Author:	Deci
*/

// the setup function runs once when you press reset or power the board
// ReSharper disable CppInconsistentNaming
#include <LiquidCrystal.h>
#include "LCDKeypad.h"

// the loop function runs over and over again until power down or reset

enum
{
	ERROR = -1,

	RIGHT = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	SELECT = 4,
	NONE = 5,

	COUNT
};

// select the pins used on the LCD panel

static LiquidCrystal g_lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons

static int lcd_key = 0;

static int adc_key_in = 0;

// read the buttons
int readLCDButtons()
{
	adc_key_in = analogRead(0);
	// read the value from the sensor
	// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
	// we add approx 50 to those values and check to see if we are close
	if (adc_key_in > 1000) return NONE;
	// We make this the 1st option for speed reasons since it will be the most likely result
	if (adc_key_in < 50) return RIGHT;
	if (adc_key_in < 195) return UP;
	if (adc_key_in < 380) return DOWN;
	if (adc_key_in < 555) return LEFT;
	if (adc_key_in < 790) return SELECT;
	return NONE; // when all others fail, return this...
}

void setup()
{
	g_lcd.begin(16, 2); // start the library
	g_lcd.setCursor(0, 0);
	g_lcd.print("Push the buttons"); // print a simple message
}

int g_rgCount[COUNT];

void loop()
{
	g_lcd.setCursor(9, 1); // move cursor to second line "1" and 9 spaces over
	g_lcd.print(millis() / 1000); // display seconds elapsed since power-up

	g_lcd.setCursor(0, 1); // move to the begining of the second line
	lcd_key = readLCDButtons(); // read the buttons

	g_rgCount[lcd_key]++;
}
