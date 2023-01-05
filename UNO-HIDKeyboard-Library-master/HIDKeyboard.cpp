

/****************************************************************************
 * HIDKeyboard.cpp
 *
 * Written by Chris Taylor for SparkFun Electronics, 10/12/12
 *
 * This code is released on the Beerware License. If you use it to make
 * something cool, and you ever happen to meet me, buy me a beer.
 *
 * For use with the Arduino-keyboard-0.3 ATMega8U2 firmware from Darran.
 * For full installation instructions, see README.txt. 
 *
 * Implements functions needed to translate keyboard values into HID
 * values and send to the ATMega8U2 on an UNO running the Arduino-keyboard
 * firmware.
 *
 * These functions work by accepting ASCII values and translating them into
 * HID values using the HIDTable in HIDKeyboard.h. They then load them into
 * an 8-byte report buffer and send them over TX to the ATMega8U2. the format
 * of the report buffer is 
 *
 * 		buf[0] = modifier (value 0 for no modifier)
 *		buf[1] = reserved
 *		buf[2] = HID value of key press
 *		buf[3:7] = reserved
 *
 * The arduino-keyboard firmware for the ATMega8U2 can be found at 
 *		
 *		http://hunt.net.nz/users/darran/weblog/b3029/Arduino_UNO_Keyboard_HID_version_03.html
 *
 * The HID tables and class description can be found at
 *
 *		http://www.usb.org/developers/devclass_docs/Hut1_11.pdf
 *    http://www.usb.org/developers/devclass_docs/HID1_11.pdf
 *
 * The keyboard values used in this library can be found on page 53 of the
 * HID table. 
 *
 * For full installation instructions of the arduino-keyboard library, please
 * see README.txt
 *
 ****************************************************************************/

#include "Arduino.h"
#include "HIDKeyboard.h"


/****************************************************************************
 * Generic constructor
 *
 * Clears buf[] report array
 ****************************************************************************/
HIDKeyboard::HIDKeyboard() 
{
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	buf[4] = 0;
	buf[5] = 0;
	buf[6] = 0;
	buf[7] = 0;	
}

/****************************************************************************
 * begin()
 *
 * parameters: none
 *
 * returns: void 
 *
 * Ends any current Serial communication and hijacks to send report buffers
 * to the ATMega8U2 at 9600 baud.
 ****************************************************************************/
void HIDKeyboard::begin() 
{
	Serial.end();
	Serial.begin(9600);
}

/****************************************************************************
 * pressKey(modifier, key)
 * 
 * parameters:
 * 	uint8_t modifier -- HID modifier value to send with key
 *	uint8_t key -- ASCII character value
 *
 * returns: void
 *
 * Converts <key> to an HID value, packages with the modifier, and sends 
 * report. 
 ****************************************************************************/
void HIDKeyboard::pressKey(uint8_t modifier, uint8_t key) 
{
	this->buf[0] = modifier;
	this->buf[2] = HIDTable[key];
	Serial.write(this->buf, 8);
}

/****************************************************************************
 * pressKey(key)
 * 
 * parameters:
 *	uint8_t key -- ASCII character value
 *
 * returns: void
 *
 * Overloaded from pressKey(modifier, key)
 *
 * Looks up key's modifier, and calls pressKey(modifier, key);
 ****************************************************************************/
void HIDKeyboard::pressKey(uint8_t key) 
{
	pressKey(modifierTable[key], key);
}


/****************************************************************************
 * pressSpecialKey(modifier, specialKey)
 * 
 * parameters:
 *	uint8_t modifier -- HID modifier to send with specialKey
 *	uint8_t specialKey -- HID table value
 *
 * returns: void
 *
 * Packages <modifier> with <specialKey> and sends report
 ***************************************************************************/
void HIDKeyboard::pressSpecialKey(uint8_t modifier, uint8_t specialKey) 
{
	this->buf[0] = modifier;
	this->buf[2] = specialKey;
	Serial.write(this->buf, 8);
}


/****************************************************************************
 * pressSpecialKey(specialKey)
 * 
 * parameters:
 *	uint8_t specialKey -- HID table value
 *
 * returns: void
 *
 * Overloaded from pressSpecialKey(modifier, specialKey)
 * 
 * Calls pressSpecialKey(0, specialKey)
 ****************************************************************************/
void HIDKeyboard::pressSpecialKey(uint8_t specialKey) 
{
	if(specialKey <= 0x08)
	{
		pressSpecialKey(specialKey, 0);
	}
	else
	{
		pressSpecialKey(0, specialKey);
	}
}


/****************************************************************************
 * releaseKey()
 * 
 * parameters: none
 *
 * returns: void
 *
 * Sends empty report, indicating that no keys are pressed
 ****************************************************************************/
void HIDKeyboard::releaseKey()
{
	this->buf[0] = 0;
	this->buf[2] = 0;
	Serial.write(this->buf, 8);
}

/****************************************************************************
 * print(sequence)
 * 
 * parameters:
 *	char* sequence -- character string
 *
 * returns: void
 *
 * Converts ASCII string <sequence> into HID values and presses the keys
 * in order. 
 ****************************************************************************/
void HIDKeyboard::print(char* sequence)
{	
	int i = 0;
	while(sequence[i] != '\0') 
	{
		pressKey(modifierTable[sequence[i]], sequence[i]);
		releaseKey();
		i++;
	}
}

/****************************************************************************
 * println(sequence)
 * 
 * parameters:
 *	char* sequence -- character string
 *
 * returns: void
 *
 * Calls print(sequence) followed by an ENTER press and release 
 ****************************************************************************/
void HIDKeyboard::println(char* sequence)
{	
	print(sequence);
	pressSpecialKey(0, ENTER);
	releaseKey();
}


