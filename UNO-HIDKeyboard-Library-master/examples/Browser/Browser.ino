/*************************************************************************
 * HID Keyboard Browser Example
 * 
 * For use with HIDKeyboard.h Arduino library
 *    https://github.com/SFE-Chris/UNO-HIDKeyboard-Library
 *
 * NOTE: This sketch will not work on it's own. Please see the github 
 * page for info on how to turn the UNO into a keyboard. 
 * 
 * In Windows 7, sends keypresses to open a browser and type in it. 
 *************************************************************************/
#include <HIDKeyboard.h>
 
HIDKeyboard keyboard; // Initialize HIDKeyboard object

void setup() {
  keyboard.begin(); // Start communication
  delay(200);
}

void loop() {
  delay(10); 
  keyboard.pressSpecialKey(GUI); // Press the GUI (Windows) key
  keyboard.releaseKey(); // Release the GUI key
  delay(200); // Wait for the Start Button to open
  keyboard.println("GOOGLE");
  keyboard.pressSpecialKey(ENTER);
  keyboard.releaseKey();// Type "run" followed by a carriage return
  delay(2000); // Wait for the run dialog to come up 
  keyboard.println("www.Youtube.com"); // Type URL into run window followed by a carriage return.
keyboard.pressSpecialKey(ENTER);
  keyboard.releaseKey();
  delay(10);
  while(1);

}

