/*
  LCDKeypad.cpp
*/
// ReSharper disable CppInconsistentNaming
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// include this library's description file

#include <LiquidCrystal.h>
#include "LCDKeypad.h"

int8_t button()
{
	constexpr static uint8_t NUM_KEYS = 5;
	constexpr static uint16_t adc_key_val[NUM_KEYS] = {
		30, 150, 360, 535, 760
	};
	int k;
	int input = analogRead(0);
	for (k = 0; k < NUM_KEYS; k++) {
		if (input < adc_key_val[k]) {
			return k;
		}
	}
	if (k >= NUM_KEYS)
		k = -1; // No valid key pressed
	return k;
}
