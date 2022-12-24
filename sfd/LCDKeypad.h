/*
  LCDKeypad.h
*/

// ensure this library description is only included once
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppPolymorphicClassWithNonVirtualPublicDestructor
// ReSharper disable CppClangTidyClangDiagnosticWeakVtables
// ReSharper disable CppClangTidyClangDiagnosticNonVirtualDtor
#ifndef LCDKeypad_h
#define LCDKeypad_h

// library interface description

enum : int8_t
{
	NONE = -1,
	RIGHT = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	SELECT = 4,

	COUNT
};

int8_t button();

#endif

