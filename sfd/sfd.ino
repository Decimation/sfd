/*
 Name:		sfd.ino
 Created:	12/17/2022 4:53:29 PM
 Author:	Deci
*/

// the setup function runs once when you press reset or power the board
// ReSharper disable CppInconsistentNaming

// ReSharper disable CppClangTidyClangDiagnosticReservedMacroIdentifier
#include <HID.h>
#include <LiquidCrystal.h>
#include "LCDKeypad.h"
#include <Print.h>
#include <avr/pgmspace.h>
#include <sys/types.h>
#include <string.h>
#include <EEPROM.h>

/*#ifndef __progmem__
#define __progmem__
#endif
#ifndef __extension__
#define __extension__
#endif*/

int freeRam()
{
	extern int __heap_start, *__brkval;

	int v;

	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void display_freeram()
{
	Serial.print(F("- SRAM left: "));

	Serial.println(freeRam());
}

//8, 9, 4, 5, 6, 7
static LiquidCrystal g_lcd(8, 9, 4, 5, 6, 7);

static int8_t lcd_key = 0;
static int8_t adc_key_in = 0;

static constexpr uint16_t HISTORY = 512;
static constexpr uint16_t BUF_SIZE = 256;

uint8_t g_btnCnt[COUNT + 1];
int g_btnHistory[HISTORY];
uint8_t g_btnHistoryIdx = 0;

static char g_buf1[BUF_SIZE];

void setup()
{
	g_lcd.begin(16, 2); // start the library
	g_lcd.setCursor(0, 0);
	g_lcd.autoscroll();
	display_freeram();
}

template <typename T, int size>
int GetArrLength(T (&)[size]) { return size; }

void write(char* buf, const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	memset(buf, 0, BUF_SIZE);
	size_t s = snprintf(buf, BUF_SIZE, fmt, va);
	g_lcd.print(buf);
	va_end(va);
}

void loop()
{
	g_lcd.home();
	lcd_key = button(); // read the buttons

	g_btnCnt[lcd_key]++;
	memset(g_buf1, 0, BUF_SIZE);

	g_btnHistoryIdx = (g_btnHistoryIdx + 1) % HISTORY;
	g_btnHistory[g_btnHistoryIdx] = lcd_key;

	static const char* const fmt PROGMEM = "%ds %dl %dr";
	static const char* const duDdDn PROGMEM = "%du %dd %dn";

	switch (lcd_key) {
	case UP:
	case DOWN:
	case LEFT:
	case RIGHT:
	case SELECT:
		g_lcd.clear();
		write(g_buf1, fmt, g_btnCnt[SELECT], g_btnCnt[LEFT], g_btnCnt[RIGHT]);
		g_lcd.setCursor(0, 1);
		write(g_buf1, duDdDn, g_btnCnt[UP], g_btnCnt[DOWN], g_btnCnt[COUNT]);

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
