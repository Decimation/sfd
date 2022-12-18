/*
 Name:		sfd.ino
 Created:	12/17/2022 4:53:29 PM
 Author:	Deci
*/

// the setup function runs once when you press reset or power the board
// ReSharper disable CppInconsistentNaming

#include <HID.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "LCDKeypad.h"
#include <Print.h>
#include "avr/pgmspace.h"
#include <sys/types.h>
#include <string.h>
// the loop function runs over and over again until power down or reset

enum:int32_t
{
	NONE = KEYPAD_NONE,
	RIGHT = KEYPAD_RIGHT,
	UP = KEYPAD_UP,
	DOWN = KEYPAD_DOWN,
	LEFT = KEYPAD_LEFT,
	SELECT = KEYPAD_SELECT,

	COUNT
};

// select the pins used on the LCD panel

//8, 9, 4, 5, 6, 7
static LiquidCrystal g_lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons

static int lcd_key = 0;
static int adc_key_in = 0;

static constexpr uint16_t HISTORY = 512;
static constexpr uint16_t BUF_SIZE = 256;

int g_btnCnt[COUNT];
int g_btnHistory[HISTORY];
int g_btnHistoryIdx = 0;

static char g_buf1[BUF_SIZE];
static char g_buf2[BUF_SIZE];

/*
// read the buttons
int32_t readLCDButtons()
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
*/

void setup()
{
	g_lcd.begin(16, 2); // start the library
	g_lcd.setCursor(0, 0);
	g_lcd.autoscroll();
}

void loop()
{
	g_lcd.home();
	lcd_key = LCDKeypad::button(); // read the buttons

	g_btnCnt[lcd_key]++;
	memset(g_buf1, 0, BUF_SIZE);

	// size_t l = sprintf(buf, "%d %d", lcd_key, g_buttonHistoryIndex);
	// Serial.println(buf);

	g_btnHistoryIdx = (g_btnHistoryIdx + 1) % HISTORY;
	g_btnHistory[g_btnHistoryIdx] = lcd_key;

	/*auto* rg = new int[6];
	rg[0] = UP;
	rg[1] = UP;
	rg[2] = DOWN;
	rg[3] = DOWN;
	rg[4] = LEFT;
	rg[5] = RIGHT;

	auto* bytesFind = bytes_find(g_buttonHistory, HISTORY, rg, 6);
	fprintf(stdout, "%d %d %d %p\n", g_buttonHistoryIndex, lcd_key, g_buttonCount[lcd_key], bytesFind);*/

	switch (lcd_key) {
	case UP:
	case DOWN:
	case LEFT:
	case RIGHT:
	case SELECT:
		g_lcd.clear();
		memset(g_buf1, 0, BUF_SIZE);
		memset(g_buf2, 0, BUF_SIZE);

		size_t l = sprintf(g_buf1, "%ds %dl %dr", g_btnCnt[SELECT], g_btnCnt[LEFT], g_btnCnt[RIGHT]);

		size_t l2 = sprintf(g_buf2, "%du %dd %dn", g_btnCnt[UP], g_btnCnt[DOWN], g_btnCnt[NONE]);

		g_lcd.print(g_buf1);
		g_lcd.setCursor(0, 1);
		g_lcd.print(g_buf2);

		if (g_btnHistory[g_btnHistoryIdx - 1] == DOWN && lcd_key == DOWN) {
			g_lcd.clear();
			g_lcd.print("hello world");
			g_lcd.setCursor(0, 1);
			g_lcd.print("my name is RDS");
			// g_lcd.blink();
			// delay(3000);
		}
		break;
	case NONE:
	default: ;
	}
}
