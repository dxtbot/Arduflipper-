/*********************************************************************************
 * KeyboardHelloWorld.ini
 * Written by Chris Taylor for SparkFun Electronics, 10/15/12
 * 
 * NOTE: This example will not work on a "normal" UNO. It requires that the 
 * arduino-keyboard firmware be installed on the ATMega8U2. Please see
 *   https://github.com/SFE-Chris/UNO-HIDKeyboard-Library
 *
 * Simple sketch to type "Hello World!" three times in three different ways
 *
 ********************************************************************************/
#include <HIDKeyboard.h>

HIDKeyboard keyboard; // Initialize HIDKeyboard object

void setup() {
  keyboard.begin(); // Start communication
  delay(2000); // Wait for device to be found as a keyboard
}

void loop() {
  
  // Send third Hello World using print() and key modifiers. 
  keyboard.pressKey(VOLUMEMUTE); // Holds Shift and presses 'h'
  keyboard.releaseKey();
   // Notice use of pressSpecialKey for ENTER
 
  while(1);
}
